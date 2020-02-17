#include <pthread.h>
#include <sched.h>
#include <signal.h>
#include <time.h>
#include <sys/time.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <sys/shm.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <inttypes.h>
#include <errno.h>
#include <math.h>

#include "ctrl.h"
#include "comm.h"

extern "C" {
  #include "glue.h"
}

// Local Variables
bool volatile ctrl_run = true;          // Control loop run state
int64_t ctrl_loop_period = 20000000;	// 20ms -> 50Hz, value in nanoseconds
int64_t tosleep;                        // slack time, nanoseconds to sleep till next ctrl loop call 

uint32_t missed_heartbeat_count = 0;    // Count missed heartbeat, estop at max cnt
uint32_t last_heartbeat = 0;              // Last heartbeat value, should increase monotonically

// Local Ctrl Variables
Ctrl_Cmd ctrl_cmd;                      // Command data from C&DH
Ctrl_Telem ctrl_telem;                  // Telemetry data to C&DH

int main(int argc, char *argv[])
{
    printf("=== ROBOCAR - CTRL: START ===\n");

    // Make sure ctrl-C stops the program under controlled circumstances
    signal(SIGINT, &sigint);

    // Initalize Communication to C&DH
    printf("Initializing comm shared memory...\n");
    if(comm_init() == 0)
    {
	    printf("ERROR: Comm init failed.\n");
	    exit(1);
    }

    // Initialize Glue Board
    printf("Initalizing glue board...\n");
    glue_init();

    // Initialize the control state machine to ESTOP mode
    ctrl_telem.mode = ESTOP;

    // Create attributes for an isolated real-time thread
    printf("Starting isolated soft real-time control loop thread...\n");
    pthread_attr_t attr = {};
    pthread_attr_init(&attr);
    // Lift the thread off core 0, which takes system interrupts
    cpu_set_t cpuset = {};
    CPU_ZERO(&cpuset);
    CPU_SET(1, &cpuset);
    pthread_attr_setaffinity_np(&attr, 1, &cpuset);
    // Make it use FIFO policy for real-time scheduling
    pthread_attr_setschedpolicy(&attr, SCHED_FIFO);
    pthread_attr_setinheritsched(&attr, PTHREAD_EXPLICIT_SCHED);
    // Set the priority
    sched_param param = {};
    param.sched_priority = 30;
    pthread_attr_setschedparam(&attr, &param);

    // Create the real-time thread
    pthread_t ctrl_thread;
    int err;
    if ((err = pthread_create(&ctrl_thread, &attr, &ctrl_timer_func, NULL)) != 0) {
        char const *emsg = ((err == EAGAIN) ? "EAGAIN" : ((err == EINVAL) ? "EINVAL" : ((err == EPERM) ? "EPERM" : "unknown")));
        fprintf(stderr, "pthread_create() failed (%d %s); are you sure you're root?\n", err, emsg);
        fprintf(stderr, "You may also need to do:\n");
        fprintf(stderr, "echo -1 > /proc/sys/kernel/sched_rt_runtime_us\n");
        exit(1);
    }

    // Wait for the program to be done
    void *ignore = NULL;
    pthread_join(ctrl_thread, &ignore);
    
    // Real time thread exited
    printf("Real-time thread exited.\n");

    // Stop all motors
    glue_estop();
    printf("ESTOP!\n");

    // Close communication shared memory
    comm_close();
    printf("Comm shared memory closed.\n");

    printf("=== ROBOCAR - CTRL: STOP ===\n");

    return 0;
}

void *ctrl_timer_func(void *) {

    //  keep track of the passage of time at a 20-millisecond quantized interval
    struct timespec last = {}, now = {};
    clock_gettime(CLOCK_MONOTONIC_RAW, &last);

    while (ctrl_run) 
    {
        clock_gettime(CLOCK_MONOTONIC_RAW, &now);
        //  20 milliseconds, as nanoseconds
        tosleep = ctrl_loop_period - (now.tv_sec - last.tv_sec) * 1000000000 - (now.tv_nsec - last.tv_nsec);

        if(tosleep < 0)
        {
            printf("ERROR: SCHEDULED CONTROL LOOP ROUTINE OVERRAN!!! - TOSLEEP = %" PRId64 "\n", tosleep);
        }
        else
        {
            printf("TOSLEEP = %" PRId64 ", ", tosleep);
        }

        last.tv_nsec += ctrl_loop_period;
        if (last.tv_nsec >= 1000000000) {
            last.tv_nsec -= 1000000000;
            last.tv_sec += 1;
        }
        if (tosleep > (2*ctrl_loop_period)) {
            //  missed by more than one full interval! re-set the clock basis
            last = now;
            tosleep = 0;
        }
        if (tosleep > 0) {
            struct timespec slp = {};
            slp.tv_nsec = (long)tosleep;
            nanosleep(&slp, NULL);
        }

        // Run the actual control loop routine
        ctrl_loop();
    }

    return NULL;
}

int ctrl_loop(void)
{
    // Update ToSleep for debug telemetry
    ctrl_telem.tosleep = tosleep;

    // Incremement timestamp
    ctrl_telem.timestamp += 1;
    
    // Read and print state
    ctrl_telem.glue_state = glue_state_update();

    // If in debug mode, print raw telem
    #ifdef DEBUG
        printf("TIME = %i,", ctrl_telem.timestamp);
        glue_print(ctrl_telem.glue_state);
        printf("CMDSTRPOS = %f, CMDDRVVEL = %f\n", ctrl_cmd.steering_pos, ctrl_cmd.drive_vel);
    #endif

    // Control state machine
    switch (ctrl_telem.mode)
    {
        case ESTOP:                             // Emergency Stop - Zero actuators
            printf("MODE: EMERGENCY STOP\n");
            glue_estop();
            break;

        case INIT:                              // Initialize drive parameters
            printf("MODE: INIT\n");

            // TODO: Wiggle steering wheels to test/set range

            break;

        case IDLE:                              // Idle waiting for race to start
            printf("MODE: IDLE\n");
            break;

        case RUN:                               // Run the race
            printf("MODE: RUN\n");

            // Delay to simulate number crunching
            // TODO: Soon to be PID controllers
            usleep(500);

            // Set Drive and Steering Motor to 5%
            glue_set_drive_motor(0.1);
            glue_set_steering_motor(0.1);
            break;

        case STOP:                              // Stop racing
            printf("MODE: STOP\n");
            glue_set_drive_motor(0);
            glue_set_steering_motor(0);
            break;

        case FAULT:                             // Error occured, ESTOP and phone home
            printf("MODE: FAULT\n");
            glue_estop();
            break;

        case CLEARFAULT:                        // Clear the error, and go to ESTOP state
            printf("MODE: CLEAR FAULT");
	    missed_heartbeat_count = 0;
            ctrl_telem.mode = ESTOP;
            break;

        default:                                // If all else fails, ESTOP and phone home
            printf("MODE: FATAL\n");
            glue_estop();
            break;
    }

    // Copy the latest telemetry/command data from shared memory to local memory
    comm_transaction(&ctrl_cmd, &ctrl_telem);

    // Fault protection and mode command handler
    if(ctrl_telem.mode != FAULT)    // No faults, continue
    {
        // Timeout based deadman switch
        // If we lose too many heartbeat counts, issue an ESTOP
        // Else accept the new command mode
        if(last_heartbeat < ctrl_cmd.heartbeat)
        {
            // Store the new heartbeat count value
            last_heartbeat = ctrl_cmd.heartbeat;

            // Reset missed heartbeat counter
            missed_heartbeat_count = 0;

            // Set the control mode for the next cycle based on what is commanded
            ctrl_telem.mode = ctrl_cmd.mode;
        }
        else
        {
            if(missed_heartbeat_count >= MAX_MISSED_HEARTBEAT)
            {
                ctrl_telem.mode = FAULT;
            }
            else
            {
                missed_heartbeat_count++;
            }
        }
    }
    else                            // Fault state, wait for clear fault command
    {
        if(ctrl_cmd.mode == CLEARFAULT)
        {
            ctrl_telem.mode = ctrl_cmd.mode;
        }
    }

    return 1;
}

void sigint(int) {
    ctrl_run = false;
}

/**************************************************************************/
/*!
    @file     ctrl.cpp
    @author   Arko
    @license  BSD (see license.txt)
    Robocar Control Program
    v1.0 - First release
*/
/**************************************************************************/

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
bool volatile ctrl_run = true;                  // Control loop run state

int64_t ctrl_loop_period = CTRL_LOOP_PERIOD;	// 50ms -> 20Hz, value in nanoseconds
int64_t tosleep;                                // slack time, nanoseconds to sleep till next ctrl loop call 

uint32_t missed_heartbeat_count = 0;            // Count missed heartbeat, estop at max cnt
uint32_t last_heartbeat = 0;                    // Last heartbeat value, should increase monotonically

// Local Communication Variables
Ctrl_Cmd ctrl_cmd;                              // Command data from C&DH
Ctrl_Telem ctrl_telem;                          // Telemetry data to C&DH

// Steering Motor PID Variables
float steer_desired_pos = 0.0;
float steer_actual_pos = 0.0;
float steer_error_pos = 0.0;
float steer_error_pos_prior = 0.0;
float steer_i_accu = 0.0;
float steer_p = 0.0;
float steer_i = 0.0;
float steer_d = 0.0;
float steer_out = 0.0;

// Drive Motor PID Variables
float drive_desired_vel = 0.0;
float drive_actual_vel = 0.0;
float drive_error_vel = 0.0;
float drive_error_vel_prior = 0.0;
float drive_i_accu = 0.0;
float drive_p = 0.0;
float drive_i = 0.0;
float drive_d = 0.0;
float drive_f = 0.0;
float drive_out = 0.0;

/**************************************************************************/
/*!
    @brief  Control program main
    @param  argc    Arguement count
    @param  argv    Arguement variables
    @return Error code
*/
/**************************************************************************/
int main(int argc, char *argv[])
{
    // ========================================================================
    // Initalize system
    // ========================================================================
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
    // ========================================================================

    // ========================================================================
    // Configure and start real-time thread
    // ========================================================================
    // Create attributes for an isolated real-time thread
    printf("Starting isolated soft real-time control loop thread...\n");
    pthread_attr_t attr = {};
    pthread_attr_init(&attr);
    // Lift the thread off core 3, which takes system interrupts
    cpu_set_t cpuset = {};
    CPU_ZERO(&cpuset);
    CPU_SET(3, &cpuset);
    pthread_attr_setaffinity_np(&attr, 1, &cpuset);
    // Make it use FIFO policy for "soft" real-time scheduling
    pthread_attr_setschedpolicy(&attr, SCHED_FIFO);
    pthread_attr_setinheritsched(&attr, PTHREAD_EXPLICIT_SCHED);
    // Set the priority
    sched_param param = {};
    param.sched_priority = 1;
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
    // ========================================================================

    // ========================================================================
    // Shutdown system
    // ========================================================================
    // Real time thread exited
    printf("Real-time thread exited.\n");

    // Stop all motors
    glue_estop();
    printf("ESTOP!\n");

    // Close communication shared memory
    comm_close();
    printf("Comm shared memory closed.\n");

    printf("=== ROBOCAR - CTRL: STOP ===\n");
    // ========================================================================

    return 0;
}

/**************************************************************************/
/*!
    @brief  Control timer function calls the control loop function at
            a specified frequency.
    @return Error code
*/
/**************************************************************************/
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
            printf("RESETTING CLOCK...");
            last = now;
            tosleep = 0;
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

        // ========================================================================
        // Control System Main Loop
        // ========================================================================
        ctrl_loop();
        // ========================================================================
    }

    return NULL;
}

/**************************************************************************/
/*!
    @brief  Control loop function implements a state machine driven by an
            external program to read the sensors and drive actuators.
            Fault protection cuts motor power when communication with the
            external program is lost.
    @return Error code
*/
/**************************************************************************/
int ctrl_loop(void)
{
    // ========================================================================
    // Update telemetry
    // ========================================================================
    // Update ToSleep for debug telemetry
    ctrl_telem.tosleep = tosleep;

    // Incremement timestamp
    ctrl_telem.timestamp += 1;

    // Read and print state
    ctrl_telem.glue_state = glue_state_update();

    // If in debug mode, print raw telem
    #ifdef TELEM_DEBUG
        printf("TIME = %i,", ctrl_telem.timestamp);
        glue_print(ctrl_telem.glue_state);
        printf("CMDSTRPOS = %f, CMDDRVVEL = %f\n", ctrl_cmd.steer_pos, ctrl_cmd.drive_vel);
    #endif
    // ========================================================================

    // ========================================================================
    // Control state machine
    // ========================================================================
    switch (ctrl_telem.mode)
    {
        case ESTOP:                             // Emergency Stop - Zero actuators
            printf("MODE: EMERGENCY STOP\n");
            glue_estop();
            break;

        case INIT:                              // Initialize drive parameters
            printf("MODE: INIT\n");
            // Zero steering and drive motor PID variables
            steer_desired_pos = 0.0;
            steer_actual_pos = 0.0;
            steer_error_pos = 0.0;
            steer_error_pos_prior = 0.0;
            steer_i_accu = 0.0;
            steer_p = 0.0;
            steer_i = 0.0;
            steer_d = 0.0;
            steer_out = 0.0;
            drive_desired_vel = 0.0;
            drive_actual_vel = 0.0;
            drive_error_vel = 0.0;
            drive_error_vel_prior = 0.0;
            drive_i_accu = 0.0;
            drive_p = 0.0;
            drive_i = 0.0;
            drive_d = 0.0;
            drive_f = 0.0;
            drive_out = 0.0;
            // Set motor values
            glue_set_steering_motor(steer_out);
            glue_set_drive_motor(drive_out);
            break;

        case IDLE:                              // Idle waiting for race to start
            printf("MODE: IDLE\n");
            break;

        case RUN:                               // Run the race
            printf("MODE: RUN\n");
            // ================================================================
            // Steering Position PID Controller
            // ================================================================
            // Convert steering encoder voltage to normalized steering position value
            steer_actual_pos = STEER_SCALER * (STEER_ZERO_VAL - ctrl_telem.glue_state.steer_position);

            // Set commanded steering position as the new goal position
            steer_desired_pos = ctrl_cmd.steer_pos;

            // Calculate steering position error
            steer_error_pos = steer_desired_pos - steer_actual_pos;

            // Update integral accumulator
            steer_i_accu = steer_i_accu + (steer_error_pos * DT);

            // Steering PID terms
            steer_p = STEER_P_GAIN * steer_error_pos;
            steer_i = STEER_I_GAIN * steer_i_accu;
            steer_d = STEER_D_GAIN * (steer_error_pos - steer_error_pos_prior);

            // Sanitize integral term
            if(steer_i > STEER_I_LIMIT)
            {
                steer_i = STEER_I_LIMIT;
            }
            if(steer_i < -1.0*STEER_I_LIMIT)
            {
                steer_i = -1.0*STEER_I_LIMIT;
            }

            // Caluclate PID output
            steer_out = steer_p + steer_i + steer_d;

            // Store position error for next D term calculation
            steer_error_pos_prior = steer_error_pos;

            // Sanitize the steering motor output value
            if(steer_out > STEER_OUT_LIMIT)
            {
                steer_out = STEER_OUT_LIMIT;
            }
            if(steer_out < -1.0 * STEER_OUT_LIMIT)
            {
                steer_out = -1.0 * STEER_OUT_LIMIT;
            }
            // ================================================================

            // ================================================================
            // Drive Velocity PIDF Controller
            // ================================================================
            // Convert drive motor voltage to normalize drive velocity value
            drive_actual_vel = DRIVE_SCALER * (DRIVE_ZERO_VAL - ctrl_telem.glue_state.drive_velocity);

            // Set commanded drive velocity as the new goal velocity
            drive_desired_vel = ctrl_cmd.drive_vel;

            // Calculate drive velocity error
            if(drive_desired_vel > 0.01 || drive_desired_vel < -0.01)
            {
                drive_error_vel = drive_desired_vel - drive_actual_vel;
            }
            else
            {
                drive_error_vel = 0.0;
            }

            // Update integral accumulator
            drive_i_accu = drive_i_accu + (drive_error_vel * DT);

            // Drive PIDF terms
            drive_p = DRIVE_P_GAIN * drive_error_vel;
            drive_i = DRIVE_I_GAIN * drive_i_accu;
            drive_d = DRIVE_D_GAIN * (drive_error_vel - drive_error_vel_prior);
            drive_f = DRIVE_F_GAIN * drive_desired_vel;

            // Sanitize integral accumulator
            if(drive_i > DRIVE_I_LIMIT)
            {
                drive_i = DRIVE_I_LIMIT;
            }
            if(drive_i < -1.0*DRIVE_I_LIMIT)
            {
                drive_i = -1.0*DRIVE_I_LIMIT;
            }

            // Calculate PIDF output
            drive_out = drive_p + drive_i + drive_d + drive_f;

            // Store velocity error for next D term calculation
            drive_error_vel_prior = drive_error_vel;

            // Sanitize the drive motor output value
            if(drive_out > DRIVE_OUT_LIMIT)
            {
                drive_out = DRIVE_OUT_LIMIT;
            }
            if(drive_out < -1.0 * DRIVE_OUT_LIMIT)
            {
                drive_out = -1.0 * DRIVE_OUT_LIMIT;
            }
            // ================================================================

            // ================================================================
            // Set steering and drive motor outputs
            // ================================================================
            glue_set_steering_motor(steer_out);
            glue_set_drive_motor(drive_out);
            // ================================================================

            #ifdef PID_TUNING_DEBUG
                ctrl_print_pid();
            #endif
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
    // ========================================================================

    // ========================================================================
    // Copy the latest telemetry/command data from shared memory to local memory
    // ========================================================================
    comm_transaction(&ctrl_cmd, &ctrl_telem);
    // ========================================================================

    // ========================================================================
    // Fault protection and mode command handler
    // ========================================================================
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
    // ========================================================================

    return 1;
}

/**************************************************************************/
/*!
    @brief  Signal interrupt to kill real-time control thread
*/
/**************************************************************************/
void sigint(int) 
{
    ctrl_run = false;
}

/**************************************************************************/
/*!
    @brief  Print PID data for debug
    @return None
*/
/**************************************************************************/
void ctrl_print_pid(void)
{
    printf("STR_ERR = %.5f, STR_OUT = %.5f, STR_P = %.5f, STR_I = %.5f, STR_D = %.5f, ",\
        steer_error_pos, \
        steer_out, \
        steer_p, \
        steer_i, \
        steer_d);

    printf("DRV_DES = %.5f, DRV_ACT = %.5f, DRV_ERR = %.5f, DRV_OUT = %.5f, DRV_P = %.5f, DRV_I = %.5f, DRV_D = %.5f, DRV_F = %.5f\r\n",\
        drive_desired_vel, \
        drive_actual_vel, \
        drive_error_vel, \
        drive_out, \
        drive_p, \
        drive_i, \
        drive_d, \
        drive_f);
}

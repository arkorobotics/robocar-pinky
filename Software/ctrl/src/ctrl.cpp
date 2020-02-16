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
bool volatile ctrl_run = true;
int64_t ctrl_loop_period = 20000000;	// 20ms -> 50Hz, value in nanoseconds

// Local Ctrl Variables
Ctrl_Cmd ctrl_cmd;
Ctrl_Telem ctrl_telem;

// Shared Ctrl Variables
Ctrl_Cmd *shared_ctrl_cmd;		// CMD shared memory
Ctrl_Telem *shared_ctrl_telem;		// TELEM shared memory

int main(int argc, char *argv[])
{
    printf("=== ROBOCAR - CTRL: START ===\n");

    // Initalize Communication to C&DH
    printf("Initializing comm shared memory...\n");
    comm_init(shared_ctrl_cmd, shared_ctrl_telem);

    // Initialize Glue Board
    printf("Initalizing glue board...\n");
    glue_init();

    // Make sure ctrl-C stops the program under controlled circumstances
    signal(SIGINT, &sigint);

    // Create attributes for an isolated real-time thread
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

    // Stop all motors
    glue_estop();

    // Detach and remove shared memory
    shmdt(&cmd_shmid);
    shmctl(cmd_shmid, IPC_RMID, NULL);
    shmdt(&telem_shmid);
    shmctl(telem_shmid, IPC_RMID, NULL);

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
        int64_t tosleep = ctrl_loop_period - (now.tv_sec - last.tv_sec) * 1000000000 - (now.tv_nsec - last.tv_nsec);

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

        ctrl_loop();
    }

    return NULL;
}

int ctrl_loop(void)
{
    // Incremement timestamp
    ctrl_telem.timestamp += 1;

    // Read and print state
    ctrl_telem.glue_state = glue_state_update();
    glue_print(ctrl_telem.glue_state);

    // TODO: Read latest command steering position and drive velocity
    // Read the Ctrl_Cmd struct directly here by locking the semaphore

    // Delay to simulate number crunching
    // TODO: Soon to be PID controllers
    usleep(500);

    // Set Drive and Steering Motor to 5%
    glue_set_drive_motor(0.1);
    glue_set_steering_motor(0.1);

    // Copy the latest telemetry/command data from shared memory to local memory
    comm_sem_acquire();
    memcpy(shared_ctrl_telem, &ctrl_telem, sizeof(struct Ctrl_Telem));
    memcpy(&ctrl_cmd, shared_ctrl_cmd, sizeof(struct Ctrl_Cmd));
    comm_sem_release();

    return 1;
}

void sigint(int) {
    ctrl_run = false;
}
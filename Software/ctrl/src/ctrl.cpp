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
int cmd_shmid;				// CMD shared memory ID
key_t cmd_key = 1000;                   // CMD shared memory key
int cmd_sem_id;				// CMD semaphore ID
key_t cmd_sem_key = 1001;		// CMD semaphore key

Ctrl_Telem *shared_ctrl_telem;		// TELEM shared memory
int telem_shmid;			// TELEM shared memory ID
key_t telem_key = 2000;                 // TELEM shared memory key
int telem_sem_id;			// TELEM semaphore ID
key_t telem_sem_key = 2001;		// TELEM semaphore key

int main(int argc, char *argv[])
{
    printf("=== ROBOCAR - CTRL: START ===\n");

    printf("Initializing ctrl shared memory...\n");

    // Setup CMD semaphore and shared memory
    if ((cmd_sem_id = semget(cmd_sem_key, 1, IPC_CREAT | 0666)) < 0)
    {
	printf("Error getting cmd semaphore id");
    	exit(1);
    }
    if (semctl(cmd_sem_id, 0, SETVAL, 1) < 0) { printf("CMD SEM UNLOCK FAILED\n"); } // UNLOCK CMD SEM

    if ((cmd_shmid = shmget(cmd_key, sizeof(Ctrl_Cmd), IPC_CREAT | 0666)) < 0)
    {
        printf("Error getting shared memory id");
        exit(1);
    }
    if ((shared_ctrl_cmd = (Ctrl_Cmd *)shmat(cmd_shmid, NULL, 0)) == (Ctrl_Cmd *) -1)
    {
        printf("Error attaching shared memory id");
        exit(1);
    }

    // Setup TELEM semaphore and shared memory
    if ((telem_sem_id = semget(telem_sem_key, 1, IPC_CREAT | 0666)) < 0)
    {
        printf("Error getting telem semaphore id");
        exit(1);
    }
    if (semctl(telem_sem_id, 0, SETVAL, 1) < 0) { printf("TELEM SEM UNLOCK FAILED\n"); } // UNLOCK TELEM SEM

    if ((telem_shmid = shmget(telem_key, sizeof(Ctrl_Telem), IPC_CREAT | 0666)) < 0)
    {
        printf("Error getting shared memory id");
        exit(1);
    }
    if ((shared_ctrl_telem = (Ctrl_Telem *)shmat(telem_shmid, NULL, 0)) == (Ctrl_Telem *) -1)
    {
        printf("Error attaching shared memory id");
        exit(1);
    }

    // Initialize Glue Board
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

void sigint(int) {
    ctrl_run = false;
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

    // Copy the latest telemetry to shared memory
    acquire(telem_sem_id);
    acquire(cmd_sem_id);

    memcpy(shared_ctrl_telem, &ctrl_telem, sizeof(struct Ctrl_Telem));
    memcpy(&ctrl_cmd, shared_ctrl_cmd, sizeof(struct Ctrl_Cmd));

    release(telem_sem_id);
    release(cmd_sem_id);

    return 1;
}

int acquire(int id)
{
    int retval;
    struct sembuf operations[1];
    /* Set up the sembuf structure. */
    operations[0].sem_num = 0;  /* index into semaphore array */
    operations[0].sem_op = -1;
    operations[0].sem_flg = 0;  /* wait if semaphore is <=0 */

    retval = semop(id, operations, 1);

    return retval;
}

int release(int id)
{
    int retval;
    struct sembuf operations[1];
    /* Set up the sembuf structure. */
    operations[0].sem_num = 0;
    operations[0].sem_op = 1;
    operations[0].sem_flg = 0;

    retval = semop(id, operations, 1);

    return retval;
}

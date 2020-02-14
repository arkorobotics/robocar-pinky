#include <pthread.h>
#include <sched.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include <time.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>
#include <inttypes.h>
#include <math.h>
#include <errno.h>

#include "ctrl.h"

extern "C" {
  #include "glue.h"
}

bool volatile running = true;

int64_t ctrl_loop_period = 20000000;	// 20ms -> 50Hz

int main(int argc, char *argv[])
{
    // Initialize Glue Board
    glue_init();

    //  make sure ctrl-C stops the program under controlled circumstances
    signal(SIGINT, &sigint);

    //  create attributes for an isolated real-time thread
    pthread_attr_t attr = {};
    pthread_attr_init(&attr);
    //  lift the thread off core 0, which takes system interrupts
    cpu_set_t cpuset = {};
    CPU_ZERO(&cpuset);
    CPU_SET(1, &cpuset);
    pthread_attr_setaffinity_np(&attr, 1, &cpuset);
    //  make it use FIFO policy for real-time scheduling
    pthread_attr_setschedpolicy(&attr, SCHED_FIFO);
    pthread_attr_setinheritsched(&attr, PTHREAD_EXPLICIT_SCHED);
    //  set the priority
    sched_param param = {};
    param.sched_priority = 30;
    pthread_attr_setschedparam(&attr, &param);

    //  create the real-time thread
    pthread_t mythread;
    int err;
    if ((err = pthread_create(&mythread, &attr, &ctrl_timer_func, NULL)) != 0) {
        char const *emsg = ((err == EAGAIN) ? "EAGAIN" : ((err == EINVAL) ? "EINVAL" : ((err == EPERM) ? "EPERM" : "unknown")));
        fprintf(stderr, "pthread_create() failed (%d %s); are you sure you're root?\n", err, emsg);
        fprintf(stderr, "You may also need to do:\n");
        fprintf(stderr, "echo -1 > /proc/sys/kernel/sched_rt_runtime_us\n");
        exit(1);
    }

    //  wait for the program to be done
    void *ignore = NULL;
    pthread_join(mythread, &ignore);

    // Stop all motors
    glue_estop();

    return 0;
}

void *ctrl_timer_func(void *) {

    //  keep track of the passage of time at a 20-millisecond quantized interval
    struct timespec last = {}, now = {};
    clock_gettime(CLOCK_MONOTONIC_RAW, &last);

    while (running) {
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
    running = false;
}

int ctrl_loop(void)
{
    // Print state
    glue_print(glue_state_update());

    // Delay to simulate number crunching
    usleep(500);

    // Set Drive and Steering Motor to 5%
    glue_set_drive_motor(0.1);
    glue_set_steering_motor(0.1);

    return 1;
}

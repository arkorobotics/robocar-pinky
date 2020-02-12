#include <pthread.h>
#include <sched.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include <time.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <inttypes.h>
#include <math.h>
#include <errno.h>

#include "ctrl.h"
#include "glue.h"

bool volatile running = true;

int ctrl_loop(void)
{
    // Print state
    glue_print(glue_state_update());
    
    usleep(100);

    // Set Drive and Steering Motor to 5%
    glue_set_drive_motor(0.1);
    glue_set_steering_motor(0.1);

    return 1;
}

void ctrl_timer_func(void) 
{

    ctrl_loop();

}

int main(int argc, char **argv) {
    
    // Initialize Glue Board
    glue_init();

    ctrl_timer_func();

    // Stop all motors
    glue_estop();

    return 0;
}
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <inttypes.h>
#include <math.h>

#include "ctrl.h"
#include "glue.h"


int main(int argc, char **argv)
{
    // Initialize Glue Board
    glue_init();

    // Print state
    glue_print(glue_state_update());

    // Set Drive and Steering Motor to 5%
    glue_set_drive_motor(0.1);
    glue_set_steering_motor(0.1);

    // Wait
    usleep(1000000);

    // Print state
    glue_print(glue_state_update());

    // Stop all motors
    glue_estop();

    return 0;
}


#ifndef __CTRL_H__
#define __CTRL_H__

// Control Configuration Parameters
//#define TELEM_DEBUG             1
#define PID_TUNING_DEBUG        1
#define CTRL_LOOP_PERIOD        20000000                   // dt in nanoseconds
#define MAX_MISSED_HEARTBEAT    25                         // Fault if exceeded

#define DT                      (CTRL_LOOP_PERIOD/1000000) // dt in milliseconds              

#define STEER_P_GAIN        1.000
#define STEER_I_GAIN        0.002
#define STEER_D_GAIN        0.700
#define STEER_I_LIMIT       0.500
#define STEER_OUT_LIMIT     0.800
#define STEER_LEFT_MAX_VAL  3.000   // TODO: Calibrate this value!
#define STEER_ZERO_VAL      2.400
#define STEER_RIGHT_MAX_VAL 1.800   // TODO: Calibrate this value!
#define STEER_SCALER        (STEER_RIGHT_MAX_VAL - STEER_LEFT_MAX_VAL)/2

#define DRIVE_P_GAIN        0.000
#define DRIVE_I_GAIN        0.000
#define DRIVE_D_GAIN        0.000
#define DRIVE_F_GAIN        0.000
#define DRIVE_I_LIMIT       0.500
#define DRIVE_OUT_LIMIT     0.500
#define DRIVE_REV_MAX_VAL   0.000   // TODO: Calibrate this value!
#define DRIVE_ZERO_VAL      0.000
#define DRIVE_FWD_MAX_VAL   0.000   // TODO: Calibrate this value!
#define DRIVE_SCALER        (DRIVE_FWD_MAX_VAL - DRIVE_REV_MAX_VAL)/2

extern "C" {
    #include "glue.h"
}

void *ctrl_timer_func(void *);
void sigint(int);
int ctrl_loop(void);
void ctrl_print_pid(void);

#endif // __CTRL_H__

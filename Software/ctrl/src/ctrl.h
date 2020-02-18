#ifndef __CTRL_H__
#define __CTRL_H__

// Control Configuration Parameters
#define DEBUG                   1

#define CTRL_LOOP_PERIOD        20000000                   // dt in nanoseconds
#define MAX_MISSED_HEARTBEAT    25                         // Fault if exceeded

#define DT                      (CTRL_LOOP_PERIOD/1000000) // dt in milliseconds              

#define STEER_P_GAIN        0.000
#define STEER_I_GAIN        0.000
#define STEER_D_GAIN        0.000
#define STEER_I_LIMIT       0.500
#define STEER_OUT_LIMIT     0.500
#define STEER_LEFT_MAX_VAL  0.000   // TODO: Calibrate this value!
#define STEER_ZERO_VAL      2.733
#define STEER_RIGHT_MAX_VAL 5.000   // TODO: Calibrate this value!
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
int acquire(int id);
int release(int id);


#endif // __CTRL_H__

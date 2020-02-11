#ifndef __GLUE_H__
#define __GLUE_H__

#include <stdint.h>
#include <inttypes.h>

#include "i2c.h"

#define VMOTOR_INA260_I2C_ADDRESS   0x4F
#define VLOGIC_INA260_I2C_ADDRESS   0x4A
#define STATE_ADS1115_I2C_ADDRESS   0x48
#define DRIVE_PCA9685_I2C_ADDRESS   0x40

typedef struct
{
  uint32_t vmotor_battery_mv;
  uint32_t vlogic_battery_mv;
  float steering_position;
  float drive_torque;
  float drive_velocity;
}
Glue_State;

int glue_init(void);
Glue_State glue_state_update(void);
int glue_set_drive_motor(float drive);
int glue_set_steering_motor(float drive);
void glue_print(Glue_State glue_state);
int glue_estop(void);
int glue_exit(void);

#endif // __GLUE_H__

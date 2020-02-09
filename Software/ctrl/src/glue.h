#ifndef __GLUE_H__
#define __GLUE_H__

#include <stdint.h>
#include "i2c.h"

#define VMOTOR_INA260_I2C_ADDRESS   0x4F
#define VLOGIC_INA260_I2C_ADDRESS   0x4A
#define STATE_ADS1115_I2C_ADDRESS   0x48
#define DRIVE_PCA9685_I2C_ADDRESS   0x40

int glue_init(void);
int glue_estop(void);
int glue_exit(void);

#endif // __GLUE_H__

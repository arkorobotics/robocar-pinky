#ifndef __CTRL_H__
#define __CTRL_H__

#include <stdint.h>
#include "i2c.h"

typedef struct
{
  I2CDevice i2c,
  uint16_t config
}
INA260_t;

typedef struct
{
  I2CDevice i2c,
  uint16_t config
}
ADS1115_t;

typedef struct
{
  I2CDevice i2c,
  uint16_t config
}
PCA9685_t;

#endif // __RC_CTRL_H__
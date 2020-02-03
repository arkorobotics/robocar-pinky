/*************************************************************************
 * pca9685.h
 *
 * This software is a devLib extension to wiringPi <http://wiringpi.com/>
 * and enables it to control the Adafruit PCA9685 16-Channel 12-bit
 * PWM/Servo Driver <http://www.adafruit.com/products/815> via I2C interface.
 *
 * Copyright (c) 2014 Reinhard Sprung
 *
 * If you have questions or improvements email me at
 * reinhard.sprung[at]gmail.com
 *
 * This software is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published
 * by the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * The given code is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU Lesser General Public License for more details.
 *
 * You can view the contents of the licence at <http://www.gnu.org/licenses/>.
 **************************************************************************
 */
 
#include "i2c.h"

// Setup a pca9685 at the specific i2c address
int pca9685_setup(PCA9685_t *pca9685, float freq = 50);

// Advanced controls
// You can use the file descriptor returned from the setup function to access the following features directly on each connected pca9685
void pca9685_PWM_freq(PCA9685_t *pca9685, float freq);
void pca9685_PWM_reset(PCA9685_t *pca9685);
void pca9685_PWM_write(PCA9685_t *pca9685, int pin, int on, int off);
void pca9685_PWM_read(PCA9685_t *pca9685, int pin, int *on, int *off);

void pca9685_fullOn(PCA9685_t *pca9685, int pin, int tf);
void pca9685_fullOff(PCA9685_t *pca9685, int pin, int tf);

int baseReg(int pin);

static void pca9685_PWM_dc(PCA9685_t *pca9685, uint16_t pin, uint16_t value);
static void pca9685_OnOffWrite(PCA9685_t *pca9685, uint16_t pin, uint16_t value);
static uint16_t pca9685_OffRead(PCA9685_t *pca9685, uint16_t pin);
static uint16_t pca9685_OnRead(PCA9685_t *pca9685, uint16_t pin);
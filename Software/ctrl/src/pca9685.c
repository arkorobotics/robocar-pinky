/*************************************************************************
 * pca9685.c
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
 @modifiedby Arko
 */

#include "pca9685.h"
#include "i2c.h"

// Setup registers
#define PCA9685_MODE1 		0x0
#define PCA9685_PRESCALE 	0xFE

// Define first LED and all LED. We calculate the rest
#define LED0_ON_L 	0x6
#define LEDALL_ON_L 	0xFA

#define PIN_ALL 	16

/**
 * Setup a PCA9685 device with wiringPi.
 *
 * pinBase: 	Use a pinBase > 64, eg. 300
 * i2cAddress:	The default address is 0x40
 * freq:		Frequency will be capped to range [40..1000] Hertz. Try 50 for servos
 */
void pca9685_setup(PCA9685_t *pca9685, float freq)
{
	uint8_t value;
  	i2c_read(&pca9685->i2c, PCA9685_MODE1, &value, 1);

	// Setup the chip. Enable auto-increment of registers.
	uint8_t settings = value & 0x7F;
	uint8_t autoInc = settings | 0x20;

	i2c_write(&pca9685->i2c, PCA9685_MODE1, &autoInc, 1);

	// Set frequency of PWM signals. Also ends sleep mode and starts PWM output.
	if (freq > 0)
	{
		pca9685_PWM_freq(pca9685, freq);
	}
}

/**
 * Sets the frequency of PWM signals.
 * Frequency will be capped to range [40..1000] Hertz. Try 50 for servos.
 */
void pca9685_PWM_freq(PCA9685_t *pca9685, float freq)
{
	// Cap at min and max
	freq = (freq > 1000 ? 1000 : (freq < 40 ? 40 : freq));

	// To set pwm frequency we have to set the prescale register. The formula is:
	// prescale = round(osc_clock / (4096 * frequency))) - 1 where osc_clock = 25 MHz
	// Further info here: http://www.nxp.com/documents/data_sheet/PCA9685.pdf Page 24
	uint8_t prescale = (int)(25000000.0f / (4096 * freq) - 0.5f);

	// Get settings and calc bytes for the different states.
	uint8_t value;
	i2c_read(&pca9685->i2c, PCA9685_MODE1, &value, 1);
	uint8_t settings = value & 0x7F;						// Set restart bit to 0
	uint8_t sleep	 = settings | 0x10;						// Set sleep bit to 1
	uint8_t wake 	 = settings & 0xEF;						// Set sleep bit to 0
	uint8_t restart  = wake | 0x80;							// Set restart bit to 1

	// Go to sleep, set prescale and wake up again.
	i2c_write(&pca9685->i2c, PCA9685_MODE1, &sleep, 1);
	i2c_write(&pca9685->i2c, PCA9685_PRESCALE, &prescale, 1);
	i2c_write(&pca9685->i2c, PCA9685_MODE1, &wake, 1);

	// Now wait a millisecond until oscillator finished stabilizing and restart PWM.
	//delay(1);
	i2c_write(&pca9685->i2c, PCA9685_MODE1, &restart, 1);
}

/**
 * Set all leds back to default values (: fullOff = 1)
 */
void pca9685_PWM_reset(PCA9685_t *pca9685)
{
	uint16_t all_on_h = 0x0000;
	uint8_t all_on_h_buf[2];
    all_on_h_buf[0] = (uint8_t)(all_on_h >> 8);
    all_on_h_buf[1] = (uint8_t)(all_on_h & 0x00FF);

	uint16_t all_on_l = 0x1000;
	uint8_t all_on_l_buf[2];
    all_on_l_buf[0] = (uint8_t)(all_on_l >> 8);
    all_on_l_buf[1] = (uint8_t)(all_on_l & 0x00FF);

	i2c_write(&pca9685->i2c, LEDALL_ON_L, &all_on_h_buf, 2);
	i2c_write(&pca9685->i2c, (LEDALL_ON_L+2), &all_on_l_buf, 2);
}

/**
 * Write on and off ticks manually to a pin
 * (Deactivates any full-on and full-off)
 */
void pca9685_PWM_write(PCA9685_t *pca9685, uint16_t pin, uint16_t on, uint16_t off)
{
	uint16_t reg = baseReg(pin);

	uint16_t on_val = (on & 0x0FFF);
	uint8_t on_val_buf[2];
    on_val_buf[0] = (uint8_t)(on_val >> 8);
    on_val_buf[1] = (uint8_t)(on_val & 0x00FF);

	uint16_t off_val = (off & 0x0FFF);
	uint8_t off_val_buf[2];
    off_val_buf[0] = (uint8_t)(off_val >> 8);
    off_val_buf[1] = (uint8_t)(off_val & 0x00FF);

	// Write to on and off registers and mask the 12 lowest bits of data to overwrite full-on and off
	i2c_write(&pca9685->i2c, reg, &on_val_buf, 2);
	i2c_write(&pca9685->i2c, (reg+2), &off_val_buf, 2);
}

/**
 * Reads both on and off registers as 16 bit of data
 * To get PWM: mask each value with 0xFFF
 * To get full-on or off bit: mask with 0x1000
 * Note: ALL_LED pin will always return 0
 */
void pca9685_PWM_read(PCA9685_t *pca9685, uint16_t pin, uint16_t *on, uint16_t *off)
{
	uint16_t reg = baseReg(pin);
	uint8_t buf[2];

	if (on)
	{
		i2c_read(&pca9685->i2c, reg, &buf, 2);
		*on = (((uint16_t)buf[0])<<8) | (uint16_t)buf[1];
	}
	if (off)
	{
		i2c_read(&pca9685->i2c, (reg + 2), &buf, 2);
		*off = (((uint16_t)buf[0])<<8) | (uint16_t)buf[1];
	}
	
}

/**
 * Enables or deactivates full-on
 * tf = true: full-on
 * tf = false: according to PWM
 */
void pca9685_fullOn(PCA9685_t *pca9685, uint16_t pin, uint16_t tf)
{
	uint16_t reg = baseReg(pin) + 1;		// LEDX_ON_H

	uint16_t state;

	uint8_t read_buf[2];
	i2c_read(&pca9685->i2c, reg, &read_buf, 1);
	state = (((uint16_t)read_buf[0])<<8) | (uint16_t)read_buf[1];

	// Set bit 4 to 1 or 0 accordingly
	state = tf ? (state | 0x10) : (state & 0xEF);

	uint8_t write_buf[2];
    write_buf[0] = (uint8_t)(state >> 8);
    write_buf[1] = (uint8_t)(state & 0x00FF);
	i2c_write(&pca9685->i2c, reg, &write_buf, 1);

	// For simplicity, we set full-off to 0 because it has priority over full-on
	if (tf)
	{
		pca9685_fullOff(pca9685, pin, 0);
	}
}

/**
 * Enables or deactivates full-off
 * tf = true: full-off
 * tf = false: according to PWM or full-on
 */
void pca9685_fullOff(PCA9685_t *pca9685, uint16_t pin, uint16_t tf)
{
	uint16_t reg = baseReg(pin) + 3;		// LEDX_OFF_H
	uint16_t state;

	uint8_t read_buf[2];
	i2c_read(&pca9685->i2c, reg, &read_buf, 1);
	state = (((uint16_t)read_buf[0])<<8) | (uint16_t)read_buf[1];

	// Set bit 4 to 1 or 0 accordingly
	state = tf ? (state | 0x10) : (state & 0xEF);

	uint8_t write_buf[2];
    write_buf[0] = (uint8_t)(state >> 8);
    write_buf[1] = (uint8_t)(state & 0x00FF);
	i2c_write(&pca9685->i2c, reg, &write_buf, 1);
}

/**
 * Helper function to get to register
 */
uint16_t baseReg(uint16_t pin)
{
	return (pin >= PIN_ALL ? LEDALL_ON_L : LED0_ON_L + 4 * pin);
}

/**
 * Simple PWM control which sets on-tick to 0 and off-tick to value.
 * If value is <= 0, full-off will be enabled
 * If value is >= 4096, full-on will be enabled
 * Every value in between enables PWM output
 */
void pca9685_PWM_dc(PCA9685_t *pca9685, uint16_t pin, uint16_t value)
{
	if (value >= 4096)
		pca9685_fullOn(pca9685, pin, 1);
	else if (value > 0)
		pca9685_PWM_write(pca9685, pin, 0, value);	// (Deactivates full-on and off by itself)
	else
		pca9685_fullOff(pca9685, pin, 1);
}

/**
 * Simple full-on and full-off control
 * If value is 0, full-off will be enabled
 * If value is not 0, full-on will be enabled
 */
void pca9685_OnOffWrite(PCA9685_t *pca9685, uint16_t pin, uint16_t value)
{
	if (value)
		pca9685_fullOn(pca9685, pin, 1);
	else
		pca9685_fullOff(pca9685, pin, 1);
}

/**
 * Reads off registers as 16 bit of data
 * To get PWM: mask with 0xFFF
 * To get full-off bit: mask with 0x1000
 * Note: ALL_LED pin will always return 0
 */
uint16_t pca9685_OffRead(PCA9685_t *pca9685, uint16_t pin)
{
	uint16_t off;
	pca9685_PWM_read(pca9685, pin, 0, &off);

	return off;
}

/**
 * Reads on registers as 16 bit of data
 * To get PWM: mask with 0xFFF
 * To get full-on bit: mask with 0x1000
 * Note: ALL_LED pin will always return 0
 */
uint16_t pca9685_OnRead(PCA9685_t *pca9685, uint16_t pin)
{
	uint16_t on;
	pca9685_PWM_read(pca9685, pin, &on, 0);

	return on;
}

/**************************************************************************/
/*!
    @file     ADS1115.c
    @author   CPV (Elektor)
    @modifiedby Arko
    @license  BSD (see license.txt)
    Driver for the ADS101x/ADS111x Sigma-Delta Analog/Digital Converters.
    This started out as a port of the Adafruit library, but ended
    up largely rewritten.
    v1.0 - First release
*/
/**************************************************************************/


#include "ads1115.h"
#include "i2c.h"

/**************************************************************************/
/*!
    @brief  Writes 16 bits to the specified destination register.
*/
/**************************************************************************/
void ads1115_write_register(ADS1115_t *adc1115, uint8_t reg, uint16_t value)
{
  uint8_t buf[2];
  buf[0] = (uint8_t)(value >> 8);
  buf[1] = (uint8_t)(value & 0x00FF);
  i2c_write(&adc1115->i2c, reg, &buf, 2);
}

/**************************************************************************/
/*!
    @brief  Read 16 bits from the specified destination register.
*/
/**************************************************************************/
uint16_t ads1115_read_register(ADS1115_t *adc1115, uint8_t reg)
{
  uint8_t buf[2];
  i2c_read(&adc1115->i2c, reg, &buf, 2);
  return (((uint16_t)buf[0])<<8) | (uint16_t)buf[1];
}

/**************************************************************************/
/*!
    @brief  Initialise a user-provided ADS1X15 configuration structure.
            The user must provide a valid pointer to an empty
            ADS1115_config_t structure.
*/
/**************************************************************************/
void ads1115_init(ADS1115_t *adc1115, ADS1115_mux_t input, ADS1115_pga_t gain)
{
  ads1115_set_os(adc1115,OS_SINGLE);
  ads1115_set_multiplexer(adc1115,input);
  ads1115_set_pga(adc1115,gain);
  ads1115_set_mode(adc1115,MODE_SINGLE_SHOT);
  ads1115_set_data_rate(adc1115,DATA_RATE_ADS111x_128);
  ads1115_set_comparator_mode(adc1115,COMPARATOR_MODE_TRADITIONAL);
  ads1115_set_comparator_polarity(adc1115,COMPARATOR_POLARITY_ACTIVE_LO);
  ads1115_set_comparator_latching(adc1115,COMPARATOR_NON_LATCHING);
  ads1115_set_comparator_queue(adc1115,COMPARATOR_QUEUE_NONE);
}


/**************************************************************************/
/*!
    @brief  Start an ADC conversion cycle.
            The user must provide a valid pointer to a
            correctly filled ADS1115_config_t structure.
*/
/**************************************************************************/
void ads1115_start_conversion(ADS1115_t *adc1115)
{
  // Write configuration to the ADC.
  ads1115_write_register(adc1115, ADS1115_REG_POINTER_CONFIG, adc1115->config);
}


/**************************************************************************/
/*!
    @brief  Read the ADC conversion result.
            The user must provide a valid pointer to a
            correctly filled ADS1115_config_t structure.
*/
/**************************************************************************/
int16_t ads1115_read(ADS1115_t *adc1115)
{
  // Read the conversion result.
  int16_t result = (int16_t)ads1115_read_register(adc1115,ADS1115_REG_POINTER_CONVERSION);
  return result;
}


void ads1115_set_threshold_lo(ADS1115_t *adc1115, uint16_t value)
{
  ads1115_write_register(adc1115,ADS1115_REG_POINTER_LO_THRESH,value);
}


void ads1115_set_threshold_hi(ADS1115_t *adc1115, uint16_t value)
{
  ads1115_write_register(adc1115,ADS1115_REG_POINTER_HI_THRESH,value);
}


void ads1115_set_config_bitfield(ADS1115_t *adc1115, uint16_t value, uint16_t mask)
{
  adc1115->config &= ~mask;
  adc1115->config |= (value & mask);
}


void ads1115_set_os(ADS1115_t *adc1115, ADS1115_os_t value)
{
  ads1115_set_config_bitfield(adc1115,(uint16_t)value,ADS1115_REG_CONFIG_OS_MASK);
}


void ads1115_set_multiplexer(ADS1115_t *adc1115, ADS1115_mux_t value)
{
  ads1115_set_config_bitfield(adc1115,(uint16_t)value,ADS1115_REG_CONFIG_MULTIPLEXER_MASK);
}


void ads1115_set_pga(ADS1115_t *adc1115, ADS1115_pga_t value)
{
  ads1115_set_config_bitfield(adc1115,(uint16_t)value,ADS1115_REG_CONFIG_PGA_MASK);
}


void ads1115_set_mode(ADS1115_t *adc1115, ADS1115_mode_t value)
{
  ads1115_set_config_bitfield(adc1115,(uint16_t)value,ADS1115_REG_CONFIG_MODE_MASK);
}


void ads1115_set_data_rate(ADS1115_t *adc1115, ADS1115_data_rate_t value)
{
  ads1115_set_config_bitfield(adc1115,(uint16_t)value,ADS1115_REG_CONFIG_DATA_RATE_MASK);
}


void ads1115_set_comparator_mode(ADS1115_t *adc1115, ADS1115_comparator_mode_t value)
{
  ads1115_set_config_bitfield(adc1115,(uint16_t)value,ADS1115_REG_CONFIG_COMPARATOR_MODE_MASK);
}


void ads1115_set_comparator_polarity(ADS1115_t *adc1115, ADS1115_comparator_polarity_t value)
{
  ads1115_set_config_bitfield(adc1115,(uint16_t)value,ADS1115_REG_CONFIG_COMPARATOR_POLARITY_MASK);
}


void ads1115_set_comparator_latching(ADS1115_t *adc1115, ADS1115_comparator_latching_t value)
{
  ads1115_set_config_bitfield(adc1115,(uint16_t)value,ADS1115_REG_CONFIG_COMPARATOR_LATCHING_MASK);
}


void ads1115_set_comparator_queue(ADS1115_t *adc1115, ADS1115_comparator_queue_t value)
{
  ads1115_set_config_bitfield(adc1115,(uint16_t)value,ADS1115_REG_CONFIG_COMPARATOR_QUEUE_MASK);
}

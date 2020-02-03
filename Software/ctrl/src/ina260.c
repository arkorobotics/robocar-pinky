#include "ina260.h"
#include "i2c.h"

/**************************************************************************/
/*!
    @file     Thanos_INA260.cpp
    @author   Thanos Kontogiannis
    @modifiedby Arko
        @license  BSD (see license.txt)
        Driver for the INA260 current sensor
        This is a library for the INA260
        @section  HISTORY
    v1.0 - First release
*/
/**************************************************************************/

/**************************************************************************/
/*!
    @brief  Configures to INA260
*/
/**************************************************************************/
static void ina260_setConfigRegister(INA260_t *ina260)
{
    i2c_write(&ina260->i2c, INA260_REG_CONFIG, &ina260->config, 2);
}

/**************************************************************************/
/*!
    @brief  Setups the HW
*/
/**************************************************************************/
void ina260_init(INA260_t *ina260)
{
    ina260_setConfigRegister(ina260);
}

/**************************************************************************/
/*!
    @brief  Gets the raw bus voltage (16-bit signed integer, so +-32767)
*/
/**************************************************************************/
static int16_t ina260_getBusVoltage_raw(INA260_t *ina260)
{
    uint16_t value;
    i2c_read(&ina260->i2c, INA260_REG_BUSVOLTAGE, &value, 2);
    return (int16_t)value;
}

/**************************************************************************/
/*!
    @brief  Gets the raw current value (16-bit signed integer, so +-32767)
*/
/**************************************************************************/
static int16_t ina260_getCurrent_raw(INA260_t *ina260)
{
    uint16_t value;
    i2c_read(&ina260->i2c, INA260_REG_CURRENT, &value, 2);
    return (int16_t)value;
}

/**************************************************************************/
/*!
    @brief  Gets the raw power value (16-bit signed integer, so +-32767)
*/
/**************************************************************************/
static int16_t ina260_getPower_raw(INA260_t *ina260)
{
    uint16_t value;
    i2c_read(&ina260->i2c, INA260_REG_POWER, &value, 2);
    return (int16_t)value;
}

/**************************************************************************/
/*!
    @brief  Gets the shunt voltage in volts
*/
/**************************************************************************/
uint32_t ina260_getBusVoltage_mV(INA260_t *ina260)
{
    uint32_t value = ina260_getBusVoltage_raw(ina260);
    return (value * 125) / 100;
}

/**************************************************************************/
/*!
    @brief  Gets the current value in mA
*/
/**************************************************************************/
uint32_t ina260_getCurrent_mA(INA260_t *ina260)
{
    uint32_t valueDec = ina260_getCurrent_raw(ina260);
    return (valueDec * 125) / 100;
}

/**************************************************************************/
/*!
    @brief  Gets the power value in mW
*/
/**************************************************************************/
uint32_t ina260_getPower_mW(INA260_t *ina260)
{
    uint32_t valueDec = ina260_getPower_raw(ina260);
    return valueDec * 10;
}

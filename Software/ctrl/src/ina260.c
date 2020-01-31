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
static void ina260_setConfigRegister(I2CDevice *dev)
{
    // Sets 4 samples average and sampling time for voltage and current to 8.244ms

    // Set Config register
    uint16_t config =
        INA260_CONFIG_AVGRANGE_4 | INA260_CONFIG_BVOLTAGETIME_8244US | INA260_CONFIG_SCURRENTTIME_8244US | INA260_CONFIG_MODE_SANDBVOLT_CONTINUOUS;
    
    i2c_write(dev, INA260_REG_CONFIG, &config, 2);

}

/**************************************************************************/
/*!
    @brief  Setups the HW
*/
/**************************************************************************/
void ina260_init(I2CDevice *dev)
{
    ina260_setConfigRegister(dev);
}

/**************************************************************************/
/*!
    @brief  Gets the raw bus voltage (16-bit signed integer, so +-32767)
*/
/**************************************************************************/
static int16_t ina260_getBusVoltage_raw(I2CDevice *dev)
{
    uint16_t value;
    i2c_read(dev, INA260_REG_BUSVOLTAGE, &value, 2);
    return (int16_t)value;
}

/**************************************************************************/
/*!
    @brief  Gets the raw current value (16-bit signed integer, so +-32767)
*/
/**************************************************************************/
static int16_t ina260_getCurrent_raw(I2CDevice *dev)
{
    uint16_t value;
    i2c_read(dev, INA260_REG_CURRENT, &value, 2);
    return (int16_t)value;
}

/**************************************************************************/
/*!
    @brief  Gets the raw power value (16-bit signed integer, so +-32767)
*/
/**************************************************************************/
static int16_t ina260_getPower_raw(I2CDevice *dev)
{
    uint16_t value;
    i2c_read(dev, INA260_REG_POWER, &value, 2);
    return (int16_t)value;
}

/**************************************************************************/
/*!
    @brief  Gets the shunt voltage in volts
*/
/**************************************************************************/
uint32_t ina260_getBusVoltage_mV(I2CDevice *dev)
{
    uint32_t value = ina260_getBusVoltage_raw(dev);
    return (value * 125) / 100;
}

/**************************************************************************/
/*!
    @brief  Gets the current value in mA
*/
/**************************************************************************/
uint32_t ina260_getCurrent_mA(I2CDevice *dev)
{
    uint32_t valueDec = ina260_getCurrent_raw(dev);
    return (valueDec * 125) / 100;
}

/**************************************************************************/
/*!
    @brief  Gets the power value in mW
*/
/**************************************************************************/
uint32_t ina260_getPower_mW(I2CDevice *dev)
{
    uint32_t valueDec = ina260_getPower_raw(dev);
    return valueDec * 10;
}

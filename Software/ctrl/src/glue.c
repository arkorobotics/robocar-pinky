/**************************************************************************/
/*!
    @file     glue.c
    @author   Arko
        @license  BSD (see license.txt)
        Driver for the INA260 current sensor
        This is a library for the INA260
        @section  HISTORY
    v1.0 - First release
*/
/**************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <inttypes.h>
#include <math.h>

#include "glue.h"
#include "i2c.h"
#include "ina260.h"
#include "ads1115.h"
#include "pca9685.h"

int bus;
char bus_name[32];
unsigned int iaddr_bytes = 1;
unsigned int page_bytes = 8;
unsigned int bus_num = 1;

INA260_t vlogic_adc;
INA260_t vmotor_adc;
ADS1115_t veh_state_adc;
PCA9685_t drive_output;

Glue_State glue_state;

/**************************************************************************/
/*!
    @brief  Setup and initialize the Glue Board via I2C.
            Configure, read and write to the ADC and PWM devices
            to test base functionality.
*/
/**************************************************************************/
int glue_init(void)
{
    // Open I2C bus
    // --------------------------------------------------------------------
    if (snprintf(bus_name, sizeof(bus_name), "/dev/i2c-%u", bus_num) < 0) {

        fprintf(stderr, "Format i2c bus name error!\n");
        exit(-3);
    }

    if ((bus = i2c_open(bus_name)) == -1) {

        fprintf(stderr, "Open i2c bus:%s error!\n", bus_name);
        exit(-3);
    }

    printf("Opened I2C Bus: %s\n", bus_name);
    // --------------------------------------------------------------------

    // Initialize I2C Devices
    // --------------------------------------------------------------------
    
    // Initialize VLOGIC ADC
    i2c_init_device(&vlogic_adc.i2c);
    vlogic_adc.i2c.bus = bus;
    vlogic_adc.i2c.addr = VLOGIC_INA260_I2C_ADDRESS;
    vlogic_adc.i2c.page_bytes = page_bytes;
    vlogic_adc.i2c.iaddr_bytes = iaddr_bytes;
    // Sets 4 samples average and sampling time to 140us
    vlogic_adc.config = INA260_CONFIG_AVGRANGE_4 |\
                        INA260_CONFIG_BVOLTAGETIME_140US |\
                        INA260_CONFIG_SCURRENTTIME_140US |\
                        INA260_CONFIG_MODE_SANDBVOLT_CONTINUOUS;
    ina260_init(&vlogic_adc);


    // Initialize VMOTOR ADC
    i2c_init_device(&vmotor_adc.i2c);
    vmotor_adc.i2c.bus = bus;
    vmotor_adc.i2c.addr = VMOTOR_INA260_I2C_ADDRESS;
    vmotor_adc.i2c.page_bytes = page_bytes;
    vmotor_adc.i2c.iaddr_bytes = iaddr_bytes;
    // Sets 4 samples average and sampling time for voltage and current to 140us
    vmotor_adc.config = INA260_CONFIG_AVGRANGE_4 |\
                        INA260_CONFIG_BVOLTAGETIME_140US |\
                        INA260_CONFIG_SCURRENTTIME_140US |\
                        INA260_CONFIG_MODE_SANDBVOLT_CONTINUOUS;
    ina260_init(&vmotor_adc);


    // Initialize VEHICLE STATE ADC
    i2c_init_device(&veh_state_adc.i2c);
    veh_state_adc.i2c.bus = bus;
    veh_state_adc.i2c.addr = STATE_ADS1115_I2C_ADDRESS;
    veh_state_adc.i2c.page_bytes = page_bytes;
    veh_state_adc.i2c.iaddr_bytes = iaddr_bytes;
    // Configure the ADC to Mux Channel 0 and +/-4.096V range = Gain 1
    ads1115_init(&veh_state_adc,MUX_SINGLE_0,PGA_4096);


    // Initialize PWM DRIVER
    i2c_init_device(&drive_output.i2c);
    drive_output.i2c.bus = bus;
    drive_output.i2c.addr = DRIVE_PCA9685_I2C_ADDRESS;
    drive_output.i2c.page_bytes = page_bytes;
    drive_output.i2c.iaddr_bytes = iaddr_bytes;
    // Configure PWM driver to 50Hz
    pca9685_setup(&drive_output, 50);
    // --------------------------------------------------------------------

    // Test devices by reading ADC and setting PWM values
    // --------------------------------------------------------------------
    uint32_t logic_bus_voltage = ina260_getBusVoltage_mV(&vlogic_adc);
    printf("VLOGIC = %i\r\n", logic_bus_voltage);

    uint32_t motor_bus_voltage = ina260_getBusVoltage_mV(&vmotor_adc);
    printf("VMOTOR = %i\r\n", motor_bus_voltage);

    ads1115_set_multiplexer(&veh_state_adc, MUX_SINGLE_0);
    ads1115_start_conversion(&veh_state_adc);
    usleep(1600);   // Wait for conversion
    printf("STATE_ADC_0 = %f\r\n", (float)ads1115_read(&veh_state_adc)*4.096/32767.0);

    ads1115_set_multiplexer(&veh_state_adc, MUX_SINGLE_1);
    ads1115_start_conversion(&veh_state_adc);
    usleep(1600);   // Wait for conversion
    printf("STATE_ADC_1 = %f\r\n", (float)ads1115_read(&veh_state_adc)*4.096/32767.0);

    ads1115_set_multiplexer(&veh_state_adc, MUX_SINGLE_3);
    ads1115_start_conversion(&veh_state_adc);
    usleep(1600);   // Wait for conversion
    printf("STATE_ADC_2_3_DIFF = %f\r\n", (float)ads1115_read(&veh_state_adc)*4.096/32767.0);

    // Drive Motor
    //
    //      Channels 0-2 of the PCA9685 are connected to a VNH5019ATR-E
    //      PCA9685 CH 0 => VNH5019ATR-E INA
    //      PCA9685 CH 1 => VNH5019ATR-E INB
    //      PCA9685 CH 2 => VNH5019ATR-E DRIVE PWM
    //
    //      PCA9685 Truth Table
    //      INA     INB     OPERATING MODE
    //       1       1      Brake to VCC
    //       1       0      TBD?Forward / Clockwise (CW)
    //       0       1      TBD?Backward / Counterclockwise (CCW)
    //       0       0      Brake to GND
    //
    pca9685_PWM_dc(&drive_output, 0, 0);	// INA          
    pca9685_PWM_dc(&drive_output, 1, 0);	// INB          
    pca9685_PWM_dc(&drive_output, 2, 0);	// DRIVE PWM    (0 = 0%, 4096 = 100%)

    // Steering Motor
    //
    //      Channels 3-4 of the PCA9685 are connected to a DRV8871
    //      DRV8871 CH 3 => DRV8871 IN1
    //      DRV8871 CH 4 => DRV8871 IN2
    //
    //      DRV8871 Truth Table
    //      IN1     IN2     OPERATING MODE
    //       1       1      Coast; H-bridge disabled to High-Z
    //       1       0      TBD?Reverse/Left (Current OUT2 → OUT1)
    //       0       1      TBD?Forward/Right (Current OUT1 → OUT2)
    //       0       0      Brake; low-side slow decay
    //
    pca9685_PWM_dc(&drive_output, 3, 0);     // IN1
    pca9685_PWM_dc(&drive_output, 4, 0);     // IN2

    printf("PWM Driver Configured. Drive and steering motor set to brake.\r\n");
    // --------------------------------------------------------------------

    return 1;
}

/**************************************************************************/
/*!
    @brief  Update the glue board state
    @return Glue State
*/
/**************************************************************************/
Glue_State glue_state_update(void)
{
    glue_state.vmotor_battery_mv = ina260_getBusVoltage_mV(&vmotor_adc);
    glue_state.vlogic_battery_mv = ina260_getBusVoltage_mV(&vlogic_adc);

    ads1115_set_multiplexer(&veh_state_adc, MUX_SINGLE_0);
    ads1115_start_conversion(&veh_state_adc);
    usleep(2000);   // Wait for conversion
    glue_state.steering_position = (float)ads1115_read(&veh_state_adc)*4.096/32767.0;

    ads1115_set_multiplexer(&veh_state_adc, MUX_SINGLE_1);
    ads1115_start_conversion(&veh_state_adc);
    usleep(2000);   // Wait for conversion
    glue_state.drive_torque = (float)ads1115_read(&veh_state_adc)*4.096/32767.0;

    ads1115_set_multiplexer(&veh_state_adc, MUX_SINGLE_3);
    ads1115_start_conversion(&veh_state_adc);
    usleep(2000);   // Wait for conversion
    glue_state.drive_velocity = (float)ads1115_read(&veh_state_adc)*4.096/32767.0;

    return glue_state;
}

/**************************************************************************/
/*!
    @brief  Sets drive motor magnitude
    @param  drive   Input drive magnitude. Ranges from -1.0 to 1.0
    @return Error code
*/
/**************************************************************************/
int glue_set_drive_motor(float drive)
{
    // Drive Motor
    //
    //      Channels 0-2 of the PCA9685 are connected to a VNH5019ATR-E
    //      PCA9685 CH 0 => VNH5019ATR-E INA
    //      PCA9685 CH 1 => VNH5019ATR-E INB
    //      PCA9685 CH 2 => VNH5019ATR-E DRIVE PWM
    //
    //      PCA9685 Truth Table
    //      INA     INB     OPERATING MODE
    //       1       1      Brake to VCC
    //       1       0      TBD?Forward / Clockwise (CW)
    //       0       1      TBD?Backward / Counterclockwise (CCW)
    //       0       0      Brake to GND
    //

    uint16_t ina = 0;
    uint16_t inb = 0;
    uint16_t duty_cycle = 0;

    // Sanitize drive value input and calculate duty cycle value
    if (drive > 1.000 || drive < -1.000)
    {
        duty_cycle = 4096;      // Saturated full speed (both FWD/REV)
    }
    else
    {
        duty_cycle = (uint16_t)((float)fabs(drive)*4096.0); 
    }

    // Set motor control direction
    if(drive > 0.000)           // Forward
    { 
        ina = 4096; 
        inb = 0; 
    }
    else if (drive < 0.000)     // Reverse
    { 
        ina = 0; 
        inb = 4096; 
    }
    else                        // Brake
    {
        ina = 0;
        inb = 0;
    }

    // Update output
    pca9685_PWM_dc(&drive_output, 0, ina);	        // INA          
    pca9685_PWM_dc(&drive_output, 1, inb);	        // INB          
    pca9685_PWM_dc(&drive_output, 2, duty_cycle);	// DRIVE PWM    (0 = 0%, 4096 = 100%)

    return 1;
}

/**************************************************************************/
/*!
    @brief  Sets steering motor magnitude
    @param  drive   Input drive magnitude. Ranges from -1.0 to 1.0
    @return Error code
*/
/**************************************************************************/
int glue_set_steering_motor(float drive)
{
    // Steering Motor
    //
    //      Channels 3-4 of the PCA9685 are connected to a DRV8871
    //      DRV8871 CH 3 => DRV8871 IN1
    //      DRV8871 CH 4 => DRV8871 IN2
    //
    //      DRV8871 Truth Table
    //      IN1     IN2     OPERATING MODE
    //       1       1      Coast; H-bridge disabled to High-Z
    //       1       0      TBD?Reverse/Left (Current OUT2 → OUT1)
    //       0       1      TBD?Forward/Right (Current OUT1 → OUT2)
    //       0       0      Brake; low-side slow decay
    //

    uint16_t in1 = 0;
    uint16_t in2 = 0;
    uint16_t duty_cycle = 0;

    // Sanitize drive value input and calculate duty cycle value
    if (drive > 1.000 || drive < -1.000)
    {
        duty_cycle = 4096;      // Saturated full speed (both FWD/REV)
    }
    else
    {
        duty_cycle = (uint16_t)((float)fabs(drive)*4096.0); 
    }

    // Set motor control direction
    if(drive > 0.000)           // Forward
    { 
        in1 = duty_cycle;
        in2 = 0;
    }
    else if (drive < 0.000)     // Reverse
    { 
        in1 = 0; 
        in2 = duty_cycle;
    }
    else                        // Brake
    {
        in1 = 0;
        in2 = 0;
    }

    // Update output
    pca9685_PWM_dc(&drive_output, 3, in1);     // IN1
    pca9685_PWM_dc(&drive_output, 4, in2);     // IN2

    return 1;
}

void glue_print(Glue_State glue_state)
{
    printf("VMOTOR = %i, VLOGIC = %i, STRPOS = %.5f, DRVTRQ = %.5f, DRVVEL = %.5f\r\n",\
            glue_state.vmotor_battery_mv,\
            glue_state.vlogic_battery_mv,\
            glue_state.steering_position,\
            glue_state.drive_torque,\
            glue_state.drive_velocity);
}

/**************************************************************************/
/*!
    @brief  Emergency Stop Function
*/
/**************************************************************************/
int glue_estop(void)
{
    // Drive Motor
    //
    // INA = 0, INB = 0  -  Brake to GND
    // DRIVE PWM = 0     -  0% Drive PWM duty cycle
    //
    pca9685_PWM_dc(&drive_output, 0, 0);	// INA          
    pca9685_PWM_dc(&drive_output, 1, 0);	// INB          
    pca9685_PWM_dc(&drive_output, 2, 0);	// DRIVE PWM

    // Steering Motor
    //
    // Halt steering motor. Steering position is ignored under e-stop.
    // IN1 = 0, IN2 = 0     Brake; low-side slow decay
    //
    pca9685_PWM_dc(&drive_output, 3, 0);     // IN1
    pca9685_PWM_dc(&drive_output, 4, 0);     // IN2

    return 1;
}



/**************************************************************************/
/*!
    @brief  Close I2C Bus
*/
/**************************************************************************/
int glue_exit(void)
{
    i2c_close(bus);

    return 1;
}

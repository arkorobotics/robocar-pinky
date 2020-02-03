#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <inttypes.h>
#include "ctrl.h"
#include "i2c.h"
#include "ina260.h"
#include "ads1115.h"
#include "pca9685.h"

#define VMOTOR_INA260_I2C_ADDRESS   0x4F
#define VLOGIC_INA260_I2C_ADDRESS   0x4A
#define STATE_ADS1115_I2C_ADDRESS   0x48
#define DRIVE_PCA9685_I2C_ADDRESS   0x40

int main(int argc, char **argv)
{
    unsigned int iaddr_bytes = 1, page_bytes = 8, bus_num = 1;

    /* Open i2c bus */
    int bus;
    char bus_name[32];
    memset(bus_name, 0, sizeof(bus_name));

    if (snprintf(bus_name, sizeof(bus_name), "/dev/i2c-%u", bus_num) < 0) {

        fprintf(stderr, "Format i2c bus name error!\n");
        exit(-3);
    }

    if ((bus = i2c_open(bus_name)) == -1) {

        fprintf(stderr, "Open i2c bus:%s error!\n", bus_name);
        exit(-3);
    }

    /* Init i2c devices */

    // Init VLOGIC ADC
    INA260_t vlogic_adc;
    memset(&vlogic_adc, 0, sizeof(vlogic_adc));
    i2c_init_device(&vlogic_adc.i2c);

    vlogic_adc.i2c.bus = bus;
    vlogic_adc.i2c.addr = VLOGIC_INA260_I2C_ADDRESS;
    vlogic_adc.i2c.page_bytes = page_bytes;
    vlogic_adc.i2c.iaddr_bytes = iaddr_bytes;
    // Sets 4 samples average and sampling time for voltage and current to 8.244ms
    vlogic_adc.config = INA260_CONFIG_AVGRANGE_4 | INA260_CONFIG_BVOLTAGETIME_8244US | INA260_CONFIG_SCURRENTTIME_8244US | INA260_CONFIG_MODE_SANDBVOLT_CONTINUOUS;

    ina260_init(&vlogic_adc);

    uint32_t logic_bus_voltage = ina260_getBusVoltage_mV(&vlogic_adc);
    printf("VLOGIC = %i\r\n", logic_bus_voltage);

    // Init VMOTOR ADC
    INA260_t vmotor_adc;
    memset(&vmotor_adc, 0, sizeof(vmotor_adc));
    i2c_init_device(&vmotor_adc.i2c);

    vmotor_adc.i2c.bus = bus;
    vmotor_adc.i2c.addr = VMOTOR_INA260_I2C_ADDRESS;
    vmotor_adc.i2c.page_bytes = page_bytes;
    vmotor_adc.i2c.iaddr_bytes = iaddr_bytes;
    // Sets 4 samples average and sampling time for voltage and current to 8.244ms
    vmotor_adc.config = INA260_CONFIG_AVGRANGE_4 | INA260_CONFIG_BVOLTAGETIME_8244US | INA260_CONFIG_SCURRENTTIME_8244US | INA260_CONFIG_MODE_SANDBVOLT_CONTINUOUS;

    ina260_init(&vmotor_adc);

    uint32_t motor_bus_voltage = ina260_getBusVoltage_mV(&vmotor_adc);
    printf("VMOTOR = %i\r\n", motor_bus_voltage);

    // Init STATE ADC
    ADS1115_t state_adc;
    memset(&state_adc, 0, sizeof(state_adc));
    i2c_init_device(&state_adc.i2c);

    state_adc.i2c.bus = bus;
    state_adc.i2c.addr = STATE_ADS1115_I2C_ADDRESS;
    state_adc.i2c.page_bytes = page_bytes;
    state_adc.i2c.iaddr_bytes = iaddr_bytes;

    ads1115_init(&state_adc,MUX_SINGLE_0,PGA_4096);

    ads1115_set_multiplexer(&state_adc, MUX_SINGLE_0);
    ads1115_start_conversion(&state_adc);
    printf("STATE_ADC_0 = %f\r\n", (float)ads1115_read(&state_adc)*4.096/32767.0);

    ads1115_set_multiplexer(&state_adc, MUX_SINGLE_1);
    ads1115_start_conversion(&state_adc);
    printf("STATE_ADC_1 = %f\r\n", (float)ads1115_read(&state_adc)*4.096/32767.0);

    ads1115_set_multiplexer(&state_adc, MUX_DIFF_2_3);
    ads1115_start_conversion(&state_adc);
    printf("STATE_ADC_2_3_DIFF = %f\r\n", (float)ads1115_read(&state_adc)*4.096/32767.0);

    // Init Drive Outputs
    PCA9685_t drive_output;
    memset(&drive_output, 0, sizeof(drive_output));
    i2c_init_device(&drive_output.i2c);

    drive_output.i2c.bus = bus;
    drive_output.i2c.addr = DRIVE_PCA9685_I2C_ADDRESS;
    drive_output.i2c.page_bytes = page_bytes;
    drive_output.i2c.iaddr_bytes = iaddr_bytes;

    pca9685_setup(&drive_output, 50);

    // Drive Motor
    pca9685_PWM_dc(&drive_output, 0, 4096);	// INA
    pca9685_PWM_dc(&drive_output, 1, 0);	// INB
    pca9685_PWM_dc(&drive_output, 2, 2048);	// DRIVE PWM

    // Steering Motor
    pca9685_PWM_dc(&drive_output, 3, 2048);     // IN1
    pca9685_PWM_dc(&drive_output, 4, 0);     // IN2

    printf("PWM configured and set to test case \r\n");

    i2c_close(bus);
    return 0;
}

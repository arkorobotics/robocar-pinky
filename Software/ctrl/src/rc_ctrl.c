#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include "i2c.h"

int main(int argc, char **argv)
{
    char i2c_dev_desc[128];
    I2C_READ_HANDLE i2c_read_handle = i2c_read;
    I2C_WRITE_HANDLE i2c_write_handle = i2c_write;
    unsigned int addr = 0, iaddr_bytes = 1, page_bytes = 8, bus_num = 1;

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

    /* Init i2c device */
    I2CDevice device;
    memset(&device, 0, sizeof(device));
    i2c_init_device(&device);

    device.bus = bus;
    device.addr = addr;
    device.page_bytes = page_bytes;
    device.iaddr_bytes = iaddr_bytes;



    i2c_close(bus);
    return 0;
}

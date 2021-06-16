#include "mcp3421_adc.h"

#define MCP3421_ADC_ADDR            0x68
#define CONFIG_16BIT_CONTINUOUS     24
#define CONFIG_16BIT_ONE_SHOT       136

unsigned char write_buf[1];
unsigned char read_buf[2];

struct device *i2c_dev;

int init_adc()
{
	i2c_dev = device_get_binding("I2C_0");
	if (!i2c_dev) {
		printk("I2C: Device driver not found.\n");
		return 1;
	}
    write_buf[0] = CONFIG_16BIT_ONE_SHOT;
    if(i2c_write(i2c_dev, write_buf, 1, MCP3421_ADC_ADDR) == 0)
	{
		return 0;
	}
	return 1;
}


int get_voltage_reading(uint32_t *val_buf)
{
    if((i2c_write(i2c_dev, write_buf, 1, MCP3421_ADC_ADDR) == 0) && (i2c_read(i2c_dev, read_buf, 2, MCP3421_ADC_ADDR) == 0))
    {
        uint16_t rslt = ((uint16_t)read_buf[0] << 8) | ((uint16_t)read_buf[1]);
        *val_buf = (uint32_t)(((double)rslt / 32767) * 2048);
        return 0;
    }
    return 1;
}


#ifndef MCP3421_ADC
#define MCP3421_ADC

#include <errno.h>
#include <zephyr.h>
#include <misc/printk.h>
#include <device.h>
#include <i2c.h>


int init_adc();

int get_voltage_reading(uint32_t* val_buf);

#endif
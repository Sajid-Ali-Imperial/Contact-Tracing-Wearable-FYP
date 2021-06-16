#include <zephyr.h>
#include "I2C/accelerometer.h"
#include "I2C/mcp3421_adc.h"
#include <errno.h>
#include <misc/printk.h>
#include <device.h>
#include <i2c.h>

#define I2C_DEV "I2C_0"


#define STACKSIZE 1024
char thread_stacks[2][STACKSIZE];
static struct k_thread threads[2];

#define APP_NAME "Test - ADC over I2C\n"
#define APP_VERSION "Version - 1.4.0\n"
#define APP_VERSION_NUM 0x010400
#define APP_LINE "=================\n"


int main(void)
{
    printk(APP_NAME);
    printk(APP_VERSION);
    printk(APP_LINE);

    //k_thread_create(&threads[0], &thread_stacks[0][0], STACKSIZE, ble_thread, 0, 0, 0, K_PRIO_COOP(4), 0, 0);
    //k_thread_create(&threads[1], &thread_stacks[1][0], STACKSIZE, uwb_thread, 0, 0, 0, K_PRIO_COOP(4), 0, 0);
    //k_thread_create(&threads[0], &thread_stacks[0][0], STACKSIZE, acc_thread, 0, 0, 0, K_PRIO_COOP(4), 0, 0);

	if(init_acc())
	{
		printk("Failed to init ACC \n");
		return;
	}

	if(init_adc())
	{
		printk("Failed to init ADC \n");
		return;
	}

	uint32_t voltage;
	while(1)
	{
		if(get_acc_reading() == 0)
		{
			printk("Got ACC Value \n");
		}
		else
		{
			printk("Failed to read ADC \n");
			return;
		}

		k_sleep(500);
		
		if(get_voltage_reading(&voltage) == 0)
		{
			printk("%u mV \n", voltage);
		}
		else
		{
			printk("Failed to read ADC \n");
			return;
		}
	}
}



// #include <errno.h>
// #include <zephyr.h>
// #include <misc/printk.h>
// #include <device.h>
// #include <i2c.h>

// #define I2C_DEV "I2C_0"
// #define ADC_ADDR 0x68


// void main(void)
// {
// 	struct device *i2c_dev;

// 	printk("Starting i2c scanner...\n");

// 	i2c_dev = device_get_binding(I2C_DEV);
// 	if (!i2c_dev) {
// 		printk("I2C: Device driver not found.\n");
// 		return;
// 	}

// 	// for (u8_t i = 4; i <= 0x77; i++) {
// 	// 	struct i2c_msg msgs[1];
// 	// 	u8_t dst;

// 	// 	/* Send the address to read from */
// 	// 	msgs[0].buf = &dst;
// 	// 	msgs[0].len = 0;
// 	// 	msgs[0].flags = I2C_MSG_WRITE | I2C_MSG_STOP;

// 	// 	if (i2c_transfer(i2c_dev, &msgs[0], 1, i) == 0) {
// 	// 		//printk("0x%2x FOUND\n", i);
// 	// 		printk("%u FOUND\n", i);
// 	// 	}
// 	// }

// 	// //uint8_t write_buf[1];
// 	unsigned char write_buf[1];
// 	uint8_t config_16b_con = 24;	// 0 0 0 1 1 0 0 0
// 	uint8_t config_16b_ss = 136;	// 1 0 0 0 1 0 0 0
// 	write_buf[0] = config_16b_ss;

// 	uint8_t mask = 15;
// 	unsigned char read_buf[3];
// 	uint8_t bin_ref[] = {1, 2, 4, 8, 16, 32, 64, 128};
// 	int i;
// 	uint16_t rslt;

// 	/* Write to ADC and confure 16 bit volatage*/
// 	k_sleep(200);
// 	if(i2c_write(i2c_dev, write_buf, 1, ADC_ADDR) == 0)
// 	{
// 		printk("Successful write \n");
// 	}
// 	else
// 	{
// 		printk("Failed to write to ADC \n");
// 		return;
// 	}


// 	while(1)
// 	{
// 		k_sleep(300);

// 		//i2c_read(i2c_dev, read_buf, 2, ADC_ADDR)
// 		if((i2c_write(i2c_dev, write_buf, 1, ADC_ADDR) == 0) && (i2c_read(i2c_dev, read_buf, 3, ADC_ADDR) == 0))
// 		{
// 			for(i=7; i>=0; i--)
// 			{
// 				if((read_buf[0] & bin_ref[i]) == bin_ref[i])
// 				{
// 					printk("1");
// 				}
// 				else
// 				{
// 					printk("0");
// 				}

// 			}

// 			printk("  ");

// 			for(i=7; i>=0; i--)
// 			{
// 				if((read_buf[1] & bin_ref[i]) == bin_ref[i])
// 				{
// 					printk("1");
// 				}
// 				else
// 				{
// 					printk("0");
// 				}

// 			}

// 			for(i=7; i>=7; i--)
// 			{
// 				if((read_buf[2] & bin_ref[i]) == bin_ref[i])
// 				{
// 					printk("1");
// 				}
// 				else
// 				{
// 					printk("0");
// 				}

// 			}

// 			// printk("\n");
// 			//read_buf[0] = read_buf[0] & mask; - use for 12 bit conversion
// 			rslt = ((uint16_t)read_buf[0] << 8) | ((uint16_t)read_buf[1]);
// 			uint32_t voltage = (uint32_t)(((double)rslt / 32767) * 2048000);
// 			printk("\n%u uV\n", voltage);
			
// 		}
// 		else
// 		{
// 			printk("DATA READ FAILED \n");
// 			return;
// 		}

// 	}

// }

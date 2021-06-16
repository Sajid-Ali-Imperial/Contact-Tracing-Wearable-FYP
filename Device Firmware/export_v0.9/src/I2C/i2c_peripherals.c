#include "i2c_peripherals.h"


/* 
	******************************************
   	********** LIS2DH Accelerometer **********
   	******************************************
*/
static struct sensor_value accel_x, accel_y, accel_z;
struct device *acc_dev;

int init_acc()
{
	acc_dev = device_get_binding("LIS2DH");
	if (!acc_dev) {
		printk("I2C: Device driver not found.\n");
		return 1;
	}
	return 0;
}


int get_acc_reading()
{
	//sensor_sample_fetch(dev);
	sensor_sample_fetch_chan(acc_dev, SENSOR_CHAN_ACCEL_XYZ);
	sensor_channel_get(acc_dev, SENSOR_CHAN_ACCEL_X, &accel_x);
	sensor_channel_get(acc_dev, SENSOR_CHAN_ACCEL_Y, &accel_y);
	sensor_channel_get(acc_dev, SENSOR_CHAN_ACCEL_Z, &accel_z);

	printf("accx: %d.%06d; accy: %d.%06d; accz: %d.%06d\n",
			accel_x.val1, accel_x.val2, accel_y.val1, accel_y.val2,
			accel_z.val1, accel_z.val2);

	return 0;
}


/* 
	******************************************
   	******** ADS1100  and  ADS7883 AD ********
   	******************************************
*/

#define ADS1100_ADC_ADDR            		0x48	// 0 1 0 0 1 0 0 0
#define ADS1100_CONFIG_16BIT_CONTINUOUS     12
#define ADS1100_CONFIG_16BIT_ONE_SHOT       156 	// 1 0 0 1 1 1 0 0 
#define SHUNT_RESISTANCE_FACTOR 			100 	// Because I = V/R and R=0.01 => I = 100 * V

#define ADS7883_ADC_ADDR            		0x48	// 0 1 0 0 1 0 0 0
#define ADS7883_CONFIG_12BIT_CONTINUOUS     24
#define ADS7883_CONFIG_12BIT_ONE_SHOT       136

unsigned char ads1100_write_buf[1];
unsigned char ads1100_read_buf[2];

unsigned char ads7883_write_buf[1];
unsigned char ads7883_read_buf[2];

struct device *i2c_dev;

static uint16_t battery_voltage;
static uint16_t battery_current;

/* Initialise ADCs by setting configuration byte for both.*/
int init_adcs()
{
	i2c_dev = device_get_binding("I2C_0");
	if (!i2c_dev) {
		printk("I2C: Device driver not found.\n");
		return 1;
	}
	/* Set configuration to one-shot mode, this way both ADCs are in slepp mode unit they are called upon. */
    ads1100_write_buf[0] = ADS1100_CONFIG_16BIT_ONE_SHOT;
	ads7883_write_buf[0] = ADS7883_CONFIG_12BIT_ONE_SHOT;
    if(i2c_write(i2c_dev, ads1100_write_buf, 1, ADS1100_ADC_ADDR) == 0)
	{
		if(i2c_write(i2c_dev, ads7883_write_buf, 1, ADS7883_ADC_ADDR) == 0)
		{
			return 0;
		}
	}
	return 1;
}

/* Returns the current battery voltage and current measurements. 
   NOTE: only call this function once init_adcs() has successfully completed.
*/
int get_battery_reading(uint16_t *voltage, uint16_t *current)
{
	/* Start by finding the volage of the battery */
    if((i2c_write(i2c_dev, ads7883_write_buf, 1, ADS7883_ADC_ADDR) == 0) && (i2c_read(i2c_dev, ads7883_read_buf, 2, ADS7883_ADC_ADDR) == 0))
    {
        uint16_t rslt = ((uint16_t)ads7883_read_buf[0] << 8) | ((uint16_t)ads7883_read_buf[1]);
        battery_voltage = (uint16_t)(((double)rslt / 32768) * 2048);

		/* Now attempt to find the current through the shunt resistor.
		   From the datasheet of the ADS1100, the output code is defined as: 
		   Output_Code = 32768 * (V1 - V2) / VDD (where VDD is equal to the battery voltage in our case),
		   Therefore: V1 - V2 = Output_Code * VBat / 32768
		   and finally, I = (V1 - V2) / R
		*/
		if((i2c_write(i2c_dev, ads1100_write_buf, 1, ADS1100_ADC_ADDR) == 0) && (i2c_read(i2c_dev, ads1100_read_buf, 2, ADS1100_ADC_ADDR) == 0))
		{
			uint16_t output_code = ((uint16_t)ads1100_read_buf[0] << 8) | ((uint16_t)ads1100_read_buf[1]);
			battery_current = (uint32_t)(((double)output_code / 32768) * battery_voltage * SHUNT_RESISTANCE_FACTOR);
			*voltage = battery_voltage;
			*current = battery_current;
			return 0;
		}
    }
    return 1;
}



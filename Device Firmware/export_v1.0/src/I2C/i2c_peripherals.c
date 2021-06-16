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

#define CURRENT_ADC_ADDR            		108	
#define CURRENT_ADC_CONFIG       			128 	
#define SHUNT_RESISTANCE_FACTOR 			1 	// Because I = V/R and R=1.0 => I = 1* V

#define VOLTAGE_ADC_ADR            			106	
#define VOLTAGE_ADC_ADR_CONFIG       		128

unsigned char current_adc_write[1];
unsigned char current_read_buf[3];

unsigned char voltage_adc_write[1];
unsigned char voltage_read_buf[3];

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
    current_adc_write[0] = CURRENT_ADC_CONFIG;
	voltage_adc_write[0] = VOLTAGE_ADC_ADR_CONFIG;
    if(i2c_write(i2c_dev, current_adc_write, 1, CURRENT_ADC_ADDR) == 0)
	{
		if(i2c_write(i2c_dev, voltage_adc_write, 1, VOLTAGE_ADC_ADR) == 0)
		{
			return 0;
		}
	}
	return 1;
}

/* Returns the current battery voltage and current measurements. 
   NOTE: only call this function once init_adcs() has successfully completed.
*/
int get_battery_reading(uint32_t *voltage, uint32_t *current)
{
	/* Start by finding the volage of the battery */
    if((i2c_write(i2c_dev, voltage_adc_write, 1, VOLTAGE_ADC_ADR) == 0) && (i2c_read(i2c_dev, voltage_read_buf, 2, VOLTAGE_ADC_ADR) == 0))
    {
        uint16_t rslt = ((uint32_t)voltage_read_buf[0] << 16) | (uint32_t)voltage_read_buf[1] << 8) | ((uint32_t)voltage_read_buf[2]);
        battery_voltage = (uint32_t)(((double)rslt / 131072) * 2048);
		*voltage = battery_voltage;

		if((i2c_write(i2c_dev, current_adc_write, 1, CURRENT_ADC_ADDR) == 0) && (i2c_read(i2c_dev, current_read_buf, 2, CURRENT_ADC_ADDR) == 0))
		{
			uint32_t output_code = ((uint32_t)current_read_buf[0] << 16) | ((uint32_t)current_read_buf[1] << 8) | ((uint32_t)current_read_buf[2]);
			battery_current = (uint32_t)(((double)output_code / 131072) * 2048 * SHUNT_RESISTANCE_FACTOR);
			*current = battery_current;
			return 0;
		}
    }
    return 1;
}



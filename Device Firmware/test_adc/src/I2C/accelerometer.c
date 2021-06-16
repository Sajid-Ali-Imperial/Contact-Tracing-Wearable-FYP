#include "accelerometer.h"


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

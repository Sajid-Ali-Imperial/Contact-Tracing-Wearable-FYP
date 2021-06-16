#include "accelerometer.h"


void acc_thread(void *arg1, void *ard2, void *arg3)
{
	struct device *dev = device_get_binding("LIS2DH");

	printf("dev %p name %s\n", dev, dev->config->name);

	while (1) {
		static struct sensor_value accel_x, accel_y, accel_z;

		//sensor_sample_fetch(dev);
		sensor_sample_fetch_chan(dev, SENSOR_CHAN_ACCEL_XYZ);
        sensor_channel_get(dev, SENSOR_CHAN_ACCEL_X, &accel_x);
        sensor_channel_get(dev, SENSOR_CHAN_ACCEL_Y, &accel_y);
        sensor_channel_get(dev, SENSOR_CHAN_ACCEL_Z, &accel_z);

		printf("accx: %d.%06d; accy: %d.%06d; accz: %d.%06d\n",
		      accel_x.val1, accel_x.val2, accel_y.val1, accel_y.val2,
		      accel_z.val1, accel_z.val2);

		k_sleep(2000);
	}
}
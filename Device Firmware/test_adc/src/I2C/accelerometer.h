#ifndef ACCELEROMETER_SCRIPT
#define ACCELEROMETER_SCRIPT

#include <zephyr.h>
#include <device.h>
#include <sensor.h>
#include <stdio.h>



int init_acc();

int get_acc_reading();


#endif
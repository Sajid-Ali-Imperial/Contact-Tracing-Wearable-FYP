#ifndef BLE_MAIN
#define BLE_MAIN

/* main.c - Application main entry point */

/*
    ------------------------ BLE SCRIPT --------------------------
    v0.7 - Configured BLE services and characteristics to  transfer data to application
           and initialise the device by accepting a unique ID identifier.

 */

/* main.c - Application main entry point */


#include <zephyr/types.h>
#include <stddef.h>
#include <string.h>
#include <errno.h>
#include <stdio.h>
#include <misc/byteorder.h>
#include <zephyr.h>

#include <settings/settings.h>

#include <bluetooth/bluetooth.h>
#include <bluetooth/hci.h>
#include <bluetooth/conn.h>
#include <bluetooth/uuid.h>
#include <bluetooth/gatt.h>

#include "../ContactData/contact_data.h"

void ble_thread(void *arg1, void *ard2, void *arg3);

#endif
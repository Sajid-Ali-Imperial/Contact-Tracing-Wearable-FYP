#ifndef BLE_SCANNING
#define BLE_SCANNING

#include <zephyr/types.h>
#include <stddef.h>
#include <errno.h>
#include <zephyr.h>
//#include <misc/printk.h>
#include <stdio.h>

#include <bluetooth/bluetooth.h>
#include <bluetooth/hci.h>
#include <bluetooth/conn.h>
#include <bluetooth/uuid.h>
#include <bluetooth/gatt.h>
#include <misc/byteorder.h>

             
void device_found(const bt_addr_le_t *addr, s8_t rssi, u8_t type,
            struct net_buf_simple *ad);

#endif
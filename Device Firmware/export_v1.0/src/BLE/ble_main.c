#include "ble_main.h"
//#include "battery_service.h"
#include "data_transfer.h"

/* Connection parameters and storage varianbles */
static struct bt_conn *default_conn;
static struct bt_gatt_read_params read_params;
static struct bt_gatt_write_params write_params;
static struct bt_gatt_discover_params discover_params;
/* Dimensional informaiton of advertising packets to listen for */
#define SCAN_PREAMBLE_LEN 10
#define EPHEMERAL_CODE_LEN 8 
/* Buffer to hold the received advertising message */
static uint8_t adv_msg_rec[SCAN_PREAMBLE_LEN + EPHEMERAL_CODE_LEN];
/* Preamble to look for in received advertising message*/
static uint8_t scan_preamble[] = {99, 111, 118, 105, 100, 116, 114, 97, 99, 107};
/* UUID of advertising characteristic of ranging devices*/
static const struct bt_uuid_128 init_uwb_uuid = BT_UUID_INIT_128(
	0xf3, 0xde, 0xbc, 0x9a, 0x78, 0x56, 0x34, 0x12,
	0x78, 0x56, 0x34, 0x12, 0x78, 0x56, 0x34, 0x12);
/* BLE broadcasting and listening configuraitons */
#define MIN_ADVERTISING_DURATION 	300
#define MIN_SCAN_DURATION 			200
/* Varaible used to infer the state of the BLE connections */
uint16_t service_handle;
static bool found_chcr=false;
/* Buffer containing the message to send to other device to initiate uwb distance ranging*/
static uint8_t gatt_init_uwb_write_buf[] = {'1', '2', '3', '4', '5', '6', '7', '8', 'a', 'a'};
/* Device EphID, NOTE: this is only a temporary values and will be randomly generated later*/
static uint8_t eph_id[] = {1, 2, 3, 4, 'a', 'b', 'c', 'd'};
/* Variable of detected device currently under examination, these are temperory and only used to store the state so they are not lost */
static s8_t current_rssi;
static uint8_t current_det_eph_id[EPHEMERAL_CODE_LEN];
static bool critial_exposure = false;
/* Define parameters to track the state of the BLE device */
#define IS_ADVERTISING 1
#define IS_SCANNING    2
#define IS_CONNECTED   3
static uint8_t ble_device_state; 
/* Define fifo variable to send to UWB thread */
struct data_item_t tx_data;

/* Define functions*/
static u8_t discover_func(struct bt_conn *conn, const struct bt_gatt_attr *attr, struct bt_gatt_discover_params *params);


/*
	Characteristic read callback function. This function is called whenever this device has
	received data from a read attribute of another device which it is connected to.
*/
static uint8_t read_func(struct bt_conn *conn, uint8_t err,
			 struct bt_gatt_read_params *params,
			 const void *data, uint16_t length)
{
	printf("Read complete: err 0x%02x length %u \n", err, length);

	if (!data) {
		printf("Error, no data received \n");
		(void)memset(params, 0, sizeof(*params));
		return BT_GATT_ITER_STOP;
	}

	return BT_GATT_ITER_CONTINUE;
}

/*
	Call this function when attempting to read to an attribute of a connected device.
*/
static int cmd_read(uint16_t handle)
{
	int err;
	/* Confirm the existence of a valid connection before attempting to read an attribute */
	if (!default_conn) 
	{
		printf("Not connected");
		return -ENOEXEC;
	}

	/* Assign valid read parameters*/
	read_params.func = read_func;
	read_params.handle_count = 1;
	read_params.single.handle = handle;
	read_params.single.offset = 0U;

	/* Call read API*/
	err = bt_gatt_read(default_conn, &read_params);
	return err;
}


/*
	Characteristic write callback function. This function is called whenever this device has
	received confirmation of a write attribute of another device which it is connected to.
	This is called to confirm the completion of a successful write operation.
*/
static void write_func(struct bt_conn *conn, uint8_t err,
		       struct bt_gatt_write_params *params)
{
	 printf("Write complete\r\n");
}

/*
	Call this function when attempting to write to an attribute of a connected device.
*/
static int cmd_write(uint16_t handle)
{
	int err;
	/* Confirm the existence of a valid connection before attempting to write to an attribute */
	if (!default_conn) 
	{
		printf("Not connected");
		return -ENOEXEC;
	}

	/* Assign valid write parameters*/
	write_params.data = gatt_init_uwb_write_buf;
	write_params.length = 10;
	write_params.handle = handle;
	write_params.offset = 0;
	write_params.func = write_func;

	/* Call write API */
	err = bt_gatt_write(default_conn, &write_params);
	return err;
}


/* Called when a device is close enought to warrent UWB prevision distance measuring */
void initiate_uwb_connection()
{
	// start by informing other device to move to uwb mode
	if(cmd_write(service_handle) == 0)
	{
		// use mutex and fifo to inform UWB thread to start DS-TWR
		k_mutex_lock(&fifo_mutex, K_FOREVER);
		tx_data.eph_id = 1;
		k_fifo_put(&uwb_init_fifo, &tx_data);
		k_mutex_unlock(&fifo_mutex);
	}

}

/*
	Called when an advertising packet has successfully been received.
*/
static bool eir_found(struct bt_data *data, void *user_data)
{
	/* Only continue if the received advertising packet is of the expected size */
    if(data->data_len != SCAN_PREAMBLE_LEN + EPHEMERAL_CODE_LEN)
    {
        return true;
    }

	//bt_addr_le_t *addr = user_data;
	/* Filter the data by type */
	switch (data->type) 
	{
		case BT_DATA_SVC_DATA16:
			/* Copy all the received bytes into a buffer for further comparison*/
			memcpy(&adv_msg_rec, data->data, SCAN_PREAMBLE_LEN + EPHEMERAL_CODE_LEN);

			/* Check to see that the preamble is the expected one*/
			if(memcmp(&adv_msg_rec, &scan_preamble, SCAN_PREAMBLE_LEN) == 0)
			{

				/* Get the ephemeral id of the device that transmitted the packet */
				memcpy(&current_det_eph_id, &adv_msg_rec[SCAN_PREAMBLE_LEN], EPHEMERAL_CODE_LEN);
				critial_exposure = get_exposure(current_det_eph_id, current_rssi);
				if(critial_exposure)
				{
					printf("Attempting to connect to device ... \n");
					int err = bt_le_scan_stop();
					if (err) {
						printf("Stop LE scan failed (err %d)\n", err);
						return false;
					}
					
					default_conn = bt_conn_create_le(user_data,
									BT_LE_CONN_PARAM_DEFAULT);

					// initiate UWB connection if successfully
				// 	printf("Connection State is: %u \n", ble_device_state);
				// 	if(ble_device_state == IS_CONNECTED)
				// 	{
				// 		printf("Trying to init UWB \n");
				// 		initiate_uwb_connection();
				// 	}
				}
				return false;
			}
	}

	return true;
}

/*
	Called when a service of a connected device has been found. This function attempts
	to identify the service or characteristic and execute appropriate commands depending 
	on the type and uuid.
*/
static u8_t discover_func(struct bt_conn *conn,
			     const struct bt_gatt_attr *attr,
			     struct bt_gatt_discover_params *params)
{
	int err;
	struct bt_gatt_chrc *gatt_chrc;
	char str[37];
	int i;

	if (!attr) {
		printf("Discover complete\n");
		(void)memset(params, 0, sizeof(*params));
		return BT_GATT_ITER_STOP;
	}

	switch (params->type) {
	case BT_GATT_DISCOVER_CHARACTERISTIC:
		gatt_chrc = attr->user_data;
		bt_uuid_to_str(gatt_chrc->uuid, str, sizeof(str));
		if(gatt_chrc->properties & BT_GATT_CHRC_WRITE)
		{
			service_handle = attr->handle + 1;
			found_chcr = true;
			printf("FOUND WRITE CHARACTERISTIC \n");
			return BT_GATT_ITER_STOP;	
		}
		break;
	default:

		break;
	}

	return BT_GATT_ITER_CONTINUE;
}

static void device_found(const bt_addr_le_t *addr, s8_t rssi, u8_t type,
			 struct net_buf_simple *ad)
{
	/* We're only interested in connectable events */
	if (type != BT_LE_ADV_IND && type != BT_LE_ADV_DIRECT_IND) {
		return;
	}
	current_rssi = rssi;
	printf("Device Found \n");
	bt_data_parse(ad, eir_found, (void *)addr);
	// if (type == BT_LE_ADV_IND || type == BT_LE_ADV_DIRECT_IND) {
	// 	bt_data_parse(ad, eir_found, (void *)addr);
	// }
}

static void connected(struct bt_conn *conn, u8_t conn_err)
{
	char addr[BT_ADDR_LE_STR_LEN];
	int err;

	bt_addr_le_to_str(bt_conn_get_dst(conn), addr, sizeof(addr));

	if (conn_err) {
		printf("Failed to connect to %s (%u)\n", addr, conn_err);
		return;
	}

	printf("Connected: %s\n", addr);

	ble_device_state = IS_CONNECTED;
	if (conn == default_conn) {
		discover_params.uuid = &init_uwb_uuid.uuid;
		discover_params.func = discover_func;
		discover_params.start_handle = 0x0001;
		discover_params.end_handle = 0xffff;
		discover_params.type = BT_GATT_DISCOVER_CHARACTERISTIC;

		err = bt_gatt_discover(default_conn, &discover_params);
		if (err) {
			printf("Discover failed(err %d)\n", err);
			return;
		}

	}

}

static void disconnected(struct bt_conn *conn, u8_t reason)
{
	char addr[BT_ADDR_LE_STR_LEN];
	int err;

	bt_addr_le_to_str(bt_conn_get_dst(conn), addr, sizeof(addr));

	printf("Disconnected: %s (reason %u)\n", addr, reason);

	// if (default_conn != conn) {
	// 	return;
	// }

	bt_conn_unref(default_conn);
	default_conn = NULL;
	found_chcr = false;
	/* This demo doesn't require active scan */
	err = bt_le_scan_start(BT_LE_SCAN_ACTIVE, device_found);
	if (err) {
		printf("Scanning failed to start (err %d)\n", err);
	}
	else
	{
		ble_device_state = IS_SCANNING;
	}
}


static struct bt_conn_cb conn_callbacks = {
	.connected = connected,
	.disconnected = disconnected,
};


static const struct bt_data ad[] = {
	BT_DATA_BYTES(BT_DATA_FLAGS, BT_LE_AD_NO_BREDR),
	BT_DATA_BYTES(BT_DATA_UUID16_ALL, 0xaa, 0xfe),
	BT_DATA_BYTES(BT_DATA_SVC_DATA16,	// every device must have this to identify it as a contact tracing device
		      'c', 'o', 'v', 'i', 'd',
		      't', 'r', 'a', 'c', 'k') 			// unique identifier of this device
};

/* 
	BLE Thread
	Initialise this thread in the main script and allocated its memory on the
	stack. The BLE thread will always be running (unlike some other) and therefore
	takes priority in terms of scheduling.
*/
void ble_thread(void *arg1, void *ard2, void *arg3)
{
	int err;
	err = bt_enable(NULL);
	if (err) {
		printf("Bluetooth init failed (err %d)\n", err);
		return;
	}

	// Wait until BLE has successfully initiated
	k_sleep(200);
	printf("Bluetooth initialized\n");

	/* Register connection status callbacks */

	/* Begin scanning for nearby devices */
	err = bt_le_scan_start(BT_LE_SCAN_ACTIVE, device_found);
	if (err) {
		printf("Scanning failed to start (err %d)\n", err);
		return;
	}
	else{
		printf("Scanning successfully started\n");
		ble_device_state = IS_SCANNING;
	}
	

	/* Begin advertising */
	// while(1)
	// {
	// 	k_sleep(10000);
	// 	if(default_conn != NULL)
	// 	{
	// 		if(found_chcr)
	// 		{
	// 			k_mutex_lock(&fifo_mutex, K_FOREVER);
	// 			tx_data.eph_id = 1;
	// 			k_fifo_put(&uwb_init_fifo, &tx_data);
	// 			k_mutex_unlock(&fifo_mutex);
	// 			cmd_write(service_handle);
	// 		}
	// 		k_sleep(500);
	// 		bt_conn_disconnect(default_conn, BT_HCI_ERR_REMOTE_USER_TERM_CONN);
	// 	}
		
	// }

	data_transfer_service_init();

	if (IS_ENABLED(CONFIG_SETTINGS)) {
		settings_load();
	}
	bt_conn_cb_register(&conn_callbacks);

	// define variables used for randomised advertising and scanning to break potential synchronisation
	unsigned  mask;
    mask = (1 << 8) - 1;	// random delay is between 0 - 256ms (i.e 8 bit binary number)
    uint32_t random_delay;

	while(1)
	{
		// Sleep while scan is running
		random_delay = mask & sys_rand32_get();
		k_sleep(K_MSEC(MIN_SCAN_DURATION + random_delay));

		/* Stop scanning if already started */
		if(ble_device_state == IS_SCANNING)
		{
			err = bt_le_scan_stop();
			if (err) {
				printf("Scanning failed to stop (err %d)\n", err);
			}
		}

		/* Beging advertising unless already connected to a device */
		if(ble_device_state != IS_CONNECTED)
		{
			err = bt_le_adv_start(BT_LE_ADV_CONN_NAME, ad, ARRAY_SIZE(ad), NULL, 0);
			if (err) {
				printf("Advertising failed to start (err %d)\n", err);
				return;
			}
			ble_device_state = IS_ADVERTISING;
		}

		// Sleep while advertising is running
		random_delay = mask & sys_rand32_get();
		k_sleep(K_MSEC(MIN_ADVERTISING_DURATION + random_delay));

		/* Stop advertising if not already done so */
		if(ble_device_state == IS_ADVERTISING)
		{
			err = bt_le_adv_stop();
			if (err) {
				printf("Advertising failed to stop (err %d)\n", err);
				return;
			}
		}

		/* Start scanning unless already connected to a device */
		if(ble_device_state != IS_CONNECTED)
		{
			err = bt_le_scan_start(BT_LE_SCAN_ACTIVE, device_found);
			if (err) {
				printf("Scanning failed to start (err %d)\n", err);
				return;
			}
			ble_device_state == IS_SCANNING;
		} else if(found_chcr)	// if connected to other device, initiate uwb connection by sending relevant data
		{
			if(!initiate_uwb_connection())	// on the completion of a successful write, disconnect
			{
				bt_conn_disconnect(default_conn, BT_HCI_ERR_REMOTE_USER_TERM_CONN);
			}
		}

		


	}

}

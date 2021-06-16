#include "ble_main.h"
//#include "battery_service.h"
//#include "data_transfer.h"

static struct bt_conn *default_conn;

#define SCAN_PREAMBLE_LEN 10
#define EPHEMERAL_CODE_LEN 8 

#define MIN_ADVERTISING_DURATION 500
#define MIN_SCAN_DURATION 500

static uint8_t adv_msg_rec[SCAN_PREAMBLE_LEN + EPHEMERAL_CODE_LEN];
static uint8_t scan_preamble[] = {99, 111, 118, 105, 100, 116, 114, 97, 99, 107};


/*
    Define connection parameters and attributes of target device
*/
static const struct bt_uuid_128 init_uwb_uuid = BT_UUID_INIT_128(
	0xf3, 0xde, 0xbc, 0x9a, 0x78, 0x56, 0x34, 0x12,
	0x78, 0x56, 0x34, 0x12, 0x78, 0x56, 0x34, 0x12);

//static struct bt_conn *default_conn;
static struct bt_gatt_discover_params discover_params;
uint16_t service_handle;
static bool found_chcr=false;

static struct bt_gatt_read_params read_params;
static struct bt_gatt_write_params write_params;
#define EPHEMERAL_ID '1', '2', '3', '4', '5', '6', '7', '8'
static uint8_t gatt_write_buf[] = {EPHEMERAL_ID, 'a', 'a'};

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

static void write_func(struct bt_conn *conn, uint8_t err,
		       struct bt_gatt_write_params *params)
{

	 printf("Write complete\r\n");
	 //(void)memset(&write_params, 0, sizeof(write_params));

}

static int cmd_read(uint16_t handle)
{
	int err;
	
	if (!default_conn) {
		printf("Not connected");
		return -ENOEXEC;
	}

	read_params.func = read_func;
	read_params.handle_count = 1;
	read_params.single.handle = handle;
	read_params.single.offset = 0U;

	//err = bt_gatt_read(default_conn, &read_params);
	err = bt_gatt_read(default_conn, &read_params);
	if (err) {
		printf("Read failed (err %d)\r\n", err);
	} else {
		printf("Read pending\r\n");
	}

	return err;
}

static int cmd_write(uint16_t handle)
{
	int err;
	
	if (!default_conn) {
		printf("Not connected");
		return -ENOEXEC;
	}

	
	write_params.data = gatt_write_buf;
	write_params.length = 10;
	write_params.handle = handle;
	write_params.offset = 0;
	write_params.func = write_func;

	//default_conn conn_index
	err = bt_gatt_write(default_conn, &write_params);
	if (err) {
		printf("Write failed (err %d)\r\n", err);
	} else {
		printf("Write pending\r\n");
	}

	return err;
}


static bool eir_found(struct bt_data *data, void *user_data)
{
	bt_addr_le_t *addr = user_data;
	int i;
    if(data->data_len != SCAN_PREAMBLE_LEN + EPHEMERAL_CODE_LEN)
    {
        return true;
    }
	printf("[AD]: %u data_len %u\n", data->type, data->data_len);

	switch (data->type) 
	{
		case BT_DATA_SVC_DATA16:
			memcpy(&adv_msg_rec, data->data, SCAN_PREAMBLE_LEN + EPHEMERAL_CODE_LEN);
			// for(i=0; i<sizeof(adv_msg_rec); i++)
			// {
			// 	printf("%u ", adv_msg_rec[i]);
			// }
			printf("\n");
			if(memcmp(&adv_msg_rec, &scan_preamble, SCAN_PREAMBLE_LEN) == 0)
			{
				// recording the rssi and use to estimate distance
				//printf("SUCCESS !! \n");
				/* Attempt to connect to device */
				printf("Attempting to connect to device ... \n");
				int err = bt_le_scan_stop();
				if (err) {
					printf("Stop LE scan failed (err %d)\n", err);
					return false;
				}

				default_conn = bt_conn_create_le(addr,
								BT_LE_CONN_PARAM_DEFAULT);
				return false;
			}
	}


	return true;
}

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
	case BT_GATT_DISCOVER_SECONDARY:
		printf("GOT SECONDARY SERVICE \n");
		break;
	case BT_GATT_DISCOVER_PRIMARY:
		printf("GOT PRIMARY SERVICE \n");
		break;
	case BT_GATT_DISCOVER_CHARACTERISTIC:
		gatt_chrc = attr->user_data;
		bt_uuid_to_str(gatt_chrc->uuid, str, sizeof(str));
		//printf("Characteristic %s found: handle %x \n",
			   // str, attr->handle);
		if(gatt_chrc->properties & BT_GATT_CHRC_WRITE)
		{
			// if (!attr->write) 
			// {
			// 	printf("Cannot write attr \n");
			// }
			// else
			// {
			// 	service_handle = attr->handle + 1;
			// 	found_chcr = true;
			// 	printf("FOUND READ CHARACTERISTIC \n");
			// 	return BT_GATT_ITER_STOP;
			// }
			service_handle = attr->handle + 1;
			found_chcr = true;
			printf("FOUND WRITE CHARACTERISTIC \n");
			return BT_GATT_ITER_STOP;

			
		}
		break;
	case BT_GATT_DISCOVER_INCLUDE:
		printf("GOT INCLUDE \n");
		break;
	default:

		break;
	}

	return BT_GATT_ITER_CONTINUE;
}

static void device_found(const bt_addr_le_t *addr, s8_t rssi, u8_t type,
			 struct net_buf_simple *ad)
{
	char addr_str[BT_ADDR_LE_STR_LEN];

	// if (default_conn) {
	// 	return;
	// }

	/* We're only interested in connectable events */
	if (type != BT_LE_ADV_IND && type != BT_LE_ADV_DIRECT_IND) {
		return;
	}

	bt_addr_le_to_str(addr, addr_str, sizeof(addr_str));
	// printf("[DEVICE]: %s, AD evt type %u, AD data len %u, RSSI %i\n",
	//        addr_str, type, ad->len, rssi);

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

	if (default_conn != conn) {
		return;
	}

	bt_conn_unref(default_conn);
	default_conn = NULL;
	found_chcr = false;

	/* This demo doesn't require active scan */
	err = bt_le_scan_start(BT_LE_SCAN_PASSIVE, device_found);
	if (err) {
		printf("Scanning failed to start (err %d)\n", err);
	}
}


static struct bt_conn_cb conn_callbacks = {
	.connected = connected,
	.disconnected = disconnected,
};


/* 
	The advertising data contains only the bare minimum information required for 
	distance sensing with neighbouring devices and communicate with the base
	station if and when it is initialised.
	NOTE: Still need to include a unique identifier that enables multiple devices
	exchanging information without conflict.
*/
static const struct bt_data ad[] = {
	BT_DATA_BYTES(BT_DATA_FLAGS, BT_LE_AD_NO_BREDR),
	BT_DATA_BYTES(BT_DATA_UUID16_ALL, 0xaa, 0xfe),
	BT_DATA_BYTES(BT_DATA_SVC_DATA16,	// every device must have this to identify it as a contact tracing device
		      'z', 'e', 'p', 'h', 'y', 'r',
		      'p', 'r', 'o', 'j', 'e', 'c', 't') 
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
		printk("Bluetooth init failed (err %d)\n", err);
		return;
	}

	k_sleep(1000);
	printk("Bluetooth initialized\n");
	bt_conn_cb_register(&conn_callbacks);

	printk("\n\n STARTED SCANN\n\n");
	err = bt_le_scan_start(BT_LE_SCAN_ACTIVE, device_found);

	if (err) {
		printk("Scanning failed to start (err %d)\n", err);
		return;
	}
	
	printk("Scanning successfully started\n");

	while(1)
	{
		k_sleep(10000);
		if(default_conn != NULL)
		{
			if(found_chcr)
			{
				cmd_write(service_handle);
			}
			k_sleep(500);
			bt_conn_disconnect(default_conn, BT_HCI_ERR_REMOTE_USER_TERM_CONN);
		}
		
	}

	// define variables used for randomised advertising and scanning to break potential synchronisation
	// unsigned  mask;
    // mask = (1 << 9) - 1;
    // uint32_t random_delay;

	// while(1)
	// {

	// 	// Sleep while scan is running
	// 	random_delay = mask & sys_rand32_get();
	// 	k_sleep(K_MSEC(MIN_SCAN_DURATION + random_delay));

	// 	err = bt_le_adv_start(BT_LE_ADV_CONN_NAME, ad, ARRAY_SIZE(ad), NULL, 0);
	// 	if (err) {
	// 		printf("Advertising failed to start (err %d)\n", err);
	// 		return;
	// 	}

	// 	// Sleep while advertising is running
	// 	random_delay = mask & sys_rand32_get();
	// 	k_sleep(K_MSEC(MIN_ADVERTISING_DURATION + random_delay));

	// 	err = bt_le_adv_stop();
	// 	if (err) {
	// 		printf("Advertising failed to stop (err %d)\n", err);
	// 		return;
	// 	}
		

	// }

}

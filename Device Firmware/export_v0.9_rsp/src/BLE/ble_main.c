#include "ble_main.h"
#include "battery_service.h"
#include "data_transfer.h"
#include "scanning.h"

#define MIN_ADVERTISING_DURATION 500
#define MIN_SCAN_DURATION 500
#define EPHEMERAL_ID 'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h'

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
		      'c', 'o', 'v', 'i', 'd',
		      't', 'r', 'a', 'c', 'k',
			  EPHEMERAL_ID) 			// unique identifier of this device
};


/*
	Define connection callbacks.
	These functions are called whenever the connection status of the device changes.
*/
static void connected(struct bt_conn *conn, u8_t err)
{
	if (err) {
		printf("Connection failed (err %u)\n", err);
	} else {
		printf("Connected\n");
	}
}

static void disconnected(struct bt_conn *conn, u8_t reason)
{
	printf("Disconnected (reason %u)\n", reason);
}

static struct bt_conn_cb conn_callbacks = {
	.connected = connected,
	.disconnected = disconnected,
};


/* 
	Define services of the device, for now only include services to initialise ranging between
	devices.
*/
static struct bt_uuid_128 vnd_uuid = BT_UUID_INIT_128(
	0xf0, 0xde, 0xbc, 0x9a, 0x78, 0x56, 0x34, 0x12,
	0x78, 0x56, 0x34, 0x12, 0x78, 0x56, 0x34, 0x12);

static const struct bt_uuid_128 init_uwb_uuid = BT_UUID_INIT_128(
	0xf3, 0xde, 0xbc, 0x9a, 0x78, 0x56, 0x34, 0x12,
	0x78, 0x56, 0x34, 0x12, 0x78, 0x56, 0x34, 0x12);

static u8_t conn_init_info[10];	// the first 8 bytes are from the ephemeral id of the requesting device,
								// the last 2 bytes are used to differentiate this connection from others
								// that the device may have.

static ssize_t write_data_transfer(struct bt_conn *conn,
			      const struct bt_gatt_attr *attr, const void *buf,
			      u16_t len, u16_t offset, u8_t flags)
{
	u8_t *value = attr->user_data;
	if (flags & BT_GATT_WRITE_FLAG_PREPARE) {
		return 0;
	}
	if (offset + len > sizeof(conn_init_info)) {
		return BT_GATT_ERR(BT_ATT_ERR_INVALID_OFFSET);
	}
	memcpy(value + offset, buf, len);
	printf("Called Transfer Data Write with: ");
	int i;
	for(i=0; i< sizeof(conn_init_info); i++)
	{
		printf("%x", conn_init_info[i]);
	}
	printf("\n");
	return len;
}

static struct bt_gatt_attr vnd_attrs[] = {
	/* Vendor Primary Service Declaration */
	BT_GATT_PRIMARY_SERVICE(&vnd_uuid),
	BT_GATT_CHARACTERISTIC(&init_uwb_uuid.uuid,
			       BT_GATT_CHRC_WRITE,
			       BT_GATT_PERM_WRITE,
			       NULL, write_data_transfer, &conn_init_info),

};

static struct bt_gatt_service vnd_svc = BT_GATT_SERVICE(vnd_attrs);


/* 
	BLE Thread
	Initialise this thread in the main script and allocated its memory on the
	stack. The BLE thread will always be running (unlike some other) and therefore
	takes priority in terms of scheduling.
*/
void ble_thread(void *arg1, void *ard2, void *arg3)
{
	printf("############# Started BLE Thread ############### \n");
	int err;

	err = bt_enable(NULL);
	if (err) {
		printf("Bluetooth init failed (err %d)\n", err);
		return;
	}

	printf("Bluetooth initialized\n");

	/* Initialise all the relevant services of the device */
	bt_gatt_service_register(&vnd_svc);
	//battery_service_init();
	//data_transfer_init();


	if (IS_ENABLED(CONFIG_SETTINGS)) {
		settings_load();
	}

	/* Begin advertising as a peripheral device */
	err = bt_le_adv_start(BT_LE_ADV_CONN_NAME, ad, ARRAY_SIZE(ad), NULL, 0);
	if (err) {
		printf("Advertising failed to start (err %d)\n", err);
		return;
	}

	/* Register connection callbacks */
	bt_conn_cb_register(&conn_callbacks);
	/* Begin Scanning */
	// err = bt_le_scan_start(BT_LE_SCAN_PASSIVE, device_found);
	// if (err) {
	// 	printf("Starting scanning failed (err %d)\n", err);
	// 	return;
	// }
	// printf("Scanning successfully started\n");

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

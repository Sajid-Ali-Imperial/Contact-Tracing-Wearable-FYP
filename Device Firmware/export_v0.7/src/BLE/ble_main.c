#include "ble_main.h"
#include "battery_service.h"
#include "data_transfer.h"


static const struct bt_data ad[] = {
	BT_DATA_BYTES(BT_DATA_FLAGS, (BT_LE_AD_GENERAL | BT_LE_AD_NO_BREDR)),
	BT_DATA_BYTES(BT_DATA_UUID128_ALL,
		      0xf0, 0xde, 0xbc, 0x9a, 0x78, 0x56, 0x34, 0x12,
		      0x78, 0x56, 0x34, 0x12, 0x78, 0x56, 0x34, 0x12),
};

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
	Called to initialise the BLE device and configure the services.
	Also begin advertising. Any errors are reported and the system
	will print these if they should occur.
	
*/
static void bt_ready(int err)
{
	if (err) {
		printf("Bluetooth init failed (err %d)\n", err);
		return;
	}

	printf("Bluetooth initialized\n");

	// initialise all the relevant services of the device
	battery_service_init();
	data_transfer_init();


	if (IS_ENABLED(CONFIG_SETTINGS)) {
		settings_load();
	}

	// Begin advertising
	err = bt_le_adv_start(BT_LE_ADV_CONN_NAME, ad, ARRAY_SIZE(ad), NULL, 0);
	if (err) {
		printf("Advertising failed to start (err %d)\n", err);
		return;
	}

	printf("Advertising successfully started\n");
}



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

	err = bt_enable(bt_ready);
	if (err) {
		printf("Bluetooth init failed (err %d)\n", err);
		return;
	}

	bt_conn_cb_register(&conn_callbacks);
	//bt_conn_auth_cb_register(&auth_cb_display);
}

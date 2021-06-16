#include "data_transfer.h"

/* Custom Service Variables */
static struct bt_uuid_128 primary_service_uuid = BT_UUID_INIT_128(
	0xf0, 0xde, 0xbc, 0x9a, 0x78, 0x56, 0x34, 0x12,
	0x78, 0x56, 0x34, 0x12, 0x78, 0x56, 0x34, 0x12);


/*
	Transfer Contact Tracing Characteristic
	This characteristic is a read only characteristic and is used to transfer the contact
	tracing data gathered from the sensor to the base station for further analysis with
	the aggregate data of the population. The data consists of the UUIDs of users that
	the device has come into contact with along with the closest seperation distance and
	the time at which the interaction took place.

*/
static const struct bt_uuid_128 transfer_constract_tracing_data_uuid = BT_UUID_INIT_128(
	0xf3, 0xde, 0xbc, 0x9a, 0x78, 0x56, 0x34, 0x12,
	0x78, 0x56, 0x34, 0x12, 0x78, 0x56, 0x34, 0x12);

static u8_t contact_tracing_data[] = {
		  'V', 'e', 'n', 'd', 'o', 'r', ' ', 'd', 'a', 't', 'a', '1',
		  'V', 'e', 'n', 'd', 'o', 'r', ' ', 'd', 'a', 't', 'a', '2',
		  'V', 'e', 'n', 'd', 'o', 'r', ' ', 'd', 'a', 't', 'a', '3',
		  'V', 'e', 'n', 'd', 'o', 'r', ' ', 'd', 'a', 't', 'a', '4',
		  'V', 'e', 'n', 'd', 'o', 'r', ' ', 'd', 'a', 't', 'a', '5',
		  'V', 'e', 'n', 'd', 'o', 'r', ' ', 'd', 'a', 't', 'a', '6',
		  '.', ' ' };

static ssize_t read_data_transfer(struct bt_conn *conn,
			     const struct bt_gatt_attr *attr, void *buf,
			     u16_t len, u16_t offset)
{
	const char *value = attr->user_data;
	printf("Called Transfer Data Read \n");
	return bt_gatt_attr_read(conn, attr, buf, len, offset, value,
				 sizeof(contact_tracing_data));
}


static ssize_t write_data_transfer(struct bt_conn *conn,
			      const struct bt_gatt_attr *attr, const void *buf,
			      u16_t len, u16_t offset, u8_t flags)
{
	u8_t *value = attr->user_data;

	if (flags & BT_GATT_WRITE_FLAG_PREPARE) {
		return 0;
	}

	if (offset + len > sizeof(contact_tracing_data)) {
		return BT_GATT_ERR(BT_ATT_ERR_INVALID_OFFSET);
	}

	memcpy(value + offset, buf, len);
	printf("Called Transfer Data Write \n");
	return len;
}

/*
	Device Configuration Characteristic
	This characteristic is a read and write characteristic that is responsible for
	initialising the device with the unique ID of the user. If the device has already
	been initialised the write capability will not be functional.

*/
static const struct bt_uuid_128 configure_uuid = BT_UUID_INIT_128(
	0xf4, 0xde, 0xbc, 0x9a, 0x78, 0x56, 0x34, 0x12,
	0x78, 0x56, 0x34, 0x12, 0x78, 0x56, 0x34, 0x12);


static u8_t device_id[] = {0xf1, 0x12, 0xbc, 0x9a, 0x78, 0x56, 0x34, 0x12,
	0x78, 0x65, 0x43, 0x21, 0x78, 0x66, 0x34, 0x12};

static ssize_t read_configure(struct bt_conn *conn,
			     const struct bt_gatt_attr *attr, void *buf,
			     u16_t len, u16_t offset)
{
	const char *value = attr->user_data;

	printf("Called Configure Read \n");
	return bt_gatt_attr_read(conn, attr, buf, len, offset, value,
				 sizeof(device_id));
}

static ssize_t write_configure(struct bt_conn *conn,
			      const struct bt_gatt_attr *attr, const void *buf,
			      u16_t len, u16_t offset, u8_t flags)
{
	u8_t *value = attr->user_data;
	if (flags & BT_GATT_WRITE_FLAG_PREPARE) {
		return 0;
	}
	if (offset + len > sizeof(device_id)) {
		return BT_GATT_ERR(BT_ATT_ERR_INVALID_OFFSET);
	}
	memcpy(value + offset, buf, len);
	printf("Called Config Write \n");
	return len;
}


/* 
	Vendor Primary Service Declaration.
	Declare the custom serice that is advertised by the device by outlining
	the permissions of each characteristic. More services will be added
	later, for now only two characteristic are exposed. 
	
*/
static struct bt_gatt_attr vnd_attrs[] = {
	/* Vendor Primary Service Declaration */
	BT_GATT_PRIMARY_SERVICE(&primary_service_uuid),
	BT_GATT_CHARACTERISTIC(&configure_uuid.uuid,
			       BT_GATT_CHRC_READ | BT_GATT_CHRC_WRITE |
			       BT_GATT_CHRC_INDICATE,
			       BT_GATT_PERM_READ_ENCRYPT |
			       BT_GATT_PERM_WRITE_ENCRYPT,
			       read_configure, write_configure, device_id),
	BT_GATT_CHARACTERISTIC(&transfer_constract_tracing_data_uuid.uuid, BT_GATT_CHRC_READ |
			       BT_GATT_CHRC_WRITE | BT_GATT_CHRC_EXT_PROP,
			       BT_GATT_PERM_READ | BT_GATT_PERM_WRITE |
			       BT_GATT_PERM_PREPARE_WRITE,
			       read_data_transfer, write_data_transfer, &contact_tracing_data),

};

static struct bt_gatt_service vnd_svc = BT_GATT_SERVICE(vnd_attrs);


/* 
	Called to initialise the BLE device and configure the services.
	Also begin advertising. Any errors are reported and the system
	will print these if they should occur.
	
*/
void data_transfer_service_init()
{
	bt_gatt_service_register(&vnd_svc);
}



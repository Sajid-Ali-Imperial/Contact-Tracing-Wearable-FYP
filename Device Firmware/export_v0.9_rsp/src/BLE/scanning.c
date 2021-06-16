#include "scanning.h"


static struct bt_conn *default_conn;

#define SCAN_PREAMBLE_LEN 13
#define UID_LEN 8
#define TOTAL_SCAN_MSG_LEN 21
static uint8_t scan_preamble[] = {122, 101, 112, 104, 121, 114, 112, 114, 111, 106, 101, 99, 116};

static bool eir_found(struct bt_data *data, void *user_data)
{
	int i;
    if(data->data_len != TOTAL_SCAN_MSG_LEN)
    {
        return true;
    }
	printf("[AD]: %u data_len %u\n", data->type, data->data_len);


	uint8_t adv_msg_rec[TOTAL_SCAN_MSG_LEN];
	switch (data->type) 
	{
		case BT_DATA_SVC_DATA16:
			memcpy(&adv_msg_rec, data->data, TOTAL_SCAN_MSG_LEN);
			if(memcmp(&adv_msg_rec, &scan_preamble, SCAN_PREAMBLE_LEN) == 0)
			{
				// recording the rssi and use to estimate distance
				printf("SUCCESS !! \n");
			}
	}


	return true;
}

void device_found(const bt_addr_le_t *addr, s8_t rssi, u8_t type,
			 struct net_buf_simple *ad)
{
	char addr_str[BT_ADDR_LE_STR_LEN];

	// if (default_conn) {
	// 	return;
	// }

	// /* We're only interested in connectable events */
	// if (type != BT_LE_ADV_IND && type != BT_LE_ADV_DIRECT_IND) {
	// 	return;
	// }

	bt_addr_le_to_str(addr, addr_str, sizeof(addr_str));
	// printf("[DEVICE]: %s, AD evt type %u, AD data len %u, RSSI %i\n",
	//        addr_str, type, ad->len, rssi);

	bt_data_parse(ad, eir_found, (void *)addr);
	// if (type == BT_LE_ADV_IND || type == BT_LE_ADV_DIRECT_IND) {
	// 	bt_data_parse(ad, eir_found, (void *)addr);
	// }
}


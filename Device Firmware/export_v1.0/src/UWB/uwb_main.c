#include "uwb_main.h"


void uwb_main_thread(void *arg1, void *ard2, void *arg3)
{
    // struct data_item_t  *rx_data;

    // while(1)
    // {
    //     k_sleep(500);

    //     k_mutex_lock(&fifo_mutex, K_FOREVER);
    //     rx_data = k_fifo_get(&uwb_init_fifo, K_NO_WAIT);
    //     k_mutex_unlock(&fifo_mutex);
    //     if(rx_data != NULL)
    //     {
    //         printk("Pulled from fifo: %u \n", rx_data->eph_id);
    //     } 

    // }

    init_uwb();

    start_ds_init();
}
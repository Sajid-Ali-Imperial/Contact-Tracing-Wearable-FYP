#include "contact_data.h"

struct k_mutex fifo_mutex;       // mutex for accessing uwb fifo
struct k_fifo uwb_init_fifo;     // fifo used to push data to uwb thread

struct k_mutex contacts_mutex;   // mutex for accessing/edits contact tracing data
uint8_t *contact_data[CTD_MAX_TRACKING * BYTES_PER_CONTACT];           // ref to array holding contact tracing data
int num_contacts=0;                // total number of contact populated

void init_contact_data()
{   
    k_mutex_init(&fifo_mutex);
    k_fifo_init(&uwb_init_fifo);
    k_mutex_init(&contacts_mutex);
}

void estimage_rssi_distance(s8_t *rssi)
{

}


bool get_exposure()
{
    return true;
}
// uint8_t getIDIndex()
// {

// }

// uint8_t estimate_rssi_distance()
// {

// }
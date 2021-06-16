#ifndef CONTACT_DATA
#define CONTACT_DATA

#include <zephyr.h>

#define CTD_EPHID_LEN 8
#define CTD_TIME_LEN 8
#define CTD_EXP_LEN 1
#define BYTES_PER_CONTACT CTD_EPHID_LEN+CTD_TIME_LEN+CTD_EXP_LEN
#define CTD_MAX_TRACKING 1000       // corresponding to 20kb of memory allocation


extern struct k_mutex fifo_mutex;       // mutex for accessing uwb fifo
extern struct k_fifo uwb_init_fifo;     // fifo used to push data to uwb thread

extern struct k_mutex contacts_mutex;   // mutex for accessing/edits contact tracing data
extern uint8_t *contact_data[];           // ref to array holding contact tracing data
extern int num_contacts;                // total number of contact populated

// fifo data
struct data_item_t {
    unsigned int *fifo_reserved;   /* 1st word reserved for use by fifo */
    uint8_t eph_id;

};

void init_contact_data();

bool get_exposure();


#endif
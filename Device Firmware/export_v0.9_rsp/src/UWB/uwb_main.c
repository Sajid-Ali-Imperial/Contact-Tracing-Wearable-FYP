#include "uwb_main.h"


void uwb_main_thread(void *arg1, void *ard2, void *arg3)
{

    init_uwb();

    start_ds_rsp();
}
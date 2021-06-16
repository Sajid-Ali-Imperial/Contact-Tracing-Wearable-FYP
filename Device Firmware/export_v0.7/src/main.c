#include <zephyr.h>
#include "UWB/uwb.h"
#include "BLE/ble_main.h"
#include "I2C/accelerometer.h"

#define STACKSIZE 1024
char thread_stacks[3][STACKSIZE];
static struct k_thread threads[3];

#define APP_HEADER "\nDWM1001 & Zephyr\n"
#define APP_NAME "Example 5a - DS TWR INIT\n"
#define APP_VERSION "Version - 1.4.0\n"
#define APP_VERSION_NUM 0x010400
#define APP_LINE "=================\n"


void thread_2(void *arg1, void *ard2, void *arg3)
{
    while(1)
    {
        printk("Second Thread \n");
        Sleep(1500);
    }
}

int main(void)
{
    printk(APP_HEADER);
    printk(APP_NAME);
    printk(APP_VERSION);
    printk(APP_LINE);

    k_thread_create(&threads[0], &thread_stacks[0][0], STACKSIZE, ble_thread, 0, 0, 0, K_PRIO_COOP(4), 0, 0);
    //k_thread_create(&threads[1], &thread_stacks[1][0], STACKSIZE, uwb_thread, 0, 0, 0, K_PRIO_COOP(4), 0, 0);
    //k_thread_create(&threads[2], &thread_stacks[2][0], STACKSIZE, acc_thread, 0, 0, 0, K_PRIO_COOP(4), 0, 0);
    while(1)
    {
        //printk("Printed Something \n");
        //k_sleep(1000);
    }
}



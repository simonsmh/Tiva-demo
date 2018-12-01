#include <RTL.h>
#include "m4_common.h"

#define TICK_COUNT_ID     1

OS_TID id_main;
OS_TID id_systick;

void (*app_main_rtx)(void);

__task void task_systick(void)
{
    os_itv_set(1);
    while (true)
    {
        sys_tick_count ++;
        os_itv_wait();
    }
}

__task void task_main(void)
{
    id_main = os_tsk_self ();
    id_systick = os_tsk_create(task_systick, 0);
    
    while(true)
    {
        (*app_main_rtx)();
    }
}

void rtx_entry(void (*func))
{
    app_main_rtx = func;
    
    os_sys_init(task_main);
}

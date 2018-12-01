#include <xdc/std.h>

#include <xdc/runtime/Error.h>
#include <xdc/runtime/System.h>

#include <ti/sysbios/BIOS.h>

#include <ti/sysbios/knl/Task.h>

#include "m4_common.h"

// app main function pointer
void (*app_main_tirtos)(void);

/*
 *  ======== taskFxn ========
 */
void taskFxn(UArg a0, UArg a1)
{
    while (1)
    {
        (*app_main_tirtos)();
    }

    System_flush(); /* force SysMin output to console */
}

/*
 *  ======== main ========
 */
void tirtos_entry(void (*func))
{
    Task_Handle task;
    Error_Block eb;

    app_main_tirtos = func;

    System_printf("enter main()\n");

    Error_init(&eb);
    task = Task_create(taskFxn, NULL, &eb);
    if (task == NULL)
    {
        System_printf("Task_create() failed!\n");
        BIOS_exit(0);
    }

    BIOS_start();    /* does not return */
}

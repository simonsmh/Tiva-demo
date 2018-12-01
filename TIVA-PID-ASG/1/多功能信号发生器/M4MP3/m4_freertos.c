//#include <stdbool.h>
//#include <stdint.h>
//#include "inc/hw_memmap.h"
//#include "inc/hw_types.h"
//#include "driverlib/gpio.h"
//#include "driverlib/pin_map.h"
//#include "driverlib/rom.h"
//#include "driverlib/sysctl.h"
//#include "FreeRTOS.h"
//#include "task.h"
//#include "queue.h"
//#include "semphr.h"
//#include "m4_common.h"

//#define TASK_STACK              1024    // Stack size in words
//#define TASK_PRIORITY           1
//#define TASK_DELAY              250

//// app main function pointer
//void (*app_main_freertos)(void);

//// This hook is called by FreeRTOS when an stack overflow error is detected.
//void vApplicationStackOverflowHook(xTaskHandle *pxTask, char *pcTaskName)
//{
//    // This function can not return, so loop forever.  Interrupts are disabled
//    // on entry to this function, so no processor interrupts will interrupt
//    // this loop.
//    while(true)
//    {
//    }
//}

//static void task_main(void *pvParameters)
//{
//    portTickType ui32WakeTime;
//    
//    while (1)
//    {
//        (*app_main_freertos)();
//        
//        ui32WakeTime = xTaskGetTickCount();
//        
//        // Wait for the required amount of time.
//        vTaskDelayUntil(&ui32WakeTime, TASK_DELAY / portTICK_RATE_MS);
//    }
//}

//// Initialize FreeRTOS and start the initial set of tasks.
//void freertos_entry(void (*func))
//{
//    app_main_freertos = func;
//    
//    if(xTaskCreate(task_main, (signed portCHAR *)"TASKMAIN", TASK_STACK, NULL,
//                   tskIDLE_PRIORITY + TASK_PRIORITY, NULL) != pdTRUE)
//    {
//        while (true)
//            ;
//    }

//    // start the scheduler. This should not return.
//    vTaskStartScheduler();

//    // in case the scheduler returns for some reason, print an error and loop forever.
//    while(true)
//        ;
//}

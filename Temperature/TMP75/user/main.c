//*****************************************************************************
//
//
//
//*****************************************************************************

#include <stdint.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdarg.h>
#include <string.h>

#include "inc/hw_ints.h"
#include "inc/hw_memmap.h"
#include "inc/hw_types.h"
#include "inc/hw_gpio.h"
#include "inc/hw_i2c.h"
#include "inc/hw_sysctl.h"

#include "driverlib/sysctl.h"
#include "driverlib/systick.h"
#include "driverlib/gpio.h"
#include "driverlib/pin_map.h"
#include "driverlib/i2c.h"
#include "driverlib/fpu.h"

#include "utils/uartstdio.h"
#include "TMP75.h"

//*****************************************************************************
//
// Application function to capture ASSERT failures and other debug conditions.
//
//*****************************************************************************
#ifdef DEBUG
void
__error__(char *pcFilename, uint32_t ui32Line)
{
}
#endif

//*****************************************************************************
//
// Configure the UART and its pins.  This must be called before UARTprintf().
//
//*****************************************************************************
void
ConfigureUART(void)
{
    //
    // Enable the GPIO Peripheral used by the UART.
    //
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOA);

    //
    // Enable UART0
    //
    SysCtlPeripheralEnable(SYSCTL_PERIPH_UART0);

    //
    // Configure GPIO Pins for UART mode.
    //
    GPIOPinConfigure(GPIO_PA0_U0RX);
    GPIOPinConfigure(GPIO_PA1_U0TX);
    GPIOPinTypeUART(GPIO_PORTA_BASE, GPIO_PIN_0 | GPIO_PIN_1);

    //
    // Initialize the UART for console I/O.
    //
    UARTStdioConfig(0, 115200, SysCtlClockGet());
}


//*****************************************************************************
//
// 读取TMP75温度传感器数据
//
//*****************************************************************************
int
main(void)
{
	volatile float Temp;
	int Tempvalue;
	
    FPULazyStackingEnable();
    FPUEnable();

	
    //
    // 设置系统时钟
    //
    SysCtlClockSet(SYSCTL_SYSDIV_1 | SYSCTL_USE_OSC | SYSCTL_OSC_MAIN |
                   SYSCTL_XTAL_16MHZ);

    //
    // 初始化 UART.
    //
    ConfigureUART();

    //
    // 打印欢迎信息到终端
    //
    UARTprintf("TMP75 Example\n");

    TMP75Init();
	 
    while(1)
     {
  	Temp=temp_read();
		Tempvalue=(int)Temp;
		UARTprintf("Temp=%d\r\n",Tempvalue);
		SysCtlDelay(SysCtlClockGet() /10);			
     }
}

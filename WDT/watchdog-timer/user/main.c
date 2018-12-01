//*****************************************************************************
//
// 
// 
//
//*****************************************************************************
#include <stdint.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdarg.h>
#include <string.h>

#include "inc/hw_memmap.h"
#include "inc/hw_types.h"
#include "inc/hw_timer.h"
#include "inc/hw_ints.h"
#include "inc/hw_gpio.h"
#include "inc/hw_i2c.h"
#include "inc/hw_sysctl.h"
#include "driverlib/timer.h"
#include "driverlib/interrupt.h"
#include "driverlib/sysctl.h"
#include "driverlib/systick.h"
#include "driverlib/gpio.h"
#include "driverlib/pin_map.h"
#include "driverlib/ssi.h"
#include "driverlib/i2c.h"
#include "driverlib/udma.h"
#include "driverlib/fpu.h"
#include "driverlib/rom.h"
#include "driverlib/watchdog.h"

#include "utils/uartstdio.h"

//*****************************************************************************
//Attention
//TM4C123 NMI unlock - To those who want to use PF0 and PD7, be reminded that these pins defaults as NMI ! ! !
//
//*****************************************************************************

//*****************************************************************************
//
// The error routine that is called if the driver library encounters an error.
//
//*****************************************************************************
#ifdef DEBUG
void
__error__(char *pcFilename, unsigned long ulLine)
{
}
#endif

//*****************************************************************************
//
// Global variables
// 
// 
//
//*****************************************************************************
unsigned char flag;
long int count,fre_count;
volatile bool g_bFeedWatchdog = true;

//*****************************************************************************
//
// The interrupt application.  
// PF4 interrupt
// 
//
//*****************************************************************************
void Key2IntHandler(void)
{
	// Clear the  interrupt
  GPIOIntClear(GPIO_PORTF_BASE,GPIO_INT_PIN_4);
	
  //
  // Set the flag that tells the interrupt handler not to clear the
  // watchdog interrupt.
  //	
  g_bFeedWatchdog = false;
}

//*****************************************************************************
//
// The interrupt handler for the watchdog.  This feeds the dog (so that the
// processor does not get reset) and winks the LED connected to GPIO A4.
// 
//
//*****************************************************************************
void WatchdogIntHandler(void)
{
    //
    // If we have been told to stop feeding the watchdog, return immediately
    // without clearing the interrupt.  This will cause the system to reset
    // next time the watchdog interrupt fires.
    //
    if(!g_bFeedWatchdog)
    {
			 return;
    }

    //
    // Clear the watchdog interrupt.
    //
    WatchdogIntClear(WATCHDOG0_BASE);

if(GPIOPinRead(GPIO_PORTA_BASE, GPIO_PIN_4))
{
GPIOPinWrite(GPIO_PORTA_BASE, GPIO_PIN_4, 0<<4);
}
else
{
GPIOPinWrite(GPIO_PORTA_BASE, GPIO_PIN_4, 1<<4);
}

}



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
// The main application.  
// led2-PA4
// PF4---KEY2
//*****************************************************************************
int
main(void)
{
	
	  //
    // The FPU should be enabled because some compilers will use floating-
    // point registers, even for non-floating-point code. If the FPU is not
    // enabled this will cause a fault.  This also ensures that floating-
    // point operations could be added to this application and would work
    // correctly and use the hardware floating-point unit.  Finally, lazy
    // stacking is enabled for interrupt handlers.  This allows floating-
    // point instructions to be used within interrupt handlers, but at the
    // expense of extra stack usage.
    //
    FPUEnable();
    FPULazyStackingEnable();

    //
    // Set the system clock to run at 50MHz
    //
	  SysCtlClockSet(SYSCTL_SYSDIV_4|SYSCTL_USE_PLL|SYSCTL_XTAL_16MHZ|SYSCTL_OSC_MAIN);

    //
    // LED init
    //
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOA);
	  GPIOPinTypeGPIOOutput(GPIO_PORTA_BASE, GPIO_PIN_4);
    GPIOPinWrite(GPIO_PORTA_BASE,GPIO_PIN_4,1<<4);
		
    //
    // key2 init
    //		
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOF);
	  GPIOPinTypeGPIOInput(GPIO_PORTF_BASE, GPIO_PIN_4);	
		
		//
    // interrupt enable--key2
    //
    GPIOIntTypeSet(GPIO_PORTF_BASE,GPIO_PIN_4,GPIO_FALLING_EDGE);	//Sets the interrupt type for PF4
    GPIOIntEnable(GPIO_PORTF_BASE,GPIO_PIN_4);	    //Enables the specified GPIO PF4 interrupts.
		GPIOIntRegister(GPIO_PORTF_BASE, Key2IntHandler);  //Registers an interrupt handler for PF4		
		
		
    //
    // Enable watchdog 
    //
    SysCtlPeripheralEnable(SYSCTL_PERIPH_WDOG0);
		
		//
    // Enable processor interrupts.
    //
    IntMasterEnable();

    //
    // Enable the watchdog interrupt.
    //
    IntEnable(INT_WATCHDOG);

    //
    // Set the period of the watchdog timer.
    //
    WatchdogReloadSet(WATCHDOG0_BASE, SysCtlClockGet());

    //
    // Enable reset generation from the watchdog timer.
    //
    WatchdogResetEnable(WATCHDOG0_BASE);

    //
    // Enable the watchdog timer.
    //
    WatchdogEnable(WATCHDOG0_BASE);
		
    //
    // Initialize the UART as a console for text I/O.
    //
    ConfigureUART();
		
		UARTprintf("start run \n");
    while(1)
    {

    }
}

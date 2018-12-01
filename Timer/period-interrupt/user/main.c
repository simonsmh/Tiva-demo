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

//*****************************************************************************
//
// The interrupt application.  
// Timer interrupt
// 
//
//*****************************************************************************
void Timer0IntHandler(void)
{
	// Clear the timer interrupt
	TimerIntClear(TIMER0_BASE, TIMER_TIMA_TIMEOUT);

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
// The main application.  
// Blink the led
// led2-PA4
//
//*****************************************************************************
int
main(void)
{
	  	uint32_t ui32Period;
	
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
    // Set the system clock to run at 40MHz
    //
	  SysCtlClockSet(SYSCTL_SYSDIV_5|SYSCTL_USE_PLL|SYSCTL_XTAL_16MHZ|SYSCTL_OSC_MAIN);

    //
    // LED init
    //
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOA);
	  GPIOPinTypeGPIOOutput(GPIO_PORTA_BASE, GPIO_PIN_4);
    GPIOPinWrite(GPIO_PORTA_BASE,GPIO_PIN_4,1<<4);
		
    //
    // Timer init
    //		
	  SysCtlPeripheralEnable(SYSCTL_PERIPH_TIMER0);
	  TimerConfigure(TIMER0_BASE, TIMER_CFG_PERIODIC);


    //
    // Timer delay period 
    //	
	  ui32Period = (SysCtlClockGet());
	  TimerLoadSet(TIMER0_BASE, TIMER_A, ui32Period -1);	
		
		//
    // interrupt enable--timer0A
    //
	  IntEnable(INT_TIMER0A);  //Enables a timer interrupt
	  TimerIntEnable(TIMER0_BASE, TIMER_TIMA_TIMEOUT);   //Enables individual timer interrupt sources
	  IntMasterEnable();  //Enables the processor interrupt

		//
    // timer0 enable--This will start the timer and interrupts
    //		
		TimerEnable(TIMER0_BASE, TIMER_A);
		
    while(1)
    {

    }
}










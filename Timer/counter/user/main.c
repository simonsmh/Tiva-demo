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
//*****************************************************************************
//
// The interrupt application.  
// Timer0 interrupt
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
  fre_count=(TimerValueGet(TIMER2_BASE, TIMER_A));
  UARTprintf("%d\n", fre_count);
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
// counter
// led2-PA4
// PF4---T2CCP0 Timer 2 timer A
//*****************************************************************************
int
main(void)
{
	  	uint32_t ui32Period;

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
    // Timer init
    //		
	  SysCtlPeripheralEnable(SYSCTL_PERIPH_TIMER0);
	  TimerConfigure(TIMER0_BASE, TIMER_CFG_PERIODIC);
		
    //
    // Timer period 
    //	
	  ui32Period = (SysCtlClockGet()/10); //100ms
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
		
    //
    // the general purpose timer and CCP pin using.
    // PF4---T2CCP0
    //
    SysCtlPeripheralEnable(SYSCTL_PERIPH_TIMER2);
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOF);
		
    //
    // Configure PF4 as the CCP0 pin for timer 2.
    //
    GPIOPinTypeTimer(GPIO_PORTF_BASE, GPIO_PIN_4);
    GPIOPinConfigure(GPIO_PF4_T2CCP0);		
		
    //
    // Set the pin to use the internal pull-up. this can be removed for there is an extern pull-up
    //
    GPIOPadConfigSet(GPIO_PORTF_BASE, GPIO_PIN_4,GPIO_STRENGTH_2MA, GPIO_PIN_TYPE_STD_WPU);	

    //
    // Configure the timers in downward edge count mode.
    //
    // TODO: Modify this to configure the specific general purpose
    // timer you are using.  The timer choice is intimately tied to
    // the pin whose edges you want to capture because specific CCP
    // pins are connected to specific timers.
    //
    TimerConfigure(TIMER2_BASE, (TIMER_CFG_SPLIT_PAIR |TIMER_CFG_A_CAP_COUNT_UP));
    TimerControlEvent(TIMER2_BASE, TIMER_A, TIMER_EVENT_POS_EDGE);
    TimerLoadSet(TIMER2_BASE, TIMER_A, 10);

    //
    // Enable the timer.
		//
    TimerEnable(TIMER2_BASE, TIMER_A);		
		
    //
    // Initialize the UART as a console for text I/O.
    //
    ConfigureUART();
		
    while(1)
    {

    }
}

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
unsigned char pulse;
unsigned char t;
//*****************************************************************************
//
// The interrupt application.  
// Timer interrupt
// 
//
//*****************************************************************************
void Timer1IntHandler(void)
{
	// Clear the timer interrupt
	TimerIntClear(TIMER1_BASE, TIMER_CAPB_EVENT);

  flag=1;
}

//*****************************************************************************
//
// The main application.  
// PF3---T1CCP1
// Breathe light
//
//*****************************************************************************
int
main(void)
{
	    pulse=40;


    //
    // Set the system clock to run at 40MHz
    //
    SysCtlClockSet(SYSCTL_SYSDIV_5 | SYSCTL_USE_OSC | SYSCTL_OSC_MAIN | SYSCTL_XTAL_16MHZ);
		
    //
    // Timer init
    //		
	  SysCtlPeripheralEnable(SYSCTL_PERIPH_TIMER1);
		SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOF);
	  GPIOPinConfigure(GPIO_PF3_T1CCP1);

    //
    // Configure the ccp settings for CCP pin
    //
    GPIOPinTypeTimer(GPIO_PORTF_BASE, GPIO_PIN_3);
		
		//
    // Configure Timer1B as a 16-bit periodic timer.
    //
    TimerConfigure(TIMER1_BASE, TIMER_CFG_SPLIT_PAIR | TIMER_CFG_B_PWM);
		
		//
    // interrupt enable--Timer1B
    //
	  IntEnable(INT_TIMER1B);  //Enables a timer interrupt
	  TimerIntEnable(TIMER1_BASE, TIMER_CAPB_EVENT);   //Enables individual timer interrupt sources
	  IntMasterEnable();  //Enables the processor interrupt		
		
		//
    // Set the Timer1B load value to 50000.  
    // loadset--->matchset low,matchset----0 high
    //
    TimerLoadSet(TIMER1_BASE, TIMER_B, 100*500);
		
		//
    // Set the Timer1B match value 
    //
    TimerMatchSet(TIMER1_BASE, TIMER_B,(100-pulse)*500);	
		
		//
    // Enable Timer1B.
    //
    TimerEnable(TIMER1_BASE, TIMER_B);
		

    while(1)
    {
			
			if(flag==1)
			{
			  flag=0;
				
				if(t==0 && pulse<=40)
				{
				  pulse--;
					if(pulse==0)
					{t=1;}
					
				}
				
				if(t==1 && pulse<=40)
				{
					pulse++;
					if(pulse==40)
				  {t=0;} 

				}
					
			  TimerMatchSet(TIMER1_BASE, TIMER_B,(100-pulse)*500);
				SysCtlDelay(10000);
			}

    }
}

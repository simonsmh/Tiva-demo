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
#include "inc/hw_ints.h"
#include "inc/hw_gpio.h"
#include "inc/hw_sysctl.h"
#include "inc/hw_comp.h"
#include "driverlib/interrupt.h"
#include "driverlib/sysctl.h"
#include "driverlib/systick.h"
#include "driverlib/gpio.h"
#include "driverlib/pin_map.h"
#include "driverlib/rom.h"
#include "driverlib/comp.h"


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
// global variable
//
//*****************************************************************************
unsigned char flag;

//*****************************************************************************
//
// interrupt application
//
//*****************************************************************************
void Comparator0IntHandler(void)
{
     // Clear the Comparator interrupt
     ComparatorIntClear(COMP_BASE, 0);
	
     flag=1;
}

//*****************************************************************************
//
// The main application.  
// BLINK-LED
// LED-PA4
//
//*****************************************************************************
int
main(void)
{
    //
    // Set the system clock to run at 50MHz
    //
    SysCtlClockSet(SYSCTL_SYSDIV_4 | SYSCTL_USE_PLL | SYSCTL_OSC_MAIN |
                       SYSCTL_XTAL_16MHZ);
	
    //
    // LED init
    //
		SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOF);
	  //PF0½âËø
		HWREG(GPIO_PORTF_BASE + GPIO_O_LOCK) = GPIO_LOCK_KEY;
    HWREG(GPIO_PORTF_BASE + GPIO_O_CR) |= 0x01;
    HWREG(GPIO_PORTF_BASE + GPIO_O_LOCK) = 0;	
	  GPIOPinTypeGPIOOutput(GPIO_PORTF_BASE, GPIO_PIN_0);
    GPIOPinWrite(GPIO_PORTF_BASE,GPIO_PIN_0,1<<0);//ÉèÖÃÀ¶É«LEDµÆ³õÊ¼×´Ì¬ÎªÃð			
	
	  SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOA);
	  GPIOPinTypeGPIOOutput(GPIO_PORTA_BASE, GPIO_PIN_4);
		GPIOPinWrite(GPIO_PORTA_BASE,GPIO_PIN_4,1<<4);
	
    //
    // enable comparator 0
    //		

		SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOC);
		
		SysCtlPeripheralEnable(SYSCTL_PERIPH_COMP0);		

	  GPIOPinConfigure(GPIO_PF0_C0O);	
    GPIOPinTypeComparator(GPIO_PORTC_BASE, GPIO_PIN_6);         // assign pin type
    GPIOPinTypeComparator(GPIO_PORTC_BASE, GPIO_PIN_7);
		//
    // Configure the internal voltage reference.
    //
    ComparatorRefSet(COMP_BASE, COMP_REF_1_65V);
		
    //
    // Configure comparator 0.
    //
    ComparatorConfigure(COMP_BASE, 0,(COMP_TRIG_NONE | COMP_INT_BOTH | COMP_ASRCP_REF | COMP_OUTPUT_NORMAL));
		
    //
    // interrupt enable
    //

		IntEnable(INT_COMP0);
    ComparatorIntEnable(COMP_BASE, 0);
    IntMasterEnable();

    while(1)
    {
		
      if(flag==1)
			{
        flag=0;				
		    //
        // Read the comparator output value.
        //
        if(ComparatorValueGet(COMP_BASE, 0)==1)
				{
				  GPIOPinWrite(GPIO_PORTA_BASE,GPIO_PIN_4,1<<4);
				}
				else
				{
				  GPIOPinWrite(GPIO_PORTA_BASE,GPIO_PIN_4,0<<4);
				}
      }

    }
}

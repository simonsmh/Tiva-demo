//*****************************************************************************
//
// 
// 
//
//*****************************************************************************
#include <stdint.h>
#include <stdint.h>
#include <stdbool.h>

#include "inc/hw_types.h"
#include "inc/hw_memmap.h"
#include "driverlib/sysctl.h"
#include "driverlib/pin_map.h"
#include "driverlib/debug.h"
#include "driverlib/hibernate.h"
#include "driverlib/gpio.h"
#include "time.h"
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


int main(void)
{
	//
  // Set the system clock to run at 40MHz
  //
	SysCtlClockSet(SYSCTL_SYSDIV_5|SYSCTL_USE_PLL|SYSCTL_XTAL_16MHZ|SYSCTL_OSC_MAIN);

	//
  // LED init
  //
  SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOA);
	GPIOPinTypeGPIOOutput(GPIO_PORTA_BASE, GPIO_PIN_4);
  GPIOPinWrite(GPIO_PORTA_BASE,GPIO_PIN_4,0<<4);

	//
	// Hibernate Configuration
	//
	SysCtlPeripheralEnable(SYSCTL_PERIPH_HIBERNATE);//enable the hibernation module
	HibernateEnableExpClk(SysCtlClockGet());//defines the clock supplied to the hibernation module
	HibernateGPIORetentionEnable();//enables the GPIO pin state unchangeable during hibernation
	SysCtlDelay(64000000);//delay 4 seconds to observe the LED
	//
	//configure the RTC wake-up parameters; 
	//reset the RTC to 0, turn the RTC on and set the wake up time for 5 seconds in the future.
	//
	HibernateRTCSet(0);
	HibernateRTCEnable();
	HibernateRTCMatchSet(0,5);
	HibernateWakeSet(HIBERNATE_WAKE_PIN | HIBERNATE_WAKE_RTC);
	
  GPIOPinWrite(GPIO_PORTA_BASE,GPIO_PIN_4,1<<4);

	HibernateRequest();//hibernation mode
	while(1)
	{
	}
}

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
#include <time.h>

#include "inc/hw_memmap.h"
#include "inc/hw_types.h"
#include "inc/hw_timer.h"
#include "inc/hw_ints.h"
#include "inc/hw_gpio.h"
#include "inc/hw_i2c.h"
#include "inc/hw_sysctl.h"
#include "inc/hw_comp.h"
#include "driverlib/timer.h"
#include "driverlib/interrupt.h"
#include "driverlib/hibernate.h"
#include "driverlib/sysctl.h"
#include "driverlib/systick.h"
#include "driverlib/gpio.h"
#include "driverlib/pin_map.h"
#include "driverlib/ssi.h"
#include "driverlib/i2c.h"
#include "driverlib/udma.h"
#include "driverlib/fpu.h"
#include "driverlib/rom.h"
#include "driverlib/comp.h"

#include "utils/ustdlib.h"
#include "utils/uartstdio.h"
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

void rtc_init(void)
{
    uint32_t rtc_tick;
    
    struct tm rtc_time; 
    
    SysCtlPeripheralEnable(SYSCTL_PERIPH_HIBERNATE);
    
    HibernateEnableExpClk(SysCtlClockGet());
		
    HibernateClockConfig(HIBERNATE_OSC_LOWDRIVE);
    HibernateRTCEnable();
    
    // current time:
    rtc_time.tm_year    = 114;
    rtc_time.tm_mon     = 7;
    rtc_time.tm_mday    = 21;
    rtc_time.tm_hour    = 1;
    rtc_time.tm_min     = 0;
    rtc_time.tm_sec     = 0;
    
    rtc_tick = umktime(&rtc_time);
    HibernateRTCSet(rtc_tick);
    
//    memset(&rtc_time, 0, sizeof(struct tm));
    
}

void rtc_get(struct tm *rtc_time)
{
    uint32_t rtc_tick;
    
    rtc_tick = HibernateRTCGet();
    ulocaltime(rtc_tick, rtc_time);
}

//*****************************************************************************
//
// The main application.  
// HIB-RTC
// LED-PA4
//
//*****************************************************************************
int
main(void)
{
    struct tm rtc_now;
    
    int rtc_last_sec;
	
  	//
    // Set the system clock to run at 50MHz
    //
    SysCtlClockSet(SYSCTL_SYSDIV_4 | SYSCTL_USE_PLL | SYSCTL_OSC_MAIN |
                       SYSCTL_XTAL_16MHZ);
    rtc_init();
    //
    // LED init
    //
		SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOA);
	  GPIOPinTypeGPIOOutput(GPIO_PORTA_BASE, GPIO_PIN_4);
		GPIOPinWrite(GPIO_PORTA_BASE,GPIO_PIN_4,1<<4);

    //
    // Initialize the UART as a console for text I/O.
    //
    ConfigureUART();    
		
    while(1)
    {
			
        rtc_get(&rtc_now);
			
        if (rtc_last_sec != rtc_now.tm_sec)
        {
            GPIOPinWrite(GPIO_PORTA_BASE,GPIO_PIN_4,0<<4);
            rtc_last_sec = rtc_now.tm_sec;        
					  UARTprintf("%d-%d-%d", rtc_now.tm_year + 1900, rtc_now.tm_mon, rtc_now.tm_mday);
			      UARTprintf("%02d:%02d:%02d\n", rtc_now.tm_hour, rtc_now.tm_min, rtc_now.tm_sec);
        }
        else
        {
            GPIOPinWrite(GPIO_PORTA_BASE,GPIO_PIN_4,1<<4);
        }				

    }
}
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
#include "inc/hw_timer.h"
#include "inc/hw_ints.h"
#include "inc/hw_gpio.h"
#include "inc/hw_i2c.h"
#include "inc/hw_sysctl.h"
#include "inc/hw_adc.h"
#include "inc/hw_ssi.h"
#include "driverlib/adc.h"
#include "driverlib/rom.h"
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


//
// A boolean that is set when a TMP75 command has completed.
//
volatile bool g_bTMP75Done;


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

void dac_writedata(uint16_t ui32Data)
{
	
GPIOPinWrite(GPIO_PORTF_BASE,GPIO_PIN_1,0<<1);
SSIDataPut(SSI1_BASE, ui32Data);
while(SSIBusy(SSI1_BASE));
GPIOPinWrite(GPIO_PORTF_BASE,GPIO_PIN_1,1<<1);

}

//*****************************************************************************
//
// Main 'C' Language entry point.
//
//*****************************************************************************
int
main(void)
{
	
    FPULazyStackingEnable();
    FPUEnable();

	
    //
    // Setup the system clock to run at 40 Mhz from PLL with crystal reference
    //
    SysCtlClockSet(SYSCTL_SYSDIV_5 | SYSCTL_USE_PLL | SYSCTL_OSC_MAIN | SYSCTL_XTAL_16MHZ);
	
	  SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOF);
	  GPIOPinTypeGPIOOutput(GPIO_PORTF_BASE, GPIO_PIN_1);
    GPIOPinWrite(GPIO_PORTF_BASE,GPIO_PIN_1,1<<1);
	
	
	  SysCtlPeripheralEnable(SYSCTL_PERIPH_SSI1);
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOD);
    GPIOPinConfigure(GPIO_PD0_SSI1CLK);
    GPIOPinConfigure(GPIO_PD3_SSI1TX);
    GPIOPinTypeSSI(GPIO_PORTD_BASE,GPIO_PIN_3|GPIO_PIN_0);
    SSIConfigSetExpClk(SSI1_BASE, SysCtlClockGet(), SSI_FRF_MOTO_MODE_1, SSI_MODE_MASTER, 10000, 16);
    SSIEnable(SSI1_BASE);
	
	
	
	
	
	
	
	
    //
    // Initialize the UART.
    //
    ConfigureUART();

    //
    // Print the welcome message to the terminal.
    //
    UARTprintf("DAC Example\n");

	 
    while(1)
     {
       dac_writedata(0x07FF);
		   SysCtlDelay(SysCtlClockGet() /10);			
     }
}



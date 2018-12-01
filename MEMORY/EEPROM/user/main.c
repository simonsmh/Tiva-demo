//*****************************************************************************
//
// 
// 
//
//*****************************************************************************
#include <stdint.h>
#include <stdbool.h>

#include "inc/hw_types.h"
#include "inc/hw_memmap.h"
#include "driverlib/sysctl.h"
#include "driverlib/pin_map.h"
#include "driverlib/debug.h"
#include "driverlib/gpio.h"
#include "driverlib/flash.h"
#include "driverlib/eeprom.h"


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
//Attention
//TM4C123 NMI unlock - To those who want to use PF0 and PD7, be reminded that these pins defaults as NMI ! ! !
//
//*****************************************************************************

//*****************************************************************************
//
// The main application. ----eeprom 
//
//*****************************************************************************
int
main(void)
{
	uint32_t pui32Data[2];
	uint32_t pui32Read[2];
	pui32Data[0] = 0x12345678;
	pui32Data[1] = 0x56789abc;	
	
    //
    //设置系统时钟为40MHz
    //
    SysCtlClockSet(SYSCTL_SYSDIV_5 | SYSCTL_USE_PLL | SYSCTL_OSC_MAIN |
                       SYSCTL_XTAL_16MHZ);
	
	
	  SysCtlPeripheralEnable(SYSCTL_PERIPH_EEPROM0);//打开eeprom外设
	  EEPROMInit();                                 //初始化eeprom
	  EEPROMMassErase();                            //擦除eeprom，非必要，此处擦除仅在于方便观察实验结果
	  EEPROMRead(pui32Read, 0x0, sizeof(pui32Read));//将擦除后的值读入pui32Read
	  EEPROMProgram(pui32Data, 0x0, sizeof(pui32Data));//将值写入eeprom
	  EEPROMRead(pui32Read, 0x0, sizeof(pui32Read));//读取写入的值	
		
    while(1)
    {
			
    }
}


#include <stdarg.h>
#include <string.h>
#include <stdbool.h>
#include <stdint.h>
#include <string.h>

#include "m4_pinmux.h"
#include "m4_common.h"
#include "m4_config.h"

#include "inc/hw_memmap.h"
#include "driverlib/gpio.h"
#include "driverlib/pin_map.h"
#include "driverlib/sysctl.h"
#include "driverlib/systick.h"
#include "driverlib/ssi.h"
#include "driverlib/rom.h"

int main(void)
{
	SysCtlClockSet(SYSCTL_SYSDIV_3 | SYSCTL_USE_PLL | SYSCTL_XTAL_16MHZ | SYSCTL_OSC_MAIN);

	PortFunctionInit();	
	
	lcd_init();
	lcd_printf(1,1,"*TI��ѧ�ƻ�*");
	lcd_printf(2,2,"�Ϻ����е��");
	lcd_printf(3,3,"�ڴ�ʵ���");
	while(1)
	{
		
	}
}

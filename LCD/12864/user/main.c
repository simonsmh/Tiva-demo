#include <stdbool.h>
#include <stdint.h>
#include <string.h>

#include "inc/hw_memmap.h"
#include "driverlib/gpio.h"
#include "driverlib/pin_map.h"
#include "driverlib/sysctl.h"
#include "driverlib/systick.h"
#include "driverlib/ssi.h"
#include "uc1701.h"

//�������
char HZ[]={
 0x20,0x24,0x2C,0x35,0xE6,0x34,0x2C,0x24,0x00,0xFC,0x24,0x24,0xE2,0x22,0x22,0x00,
 0x21,0x11,0x4D,0x81,0x7F,0x05,0x59,0x21,0x18,0x07,0x00,0x00,0xFF,0x00,0x00,0x00,
 0x40,0x20,0x10,0x0C,0xE3,0x22,0x22,0x22,0xFE,0x22,0x22,0x22,0x22,0x02,0x00,0x00,
 0x04,0x04,0x04,0x04,0x07,0x04,0x04,0x04,0xFF,0x04,0x04,0x04,0x04,0x04,0x04,0x00,
 0x70,0x00,0xFF,0x08,0xB0,0x88,0x88,0x88,0xFF,0x88,0x88,0x88,0xF8,0x80,0x80,0x00,
 0x00,0x00,0xFF,0x00,0x80,0x40,0x30,0x0E,0x01,0x06,0x08,0x30,0x60,0xC0,0x40,0x00,
 0x00,0x00,0x40,0xFC,0x44,0x44,0x44,0x46,0xFA,0x42,0x43,0x43,0x42,0x40,0x00,0x00,
 0x00,0x20,0x18,0x0C,0x07,0x12,0x20,0x40,0x3F,0x00,0x00,0x02,0x0C,0x38,0x10,0x00
 };
int
main(void)
{
	
    SysCtlClockSet(SYSCTL_SYSDIV_1 | SYSCTL_USE_OSC | SYSCTL_OSC_MAIN |
                   SYSCTL_XTAL_16MHZ);
	
    UC1701Init(60000);
    UC1701Clear();
    UC1701CharDispaly(0, 1, "I love tiva-pb?");
    UC1701CharDispaly(1, 1, "Yes,I love!");
    UC1701DisplayN(2,1,5201314);
	  UC1701ChineseDispaly(3, 1, 4,HZ);
    SysCtlDelay(1000000);
	
    while(1)
    {

    }
}

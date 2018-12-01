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
long int count;

//*****************************************************************************
//
// The interrupt application.  
// key2-PF4
// 
//
//*****************************************************************************
void Key2IntHandler()
  {
    flag=1;
    GPIOIntClear(GPIO_PORTF_BASE,GPIO_INT_PIN_4);
  }


//*****************************************************************************
//
// The main application.  
// Blink the GLED
// led2-PA4
//
//*****************************************************************************
int
main(void)
{
	  unsigned char key_flag;

    //
    // Set the system clock to run at 50MHz
    //
    SysCtlClockSet(SYSCTL_SYSDIV_4 | SYSCTL_USE_PLL | SYSCTL_OSC_MAIN |
                       SYSCTL_XTAL_16MHZ);

    //
    // LED init
    //
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOA);
	  GPIOPinTypeGPIOOutput(GPIO_PORTA_BASE, GPIO_PIN_4);
    GPIOPinWrite(GPIO_PORTA_BASE,GPIO_PIN_4,1<<4);
		
	
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOF);
	  //PF0解锁
	  HWREG(GPIO_PORTF_BASE + GPIO_O_LOCK) = GPIO_LOCK_KEY;
    HWREG(GPIO_PORTF_BASE + GPIO_O_CR) |= 0x01;
    HWREG(GPIO_PORTF_BASE + GPIO_O_LOCK) = 0;	
		GPIOPinTypeGPIOOutput(GPIO_PORTF_BASE, GPIO_PIN_0);
    GPIOPinWrite(GPIO_PORTF_BASE,GPIO_PIN_0,1<<0);//设置蓝色LED灯初始状态为灭
		
		//
    // key2 init
    //	
	  GPIOPinTypeGPIOInput(GPIO_PORTF_BASE, GPIO_PIN_4);	
		
		//
    // interrupt enable--key2
    //
    GPIOIntTypeSet(GPIO_PORTF_BASE,GPIO_PIN_4,GPIO_FALLING_EDGE);	//设置PF4中断类型
    GPIOIntEnable(GPIO_PORTF_BASE,GPIO_PIN_4);	    //使能PF4中断
		GPIOIntRegister(GPIO_PORTF_BASE, Key2IntHandler);  //为PF4注册一个中断处理句柄		
		
    while(1)
    {
			
			if(flag==1)
			{
			  count++;
				key_flag=count%2;
				
				if(key_flag==1)
				{GPIOPinWrite(GPIO_PORTA_BASE,GPIO_PIN_4,0<<4);}
				
				if(key_flag==0)
				{GPIOPinWrite(GPIO_PORTA_BASE,GPIO_PIN_4,1<<4);}
			
			  flag=0;	  
			}
        __asm(
            "MOV r0,r0;"
            "BX LR"
        );
    }
}

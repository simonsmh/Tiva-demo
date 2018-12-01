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
#include "inc/hw_gpio.h"
#include "inc/hw_sysctl.h"
#include "driverlib/sysctl.h"
#include "driverlib/gpio.h"
#include "driverlib/fpu.h"


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
// The main application.  
// BLINK-LED
// BLED--PF0 GLED-PA4 RLED-PD6
//
//*****************************************************************************
int
main(void)
{
    int i,j;

    //
    //设置系统时钟为50MHz
    //
    SysCtlClockSet(SYSCTL_SYSDIV_4 | SYSCTL_USE_PLL | SYSCTL_OSC_MAIN |
                       SYSCTL_XTAL_16MHZ);

    //
    // LED灯初始化
    //
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOF);
	  //PF0解锁
	  HWREG(GPIO_PORTF_BASE + GPIO_O_LOCK) = GPIO_LOCK_KEY;
    HWREG(GPIO_PORTF_BASE + GPIO_O_CR) |= 0x01;
    HWREG(GPIO_PORTF_BASE + GPIO_O_LOCK) = 0;		
		
	  GPIOPinTypeGPIOOutput(GPIO_PORTF_BASE, GPIO_PIN_0);
    GPIOPinWrite(GPIO_PORTF_BASE,GPIO_PIN_0,1<<0);//设置蓝色LED灯初始状态为灭
		
		SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOA);
	  GPIOPinTypeGPIOOutput(GPIO_PORTA_BASE, GPIO_PIN_4);
    GPIOPinWrite(GPIO_PORTA_BASE,GPIO_PIN_4,1<<4);//设置绿色LED灯初始状态为灭
		
		SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOD);
	  GPIOPinTypeGPIOOutput(GPIO_PORTD_BASE, GPIO_PIN_6);
    GPIOPinWrite(GPIO_PORTD_BASE,GPIO_PIN_6,1<<6);//设置红色LED灯初始状态为灭
		
    while(1)
    {
			
	   	GPIOPinWrite(GPIO_PORTF_BASE,GPIO_PIN_0,0<<0);//设置蓝色LED灯状态为亮
	for(i=1000;i>0;i--)
			for(j=600;j>0;j--);                          //软件延时
		GPIOPinWrite(GPIO_PORTF_BASE,GPIO_PIN_0,1<<0);//设置蓝色LED灯状态为灭
	for(i=1000;i>0;i--)
			for(j=600;j>0;j--);                          //软件延时
			
	   	GPIOPinWrite(GPIO_PORTA_BASE,GPIO_PIN_4,0<<4);//设置绿色LED灯状态为亮
	for(i=1000;i>0;i--)
			for(j=600;j>0;j--);                          //软件延时
		GPIOPinWrite(GPIO_PORTaA_BASE,GPIO_PIN_4,1<<4);//设置绿色LED灯状态为灭
	for(i=1000;i>0;i--)
			for(j=600;j>0;j--);                          //软件延时

	   	GPIOPinWrite(GPIO_PORTD_BASE,GPIO_PIN_6,0<<6);//设置红色LED灯状态为亮
	for(i=1000;i>0;i--)
			for(j=600;j>0;j--);                          //软件延时
		GPIOPinWrite(GPIO_PORTD_BASE,GPIO_PIN_6,1<<6);//设置红色LED灯状态为灭
	for(i=1000;i>0;i--)
			for(j=600;j>0;j--);                          //软件延时					

    }
}



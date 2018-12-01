
/***************************
//UART_App
//
****************************/
#include "Head.h"
#include <stdarg.h>
#include <string.h>

#include "inc/hw_memmap.h"
#include "driverlib/debug.h"

#include "driverlib/pin_map.h"
#include "driverlib/rom.h"

#include "driverlib/systick.h"

#include "m4_pinmux.h"
#include "m4_common.h"
#include "m4_config.h"

#define MAX_UART_NUM        1       // uart0 & uart1

uint8_t count = 0;
//int32_t Receive;
int32_t Receive,Send,receive;
unsigned char keypressed_flag;

void My_delay(uint32_t ms)
{
	uint32_t count;
	count = SysCtlClockGet() /1000;
	SysCtlDelay(ms*count);
}


void uart_init(uint32_t base, const uint32_t baud)
{
	
		GPIOPinConfigure(GPIO_PB0_U1RX);
    GPIOPinConfigure(GPIO_PB1_U1TX);
		GPIOPinTypeUART(GPIO_PORTB_BASE, GPIO_PIN_0 | GPIO_PIN_1);
    // config the UART for 115200, 8-N-1 operation.
    UARTConfigSetExpClk(UART1_BASE, SysCtlClockGet(), 115200, (UART_CONFIG_WLEN_8 | UART_CONFIG_STOP_ONE | UART_CONFIG_PAR_NONE));

	while(UARTCharsAvail(base))
	{
	}

}

void uart_init0(uint32_t base, const uint32_t baud)
{
	
		GPIOPinConfigure(GPIO_PA0_U0RX);
    GPIOPinConfigure(GPIO_PA1_U0TX);
		GPIOPinTypeUART(GPIO_PORTA_BASE, GPIO_PIN_0 | GPIO_PIN_1);
    // config the UART for 115200, 8-N-1 operation.
    UARTConfigSetExpClk(UART0_BASE, SysCtlClockGet(), 115200, (UART_CONFIG_WLEN_8 | UART_CONFIG_STOP_ONE | UART_CONFIG_PAR_NONE));

	while(UARTCharsAvail(base))
	{
	}

}


//this code is the handler function for key2 
//void Key2IntHandler()
//{

//	keypressed_flag=1;
//  GPIOIntClear(GPIO_PORTF_BASE,GPIO_INT_PIN_4);
//}
//void key2init()
//{
// SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOF);
// GPIOPinTypeGPIOInput(GPIO_PORTF_BASE,GPIO_PIN_4);	 //PF4
// GPIOIntRegister(GPIO_PORTF_BASE, Key2IntHandler);
// GPIOIntTypeSet(GPIO_PORTF_BASE,GPIO_PIN_4,GPIO_FALLING_EDGE);	
// GPIOIntEnable(GPIO_PORTF_BASE,GPIO_PIN_4);	    //Enable Key2 handler
//}
//void ledinit()
//{

// SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOF);
// GPIOPinTypeGPIOOutput(GPIO_PORTF_BASE,GPIO_PIN_1);//led
//	
//}

int main(void)
{
//	uint8_t alphabet = 'A';

	SysCtlClockSet(SYSCTL_SYSDIV_3 | SYSCTL_USE_PLL | SYSCTL_XTAL_16MHZ | SYSCTL_OSC_MAIN);

	PortFunctionInit();	
	


//SysCtlPeripheralEnable(SYSCTL_PERIPH_UART0);
//SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOA);	
//GPIOPinConfigure(GPIO_PA1_U0TX);
//GPIOPinTypeUART(GPIO_PORTA_BASE, GPIO_PIN_0 | GPIO_PIN_1);	
//UARTConfigSetExpClk(UART0_BASE, SysCtlClockGet(), 115200,(UART_CONFIG_WLEN_8 | UART_CONFIG_STOP_ONE | UART_CONFIG_PAR_NONE));	
//UARTEnable(UART0_BASE);	
//UARTIntEnable(UART0_BASE, UART_INT_RX | UART_INT_RT);
	
uart_init(UART1_BASE,115200);	
//uart_init0(UART0_BASE,115200);
	
	
	
	
//  ledinit();
//	key2init();
	lcd_init();
	//lcd_printf(0,0,"UART T<---->R..");
	lcd_printf(0,0,"中国");
	while(1)
	{
		
//		while(!UARTCharsAvail(UART0_BASE));
//		while(UARTCharGetNonBlocking(UART0_BASE));
//		UARTCharPut(UART0_BASE, );
		
		
		
		
		
//		if(uart0收到uart0数据)
//		{数据送给PC 回显}
//		
//		if(uart1收到uart0数据)
//		{无线发送}
//		
//		if(uart1收到uart1(无线)数据)
//		{uart1把数据送给uart0}
//		
//		if(uart0收到uart1数据)
//		{数据送给PC}
		


		
		
		
//		Receive = UARTCharGet(UART0_BASE);
//		if(Receive!=0)
//		{
//		  UARTCharPut(UART1_BASE, Receive);					//send data to UART	
//		}
//	while(UARTCharsAvail(UART1_BASE))
//	{
//	}		
		receive = UARTCharGet(UART1_BASE);		
		if(receive!=0)
		{
//		  UARTCharPut(UART0_BASE, receive);					//send data to UART	
			lcd_printf(0,1,"Receive=%c",receive);
		}
		
		
		
		
		
		
//		if(keypressed_flag==1)
//		{
//		  keypressed_flag=0;
//			UARTCharPut(UART1_BASE, alphabet);					//send data to UART		
//      Send=UARTCharGet(UART1_BASE);
//       if(Send!=0x00)
//			 {
//			  lcd_printf(0,1,"Send=%c",Send); 
//			 }
//			alphabet ++;
//		}
		
//		Receive = UARTCharGet(UART1_BASE);
//		
//		if(Receive!=0)
//		{
//		  lcd_printf(0,2,"Receive=%c",Receive);
//		}
		
//			lcd_printf(0,1,"Send=%c",alphabet);
//			UARTCharPut(UART0_BASE, 0x5b);					//send data to UART			
//			Receive = UARTCharGet(UART1_BASE);
//      GPIOPinWrite(GPIO_PORTF_BASE,GPIO_PIN_1,0<<1);//led on
//      My_delay(200);
//			GPIOPinWrite(GPIO_PORTF_BASE,GPIO_PIN_1,1<<1);//led off
//      My_delay(200);
			
//			lcd_printf(0,2,"Receive=%c",Receive);
//			alphabet ++;
//			My_delay(200);
//		if(alphabet > 'Z')
//			alphabet = 'A';
		
//		file_test();
//		while(1);
		
	}
}

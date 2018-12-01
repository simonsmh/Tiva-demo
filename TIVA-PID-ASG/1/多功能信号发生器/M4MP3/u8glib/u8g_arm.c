/*

u8g_arm.c


u8g utility procedures for LPC122x

Universal 8bit Graphics Library

Copyright (c) 2013, olikraus@gmail.com
All rights reserved.

Redistribution and use in source and binary forms, with or without modification, 
are permitted provided that the following conditions are met:

* Redistributions of source code must retain the above copyright notice, this list 
of conditions and the following disclaimer.

* Redistributions in binary form must reproduce the above copyright notice, this 
list of conditions and the following disclaimer in the documentation and/or other 
materials provided with the distribution.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND 
CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, 
INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF 
MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE 
DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR 
CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, 
SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT 
NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; 
LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER 
CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, 
STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) 
ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF 
ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.  


The following delay procedures must be implemented for u8glib. This is done in this file:

void u8g_Delay(uint16_t val)		Delay by "val" milliseconds
void u8g_MicroDelay(void)		Delay be one microsecond
void u8g_10MicroDelay(void)	Delay by 10 microseconds

Additional requirements:

SysTick must be enabled, but SysTick IRQ is not required. Any LOAD values are fine,
it is prefered to have at least 1ms
Example:
SysTick->LOAD = (SystemCoreClock/1000UL*(unsigned long)SYS_TICK_PERIOD_IN_MS) - 1;
SysTick->VAL = 0;
SysTick->CTRL = 7;   // enable, generate interrupt (SysTick_Handler), do not divide by 2
*/

#include "u8g_arm.h"

#include <stdint.h>
#include <stdbool.h>

#include "inc/hw_memmap.h"
#include "driverlib/pin_map.h"
#include "driverlib/rom_map.h"
#include "driverlib/gpio.h"
#include "driverlib/ssi.h"

#include "../uc1701.h"

#include "../m4_config.h"

/*========================================================================*/
/* system clock setup for LPC122x */

uint32_t SystemCoreClock = 12000000;


/*
Delay by the provided number of micro seconds.
Limitation: "us" * System-Freq in MHz must now overflow in 32 bit.
Values between 0 and 1.000.000 (1 second) are ok.
*/
void delay_micro_seconds(uint32_t us)
{
    uint32_t sys_ctl_clock, tick_delay;
    sys_ctl_clock = SysCtlClockGet();
    tick_delay = (sys_ctl_clock/(1000*4))*us;
    SysCtlDelay(tick_delay);    
}

/*========================================================================*/
/* generic gpio procedures (not required for u8glib) */

struct _lpc_pin_info_struct
{
    uint16_t offset;
    uint8_t iocon_gpio_value_no_pullup;
    uint8_t iocon_gpio_value_pullup;		/* identical to iocon_gpio_value_no_pullup if there is no pullup */
};
typedef struct _lpc_pin_info_struct lpc_pin_info_struct;

//const lpc_pin_info_struct lpc11xx_pin_info[] = 
//{
//    { offsetof(LPC_IOCON_Type,PIO0_0), 			128, 128+16},
//    { offsetof(LPC_IOCON_Type,PIO0_1), 			128, 128+16},
//    { offsetof(LPC_IOCON_Type,PIO0_2), 			128, 128+16},
//    { offsetof(LPC_IOCON_Type,PIO0_3), 			128, 128+16},
//    { offsetof(LPC_IOCON_Type,PIO0_4), 			128, 128+16},
//    { offsetof(LPC_IOCON_Type,PIO0_5), 			128, 128+16},
//    { offsetof(LPC_IOCON_Type,PIO0_6), 			128, 128+16},
//    { offsetof(LPC_IOCON_Type,PIO0_7), 			128, 128+16},
//    { offsetof(LPC_IOCON_Type,PIO0_8), 			128, 128+16},
//    { offsetof(LPC_IOCON_Type,PIO0_9), 			128, 128+16},
//    { offsetof(LPC_IOCON_Type,PIO0_10), 			128, 128},	/* no pullup available */
//    { offsetof(LPC_IOCON_Type,PIO0_11), 			128, 128},	/* no pullup available */
//    { offsetof(LPC_IOCON_Type,PIO0_12), 			128, 128+16},
//    { offsetof(LPC_IOCON_Type,RESET_PIO0_13), 		128+16, 128+16},  /* reset pin, disallow GPIO here, always keep pullup */
//    { offsetof(LPC_IOCON_Type,PIO0_14), 		128, 128+16}, 
//    { offsetof(LPC_IOCON_Type,PIO0_15), 		128, 128+16},
//    { offsetof(LPC_IOCON_Type,PIO0_16), 		128, 128+16},
//    { offsetof(LPC_IOCON_Type,PIO0_17), 		128, 128+16},
//    { offsetof(LPC_IOCON_Type,PIO0_18), 		128, 128+16},
//    { offsetof(LPC_IOCON_Type,PIO0_19), 			128, 128+16},
//    { offsetof(LPC_IOCON_Type,PIO0_20), 			128, 128+16},
//    { offsetof(LPC_IOCON_Type,PIO0_21), 			128, 128+16},
//    { offsetof(LPC_IOCON_Type,PIO0_22), 			128, 128+16},
//    { offsetof(LPC_IOCON_Type,PIO0_23), 			128, 128+16},
//    { offsetof(LPC_IOCON_Type,PIO0_24), 			128, 128+16},
//    { offsetof(LPC_IOCON_Type,SWDIO_PIO0_25), 		128+6, 128+16+6},
//    { offsetof(LPC_IOCON_Type,SWCLK_PIO0_26), 		128+6, 128+16+6},
//    { offsetof(LPC_IOCON_Type,PIO0_27), 			128, 128+16},
//    { offsetof(LPC_IOCON_Type,PIO0_28), 			128, 128+16},
//    { offsetof(LPC_IOCON_Type,PIO0_29), 			128, 128+16},
//    { offsetof(LPC_IOCON_Type,R_PIO0_30), 			128+1, 128+16+1},
//    { offsetof(LPC_IOCON_Type,R_PIO0_31), 			128+1, 128+16+1},

//    { offsetof(LPC_IOCON_Type,R_PIO1_0), 			128+1, 128+16+1},
//    { offsetof(LPC_IOCON_Type,R_PIO1_1), 			128+1, 128+16+1},
//    { offsetof(LPC_IOCON_Type,PIO1_2), 			128, 128+16},
//    { offsetof(LPC_IOCON_Type,PIO1_3), 			128, 128+16},
//    { offsetof(LPC_IOCON_Type,PIO1_4), 			128, 128+16},
//    { offsetof(LPC_IOCON_Type,PIO1_5), 			128, 128+16},
//    { offsetof(LPC_IOCON_Type,PIO1_6), 			128, 128+16},
//    { 0, 									0, 0},
//    { 0, 									0, 0},
//    { 0, 									0, 0},
//    { 0, 									0, 0},
//    { 0, 									0, 0},
//    { 0, 									0, 0},
//    { 0, 									0, 0},
//    { 0, 									0, 0},
//    { 0, 									0, 0},
//    { 0, 									0, 0},
//    { 0, 									0, 0},
//    { 0, 									0, 0},
//    { 0, 									0, 0},
//    { 0, 									0, 0},
//    { 0, 									0, 0},
//    { 0, 									0, 0},
//    { 0, 									0, 0},
//    { 0, 									0, 0},
//    { 0, 									0, 0},
//    { 0, 									0, 0},
//    { 0, 									0, 0},
//    { 0, 									0, 0},
//    { 0, 									0, 0},
//    { 0, 									0, 0},
//    { 0, 									0, 0},

//    { offsetof(LPC_IOCON_Type,PIO2_0), 			128, 128+16},
//    { offsetof(LPC_IOCON_Type,PIO2_1), 			128, 128+16},
//    { offsetof(LPC_IOCON_Type,PIO2_2), 			128, 128+16},
//    { offsetof(LPC_IOCON_Type,PIO2_3), 			128, 128+16},
//    { offsetof(LPC_IOCON_Type,PIO2_4), 			128, 128+16},
//    { offsetof(LPC_IOCON_Type,PIO2_5), 			128, 128+16},
//    { offsetof(LPC_IOCON_Type,PIO2_6), 			128, 128+16},
//    { offsetof(LPC_IOCON_Type,PIO2_7), 			128, 128+16},
//    { offsetof(LPC_IOCON_Type,PIO2_8), 			128, 128+16},
//    { offsetof(LPC_IOCON_Type,PIO2_9), 			128, 128+16},
//    { offsetof(LPC_IOCON_Type,PIO2_10), 			128, 128+16},
//    { offsetof(LPC_IOCON_Type,PIO2_11), 			128, 128+16},
//    { offsetof(LPC_IOCON_Type,PIO2_12), 			128, 128+16},
//    { offsetof(LPC_IOCON_Type,PIO2_13), 			128, 128+16},
//    { offsetof(LPC_IOCON_Type,PIO2_14), 			128, 128+16},
//    { offsetof(LPC_IOCON_Type,PIO2_15), 			128, 128+16},


//};

//LPC_GPIO_Type   *lpc11xx_gpio_base[4] = {LPC_GPIO0, LPC_GPIO1, LPC_GPIO2};

/*========================================================================*/
/* 
SPI 
typedef struct {                             (@ 0x40040000) SSP Structure          
union{
__IO uint32_t CR[2];						 (@ 0x40040000) Control Registers. 
struct{
__IO uint32_t CR0;                         (@ 0x40040000) Control Register 0. Selects the serial clock rate, bus type, and data size. 
__IO uint32_t CR1;                         (@ 0x40040004) Control Register 1. Selects master/slave and other modes. 
};
};
__IO uint32_t DR;                          (@ 0x40040008) Data Register. Writes fill the transmit FIFO, and reads empty the receive FIFO. 
__I  uint32_t SR;                          (@ 0x4004000C) Status Register        
__IO uint32_t CPSR;                        (@ 0x40040010) Clock Prescale Register 
__IO uint32_t IMSC;                        (@ 0x40040014) Interrupt Mask Set and Clear Register 
__I  uint32_t RIS;                         (@ 0x40040018) Raw Interrupt Status Register 
__I  uint32_t MIS;                         (@ 0x4004001C) Masked Interrupt Status Register 
__IO uint32_t ICR;                         (@ 0x40040020) SSPICR Interrupt Clear Register 
__IO uint32_t DMACR;                       (@ 0x40040024) DMA Control Register   
} LPC_SSP_Type;


LPC_SYSCON->SYSAHBCLKCTRL |= 1<<16;	enable IOCON clock
LPC_SYSCON->SYSAHBCLKCTRL |= 1<<11;	enable SSP clock
LPC_SYSCON->SSPCLKDIV = 1;

//LPC_IOCON->SCK_LOC = 0;			SCK0 at PIO0_10
LPC_IOCON->PIO0_14 = 2;			select SCK at PIO0_14, no pullup
LPC_IOCON->PIO0_17 = 2;			select MOSI at PIO0_17, no pullup

LPC_SSP->CR1 = 0;								disable SPI, enable master mode
LPC_SSP->CR0 = 7 | (CPOL << 6) | (CPHA <<7); 		8 bit, SPI mode, SCR = 1 (prescale)
LPC_SSP->CPSR = 12;			
LPC_SSP->CR1 = 2;								enable SPI, (enable master mode)

*/

/*========================================================================*/
/*
The following delay procedures must be implemented for u8glib

void u8g_Delay(uint16_t val)		Delay by "val" milliseconds
void u8g_MicroDelay(void)		Delay be one microsecond
void u8g_10MicroDelay(void)	Delay by 10 microseconds

*/

Pin_Map u8g_pin_cs = { LCD_GPIO_SPI_CS, LCD_PIN_SPI_CS };
Pin_Map u8g_pin_a0 = { LCD_GPIO_CD, LCD_PIN_CD };

// wrapper for ti api:
void set_gpio_mode(Pin_Map pin, uint8_t is_output, uint8_t is_pullup)
{
    if (is_output == 1)
    {
        MAP_GPIOPinTypeGPIOOutput(pin.ui32Port, pin.ui8Pins);
    }
    else // input
    {
        MAP_GPIOPinTypeGPIOInput(pin.ui32Port, pin.ui8Pins);
        
        if (is_pullup)
        {
            MAP_GPIOPadConfigSet(pin.ui32Port, pin.ui8Pins, GPIO_STRENGTH_2MA, GPIO_PIN_TYPE_STD_WPU);
        }
        else
        {
            MAP_GPIOPadConfigSet(pin.ui32Port, pin.ui8Pins, GPIO_STRENGTH_2MA, GPIO_PIN_TYPE_STD_WPD);
        }
    }
}

uint8_t get_gpio_level(Pin_Map pin)
{
    int32_t gpio_val;
    gpio_val = MAP_GPIOPinRead(pin.ui32Port, pin.ui8Pins);
    
    return gpio_val;
}

void u8g_Delay(uint16_t val)
{
    delay_micro_seconds((uint32_t)val);
}

void u8g_MicroDelay(void)
{
    delay_micro_seconds(1);
}

void u8g_10MicroDelay(void)
{
    delay_micro_seconds(10);
}




/*========================================================================*/
/* u8glib com procedure */

uint8_t u8g_com_hw_spi_fn(u8g_t *u8g, uint8_t msg, uint8_t arg_val, void *arg_ptr)
{
    switch(msg)
    {
    case U8G_COM_MSG_STOP:
        break;

    case U8G_COM_MSG_INIT:

//        if ( arg_val <= U8G_SPI_CLK_CYCLE_50NS )
//        {
//            spi_init(50);
//        }
//        else if ( arg_val <= U8G_SPI_CLK_CYCLE_300NS )
//        {
//            spi_init(300);
//        }
//        else if ( arg_val <= U8G_SPI_CLK_CYCLE_400NS )
//        {
//            spi_init(400);
//        }
//        else
//        {
//            spi_init(1200);
//        }
        MAP_SysCtlPeripheralEnable(LCD_PERIPH_SPI_CS);
        MAP_SysCtlPeripheralEnable(LCD_PERIPH_CD);
    
        MAP_SSIConfigSetExpClk(LCD_PIN_SPI_PORT, SysCtlClockGet(), SSI_FRF_MOTO_MODE_3,
            SSI_MODE_MASTER, SSI2_RATE, 8);
        MAP_SSIEnable(LCD_PIN_SPI_PORT);

#if 1
        // set_gpio_mode(u8g_pin_rst, 1, 0);		/* output, no pullup */
        set_gpio_mode(u8g_pin_cs, 1, 0);		/* output, no pullup */
        set_gpio_mode(u8g_pin_a0, 1, 0);		/* output, no pullup */
#else
        MAP_GPIOPinTypeGPIOOutput(LCD_GPIO_SPI_CS, LCD_PIN_SPI_CS);
        MAP_GPIOPinTypeGPIOOutput(LCD_GPIO_CD, LCD_PIN_CD);
#endif

        u8g_MicroDelay();      
        break;

    case U8G_COM_MSG_ADDRESS:                     /* define cmd (arg_val = 0) or data mode (arg_val = 1) */
        u8g_10MicroDelay();
        
#if 0
        set_gpio_level(u8g_pin_a0, arg_val);
#else
        if (arg_val == 0)
        {
            MAP_GPIOPinWrite(LCD_GPIO_CD, LCD_PIN_CD, ~LCD_PIN_CD);
        }
        else
        {
            MAP_GPIOPinWrite(LCD_GPIO_CD, LCD_PIN_CD, LCD_PIN_CD);
        }
#endif
    
        u8g_10MicroDelay();
        break;

    case U8G_COM_MSG_CHIP_SELECT:
        if ( arg_val == 0 )
        {
            /* disable */
            uint8_t i;
            /* this delay is required to avoid that the display is switched off too early --> DOGS102 with LPC1114 */
            for( i = 0; i < 5; i++ )
                u8g_10MicroDelay();
#if 0
            set_gpio_level(u8g_pin_cs, 1);
#else
            MAP_GPIOPinWrite(LCD_GPIO_SPI_CS, LCD_PIN_SPI_CS, LCD_PIN_SPI_CS);
#endif
        }
        else
        {
            /* enable */
#if 0
            set_gpio_level(u8g_pin_cs, 0);
#else
            MAP_GPIOPinWrite(LCD_GPIO_SPI_CS, LCD_PIN_SPI_CS, ~LCD_PIN_SPI_CS);
#endif
        }
        u8g_MicroDelay();
        break;

    case U8G_COM_MSG_RESET:
#if 0
//        set_gpio_level(u8g_pin_rst, arg_val);
#else
        //MAP_GPIOPinWrite(LCD_GPIO_CD, LCD_PIN_CD, LCD_PIN_CD);
#endif
        u8g_10MicroDelay();
        break;

    case U8G_COM_MSG_WRITE_BYTE:
        {
            uint32_t ulTemp;
    
//            spi_out(arg_val);
            ulTemp = arg_val;
            MAP_SSIDataPut(LCD_PIN_SPI_PORT, ulTemp);
            MAP_SSIDataGet(LCD_PIN_SPI_PORT, &ulTemp); // flush data
            
            // TODO: use isr handle this
            while(MAP_SSIBusy(LCD_PIN_SPI_PORT))
                ;
        
            u8g_MicroDelay();
        }
        break;

    case U8G_COM_MSG_WRITE_SEQ:
    case U8G_COM_MSG_WRITE_SEQ_P:
        {
            register uint8_t *ptr = arg_ptr;
            uint32_t ulTemp;
            
            while( arg_val > 0 )
            {
//                spi_out(*ptr++);
                ulTemp = *ptr++;
                MAP_SSIDataPut(LCD_PIN_SPI_PORT, ulTemp);
                MAP_SSIDataGet(LCD_PIN_SPI_PORT, &ulTemp); // flush data
                
                // TODO: use isr handle this
                //while(MAP_SSIBusy(LCD_PIN_SPI_PORT))
                //    ;
                
                arg_val--;
            }
        }
        break;
    }
    return 1;
}


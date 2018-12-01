#include <stdint.h>
#include <stdbool.h>
#include <stdio.h>

#include "inc/hw_ints.h"
#include "inc/hw_memmap.h"
#include "driverlib/debug.h"
#include "driverlib/fpu.h"
#include "driverlib/gpio.h"
#include "driverlib/interrupt.h"
#include "driverlib/pin_map.h"
#include "driverlib/rom.h"
#include "driverlib/sysctl.h"

#include "driverlib/uart.h"

#define MAX_UART_NUM        1       // uart0 & uart1

void (*uart_int_handler)(char );

void uart_init(const uint32_t base, const uint32_t baud)
{
    // config the UART for 115,200, 8-N-1 operation.
    ROM_UARTConfigSetExpClk(base, ROM_SysCtlClockGet(), baud, 
        UART_CONFIG_WLEN_8 | UART_CONFIG_STOP_ONE | UART_CONFIG_PAR_NONE);
    
    if (base == UART0_BASE)
    {
        ROM_IntEnable(INT_UART0);
    }
    else
    {
        ROM_IntEnable(INT_UART1);
    }
    
    ROM_UARTIntEnable(base, UART_INT_RX | UART_INT_RT);
}

void uart_send(const uint32_t base, const char *send_buffer, uint16_t len)
{
    while (len--)
    {
        ROM_UARTCharPut(base, *(send_buffer ++));
    }
}

void uart0_int_isr(void)
{
    uint32_t uart_ints;

    uart_ints = ROM_UARTIntStatus(UART0_BASE, true);
    ROM_UARTIntClear(UART0_BASE, uart_ints);
    // simply write back char
    while(ROM_UARTCharsAvail(UART0_BASE))
    {
        ROM_UARTCharPutNonBlocking(UART1_BASE, ROM_UARTCharGetNonBlocking(UART0_BASE));
    }
}

void uart1_int_isr(void)
{
    char data;
    uint32_t uart_ints;
    
    uart_ints = ROM_UARTIntStatus(UART1_BASE, true);
    ROM_UARTIntClear(UART1_BASE, uart_ints);
    
    // simply write back char
    while(ROM_UARTCharsAvail(UART1_BASE))
    {
        data = ROM_UARTCharGetNonBlocking(UART1_BASE);
#if 0
        ROM_UARTCharPutNonBlocking(UART1_BASE, data);
#endif
    }
    
    if (uart_int_handler != NULL)
    {
        (*uart_int_handler)(data);
    }
}

void uart_int_register(void (*func)(char ascii))
{
    uart_int_handler = func;
}

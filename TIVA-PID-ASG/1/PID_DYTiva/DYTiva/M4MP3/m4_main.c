#include <stdint.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdarg.h>
#include <string.h>

#include "inc/hw_ints.h"
#include "inc/hw_memmap.h"
#include "driverlib/debug.h"
#include "driverlib/fpu.h"
#include "driverlib/gpio.h"
#include "driverlib/interrupt.h"
#include "driverlib/pin_map.h"
#include "driverlib/rom.h"
#include "driverlib/sysctl.h"
#include "driverlib/systick.h"
#include "driverlib/uart.h"

#include "m4_pinmux.h"
#include "m4_common.h"
#include "m4_config.h"
#include "m4_ver.h"

typedef enum __Boot_Reason_
{
    BR_FIRST,
    BR_NORMAL   = BR_FIRST,
    BR_PLAYER,
    BR_LAST,
} Boot_Reason;

// global systick count
uint32_t sys_tick_count = 0;

// print the debug info to uart
void m4_dbgprt(const char *format, ...)
{
#ifdef DEBUG
    char msg_buffer[MAX_MSG_LEN] = { 0 };

    va_list vl;
    va_start(vl, format);
    vsprintf(msg_buffer, format, vl);
    va_end(vl);

    uart_send(DEBUG_PORT, (const char *)msg_buffer, strlen(msg_buffer));
#endif
}

#ifdef DEBUG
// The error routine that is called if the driver library encounters an error.
void __error__(char *pcFilename, uint32_t ui32Line)
{
    m4_dbgprt("library error: %s:%d\r\n", pcFilename, ui32Line);
}
#endif

#if 0
void sleep(uint32_t ms)
{
    uint32_t sys_ctl_clock, tick_delay;
    sys_ctl_clock = SysCtlClockGet();
    tick_delay = (sys_ctl_clock/(1000*4))*ms;
    SysCtlDelay(tick_delay);
}
#else
void sleep(uint32_t ms)
{
    uint32_t delay;
    while (ms --)
        for (delay = 18000; delay; delay --)
            ;
}
#endif

void systick_int_isr(void)
{
    sys_tick_count += 10;
#ifdef M4_USBMSC
    usb_msc_timeout_cb();
#endif // M4_USBMSC
}

#ifdef UART_PROXY
void uart_proxy(char ascii)
{
    uart_send(UART1_BASE, (const char *)&ascii, 1);
}
#endif // UART_PROXY

//volatile float a, b;
int main(void)
{
    char abc = 'a';
    uint8_t boot_reason;
    
    // enabling FPU
    ROM_FPUEnable();
    ROM_FPULazyStackingEnable();

    // set the clocking to run directly from the crystal.
#ifdef M4_80MHZ
    ROM_SysCtlClockSet(SYSCTL_SYSDIV_2_5 | SYSCTL_USE_PLL | SYSCTL_XTAL_16MHZ | SYSCTL_OSC_MAIN);
#else
    ROM_SysCtlClockSet(SYSCTL_SYSDIV_1 | SYSCTL_USE_OSC | SYSCTL_XTAL_16MHZ | SYSCTL_OSC_MAIN);
#endif
    
    // enable processor interrupts
    ROM_IntMasterEnable();
    
    // enable the GPIO pin mux
    PortFunctionInit();
    
#if (defined(M4_SYSTICK) && (!defined(M4_RTX)) && (!defined(M4_FREERTOS)) && (!defined(M4_TIRTOS)))
    // enable systick (10ms)
    ROM_SysTickPeriodSet(SysCtlClockGet() / 100);
    ROM_SysTickIntEnable();
    ROM_SysTickEnable();
#endif // M4_SYSTICK
    
#ifdef DEBUG
    // init UART0 for debug purpose
    uart_init(UART0_BASE, UART0_BAUD_RATE);
#endif // DEBUG

//    while (1)
//    {
//        sleep(50);
//        m4_dbgprt("%u\r\n", SysTickValueGet());
//    }

//    float a = 0, b;
//    a = 10;
//    b = sin(a);
//    m4_dbgprt("%f", b);

#ifdef M4_UART
    uart_init(UART1_BASE, UART5_BAUD_RATE);
#ifdef UART_PROXY
    uart_int_register(&uart_proxy);
    while (true)
    {
        abc = UARTCharGet(UART1_BASE);
        uart_send(UART0_BASE, (const char *)&abc, 1);
    }
#endif // UART_PROXY
#endif // M4_UART

    m4_dbgprt("+++");
    
    sleep(100);
    
    //m4_dbgprt("clock=%u\r\n", SysCtlClockGet());

#ifdef M4_LCD
    lcd_init();
    lcd_flash();
    //lcd_printf(0, 0, "ÄãºÃ!"); while(1);
    
    // printf debug info
    lcd_clear();
    lcd_printf(0, 0, "Build " BUILD_NUM);
    
#if defined(USE_DY_PB_FINAL)
    lcd_printf(0, 1, "Tiva-PB V1.2");
#elif defined(USE_DY_PB_3)
    lcd_printf(0, 1, "Tiva-PB V1.1");
#elif defined(USE_DY_PB_2)
    lcd_printf(0, 1, "Tiva-PB V1.0");
#else
    lcd_printf(0, 1, "Tiva-PB Unknown");
#endif

#if defined(M4_RTX)
    lcd_printf(0, 2, "With Keil RTX");
#elif defined(M4_FREERTOS)
    lcd_printf(0, 2, "With FreeRTOS");
#endif

    sleep(100);
#endif // M4_LCD

#ifdef M4_AC
    ac_init();
#endif // M4_AC

#ifdef M4_DAC
    dac_init();
#endif // M4_DAC

#ifdef M4_GPIO
    gpio_init();
#endif // M4_GPIO

#ifdef M4_TEMP
    temp_init();
#endif // M4_TEMP

#ifdef M4_CODEC
    codec_init();
#endif // M4_CODEC

#ifdef M4_SPWM
    spwm_init();
#endif // M4_SPWM

#ifdef M4_PWM
    pwm_init();
#endif // M4_PWM

#ifdef M4_RTC
    rtc_init();
#endif // M4_RTC

#ifdef M4_TMR_CCP
    timer_ccp_init();
#endif // M4_TMR_CCP

#ifdef M4_WDT
    wdt_init();
#endif // M4_WDT

#ifdef M4_CC1101
    cc1101_init();
#endif // M4_CC1101
    
#ifdef M4_USBMSC
    // once usb is plugged in, this function will never return
    usb_msc_init();
#endif // M4_USBMSC

#ifdef M4_CC1101
    cc1101_init();
#endif // M4_CC1101

#if defined (M4_RTX)
    rtx_entry(app_main);
#elif defined (M4_FREERTOS)
    freertos_entry(app_main);
#elif defined (M4_TIRTOS)
    tirtos_entry(app_main);
//#elif defined (APP_PLAYER)
//    app_main_player();
#elif defined (APP_GUI)
    app_main_gui();
#else // default boot:
    boot_reason = gpio_bootpin_test();
    if (boot_reason == BR_PLAYER)
    {
        lcd_clear();
        lcd_printf(0, 0, "boot into mp3..");
        sleep(250);
        app_main_player();
    }
    else
    {
        app_main_demo();
    }
#endif

    return 0;
}

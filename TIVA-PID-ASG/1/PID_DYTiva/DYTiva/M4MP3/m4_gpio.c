#include <stdint.h>
#include <stdbool.h>

#include "inc/hw_ints.h"
#include "inc/hw_memmap.h"
#include "inc/hw_gpio.h"
#include "inc/hw_types.h"
#include "driverlib/debug.h"
#include "driverlib/fpu.h"
#include "driverlib/gpio.h"
#include "driverlib/interrupt.h"
#include "driverlib/pin_map.h"
#include "driverlib/rom.h"
#include "driverlib/rom_map.h"
#include "driverlib/sysctl.h"

#include "m4_config.h"
#include "m4_common.h"

// callback array
void (*gpio_int_handler)(Button );

void gpio_init(void)
{
    // init onboard leds, D2~D6
    MAP_SysCtlPeripheralEnable(LED_PERIPH_D2);
    MAP_SysCtlPeripheralEnable(LED_PERIPH_D3);
    MAP_SysCtlPeripheralEnable(LED_PERIPH_D4);
#ifndef USE_DY_PB_FINAL
    MAP_SysCtlPeripheralEnable(LED_PERIPH_D5);
#endif // USE_DY_PB_FINAL

#ifndef USE_DY_PB_2
    MAP_SysCtlPeripheralEnable(LED_PERIPH_D6);
#endif // USE_DY_PB_2
#ifdef USE_DY_PB_3
    MAP_SysCtlPeripheralEnable(THERMAL_RESISTOR);
#endif // USE_DY_PB_3
    
    // Unlock PF0 so we can change it to a GPIO input
    // Once we have enabled (unlocked) the commit register then re-lock it
    // to prevent further changes.  PF0 is muxed with NMI thus a special case.
    //
    HWREG(LED_GPIO_D2 + GPIO_O_LOCK) = GPIO_LOCK_KEY;
    HWREG(LED_GPIO_D2 + GPIO_O_CR) |= 0x01;
    HWREG(LED_GPIO_D2 + GPIO_O_LOCK) = 0;
    
    MAP_GPIOPinTypeGPIOOutput(LED_GPIO_D2 , LED_PIN_D2);    // PF0
    MAP_GPIOPinTypeGPIOOutput(LED_GPIO_D3 , LED_PIN_D3);    // PA4
    MAP_GPIOPinTypeGPIOOutput(LED_GPIO_D4 , LED_PIN_D4);    // PD6
#ifndef USE_DY_PB_FINAL
    MAP_GPIOPinTypeGPIOOutput(LED_GPIO_D5 , LED_PIN_D5);    // PB3
#endif // USE_DY_PB_FINAL
#ifndef USE_DY_PB_2
    MAP_GPIOPinTypeGPIOOutput(LED_GPIO_D6 , LED_PIN_D6);    // PF3
#endif // USE_DY_PB_2
#ifdef USE_DY_PB_3
    MAP_GPIOPinTypeGPIOOutput(THERMAL_GPIO , THERMAL_PIN);  // PF2
    gpio_thermal_on(0);
#endif // USE_DY_PB_3
    
    HWREG(BUTTON_GPIO_K1 + GPIO_O_LOCK) = GPIO_LOCK_KEY;
    HWREG(BUTTON_GPIO_K1 + GPIO_O_CR) |= 0x80;
    HWREG(BUTTON_GPIO_K1 + GPIO_O_LOCK) = 0;
    
#ifdef M4_BUTTON
    // init push buttons, K1~K4
    // the push button shares the same peripherals with led
    MAP_GPIOPinTypeGPIOInput(BUTTON_GPIO_K1 , BUTTON_PIN_K1);   // PD7
    MAP_GPIOPinTypeGPIOInput(BUTTON_GPIO_K2 , BUTTON_PIN_K2);   // PF4
    MAP_GPIOPinTypeGPIOInput(BUTTON_GPIO_K3 , BUTTON_PIN_K3);   // PA3
    MAP_GPIOPinTypeGPIOInput(BUTTON_GPIO_K4 , BUTTON_PIN_K4);   // PA2
    
    // enable interrupts
    MAP_IntEnable(BUTTON_INT_K1);
    MAP_IntEnable(BUTTON_INT_K2);
    MAP_IntEnable(BUTTON_INT_K3);
    MAP_IntEnable(BUTTON_INT_K4);
    
    MAP_GPIOIntTypeSet(BUTTON_GPIO_K1, BUTTON_PIN_K1, GPIO_FALLING_EDGE);
    MAP_GPIOIntTypeSet(BUTTON_GPIO_K2, BUTTON_PIN_K2, GPIO_FALLING_EDGE);
    MAP_GPIOIntTypeSet(BUTTON_GPIO_K3, BUTTON_PIN_K3, GPIO_FALLING_EDGE);
    MAP_GPIOIntTypeSet(BUTTON_GPIO_K4, BUTTON_PIN_K4, GPIO_FALLING_EDGE);
    
    GPIOIntEnable(BUTTON_GPIO_K1, BUTTON_PIN_K1);
    GPIOIntEnable(BUTTON_GPIO_K2, BUTTON_PIN_K2);
    GPIOIntEnable(BUTTON_GPIO_K3, BUTTON_PIN_K3);
    GPIOIntEnable(BUTTON_GPIO_K4, BUTTON_PIN_K4);
#endif // M4_BUTTON
}

void gpio_led_runner(int8_t direction)
{
    int8_t i_led_num = 1, j_led_num;
    if (direction < 0)
    {
        i_led_num = MAX_LED_NUM - 1;
    }
    
    for (; (i_led_num < MAX_LED_NUM) && (i_led_num >= 0) ; i_led_num += direction)
    {
        for (j_led_num = 0; j_led_num < MAX_LED_NUM; j_led_num ++)
        {
            MAP_GPIOPinWrite(LED_GPIO_D2, LED_PIN_D2, LED_PIN_D2*(0 != i_led_num));   // D2 overlapped
            MAP_GPIOPinWrite(LED_GPIO_D3, LED_PIN_D3, LED_PIN_D3*(1 != i_led_num));
            MAP_GPIOPinWrite(LED_GPIO_D4, LED_PIN_D4, LED_PIN_D4*(2 != i_led_num));
#ifndef USE_DY_PB_FINAL
            MAP_GPIOPinWrite(LED_GPIO_D5, LED_PIN_D5, LED_PIN_D5*(3 != i_led_num));
#endif // USE_DY_PB_FINAL
#ifdef USE_DY_PB_1
            MAP_GPIOPinWrite(LED_GPIO_D6, LED_PIN_D6, LED_PIN_D6*(4 != i_led_num));
#endif // USE_DY_PB_1
        }
        sleep(50);
    }
}

void gpio_led_test()
{
    gpio_led_runner(1);
    gpio_led_runner(-1);
}

void gpio_led_all(uint8_t on)
{
    MAP_GPIOPinWrite(LED_GPIO_D2, LED_PIN_D2, ~(LED_PIN_D2*on));
    MAP_GPIOPinWrite(LED_GPIO_D3, LED_PIN_D3, ~(LED_PIN_D3*on));
    MAP_GPIOPinWrite(LED_GPIO_D4, LED_PIN_D4, ~(LED_PIN_D4*on));
}

void gpio_thermal_on(uint8_t on)
{
    MAP_GPIOPinWrite(THERMAL_GPIO, THERMAL_PIN, THERMAL_PIN*on);
}

uint8_t gpio_bootpin_test(void)
{
    uint32_t gpio_val;
    uint8_t boot_reason = 0;
    
    gpio_val = GPIOPinRead(GPIO_PORTA_BASE, BUTTON_PIN_K3 | BUTTON_PIN_K4);
    
    switch ((~gpio_val) & (BUTTON_PIN_K3 | BUTTON_PIN_K4))
    {
    case BUTTON_PIN_K3:
        boot_reason = 1;
        m4_dbgprt("boot reason=1\r\n");
        break;
    case BUTTON_PIN_K4:
        boot_reason = 2;
        m4_dbgprt("boot reason=2\r\n");
        break;
    }
    
    return boot_reason;
}

void gpioa_int_isr(void)
{
    uint32_t gpio_ints;
    uint32_t gpio_val;
    
    gpio_ints = GPIOIntStatus(GPIO_PORTA_BASE, true);    
    GPIOIntClear(GPIO_PORTA_BASE, gpio_ints);
    
    gpio_val = GPIOPinRead(GPIO_PORTA_BASE, BUTTON_PIN_K3 | BUTTON_PIN_K4);
    switch ((~gpio_val) & (BUTTON_PIN_K3 | BUTTON_PIN_K4))
    {
    case BUTTON_PIN_K3:
        m4_dbgprt("K3 pressed\r\n");
//        (*gpio_int_handler)(Key_3);
        break;
    case BUTTON_PIN_K4:
        m4_dbgprt("K4 pressed\r\n");
//        (*gpio_int_handler)(Key_4);
        break;
    default:
        m4_dbgprt("K3/K4\r\n");
//        (*gpio_int_handler)(Key_3);
    }
}

void gpiod_int_isr(void)
{
    uint32_t gpio_ints;
    uint32_t gpio_val;
    
    gpio_ints = GPIOIntStatus(GPIO_PORTD_BASE, true);
    GPIOIntClear(GPIO_PORTD_BASE, gpio_ints);
    
    gpio_val = GPIOPinRead(GPIO_PORTD_BASE, BUTTON_PIN_K1);
    switch (gpio_val & (BUTTON_PIN_K1))
    {
    case ~BUTTON_PIN_K1:
    default:    // gpio unable to figure out which pin
        m4_dbgprt("K1 pressed\r\n");
        (*gpio_int_handler)(Key_1);
        break;
    }
}

void gpiof_int_isr(void)
{
    uint32_t gpio_ints;
    uint32_t gpio_val;
    
    gpio_ints = GPIOIntStatus(GPIO_PORTF_BASE, true);
    GPIOIntClear(GPIO_PORTF_BASE, gpio_ints);
    
    gpio_val = GPIOPinRead(GPIO_PORTF_BASE, BUTTON_PIN_K3 | BUTTON_PIN_K4);
    switch ((~gpio_val) & (BUTTON_PIN_K2))
    {
    case BUTTON_PIN_K2:
    default:
        m4_dbgprt("K2 pressed\r\n");
        (*gpio_int_handler)(Key_2);
        break;
    }
}

void gpio_int_register(void (*func)(Button button))
{
    gpio_int_handler = func;
}

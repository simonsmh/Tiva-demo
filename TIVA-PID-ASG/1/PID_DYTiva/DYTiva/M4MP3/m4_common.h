#ifndef __M4_COMMON
#define __M4_COMMON

#include <stdint.h>
#include <stdbool.h>

#include "FatFs/ff.h"

#include "m4_config.h"

#ifdef USE_DY_PB_2

#ifdef USE_DY_PB_FINAL
#define MAX_LED_NUM             3       // D2~D4
#else // USE_DY_PB_FINAL
#define MAX_LED_NUM             4       // D2~D5
#endif // USE_DY_PB_FINAL

#else // USE_DY_PB_2
#define MAX_LED_NUM             5       // D2~D6
#endif // USE_DY_PB_2

#define MAX_BUTTON_NUM          4       // K1~K4

/* gpio definition for led */
#define LED_PERIPH_D2           SYSCTL_PERIPH_GPIOF
#define LED_PERIPH_D3           SYSCTL_PERIPH_GPIOA
#define LED_PERIPH_D4           SYSCTL_PERIPH_GPIOD
#define LED_PERIPH_D5           SYSCTL_PERIPH_GPIOB
#define LED_PERIPH_D6           SYSCTL_PERIPH_GPIOF

#define THERMAL_RESISTOR        SYSCTL_PERIPH_GPIOF

#define LED_GPIO_D2             GPIO_PORTF_BASE
#define LED_GPIO_D3             GPIO_PORTA_BASE
#define LED_GPIO_D4             GPIO_PORTD_BASE
#define LED_GPIO_D5             GPIO_PORTB_BASE
#define LED_GPIO_D6             GPIO_PORTF_BASE

#define LED_PIN_D2              GPIO_PIN_0
#define LED_PIN_D3              GPIO_PIN_4
#define LED_PIN_D4              GPIO_PIN_6
#define LED_PIN_D5              GPIO_PIN_3
#define LED_PIN_D6              GPIO_PIN_3

/* gpio definition for thermal */
#define THERMAL_GPIO            GPIO_PORTF_BASE
#define THERMAL_PIN             GPIO_PIN_2


/* gpio definition for button */
#define BUTTON_GPIO_K1          GPIO_PORTD_BASE
#define BUTTON_GPIO_K2          GPIO_PORTF_BASE
#define BUTTON_GPIO_K3          GPIO_PORTA_BASE
#define BUTTON_GPIO_K4          GPIO_PORTA_BASE

#define BUTTON_PIN_K1           GPIO_PIN_7
#define BUTTON_PIN_K2           GPIO_PIN_4
#define BUTTON_PIN_K3           GPIO_PIN_3
#define BUTTON_PIN_K4           GPIO_PIN_2

#define BUTTON_INT_K1           INT_GPIOD
#define BUTTON_INT_K2           INT_GPIOF
#define BUTTON_INT_K3           INT_GPIOA
#define BUTTON_INT_K4           INT_GPIOA

typedef enum __Button_
{
    Key_1,
    Key_2,
    Key_3,
    Key_4,
} Button;

extern void m4_dbgprt(const char *format, ...);
extern void sleep(uint32_t );

//#ifdef M4_UART
extern void uart_init	(const uint32_t , const uint32_t );
extern void uart_send	(const uint32_t , const char *, uint16_t );
extern void uart_int_register(void (*func)(char ));
//#endif // M4_UART

#ifdef M4_LCD
extern void lcd_init(void);
extern void lcd_clear(void);
extern void lcd_printf(uint8_t , uint8_t , const char *, ...);
extern void lcd_flash(void);
extern void lcd_scroll(bool );
#endif // M4_LCD

#ifdef M4_ADC
extern void adc_init(const uint32_t , const uint32_t );
extern uint32_t adc_read(const uint32_t );
#endif // M4_ADC

#ifdef M4_AC
extern void ac_init();
extern bool ac_get();
#endif // M4_AC

#ifdef M4_DAC
extern void dac_init(void);
extern void dac_write(uint16_t );
extern void dac_play(char * );
extern void dac_sinwave(uint16_t );
extern void dac_sinewave(void);
extern void dac_squarewave(void);
#endif // M4_DAC

#ifdef M4_GPIO
extern void gpio_init(void);
extern void gpio_led_runner(int8_t );
extern void gpio_led_all(uint8_t );
extern void gpio_thermal_on(uint8_t );
extern uint8_t gpio_bootpin_test(void);
extern void gpio_int_register(void (*func)(Button ));
#endif // M4_GPIO

#ifdef M4_TEMP
extern void temp_init(void);
extern float temp_read(void);
#endif // M4_TEMP

#ifdef M4_CODEC
extern void codec_init(void);
extern void codec_get_file_name(char *);
//extern ();
#endif

#ifdef M4_SDC
#endif // M4_SDC

#ifdef M4_SPWM
extern void spwm_init(void);
extern void spwm_play(void);
extern void spwm_scanfreq(int8_t );
#endif // M4_SPWM

#ifdef M4_PWM
extern void pwm_init(void);
extern void pwm_set(uint8_t );
extern void pwm_set_ex(uint16_t );
#endif // M4_PWM

#ifdef M4_RTC
extern void rtc_init(void);
extern void rtc_get(struct tm * );
#endif // M4_RTC

#ifdef M4_TMR_CCP
extern void timer_ccp_init(void);
extern uint32_t timer_ccp_get();
#endif // M4_TMR_CCP

#ifdef M4_WDT
extern void wdt_init(void);
extern void wdt_feed(void);
#endif // M4_WDT

#ifdef M4_CC1101
extern void cc1101_init(void);

#endif // M4_CC1101

#ifdef M4_USBMSC
extern void usb_msc_timeout_cb(void);
extern void usb_msc_init(void);
extern int usb_msc_stat(void);
#endif // M4_USBMSC

#ifdef M4_CC1101
extern void cc1101_init(void);
extern void cc1101_send(char );
#endif // M4_CC1101

#ifdef M4_RTX
extern void rtx_entry(void (*func));
#endif // M4_RTX

#ifdef M4_FREERTOS
extern void freertos_entry(void (*func));
#endif // M4_FREERTOS

#ifdef M4_TIRTOS
extern void tirtos_entry(void (*func));
#endif // M4_TIRTOS

//extern "C" void app_main_gui(void);
#ifdef APP_PLAYER
extern void app_main_player(void);
#endif // APP_PLAYER

extern void app_main_demo(void);
#endif // __M4_COMMON

extern uint32_t sys_tick_count;

extern char memory_card_28_bits[];
extern char tools_28_bits[];

extern void timerInit_pid();

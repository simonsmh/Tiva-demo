#ifndef __M4_COMMON
#define __M4_COMMON

#include <stdint.h>
#include <stdbool.h>


#include "m4_config.h"

extern void m4_dbgprt(const char *format, ...);
extern void sleep(uint32_t );


#ifdef M4_LCD
extern void lcd_init(void);
extern void lcd_clear(void);
extern void lcd_printf(uint8_t , uint8_t , const char *, ...);
extern void lcd_flash(void);
extern void lcd_scroll(bool );
#endif // M4_LCD


#ifdef M4_GPIO
extern void gpio_init(void);
#endif // M4_GPIO



extern void app_main(void);


#endif

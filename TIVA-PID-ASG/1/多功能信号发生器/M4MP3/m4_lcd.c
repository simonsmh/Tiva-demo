//  lcd middleware
#include <stdio.h>
#include <stdarg.h>
#include <string.h>

#include "uc1701.h"
#include "gt20l16s1y.h"

#include "m4_config.h"
#include "m4_common.h"

#ifdef M4_LCD

#ifdef USE_FONT_CHIP
bool is_fontchip_ok = false;
#endif // USE_FONT_CHIP

void lcd_init(void)
{
#define TEST_GB_CODE    0xBBB6  // »¶
    
#ifdef USE_FONT_CHIP
    uint8_t font_nfo;
    uint8_t font_buffer[MAX_FONT_BUFFER_LEN] = { 0 };
#endif // USE_FONT_CHIP
    
    // init lcd module
    UC1701Init(UC1701_RATE);
    UC1701Clear();
    
#ifdef USE_FONT_CHIP
    // init font module
    GT20Init(GT20L16S1Y_RATE);
    // test if font chip works
    GT20Read(GB2312_15x16, TEST_GB_CODE, &font_nfo, &font_nfo, (uint16_t *)font_buffer);
    // let's test 3 bytes
    if ((font_buffer[0] | font_buffer[MAX_FONT_BUFFER_LEN/4] | font_buffer[MAX_FONT_BUFFER_LEN/2]) > 0)
    {
        is_fontchip_ok = true;
    }
    else
    {
        lcd_printf(0, 0, "FONT CHIP ERR");
        sleep(1000);
    }
#endif // USE_FONT_CHIP
}

void lcd_clear(void)
{
    UC1701Clear();
}

// x=0xFF denotes title
void lcd_printf(uint8_t x, uint8_t y, const char *format, ...)
{
    char msg_buffer[MAX_MSG_LEN] = { 0 };
    uint8_t cursor_x = x % 0xFF, cursor_y = y;

#ifdef USE_FONT_CHIP
    uint8_t font_width, font_height;
    uint8_t font_buffer[MAX_FONT_BUFFER_LEN];
#endif // USE_FONT_CHIP

    uint16_t msg_len;
    uint8_t i_msg;

    va_list vl;
    va_start(vl, format);
    msg_len = vsprintf(msg_buffer, format, vl);

    if (msg_len >= MAX_MSG_LEN)
        while (1)
            ;

    va_end(vl);

    msg_len = strlen(msg_buffer);

    if (x == 0xFF)
    {
        UC1701InverseEnable();
        // draw title background
        UC1701CharDispaly(cursor_y, cursor_x, "                ");
    }
            
    for (i_msg = 0; i_msg < msg_len; i_msg ++)
    {
        // test whether current char is Hanzi
        if ((msg_buffer[i_msg] & 0x80) == 0x80)
        {
#ifdef USE_FONT_CHIP
            if (is_fontchip_ok)
            { // fetch from font chip
                GT20Read(GB2312_15x16, *(uint16_t *)&msg_buffer[i_msg], &font_width, &font_height, (uint16_t *)font_buffer);
                
                // paint to screen
                UC1701FontDisplay(cursor_y, cursor_x, font_width, font_height, font_buffer);
            }
            else
#endif // USE_FONT_CHIP
            { // use the built in chinese font
                UC1701ChineseDispaly(cursor_y, cursor_x, *(uint16_t *)&msg_buffer[i_msg]);
            }
            cursor_x += 2;
            i_msg ++;
        }
        else
        {
            UC1701Display(cursor_y, cursor_x, msg_buffer[i_msg]);
            cursor_x ++;
        }
    }
    
    UC1701InverseDisable();
    
#if 0   // test lcd response delay
    for(int l = 0; 1; l = (l+1)% 63)
    {
        sleep(100);
        UC1701ScrollLineSet(l);
    }
#endif
}

void lcd_flash(void)
{
    UC1701Clear();
    UC1701Test();
    UC1701AllPixelOnEnable();
    sleep(250);
    UC1701AllPixelOnDisable();
    sleep(250);
}

void lcd_scroll(bool reset)
{
    static uint8_t scroll_line = 0;
    if (reset)
        scroll_line = 0;
    UC1701ScrollLineSet(scroll_line);
    scroll_line ++;
}

#endif // M4_LCD

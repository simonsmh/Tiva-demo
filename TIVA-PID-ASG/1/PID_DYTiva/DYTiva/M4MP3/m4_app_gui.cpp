#include "m4_common.h"
#include "m4_config.h"

#include "inc/hw_memmap.h"
#include "driverlib/gpio.h"

#include "m2tklib/m2ghu8g.h"

//#include "u8glib/u8g_arm.h"
#include "m2tklib/m2.h"

#include "m4_app_res.h"

/*
    Main Screen Design:

        128 -->
    64  Menu Area, size: 20x20, icons: 16x16, items: Main | Temp | ADC | Buzzer | UART | Audio
     |  Functional Area, size: 
     v
    
*/

extern "C"
{
    #include "u8glib/u8g_arm.h"
    uint8_t u8g_com_hw_spi_fn(u8g_t *u8g, uint8_t msg, uint8_t arg_val, void *arg_ptr);
}

#ifdef APP_GUI

extern "C" void app_main_gui(void);

class M2tk
{
    public:
        M2tk(m2_rom_void_p element, m2_es_fnptr es, m2_eh_fnptr eh, m2_gfx_fnptr gh) { m2_Init(element, es, eh, gh); }
        void checkKey(void) { m2_CheckKey(); }
        uint8_t handleKey(void) { return m2_HandleKey(); }
        void draw(void) { m2_Draw(); }
        void setKey(uint8_t key) { m2_SetKey(key); }
        uint8_t getKey(void) { return m2_GetKey(); }
        void setFont(uint8_t font_idx, const void *font_ptr) { m2_SetFont(font_idx, font_ptr); }
#if 1
        void setPin(uint8_t key, Pin_Map pin) { m2_SetPin(key, pin); }
#else
        void setPin(uint8_t key, uint8_t pin) { m2_SetPin(key, pin); }
#endif
        void setRoot(m2_rom_void_p element, uint8_t next_cnt = 0, uint8_t cv = 0) { m2_SetRootExtended(element, next_cnt, cv); }
        void setHome(m2_rom_void_p element) { m2_SetHome(element); }
        void setHome2(m2_rom_void_p element) { m2_SetHome2(element); }
        void setRootChangeCallback(m2_root_change_fnptr cb) { m2_SetRootChangeCallback(cb); }

        void clear(void) { m2_Clear(); }
        m2_rom_void_p getRoot() { return m2_GetRoot(); }
};

u8g_t u8g;

Pin_Map dy_key_1 = { GPIO_PORTD_BASE, GPIO_PIN_7 };
Pin_Map dy_key_2 = { GPIO_PORTF_BASE, GPIO_PIN_4 };
Pin_Map dy_key_3 = { GPIO_PORTA_BASE, GPIO_PIN_3 };
Pin_Map dy_key_4 = { GPIO_PORTA_BASE, GPIO_PIN_2 };


M2_EXTERN_ALIGN(el_top);	// forward declaration of the top level element

// tools menu 
M2_LABEL(el_tools_label, NULL, "Tools selected.");
M2_ROOT(el_tools_ok, "f4", " Ok ", &el_top);
M2_LIST(list_tools) = { &el_tools_label, &el_tools_ok };
M2_VLIST(el_tools_vl, NULL, list_tools);
M2_ALIGN(el_tools, "W64H64", &el_tools_vl);  

// menu for memory card 
M2_LABEL(el_mc_label, NULL, "Memory card selected.");
M2_ROOT(el_mc_ok, "f4", " Ok ", &el_top);
M2_LIST(list_mc) = { &el_mc_label, &el_mc_ok };
M2_VLIST(el_mc_vl, NULL, list_mc);
M2_ALIGN(el_mc, "W64H64", &el_mc_vl);  

// main menu with two icons as buttons  
// w30h30: Define visible size of the icon, center icon within this frame
// 28, 28: Informs m2tklib about the actual size of the icon
M2_XBMROOTP(el_xbm_ti       , "w18h18", 16, 16, icon_ti     , &el_mc);
M2_XBMROOTP(el_xbm_temp     , "w18h18", 16, 16, icon_temp   , &el_mc);
M2_XBMROOTP(el_xbm_adc      , "w18h18", 16, 16, icon_adc    , &el_mc);
M2_XBMROOTP(el_xbm_buzzer   , "w18h18", 16, 16, icon_buzzer , &el_mc);
M2_XBMROOTP(el_xbm_uart     , "w18h18", 16, 16, icon_uart   , &el_mc);
M2_XBMROOTP(el_xbm_audio    , "w18h18", 16, 16, icon_audio  , &el_mc);

M2_LIST(el_xbm_list) = { &el_xbm_ti, &el_xbm_temp, &el_xbm_adc, &el_xbm_buzzer, &el_xbm_uart, &el_xbm_audio };
M2_HLIST(el_xbm_hlist, NULL, el_xbm_list);
M2_ALIGN(el_xbm, NULL, &el_xbm_hlist);

void mc_cb(m2_el_fnarg_p fnarg)
{
  m2_SetRoot(&el_mc);
}

M2_XBMBUTTONP(el_buttonxbm_ti       , "w18h18", 16, 16, icon_ti       , mc_cb);
M2_XBMBUTTONP(el_buttonxbm_temp     , "w18h18", 16, 16, icon_temp     , mc_cb);
M2_XBMBUTTONP(el_buttonxbm_adc      , "w18h18", 16, 16, icon_adc      , mc_cb);
M2_XBMBUTTONP(el_buttonxbm_buzzer   , "w18h18", 16, 16, icon_buzzer   , mc_cb);
M2_XBMBUTTONP(el_buttonxbm_uart     , "w18h18", 16, 16, icon_uart     , mc_cb);
M2_XBMBUTTONP(el_buttonxbm_audio    , "w18h18", 16, 16, icon_audio    , mc_cb);

M2_LIST(el_buttonxbm_list) = { &el_buttonxbm_ti, &el_buttonxbm_temp, &el_buttonxbm_adc, &el_buttonxbm_buzzer, &el_buttonxbm_uart, &el_buttonxbm_audio };
M2_HLIST(el_buttonxbm_hlist, NULL, el_buttonxbm_list);
M2_ALIGN(el_buttonxbm, NULL, &el_buttonxbm_hlist);

M2_LABEL(el_goto_title, NULL, "XBM Example");
M2_ROOT(el_goto_part1, NULL, "XBMROOT", &el_xbm);
M2_ROOT(el_goto_part2, NULL, "XBMBUTTON", &el_buttonxbm);
M2_LIST(list_menu) = {&el_goto_title, &el_goto_part1, &el_goto_part2};
M2_VLIST(el_menu_vlist, NULL, list_menu);
M2_ALIGN(el_top, NULL, &el_menu_vlist);


M2tk m2(&el_top, m2_es_arm_u8g, m2_eh_2bs, m2_gh_u8g_bfs);

void paint()
{
    u8g_FirstPage(&u8g);
    do
    {
        m2.draw();
    } while (u8g_NextPage(&u8g));
}

void setup()
{
    // Connect u8glib with m2tklib
    m2_SetU8g(&u8g, m2_u8g_box_icon);

    // Assign u8g font to index 0
    m2.setFont(0, u8g_font_unifont);
    
    m2.setPin(M2_KEY_EXIT   , dy_key_1);
    m2.setPin(M2_KEY_NEXT   , dy_key_2);
    m2.setPin(M2_KEY_PREV   , dy_key_3);
    m2.setPin(M2_KEY_SELECT , dy_key_4);
}

void app_main_gui(void)
{
    uint8_t pos = 0;

    u8g_InitComFn(&u8g, &u8g_dev_uc1701_mini12864_hw_spi, u8g_com_hw_spi_fn);
    u8g_SetDefaultForegroundColor(&u8g);
    
    setup();
    
    while (true)
    {
        m2.checkKey();
        if (m2.handleKey())
        {
            paint();
        }
    }
}

#endif // USE_GUI

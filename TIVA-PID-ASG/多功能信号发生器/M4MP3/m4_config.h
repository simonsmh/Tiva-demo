#ifndef __M4_CONFIG
#define __M4_CONFIG

#define DEBUG
#define DEBUG_PORT              UART0_BASE

//#define USE_DY_PB_2
/*
    The USE_DY_PB_2 involves several pin mux remap & different peripherals:

   ----------------------------
        |  DY-PB1  |  DY-PB2
   -----+----------+-----------
    LCD |  UC1701  |  UC1705
   -----+----------+-----------
    232 |  UART    |  MAX232?
   -----+----------+-----------
    TMP |  TMP100  |  LM75A
   ----------------------------
*/
#define M4_80MHZ
#define M4_SYSTICK
#define M4_UART                     // uart5
#define M4_LCD                      // uc1701 + gt20
#define M4_ADC
#define M4_AC
#define M4_DAC
#define M4_BUZZER                   // dac7512 | spwm
#define M4_SDC                      // sdcard
#define M4_GPIO                     // push button + led
#define M4_TEMP                     // lm75a/tmp100
#define M4_CODEC                    // tlv320aic23b
#define M4_SPWM                     // beeper
#define M4_PWM                      // led brightness

//#define M4_RTX                      // run basic rtx demo
//#define M4_FREERTOS                 // run basic freertos demo
//#define M4_TIRTOS

//#define UART_PROXY

#ifdef M4_LCD
#define USE_FONT_CHIP               // disable to use built-in font
//#define USE_GUI                     // enable GUI mode(m2tk), else would use cli mode
#endif // M4_LCD

#ifdef M4_SDC
#define M4_FATFS
#endif // M4_SDC

#ifdef M4_GPIO
#define M4_BUTTON                  // disable to use gpio key handler from m2tk lib
#endif // M4_GPIO

#define UART0_BAUD_RATE         115200
#define UART5_BAUD_RATE         57600

#ifdef M4_80MHZ
#define SSI1_RATE               20000000
#define SSI2_RATE               10000000
#else // M4_80MHZ
#define SSI1_RATE               100000
#define SSI2_RATE               100000
#endif // M4_80MHZ

#ifdef USE_DY_PB_2      //      new pcb layout
#define UC1701_RATE             SSI2_RATE
#define GT20L16S1Y_RATE         SSI2_RATE
#define SDC_RATE                SSI2_RATE
#else // USE_DY_PB_2            old pcb layout
// TODO: define rate of PB1
#define UC1701_RATE             SSI1_RATE
#define GT20L16S1Y_RATE         SSI2_RATE
#endif /// USE_DY_PB_2


#define DAC7512_RATE            GT20L16S1Y_RATE

#define MAX_MSG_LEN             256             // generic message length
#define MAX_FONT_BUFFER_LEN     32              // a maximum 16x16 pixel

#endif

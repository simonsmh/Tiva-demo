#ifndef __M4_CONFIG
#define __M4_CONFIG

#define DEBUG
#define DEBUG_PORT              UART0_BASE

#define M4_80MHZ


#define M4_LCD                  // uc1701 + gt20
//#define M4_SDC                  // sdcard
#define M4_GPIO                 // push button + led


#ifdef M4_LCD
#define USE_FONT_CHIP           // disable to use built-in font
#endif // M4_LCD

#ifdef M4_SDC
#define M4_FATFS
#endif // M4_SDC

#ifdef M4_80MHZ
#define SSI1_RATE               1000000
#define SSI2_RATE               1000000
#else // M4_80MHZ
#define SSI1_RATE               10000
#define SSI2_RATE               10000
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

#define MAX_MSG_LEN             256             // generic message length
#define MAX_FONT_BUFFER_LEN     32              // a maximum 16x16 pixel

#endif

// note: the uc1701.h shares configuration with u8g
//

#ifndef __UC1701_H__
#define __UC1701_H__
#include <stdint.h>
#include <stdbool.h>
#include "driverlib/sysctl.h"

#include "hw_uc1701.h"

#define DY_PA5_FIX                      // fix for DYTiva-PB 1.0, 1.1 (SSI1 rx)
#define DY_1_5V_FIX                     // fix for LaunchPad not output 1.5v

#define BUILT_IN_FONT               // disable this to save flash


#if defined(USE_DY_PB_3)
// CS   : PA5   : CS0
// CD   : PE0   : CD=0, command; CD=1, data
// BKL  : PF3   : PWM
#define LCD_PERIPH_SPI_CS           SYSCTL_PERIPH_GPIOA

#ifdef USE_DY_PB_FINAL
#define LCD_PERIPH_CD               SYSCTL_PERIPH_GPIOB
#else // USE_DY_PB_FINAL
#define LCD_PERIPH_CD               SYSCTL_PERIPH_GPIOE
#endif // USE_DY_PB_FINAL

#define LCD_GPIO_SPI_CS             GPIO_PORTA_BASE
#ifdef USE_DY_PB_FINAL
#define LCD_GPIO_CD                 GPIO_PORTB_BASE
#else // USE_DY_PB_FINAL
#define LCD_GPIO_CD                 GPIO_PORTE_BASE
#endif // USE_DY_PB_FINAL

#define LCD_PIN_SPI_CS              GPIO_PIN_5
#ifdef USE_DY_PB_FINAL
#define LCD_PIN_CD                  GPIO_PIN_3
#else // USE_DY_PB_FINAL
#define LCD_PIN_CD                  GPIO_PIN_0
#endif // USE_DY_PB_FINAL

#define LCD_PIN_SPI_PORT            SSI2_BASE
#elif defined(USE_DY_PB_2)
// CS   : PF2   : CS0
// CD   : PF3   : CD=0, command; CD=1, data
// RESET:

#define LCD_PERIPH_SPI_CS           SYSCTL_PERIPH_GPIOF
#define LCD_PERIPH_CD               SYSCTL_PERIPH_GPIOF
//#define LCD_PERIPH_RESET            SYSCTL_PERIPH_GPIOE

#define LCD_GPIO_SPI_CS             GPIO_PORTF_BASE
#define LCD_GPIO_CD                 GPIO_PORTF_BASE
//#define LCD_GPIO_RESET              GPIO_PORTE_BASE

#define LCD_PIN_SPI_CS              GPIO_PIN_2
#define LCD_PIN_CD                  GPIO_PIN_3
//#define LCD_PIN_RESET               GPIO_PIN_0

#define LCD_PIN_SPI_PORT            SSI2_BASE


#define LCD_PERIPH_SPI_CS           SYSCTL_PERIPH_GPIOB
#define LCD_PERIPH_CD               SYSCTL_PERIPH_GPIOD
#define LCD_PERIPH_RESET            SYSCTL_PERIPH_GPIOE

#define LCD_GPIO_SPI_CS             GPIO_PORTB_BASE
    
#ifndef DY_PA5_FIX
#define LCD_GPIO_CD                 GPIO_PORTD_BASE
#else // DY_PA5_FIX
#define LCD_GPIO_CD                 GPIO_PORTA_BASE
#endif // DY_PA5_FIX

#define LCD_GPIO_RESET              GPIO_PORTE_BASE


#define LCD_PIN_BKL                 GPIO_PIN_5
#define LCD_PIN_SPI_CS              GPIO_PIN_2

#ifndef DY_PA5_FIX
#define LCD_PIN_CD                  GPIO_PIN_2
#else
#define LCD_PIN_CD                  GPIO_PIN_5
#endif

#define LCD_PIN_RESET               GPIO_PIN_0

#define LCD_PIN_SPI_PORT            SSI1_BASE
#endif // USE_DY_PB_2

extern void UC1701Init(unsigned long );
extern void UC1701DataWrite(unsigned char );
extern void UC1701CmdWrite(unsigned char );
extern void UC1701DoubleCmdWrite(unsigned char , unsigned char );
extern void UC1701AddressSet(unsigned char , unsigned char );
#ifdef BUILT_IN_FONT
extern void UC1701Display(unsigned char , unsigned char , unsigned char );
extern void UC1701ChineseDispaly(unsigned char , unsigned char , unsigned short );
extern void UC1701CharDispaly(unsigned char , unsigned char , char *);
extern void UC1701DisplayN(unsigned char , unsigned char , unsigned long );
#endif // BUILT_IN_FONT
extern void UC1701FontDisplay(unsigned char , unsigned char , unsigned char , unsigned char , unsigned char * );
extern void UC1701Clear(void);
extern void UC1701InverseEnable(void);
extern void UC1701InverseDisable(void);
extern void UC1701AllPixelOnEnable(void);
extern void UC1701AllPixelOnDisable(void);
extern void UC1701DisplayOn(void);
extern void UC1701DisplayOff(void);
extern void UC1701ScrollLineSet(unsigned char ucLine);
extern void UC1701PMSet(unsigned char ucPM);
extern void UC1701BRSet(unsigned char ucBR);
extern void UC1701SegDirSet(unsigned char ucSegDir);
extern void UC1701ComDirSet(unsigned char ucComDir);
extern void UC1701Test(void);

#endif //__UC1701_H__

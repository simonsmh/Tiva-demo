#ifndef __GT20L16S1Y_H__
#define __GT20L16S1Y_H__

#define USE_8BIT_SSI                    // 8bit ssi to compatible with uc1705 on ssi2

#define FONT_PIN_SPI_PORT           SSI2_BASE

#define FONT_PERIPH_SPI_CS          SYSCTL_PERIPH_GPIOC
#define FONT_GPIO_SPI_CS            GPIO_PORTC_BASE
#define FONT_PIN_SPI_CS             GPIO_PIN_6

typedef enum __FontType_
{
    GB2312_15x16,
    ASCII_7x8,
    GB2312_8x16,
    ASCII_8x16,
    ASCII_5x7,
    ASCII_Arial,
    ASCII_8x16_Bold,
    ASCII_TimesNewRoman
} FontType;

typedef struct __FontSize_
{
    FontType name;
    int height;
    int width;
} FontSize;

extern void GT20Init(uint32_t );
extern void GT20Read(FontType , uint16_t , uint8_t *, uint8_t *, uint16_t *);

#endif //__GT20L16S1Y_H__

// copyleft pxj 2013

#ifndef __DAC7512_H__
#define __DAC7512_H__

//#define DAC_USE_DMA                 // use dma to archive 44100Hz
                                    // else timer would be used
#ifdef USE_DY_PB_2
#define DAC_PIN_SPI_PORT            SSI1_BASE
#else // USE_DY_PB_2
#define DAC_PIN_SPI_PORT            SSI2_BASE
#endif // USE_DY_PB_2

#define DAC_TIMER_PIN_PORT              TIMER0_BASE
#define DAC_TIMER_PIN_PIN               TIMER_A

#define DAC_TIMER_PERIPH                SYSCTL_PERIPH_TIMER0
#define DAC_TIMER_INT                   INT_TIMER0A

// microwire?!
// since ssi2 is connect to font chip, we may manually trigger the sync pin
#define DAC_PERIPH_SPI_CS           SYSCTL_PERIPH_GPIOF
#define DAC_GPIO_SPI_CS             GPIO_PORTF_BASE
#define DAC_PIN_SPI_CS              GPIO_PIN_1

#define BIT_DEPTH                   16      // bit
#define SAMPLE_RATE                 8000    // Hz

#define DAC_BUFFER_LEN              128     // bytes
#define SD_BUFFER_LEN               DAC_BUFFER_LEN/4

typedef struct __CircularBuffer_
{
    uint16_t start;
    uint16_t count;
    uint16_t elems[DAC_BUFFER_LEN/2];
} CircularBuffer;

extern void DAC7512Init(uint32_t );
void DAC7512Write(uint16_t );
extern void DAC7512EnableTrans(void);
extern void DAC7512DisableTrans(void);
extern void DAC7512FillBuffer(uint16_t *);

#endif // __DAC7512_H__

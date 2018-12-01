// copyleft pxj 2013
#include <stdint.h>
#include <stdbool.h>

#include "hw_gt20l16s1y.h"

#include "gt20l16s1y.h"

#include "inc/hw_memmap.h"
#include "inc/hw_gpio.h"

#include "driverlib/sysctl.h"
#include "driverlib/rom_map.h"

#include "driverlib/ssi.h"
#include "driverlib/gpio.h"

FontSize fontSize[] = {
    { GB2312_15x16          , 16, 16 }, // Hanzi
    { ASCII_7x8             ,  7,  8 }, // 'd be square
    { GB2312_8x16           ,  8, 16 },
    { ASCII_8x16            ,  8, 16 }, // Alphabet
    { ASCII_5x7             ,  5,  7 },
    { ASCII_Arial           ,  0,  0 },
    { ASCII_8x16_Bold       ,  8, 16 },
    { ASCII_TimesNewRoman   ,  0,  0 },
};

uint32_t GT20GetFontAddr(FontType fontType, uint16_t fontCode)
{
    uint32_t fontAddr, baseAddr;
    uint16_t addrMSB, addrLSB;

    addrMSB = (fontCode >> 8) & 0xFF;
    addrLSB = fontCode & 0xFF;

    switch (fontType)
    {
    case GB2312_15x16:
        baseAddr = 0;
        if ((addrMSB == 0xA9) && (addrLSB >= 0xA1))
            fontAddr = (282 + (addrLSB - 0xA1 ))*32 + baseAddr;
        else if ((addrMSB >= 0xA1) && (addrMSB <= 0xA3) && (addrLSB >= 0xA1))
            fontAddr =((addrMSB - 0xA1)*94 + (addrLSB - 0xA1))*32 + baseAddr;
        else if ((addrMSB >= 0xB0) && (addrMSB <= 0xF7) && (addrLSB >= 0xA1))
            fontAddr = ((addrMSB - 0xB0)*94 + (addrLSB - 0xA1) + 846)*32 + baseAddr;
        break;
    case ASCII_7x8:
        baseAddr = 0x66c0;
        if ((fontCode >= 0x20) && (fontCode <= 0x7E))
            fontAddr = (fontCode - 0x20 )*8 + baseAddr;
        break;
    case GB2312_8x16:
        baseAddr = 0x3B7D0;
        if ((fontCode >= 0xAAA1) && (fontCode <= 0xAAFE))
            fontAddr = (fontCode - 0xAAA1 )*16 + baseAddr;
        else if((fontCode >= 0xABA1) && (fontCode <= 0xABC0))
            fontAddr = (fontCode - 0xABA1 + 95)*16 + baseAddr;
        break;
    case ASCII_8x16:
        baseAddr = 0x3B7C0;
        if ((fontCode >= 0x20) && (fontCode <= 0x7E))
            fontAddr = (fontCode - 0x20)*16 + baseAddr;
        break;
    case ASCII_5x7:
        baseAddr = 0x3BFC0;
        if ((fontCode >= 0x20) && (fontCode <= 0x7E))
            fontAddr = (fontCode - 0x20)*8 + baseAddr;
        break;
    case ASCII_Arial:
        baseAddr = 0x3C2C0;
        if ((fontCode >= 0x20) && (fontCode <= 0x7E))
            fontAddr = (fontCode - 0x20)*34 + baseAddr;
        break;
    case ASCII_8x16_Bold:
        baseAddr = 0x3CF80;
        if ((fontCode >= 0x20) && (fontCode <= 0x7E))
            fontAddr = (fontCode - 0x20 )*16 + baseAddr;
        break;
    case ASCII_TimesNewRoman:
        baseAddr = 0x3D580;
        if ((fontCode >= 0x20) && (fontCode <= 0x7E))
            fontAddr = (fontCode - 0x20)*34 + baseAddr;
        break;
    }
    return fontAddr;
}

void GT20Init(uint32_t spiClock)
{
    MAP_SysCtlPeripheralEnable(FONT_PERIPH_SPI_CS);
    
    MAP_GPIOPinTypeGPIOOutput(FONT_GPIO_SPI_CS, FONT_PIN_SPI_CS);
    
    // disable chip select
    MAP_GPIOPinWrite(FONT_GPIO_SPI_CS, FONT_PIN_SPI_CS, FONT_PIN_SPI_CS);  // PC6 - CS
    
#ifdef USE_8BIT_SSI
    // uc1701 would be init first, which is configured as 8 bit
#else // USE_8BIT_SSI
    // use 16 bitwidth to compatitible with dac
    SSIConfigSetExpClk(FONT_PIN_SPI_PORT, SysCtlClockGet(), SSI_FRF_MOTO_MODE_3,
        SSI_MODE_MASTER, spiClock, 16);     // a maximum bit width of 16
#endif // USE_8BIT_SSI
    SSIEnable(FONT_PIN_SPI_PORT);
}

void GT20Read(FontType fontType, uint16_t unicode, uint8_t *width, uint8_t *height, uint16_t *fontBuffer)
{
    uint32_t spiWriteBuffer, spiReadBuffer;
    
    uint32_t fontCode, fontAddr;

    uint16_t fontBufferLen;

    // 1. write instruction to chip
    MAP_GPIOPinWrite(FONT_GPIO_SPI_CS, FONT_PIN_SPI_CS, ~FONT_PIN_SPI_CS);  // PC6 - CS
    
    // may be some conversion from unicode<=>fontcode
    fontCode = ((unicode >> 8) & 0xFF) | ((unicode & 0xFF) << 8);
    //fontCode = 0xBBB6;        // »¶
    
    fontAddr = GT20GetFontAddr(fontType, fontCode);
    //fontAddr = 0x0000EDA0;    // »¶
    
    fontBufferLen = (fontSize[fontType].width*fontSize[fontType].height)/16;  // read by 16 bit

    //fontAddr = 0x0103070F;
    //fontAddr = 0x00003301;
    
    //while (1)
    //    MAP_SSIDataPut(FONT_PIN_SPI_PORT, fontAddr);
    
#ifdef USE_8BIT_SSI
    // note: 8bit ssi would cause more overhead
    
    spiWriteBuffer = GT20_READ;
    MAP_SSIDataPut(FONT_PIN_SPI_PORT, spiWriteBuffer);
    spiWriteBuffer = ((fontAddr >> 16) & 0xFF);
    MAP_SSIDataPut(FONT_PIN_SPI_PORT, spiWriteBuffer);
    spiWriteBuffer = ((fontAddr >> 8) & 0xFF);
    MAP_SSIDataPut(FONT_PIN_SPI_PORT, spiWriteBuffer);
    spiWriteBuffer = (fontAddr & 0xFF);
    MAP_SSIDataPut(FONT_PIN_SPI_PORT, spiWriteBuffer);
#else // USE_8BIT_SSI
    // forge the request packet
    // little edian..?
    spiWriteBuffer = (GT20_READ << 8) + ((fontAddr >> 16) & 0xFF);
    MAP_SSIDataPut(FONT_PIN_SPI_PORT, spiWriteBuffer);
    spiWriteBuffer = (fontAddr & 0xFFFF);
    MAP_SSIDataPut(FONT_PIN_SPI_PORT, spiWriteBuffer);
#endif // USE_8BIT_SSI
    
    // collect shit data
    while (SSIDataGetNonBlocking(FONT_PIN_SPI_PORT, &spiReadBuffer))
       ;
    
    // TODO: use isr handle this
    while(MAP_SSIBusy(FONT_PIN_SPI_PORT))
        ;
    
    // 2. wait for data
    while (fontBufferLen --)
    {
        // collect shit data
        while (SSIDataGetNonBlocking(FONT_PIN_SPI_PORT, &spiReadBuffer))
            ;
        
#ifdef USE_8BIT_SSI
        // write dummy data
        SSIDataPut(FONT_PIN_SPI_PORT, 0xFF);
        SSIDataPut(FONT_PIN_SPI_PORT, 0xFF);
        
        SSIDataGet(FONT_PIN_SPI_PORT, &spiReadBuffer);
        *fontBuffer = spiReadBuffer & 0xFF;
        SSIDataGet(FONT_PIN_SPI_PORT, &spiReadBuffer);
        *fontBuffer = ((*fontBuffer) | ((spiReadBuffer & 0xFF) << 8));
        
        //if (spiReadBuffer != 0)
        //    while (1);
        
#else // USE_8BIT_SSI
        // write dummy data
        SSIDataPut(FONT_PIN_SPI_PORT, 0xFFFF);

        // get fifo data
        SSIDataGet(FONT_PIN_SPI_PORT, &spiReadBuffer);
    
        // reverse the bytes
        spiReadBuffer = (spiReadBuffer >> 8) | (spiReadBuffer << 8);
        
        *fontBuffer = spiReadBuffer & 0xFFFF;
#endif // USE_8BIT_SSI
        fontBuffer ++;
    }
    // TODO: use isr handle this
    while(MAP_SSIBusy(FONT_PIN_SPI_PORT))
        ;

    MAP_GPIOPinWrite(FONT_GPIO_SPI_CS, FONT_PIN_SPI_CS, FONT_PIN_SPI_CS);   // CS

    *width  = fontSize[fontType].width;
    *height = fontSize[fontType].height;
}

void Gt20ReadHS()
{
    
}

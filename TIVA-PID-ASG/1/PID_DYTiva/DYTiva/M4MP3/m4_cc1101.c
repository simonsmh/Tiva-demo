
#include "cc1101/CC1100-CC2500.h"
#include "cc1101/TI_CC_spi.h"
#include "cc1101/TI_CC_CC1100-CC2500.h"

#include "m4_config.h"
#include "m4_common.h"

extern char paTable[];
extern char paTableLen;

char txBuffer[4];
char rxBuffer[4];

// callback array
void (*cc1101_int_handler)(char * );

// pin:
// PF2=GDO(ATEST)
// PF3=LED

void cc1101_init(void)
{
    TI_CC_SPISetup(CC1101_RATE);
    
    TI_CC_PowerupResetCCxxxx();
    
    writeRFSettings();
    
    TI_CC_SPIWriteBurstReg(TI_CCxxx0_PATABLE, paTable, paTableLen);//Write PATABLE

    TI_CC_SPIStrobe(TI_CCxxx0_SRX);           // Initialize CCxxxx in RX mo
}

void cc1101_send(char data)
{
    RFSendPacket(&data, 1);
}

void cc1101_int_isr(void)
{
  char len=2;                               // Len of pkt to be RXed (only addr
                                            // plus data; size byte not incl b/c
                                            // stripped away within RX function)
  if (RFReceivePacket(rxBuffer,&len))       // Fetch packet from CCxxxx
    //TI_CC_LED_PxOUT ^= rxBuffer[1];         // Toggle LEDs according to pkt data
    MAP_GPIOPinWrite(CC1101_GPIO_LED, CC1101_PIN_LED, CC1101_PIN_LED | rxBuffer[1]);

  //TI_CC_GDO0_PxIFG &= ~TI_CC_GDO0_PIN;                 // Clear flag
  lcd_printf(0, 1, "received");
//  if (cc1101_int_handler != NULL)
//  {
//    (*cc1101_int_handler)();
//  }
}

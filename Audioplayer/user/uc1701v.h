#ifndef __UC1701_H__
#define __UC1701_H__

#include "hw_uc1701.h"
#include "uc1701v.h"

extern void UC1701Init(unsigned long ulSpiClock);
extern void UC1701DataWrite(unsigned char ucData);
extern void UC1701CmdWrite(unsigned char ucCmd);
extern void UC1701DoubleCmdWrite(unsigned char ucCmd, unsigned char ucData);
extern void UC1701AddressSet(unsigned char ucPA, unsigned char ucCA);
extern void UC1701Dispaly(unsigned char ucLine, unsigned char ucRow,
                           unsigned char ucAsciiWord);
extern void UC1701InverseDispaly(unsigned char ucLine, unsigned char ucRow,
                                 unsigned char ucAsciiWord);
extern void UC1701CharDispaly(unsigned char ucLine, unsigned char ucRow,
                              char *pcChar);
extern void UC1701ChineseDispaly(unsigned char ucLine, unsigned char ucRow,
                                 unsigned char ucLength, char *pcChar);
extern void UC1701DisplayN(unsigned char ucLine, unsigned char ucRow,
                             unsigned long n);
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


#endif

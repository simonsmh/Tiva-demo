#ifndef __HW_UC1701_H__
#define __HW_UC1701_H__

//
//
#define UC1701_WRITE_DATA       0x00
  
//
//
#define UC1701_READ_DATA        0x00
  
//
//
#define UC1701_GET_STATUS       0x00
  
//
//
#define UC1701_SET_CA_LSB       0x00
  
//
//
#define UC1701_SET_CA_MSB       0x10
  
//
//
#define UC1701_SET_CA_MASK      0x0F

//
//
#define UC1701_SET_PC           0x28
#define UC1701_SET_PC_MASK      0x07
  
//
//
#define UC1701_SET_SL           0x40
#define UC1701_SET_SL_MASK      0x1F
  
//
//
#define UC1701_SET_PA           0xB0
#define UC1701_SET_PA_MASK      0x0F
  
//
//
#define UC1701_SET_PM           0x81
#define UC1701_SET_PM_MASK      0x3F
  
//
//
#define UC1701_SET_DC1          0xA4
#define UC1701_SET_DC1_EN       0xA5
  
//
//
#define UC1701_SET_DC0          0xA6
#define UC1701_SET_DC0_EN       0xA7
  
//
//
#define UC1701_SET_DC2          0xAE
#define UC1701_SET_DC2_EN       0xAF
  
//
//
#define UC1701_SET_LC21         0xC0
#define UC1701_SET_LC21_MASK    0x06
  
//
//
#define UC1701_SET_SC           0xE2
  
//
//
#define UC1701_SET_NOP          0xE3
  
//
//
#define UC1701_SET_BR_MASK      0xA3
#define UC1701_SET_BR_7         0xA2
#define UC1701_SET_BR_9         0xA3

//
//
#define UC1701_SEG_DIR_MASK     0xA1
#define UC1701_SEG_DIR_NORMAL   0xA0
#define UC1701_SET_DIR_INVERSE  0xA1

//
//
#define UC1701_COM_DIR_MASK     0xC8
#define UC1701_COM_DIR_NORMAL   0xC0
#define UC1701_COM_DIR_INVERSE  0xC8
  
//
//
#define UC1701_READ_DATA_CMD    0xFF
#define UC1701_READ_DATA_MASK   0xFF
  
//
//
#define UC1701_GET_STATUS_CMD   0xFE
#define UC1701_READ_DATA_MASK   0xFF


#endif // __HW_UC1701_H__

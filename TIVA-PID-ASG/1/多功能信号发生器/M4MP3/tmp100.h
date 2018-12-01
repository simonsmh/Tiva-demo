// copyleft pxj 2013

#ifndef __TMP100_H__
#define __TMP100_H__

#define DY_I2C_SPEED_FIX                        // fix for DYTiva-PB 1.0, 1.1 (I2C 10K resistor)

#define TMP_PIN_I2C_PORT            I2C1_BASE

#define TMP_READ_DELAY              400         // fix for I2CMasterBusBusy

#ifdef USE_DY_PB_2
#define TMP_I2C_ADDR                0x49        // slave address (1001001)
#else // USE_DY_PB_2
#define TMP_I2C_ADDR                0x48        // slave address (1001000)
#endif // USE_DY_PB_2

#define TMP_TEMP_REG                0x0
#define TMP_CONFIG_REG              0x1
#define TMP_LOW_REG                 0x2
#define TMP_HIGH_REG                0x3

#define TMP_12                      0x60        // 01100000
    
void TMP100Init(void);
void TMP100ResolutionSet(void);
uint16_t TMP100DataRead(void);

#endif // __TMP100_H__

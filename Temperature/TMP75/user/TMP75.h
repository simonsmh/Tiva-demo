#ifndef _TMP75_H_
#define _TMP75_H_


#include <stdint.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdarg.h>
#include <string.h>


#include "inc/hw_ints.h"
#include "inc/hw_memmap.h"
#include "inc/hw_types.h"
#include "inc/hw_timer.h"
#include "inc/hw_ints.h"
#include "inc/hw_gpio.h"
#include "inc/hw_i2c.h"
#include "inc/hw_sysctl.h"
#include "inc/hw_adc.h"
#include "inc/hw_ssi.h"
#include "driverlib/adc.h"
#include "driverlib/rom.h"
#include "driverlib/timer.h"
#include "driverlib/interrupt.h"
#include "driverlib/sysctl.h"
#include "driverlib/systick.h"
#include "driverlib/gpio.h"
#include "driverlib/pin_map.h"
#include "driverlib/ssi.h"
#include "driverlib/i2c.h"
#include "driverlib/udma.h"
#include "driverlib/fpu.h"
#include "driverlib/rom.h"

#include "utils/uartstdio.h"
#include "I2C_Stellaris_API.h"

#define TMP_PIN_I2C_PORT            I2C1_BASE

#define TMP_READ_DELAY              600         // fix for I2CMasterBusBusy

#define TMP_I2C_ADDR                0x48        // slave address (1001000)


#define TMP_TEMP_REG                0x0
#define TMP_CONFIG_REG              0x1
#define TMP_LOW_REG                 0x2
#define TMP_HIGH_REG                0x3

#define TMP_12                      0x60        // 01100000


void TMP75Init(void);
void TMP75ResolutionSet(void);
uint16_t TMP75DataRead(void);
uint8_t TMP75AddrTest(uint8_t addr);
float temp_read(void);

#endif /* _TMP75_H_ */

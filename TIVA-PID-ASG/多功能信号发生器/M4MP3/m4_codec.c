#include <stdint.h>
#include <stdbool.h>
#include <math.h>

#include "inc/hw_ints.h"
#include "inc/hw_memmap.h"
#include "driverlib/adc.h"
#include "driverlib/debug.h"
#include "driverlib/fpu.h"
#include "driverlib/gpio.h"
#include "driverlib/interrupt.h"
#include "driverlib/pin_map.h"
#include "driverlib/rom.h"
#include "driverlib/sysctl.h"
#include "driverlib/ssi.h"
#include "driverlib/rom.h"
#include "driverlib/rom_map.h"
#include "driverlib/timer.h"

#include "m4_config.h"
#include "m4_common.h"

#include "tlv320aic23b.h"

#define ROOT_DIRECTORY                  "/"

#define SDC_DISKPROC_INTERVAL           100                     // ms

#define SDC_TIMER_PIN_PORT              TIMER0_BASE
#define SDC_TIMER_PIN_PIN               TIMER_B

#define SDC_TIMER_PERIPH                SYSCTL_PERIPH_TIMER0
#define SDC_TIMER_INT                   INT_TIMER0B

// fatfs global variable
FATFS fs;
FRESULT file_result;

extern void disk_timerproc (void);

// we only initialize the sdc in audio mode

void disk_timer_init()
{
    uint32_t sys_ctl_clock, tick_delay;
    
    MAP_SysCtlPeripheralEnable(SDC_TIMER_PERIPH);

    TimerConfigure(SDC_TIMER_PIN_PORT, TIMER_CFG_SPLIT_PAIR | TIMER_CFG_B_PERIODIC);
    
    TimerControlEvent(SDC_TIMER_PIN_PORT, TIMER_B, TIMER_EVENT_BOTH_EDGES);
    
    // 100Hz timer
    sys_ctl_clock = SysCtlClockGet();
    tick_delay = (sys_ctl_clock/(1000*4))*SDC_DISKPROC_INTERVAL;
    TimerLoadSet(SDC_TIMER_PIN_PORT, SDC_TIMER_PIN_PIN, tick_delay);
    
    TimerIntEnable(SDC_TIMER_PIN_PORT, TIMER_TIMB_TIMEOUT);

    IntEnable(SDC_TIMER_INT);
    
    TimerEnable(SDC_TIMER_PIN_PORT, SDC_TIMER_PIN_PIN);
}

void disk_timer_destory()
{
    TimerDisable(SDC_TIMER_PIN_PORT, SDC_TIMER_PIN_PIN);
}

void timer0b_int_isr(void)
{
    TimerIntClear(SDC_TIMER_PIN_PORT, TIMER_TIMB_TIMEOUT);
    
    disk_timerproc();
}

#if 1
void file_test()
{
    FIL fsrc, fdst;
    BYTE buffer[16];
    FRESULT fr;
    UINT br, bw;

    /* Create destination file on the drive 0 */
    fr = f_open(&fdst, "0:file.bin", FA_CREATE_ALWAYS | FA_READ | FA_WRITE);
    
    buffer[0] = 0x12;
    buffer[1] = 0x34;
    
    fr = f_write(&fdst, buffer, br, &bw);
    fr = f_read(&fsrc, buffer, sizeof(buffer), &br);

    f_close(&fsrc);
}
#endif

void sdc_init()
{
#ifdef M4_SDC
    
    file_result = f_mount(&fs, "0:", 1);
    disk_timer_init();
    
#if 1
    file_test();
#endif
    
#endif // M4_SDC
}

void codec_init()
{
    sdc_init();
    TLV320AIC23BInit();
}

void codec_get_file_name(char *file_name)
{
    FRESULT f_result;
    FILINFO f_nfo;
    char *f_name;
    DIR dir;    
    
    file_result = f_opendir(&dir, ROOT_DIRECTORY);
    
//    if (file_result == FR_OK)
//    {
//        i = strlen(path);
//        while (true)
//        {
//            f_result = f_readdir(&dir, &f_nfo);                   /* Read a directory item */
//            if (file_result != FR_OK || f_nfo.fname[0] == 0)
//                break;  /* Break on error or end of dir */
//            if (f_nfo.fname[0] == '.')
//                continue;             /* Ignore dot entry */
//            file_name = f_nfo.fname;
//            if (f_nfo.fattrib & AM_DIR) {                    /* It is a directory */
//                sprintf(&path[i], "/%s", fn);
//                f_result = scan_files(path);
//                if (file_result != FR_OK)
//                    break;+
//                path[i] = 0;
//            } else {                                       /* It is a file. */
//                m4_dbgprt("%s/%s\n", path, fn);
//            }
//        }
//        f_closedir(&dir)
//    }
}

void codec_play(char *file_name)
{
    uint16_t wave_freq = 0;

    FIL file;
    FRESULT file_result;
    

/*
     uint8_t sd_data[SD_BUFFER_LEN];
     uint32_t sd_data_len;

     int pos;

     file_result = f_open(&file, file_name, FA_READ);

     if (file_result == FR_OK)
     {
         // escape wave file header
         file_result = f_lseek(&file, 43);

         // enable mcu>>>dac transfer
         DAC7512EnableTrans();

         while (1)
         { // continuously fetch data from file
             f_read(&file, sd_data, SD_BUFFER_LEN, &sd_data_len);

             DAC7512FillBuffer((uint16_t *)&sd_data);
         }

         // disable transfer
         DAC7512DisableTrans();

         f_close(&file);
     }
*/
}

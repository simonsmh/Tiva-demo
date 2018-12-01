#include <stdint.h>
#include <stdbool.h>
#include <math.h>
#include <string.h>

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
#define MP3_SUFFIX                      ".wav"

// fatfs global variable
FATFS fs;
FRESULT file_result;

// we only initialize the sdc in audio mode

#if 1
void file_test()
{
    FIL fs;
    BYTE buffer[16];
    FRESULT fr;
    UINT btw, bw;
    UINT btr, br;

    /* Create destination file on the drive 0 */
    fr = f_open(&fs, "0:file.bin", FA_CREATE_ALWAYS | FA_READ | FA_WRITE);
    
    buffer[0] = 0x12;
    buffer[1] = 0x34;
    
    btw = 16;
    
    fr = f_write(&fs, buffer, btw, &bw);
    fr = f_read(&fs, buffer, sizeof(buffer), &br);

    fr = f_close(&fs);
}
#endif

void sdc_init()
{
#ifdef M4_SDC
    
    file_result = f_mount(&fs, "0:", 1);
    
#if 0
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
    DIR dir;
    
    file_result = f_opendir(&dir, ROOT_DIRECTORY);
    
    if (file_result == FR_OK)
    {
        while (1)
        {
            f_result = f_readdir(&dir, &f_nfo);                   /* Read a directory item */
            if (file_result != FR_OK || f_nfo.fname[0] == 0)
                break;  /* Break on error or end of dir */
            if (f_nfo.fname[0] == '.')
                continue;             /* Ignore dot entry */
            if (f_nfo.fattrib & AM_DIR)
            { // It is a directory
                m4_dbgprt("%s\r\n", f_nfo.fname);
                // f_result = scan_files(path); disable recursive
                if (file_result != FR_OK)
                    break;
            }
            else
            { // It is a file
                m4_dbgprt("%s\r\n", f_nfo.fname);
                
                // TODO: check the file suffix
                
                strcpy(file_name, f_nfo.fname);
                
                break;
            }
        }
        f_closedir(&dir);
    }
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

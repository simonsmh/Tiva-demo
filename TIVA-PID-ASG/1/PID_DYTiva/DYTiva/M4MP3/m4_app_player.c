#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>

#include <driverlib/sysctl.h>

#include "sound.h"
#include "m4_common.h"

typedef enum __Player_State_
{
    S_FIRST,
    S_STOP = S_FIRST,
    S_PLAY,
    S_PAUSE,
    S_LAST,
} Player_State;

typedef enum __Player_Button_
{
    BT_FIRST,
    BT_NONE = BT_FIRST,
    BT_SELECT,
    BT_CANCEL,
    BT_LEFT,
    BT_RIGHT,
    BT_UP,
    BT_DOWN,
    BT_LAST
} Player_Button;

Player_State g_sSoundState = S_STOP;
Player_Button g_button_press = BT_NONE;

#define NUM_LIST_STRINGS 48
#define MAX_FILENAME_STRING_LEN (3 + 8 + 1 + 3 + 1)
char g_pcFilenames[NUM_LIST_STRINGS][MAX_FILENAME_STRING_LEN];

unsigned long g_DACVolume = 20;
unsigned long g_ulWaveIndex = 0;

static FATFS g_sFatFs;
static FILINFO g_sFileInfo;
static unsigned long g_ulWavCount;
static DIR g_sDirObject;

tWaveHeader g_sWaveHeader;

void usb_msc_loop()
{
    lcd_clear();
    lcd_printf(0xFF, 0, "USB Card Reader");
    
    while (1)
    {
        switch (usb_msc_stat())
        {
            case 0: // MSC_DEV_DISCONNECTED
                lcd_printf(0, 1, "RESET");
                break;
            case 1: // MSC_DEV_CONNECTED
                // this should never happen
                break;
            case 2: // MSC_DEV_IDLE
                lcd_printf(0, 1, "IDLE ");
                break;
            case 3: // MSC_DEV_READ
                lcd_printf(0, 1, "READ ");
                break;
            case 4: // MSC_DEV_WRITE
                lcd_printf(0, 1, "WRITE");
                break;
        }
    }
}

void gpio_button_down_player(Button button)
{
    static uint32_t last_key_down_tick;

    if ((sys_tick_count - last_key_down_tick) < KEY_INTERVAL_THRESHOLD)
    {
        m4_dbgprt("Jitter\r\n");
        return ;
    }
    last_key_down_tick = sys_tick_count;
    
    switch (button)
    {
        case Key_1:
            // PLAY / PAUSE
            g_button_press = BT_SELECT;
            break;
        case Key_2:
            // RECORD
            g_button_press = BT_CANCEL;
            break;
        case Key_3:
            // PREVIOUS
            g_button_press = BT_LEFT;
            break;
        case Key_4:
            // NEXT
            g_button_press = BT_RIGHT;
            break;
    }
}

//*****************************************************************************
//
// This function is called to read the contents of the root directory on
// a given FAT logical drive and fill the listbox containing the names of all
// audio WAV files found.
//
//*****************************************************************************
static int
FindWaveFilesOnDrive(const char *pcDrive, int iStartIndex)
{
    FRESULT fresult;
    int iCount;

    //
    // Open the current directory for access.
    //
    fresult = f_opendir(&g_sDirObject, pcDrive);

    //
    // Check for error and return if there is a problem.
    //
    if(fresult != FR_OK)
    {
        //
        // Ensure that the error is reported.
        //
        return(0);
    }

    //
    // Start by inserting at the next entry in the list box.
    //
    iCount = iStartIndex;

    //
    // Enter loop to enumerate through all directory entries.
    //
    while(1)
    {
        //
        // Read an entry from the directory.
        //
        fresult = f_readdir(&g_sDirObject, &g_sFileInfo);

        //
        // Check for error and return if there is a problem.
        //
        if(fresult != FR_OK)
        {
            return(0);
        }

        //
        // If the file name is blank, then this is the end of the
        // listing.
        //
        if(!g_sFileInfo.fname[0])
        {
            break;
        }

        //
        // Add the information as a line in the listbox widget if there is
        // space left and the filename contains ".wav".
        //
        if((g_ulWavCount < NUM_LIST_STRINGS) &&
           ((ustrstr(g_sFileInfo.fname, ".wav")) ||
            (ustrstr(g_sFileInfo.fname, ".WAV"))))
        {
            //
            // Ignore directories.
            //
            if((g_sFileInfo.fattrib & AM_DIR) == 0)
            {
                usnprintf(g_pcFilenames[iCount], MAX_FILENAME_STRING_LEN,
                          "%s", g_sFileInfo.fname);
                // If folder name is needed.
                //usnprintf(g_pcFilenames[iCount], MAX_FILENAME_STRING_LEN,
                //          "%s%s", pcDrive, g_sFileInfo.fname);

                //
                // Move on to the next entry in the list box.
                //
                iCount++;
            }
        }
    }

    //
    // Made it to here, return the number of files we found.
    //
    return(iCount);
}

//*****************************************************************************
//
// This function is called to read the contents of the current directory on
// the SD card and fill the listbox containing the names of all files.
//
//*****************************************************************************
static int
PopulateFileList()
{
    //
    // How many files can we find on the SD card (if present)?
    //
    g_ulWavCount = (unsigned long)FindWaveFilesOnDrive("0:/", 0);

    //
    // Did we find any files at all?
    //
    return(g_ulWavCount ? 0 : FR_NO_FILE);
}

void player_sm()
{
    switch(g_sSoundState)
    {
        //
        // Not playing audio. Loop, waiting for button presses, until
        // the user selects an audio file to play.
        //
        case S_STOP:
        {
            //
            // PLAY - Select button was pressed.
            //
            switch (g_button_press)
            {
                case BT_SELECT:
                {
                    //
                    // Try to open the selected filename. If successful,
                    // change state to playing, update the screen title to
                    // "PLAYING", and begin playing the audio file.
                    //
                    if(SoundOpen(g_pcFilenames[g_ulWaveIndex], 
                                  &g_sWaveHeader))
                    {
                        g_sSoundState = S_PLAY;
                        //updateTitle(g_sSoundState);

                        // 
                        // Begin to update playback time on the display.
                        //
                        //TimerEnable(TIMER0_BASE, TIMER_A);

                        SoundPlay();
                    }
                    //
                    // If opening the file failed, clear the screen and
                    // display error message.
                    //
                    else
                    {
                        //ClearScreen(&g_sContext, &maxRes);
//                        GrStringDrawCentered(&g_sContext, "ERROR", -1,
//                                           GrContextDpyWidthGet(&g_sContext)/2,
//                                           40, 0);
                    }
                    
                    break;
                }
                //
                // NEXT SONG - Left button was pressed.
                //
                case BT_LEFT:
                {
                    //
                    // Decrement the wave clip index.
                    //
                    g_ulWaveIndex = g_ulWaveIndex ?
                                   (g_ulWaveIndex - 1) : (g_ulWavCount - 1);

                    //
                    // Draw the next filename.
                    //
                    //updateSongTitle();
                    
                    break;
                }

                //
                // NEXT SONG - Right button was pressed.
                //
                case BT_RIGHT:
                {
                    //
                    // Increment the wave clip index.
                    //
                    g_ulWaveIndex++;

                    if(g_ulWaveIndex >= g_ulWavCount)
                    {
                        g_ulWaveIndex = 0;
                    }

                    //
                    // Draw the next filename.
                    //
                    //updateSongTitle();
                    
                    break;
                }
                default:
                {
                    break;
                }
            }

            break;
        }

        //
        // Song is being played. Just loop, waiting for the song to
        // finish, or the user to press a button.
        //
        case S_PLAY:
        {
            //
            // Check to see if audio was finished playing.
            //
            if(!SoundPlaybackStatus())
            {
                SoundStop();
                //TimerDisable(TIMER0_BASE, TIMER_A);
                g_sSoundState = S_STOP;
                //initScreen();
            }

            //
            // PAUSE - Select button was pressed.
            //                
            if(g_button_press == BT_SELECT)
            {
                SoundPause();
            }

            //
            // STOP - Left button was pressed.
            //                    
            if(g_button_press == BT_LEFT)
            {
                SoundStop();
                //TimerDisable(TIMER0_BASE, TIMER_A);
                g_sSoundState = S_STOP;
                //initScreen();
            }

            break;
        }
        default:
        {
            break;
        }
    }// end switch

    //
    // NOTE: Volume functionality during playback may
    // require a stronger I2C pull-up. This is verified
    // for operation using an effective pull-up R of
    // 750 ohms.

    //
    // VOLUME UP - Up button was pressed.
    //
    if(g_button_press == BT_UP)
    {
        //
        // Increase the volume by 10% and update the display.
        //
        SoundVolumeUp( 10 );
        g_DACVolume = SoundVolumeGet();
        //updateVolumeSlider();
    }

    //
    // VOLUME DOWN - Down button was pressed.
    //
    else if(g_button_press == BT_DOWN)
    {
        //
        // Decrease the volume by 10% and update the display.
        //
        SoundVolumeDown( 10 );
        g_DACVolume = SoundVolumeGet();
        //updateVolumeSlider();
    }
    
}

void app_main_player(void)
{
    int i_tries;    // try to enum the msc connected status
    char file_name[_MAX_LFN];
    
    // usb mode:
    for (i_tries = 0; i_tries < 100; i_tries ++)
    {
        if (usb_msc_stat() == 1) // MSC_DEV_CONNECTED
        {
            usb_msc_loop();    // never return
        }
        sleep(10);
    }
    
    // mp3 mode:
    lcd_clear();
    lcd_printf(0xFF, 0, "   MP3 Player");
    
    SoundInit();
    gpio_int_register(&gpio_button_down_player);
    
    PopulateFileList();
    
    g_DACVolume = SoundVolumeGet();
    
    while (1)
    {
        // check if usb connected
        if (usb_msc_stat() > 0)
        {
            SysCtlReset();
        }
        
        while (1)
        {
            player_sm();
        }

        codec_get_file_name(file_name);
        
        lcd_printf(0, 1, "%s", file_name);
        
        if (file_name == NULL)
        {
            lcd_printf(0, 1, "NO FILE!!");
        }
    }
}

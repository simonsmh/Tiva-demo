#ifndef __SOUND_H__
#define __SOUND_H__

//*****************************************************************************
//
// If building with a C++ compiler, make all of the definitions in this header
// have a C binding.
//
//*****************************************************************************
#ifdef __cplusplus
extern "C"
{
#endif

//*****************************************************************************
//
// Event definitions for the buffer callback function.
//
//*****************************************************************************
#define BUFFER_EVENT_FREE       0x00000001
#define BUFFER_EVENT_FULL       0x00000002

//*****************************************************************************
//
//! The header information parsed from a ``.wav'' file during a call to the
//! function WaveOpen.
//
//*****************************************************************************
typedef struct
{
    //
    //! Sample rate in bytes per second.
    //
    unsigned long ulSampleRate;

    //
    //! The average byte rate for the wav file.
    //
    unsigned long ulAvgByteRate;

    //
    //! The size of the wav data in the file.
    //
    unsigned long ulDataSize;

    //
    //! The number of bits per sample.
    //
    unsigned short usBitsPerSample;

    //
    //! The wav file format.
    //
    unsigned short usFormat;

    //
    //! The number of audio channels.
    //
    unsigned short usNumChannels;
}
tWaveHeader;

//*****************************************************************************
//
//! Possible return codes from the WaveOpen function.
//
//*****************************************************************************
typedef enum
{
    //
    //! The wav data was parsed successfully.
    //
    WAVE_OK = 0,

    //
    //! The RIFF information in the wav data is not supported.
    //
    WAVE_INVALID_RIFF,

    //
    //! The chunk size specified in the wav data is not supported.
    //
    WAVE_INVALID_CHUNK,

    //
    //! The format of the wav data is not supported.
    //
    WAVE_INVALID_FORMAT
}
tWaveReturnCode;

typedef struct WAV_HEADER
{
    char                ChunkID[4];     // "RIFF" string
    int                 ChunkSize;      // = 4 + (8 + SubChunk1Size) + (8 + SubChunk2Size)
    char                Format[4];      // "WAVE" string
    char                Subchunk1ID[4]; // "fmt " string
    int                 Subchunk1Size;  // 16 for PCM
    short int           AudioFormat;    // PCM = 1
    short int           NumChannels;    // Mono = 1, Stereo = 2
    int                 SampleRate;     // 8000, 44100, etc.
    int                 ByteRate;       // = SampleRate*NumChannels*BitsPerSample/8
    short int           BlockAlign;     // = NumChannels*BitsPerSample/8
    short int           BitsPerSample;  // 8 bits = 8, 16 bits = 16, etc.
    char                Subchunk2ID[4]; // "data" string
    int                 Subchunk2Size;  // = NumSamples*NumChannels*BitsPerSample/8
} wav_Hdr; 

//*****************************************************************************
//
// Prototypes for the APIs.
//
//*****************************************************************************
extern void SoundInit(void);
extern void SoundPlay(void);
extern void SoundStop(void);
extern void SoundPause(void);
extern bool SoundOpen(const char *pcFileName,
                                tWaveHeader *pWaveHeader);
extern unsigned long SoundSampleRateGet(void);
extern void SoundVolumeSet(unsigned long ulPercent);
extern unsigned char SoundVolumeGet(void);
extern void SoundVolumeDown(unsigned long ulPercent);
extern void SoundVolumeUp(unsigned long ulPercent);
extern unsigned long SoundGetTime(tWaveHeader *pWaveHeader, char *pcTime,
                                 unsigned long ulSize);
extern bool SoundPlaybackStatus(void);
extern unsigned long SoundGetLength(void);

extern void mic_sidetone();
extern void mic_record_start(const char *, tWaveHeader *);
extern void mic_record_stop();

extern void phone_init();
extern uint8_t phone_status_get();

bool SoundPlayContinue(tWaveHeader *pWaveHeader);

//*****************************************************************************
//
// Mark the end of the C bindings section for C++ compilers.
//
//*****************************************************************************
#ifdef __cplusplus
}
#endif

#endif // __SOUND_H__

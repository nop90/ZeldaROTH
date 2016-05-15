
#ifndef __SOUND_3DS_H__
#define __SOUND_3DS_H__


//#include "os_3ds.h"

#define FSOUND_FREE 1
#define NUMSFX (32)

typedef struct FSOUND_SAMPLE
{
	u8* data;
	u32 size;
	u32 format;
	bool used;
	bool loop;
} FSOUND_SAMPLE;


typedef FSOUND_SAMPLE FMUSIC_MODULE;

//#define FMUSIC_MODULE FSOUND_SAMPLE

int FSOUND_Init(u32 freq, u32 bpd, u32 unkn);

int FSOUND_GetSFXMasterVolume(); //
int FMUSIC_GetMasterVolume(FMUSIC_MODULE* s); 


void FMUSIC_SetMasterVolume(FMUSIC_MODULE* s, u8 previous_volume);
void FMUSIC_FreeSong(FMUSIC_MODULE* s);
void FSOUND_SetSFXMasterVolume(u8 previous_volson);

void initSFX(FSOUND_SAMPLE* s);
void loadSFX(FSOUND_SAMPLE* s, const char* filename, u32 format);
//FSOUND_SAMPLE* createSFX(const char* filename, u32 format);


void FSOUND_PlaySound(int chl,FSOUND_SAMPLE* s);
void FMUSIC_StopSong(FMUSIC_MODULE* s);
void FMUSIC_SetLooping(FMUSIC_MODULE* s, bool flag);
void FMUSIC_PlaySong(FMUSIC_MODULE* s);

FSOUND_SAMPLE* FSOUND_Sample_Load(int flag, const char * f,int a, int b, int c);
FMUSIC_MODULE* FMUSIC_LoadSong(const char * f);

void FSOUND_Sample_Free(FSOUND_SAMPLE* s);

void FSOUND_Close();


#endif
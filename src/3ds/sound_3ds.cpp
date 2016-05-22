
//#include "os_3ds.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <3ds.h>
#include "sound_3ds.h"

bool soundEnabled;

FSOUND_SAMPLE SFX[NUMSFX];

int SFXMasterVolume = 255;
int MasterVolume = 255;
int frequency=0;
int channel;

void soundInit()
{
	int i;
	for(i=0;i<NUMSFX;i++)
	{
		SFX[i].used=false;
	}

	if(csndInit()==0)soundEnabled=true;
	else soundEnabled=false;
}

void soundClose()
{
	int i;
	for(i=0;i<NUMSFX;i++)
	{
		if(SFX[i].used)
		{
			if(SFX[i].data)
			{
				linearFree(SFX[i].data);
				SFX[i].data=NULL;
			}
			SFX[i].used=false;
		}
	}
	if(soundEnabled)csndExit();
}

FILE* openFile(const char* fn, const char* mode)
{
	if(!fn || !mode)return NULL;
	return fopen(fn, mode);
}

void* bufferizeFile(const char* filename, u32* size, bool binary, bool linear)
{
	FILE* file;
	
	if(!binary)file = openFile(filename, "r");
	else file = openFile(filename, "rb");
	
	if(!file) return NULL;
	
	u8* buffer;
	long lsize;
	fseek (file, 0 , SEEK_END);
	lsize = ftell (file);
	rewind (file);
	if(linear)buffer=(u8*)linearMemAlign(lsize, 0x80);
	else buffer=(u8*)malloc(lsize);
	if(size)*size=lsize;
	
	if(!buffer)
	{
		fclose(file);
		return NULL;
	}
		
	fread(buffer, 1, lsize, file);
	fclose(file);
	return buffer;
}


int FSOUND_Init(u32 freq, u32 bps, u32 unkn)
{

	frequency = freq;
	
	return soundEnabled;
}

void initSFX(FMUSIC_MODULE* s)
{
	if(!s)return;

	s->data=NULL;
	s->size=0;
	s->used=true;
	s->loop=false;
}

void loadSFX(FMUSIC_MODULE* s, const char* filename, u32 format)
{
	if(!s)return;

	initSFX(s);

	s->data=(u8*) bufferizeFile(filename, &s->size, true, true);
	s->format=format;
}

int FSOUND_GetSFXMasterVolume()
{
	return SFXMasterVolume;
}

int FMUSIC_GetMasterVolume(FMUSIC_MODULE* s)
{
	return MasterVolume;
}

void FMUSIC_SetMasterVolume(FMUSIC_MODULE* s, u8 previous_volume)
{
	MasterVolume = previous_volume;
}

void FSOUND_SetSFXMasterVolume(u8 previous_volson)
{
	SFXMasterVolume = previous_volson;
}

void FSOUND_PlaySound(int chl,FSOUND_SAMPLE* s)
{
	if(!s || !s->used || !s->data || !soundEnabled)return;

	channel++;
	channel%=7;

	csndPlaySound(channel+8, s->format, frequency, 1.0, 0.0, (u32*)s->data, (u32*)s->data, s->size);
}

void FMUSIC_StopSong(FMUSIC_MODULE* s)
{
	CSND_SetPlayState(15, 0);//Stop music audio playback.
	csndExecCmds(0);
}

void FMUSIC_PlaySong(FMUSIC_MODULE* s)
{
	int flag;
	if(!s || !s->used || !s->data || !soundEnabled)return;
	flag = s->format;
	if(s->loop) flag |= SOUND_REPEAT;
	csndPlaySound(15, flag, 8000, 1.0, 0.0, (u32*)s->data, (u32*)s->data, s->size);
}


FSOUND_SAMPLE* FSOUND_Sample_Load(int flag, const char * f,int a, int b, int c)
{
	int i;
	for(i=0;i<NUMSFX;i++)
	{
		if(!SFX[i].used)
		{
			loadSFX(&SFX[i], f, SOUND_FORMAT_16BIT);

			if(!SFX[i].data)return NULL;
			SFX[i].used = true;
			SFX[i].loop=false;
			return &SFX[i];
		}
	}
	return NULL;
}

FMUSIC_MODULE* FMUSIC_LoadSong(const char * f)
{
	int size;
	int i;
	for(i=0;i<NUMSFX;i++)
	{
		if(!SFX[i].used)
		{
			loadSFX(&SFX[i], f, SOUND_FORMAT_16BIT);
			
			if(!SFX[i].data) return NULL;
			SFX[i].used = true;
			SFX[i].loop=false;
			return &SFX[i];
		}
	}
	return NULL;
}

void FSOUND_Close()
{
	return;
}


void FMUSIC_SetLooping(FMUSIC_MODULE* s, bool flag)
{
	if (s)
		s->loop=flag;
}

void FSOUND_Sample_Free(FSOUND_SAMPLE* s)
{
	if(s) {
		if (s->data)
			linearFree(s->data);
		s->size=0;
		s->used=false;
		s->loop=false;
	}
}


void FMUSIC_FreeSong(FMUSIC_MODULE* s)
{
	if(s) {
		if (s->data)
			linearFree(s->data);
		s->size=0;
		s->used=false;
		s->loop=false;
	}
}



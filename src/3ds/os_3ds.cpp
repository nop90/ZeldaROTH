
#include <sys/stat.h>
#include <sf2d.h>
#include <sfil.h>
#include "os_3ds.h"
#include "sound_3ds.h"

#define TICKS_PER_MSEC (268123.480)

u8 language = 1;

int SDL_Init(int flag) //returns 0 on success, -1 on failure 
{
    sf2d_init();
	sf2d_set_3D(false);
	sf2d_set_vblank_wait(true);
	
	osSetSpeedupEnable(true);
	
	romfsInit();
	cfguInit();
	soundInit();

	// Read the language field from the config savegame.
	CFGU_GetSystemLanguage(&language);
	
// In case game dir doesn't exist
    mkdir("/3ds", 0777);
    mkdir("/3ds/ZeldaROTH", 0777);

    return 0;
}


void SDL_Quit()
{
	soundClose();
	cfguExit();
    romfsExit();
	sf2d_fini();

}

int SDL_GetTicks()
{
    return svcGetSystemTick()/TICKS_PER_MSEC;
}

void SDL_Delay(int ms)
{
	svcSleepThread(ms*1000000);
}

// stubbed win functions
void SDL_WM_SetCaption(const char* caption,void* p) {return;}
void SDL_WM_SetIcon(void* i, void* p) {return;}
void SDL_ShowCursor(int s) {return;}

int SDL_PollEvent(int * event)
{
	int ret = 0;
	hidScanInput();
	*event = hidKeysHeld();
	if (event) ret= SDL_KEYDOWN;
	if (*event & KEY_TOUCH) ret|= SDL_FINGERDOWN;
	if(!aptMainLoop()) ret|= SDL_QUIT;
	return ret;
}

int getTouchX(){
	touchPosition pos;	
	hidTouchRead(&pos);
	return pos.px;
}

int getTouchY(){
	touchPosition pos;	
	hidTouchRead(&pos);
	return pos.py;
}

int SDL_GetKeyState(void* p)
{
	return hidKeysHeld();
}

int getLanguage(void)
{
	return language;
}

void setLanguage(int languageID)
{
	if (languageID>5) language = 1;
	else if (languageID<1) language = 5;
	else language = languageID;
 
}

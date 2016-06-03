
#ifndef __OS_3DS_H__
#define __OS_3DS_H__


#include <stdlib.h>
#include <3ds.h>

#define SDL_INIT_VIDEO 1
typedef int Uint32;
typedef u8 Uint8;

/*
Languages
ID 	Description
0 	JP
1 	EN Supported and default for unsupported
2 	FR Supported
3 	DE Supported
4 	IT 
5 	ES Supported
6 	ZH
7 	KO
8 	NL
9 	PT
10 	RU
11 	TW 
*/

int SDL_Init(int flag); //returns 0 on success, -1 on failure 
void SDL_Quit();

int SDL_GetTicks();
void SDL_Delay(int ms);

// stubbed win functions
void SDL_WM_SetCaption(const char* caption,void* p);
void SDL_WM_SetIcon(void* i, void* p);

#define SDL_Event int

#define SDL_QUIT 1<<1
#define SDL_KEYDOWN 1<<2
#define SDL_FINGERDOWN 1<<3

int SDL_PollEvent(int * event);
int getTouchX();
int getTouchY();

int SDL_GetKeyState(void* p);

#define SDL_DISABLE 0
#define SDL_ENABLE 1
void SDL_ShowCursor(int s);

int getLanguage(void);
void setLanguage(int languageID);


#define SDLK_ESCAPE	KEY_SELECT
#define SDLK_F1 	KEY_START
#define SDLK_F4 0
#define SDLK_RETURN	KEY_A
#define SDLK_LEFT	KEY_LEFT
#define SDLK_RIGHT	KEY_RIGHT
#define SDLK_UP		KEY_UP
#define SDLK_DOWN	KEY_DOWN

#define SDLK_CAPSLOCK 0
#define SDLK_LSHIFT KEY_R
#define SDLK_LCTRL	KEY_L
#define SDLK_z		KEY_B
#define SDLK_w		0
#define SDLK_x		KEY_Y
#define SDLK_c		KEY_X
#define SDLK_SPACE KEY_A
#define SDLK_p 		KEY_START
#define SDLK_m 		0
#define SDLK_SEMICOLON 0

#define KMOD_ALT KEY_L
#define KMOD_CTRL 0

#endif

#ifndef __GFX_3DS_H__
#define __GFX_3DS_H__


//#include "os_3ds.h"
#include <sf2d.h>
#include <sfil.h>
#include <3ds.h>
#include "gfx_3ds.h"


typedef struct SDL_Surface {
        u32 flags;                           /* Read-only */
//        SDL_PixelFormat *format;                /* Read-only */
        int format;                /* Read-only */
        int w, h;                               /* Read-only */
//        Uint16 pitch;                           /* Read-only */
//        void *pixels;                           /* Read-write */
		sf2d_texture* texture;
//		int colorKey;

//        SDL_Rect clip_rect;                     /* Read-only */
//        int refcount;                           /* Read-mostly */
} SDL_Surface;

typedef struct{
  int x, y;
  int w, h;
} SDL_Rect;

typedef struct{
  int x, y;
} SDL_Point;


#define SDL_SRCCOLORKEY 1
#define SDL_FULLSCREEN 1
#define SDL_DOUBLEBUF 2
#define SDL_HWSURFACE 4

SDL_Surface* SDL_SetVideoMode(int width, int height, int bpp, int flags);

SDL_Surface* IMG_Load(const char* f);
SDL_Surface* SDL_CreateRGBSurface(int type, int width, int height, int bpp, int a, int b, int c, int d);

void SDL_FreeSurface(SDL_Surface* s);

void SDL_SetColorKey(SDL_Surface* s, int flag, u32 color);

u32 SDL_MapRGB(int flag , u8 r, u8 g, u8 b);


void SDL_FillRect(SDL_Surface* s, SDL_Rect* rect, u32 color);
void filledEllipseRGBA(SDL_Surface* s, int x, int y, int rx, int ry, u8 r, u8 g, u8 b, u8 a);

void SDL_BlitSurface(SDL_Surface* s, SDL_Rect * src, SDL_Surface* d, SDL_Rect * dst);

void SDL_Flip(void * p);

#endif
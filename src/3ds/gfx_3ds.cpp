
#include "os_3ds.h"
#include "gfx_3ds.h"
#include "sound_3ds.h"

bool drawing,drawing2;
int offset;
float scalepos;
float scalewidth;

void endTopFrame(void)
{
	if(drawing)
	{ 
		if (offset)
		{
			sf2d_draw_rectangle(0, 0, 40, 240, RGBA8(0, 0, 0, 0xff)); 
			sf2d_draw_rectangle(360, 0, 40, 240, RGBA8(0, 0, 0, 0xff));
		}
		sf2d_end_frame();
	}
	drawing = false;
}

void endBottomFrame(void)
{
	if(drawing2)
		sf2d_end_frame();
	drawing2 = false;
}

void SDL_Flip(void * p)
{
	endTopFrame();
	endBottomFrame();
	sf2d_swapbuffers();
}

SDL_Surface* SDL_SetVideoMode(int width, int height, int bpp, int flag)
{
	drawing=false;
	drawing2=false;
	if(flag & SDL_FULLSCREEN)
	{
		offset = 0;
		scalepos = 1.25; 
		scalewidth = 1.25; //1.28; //should be 1.25 but setting a slight bigger value avoid tiles positioning defects caused by scaled width approximation
	} else {
		offset = 1;
		scalepos = 1; 
		scalewidth = 1; 
	}
	if (width == 320) return (SDL_Surface*) 2;
	else return (SDL_Surface*) 1;
}

SDL_Surface* IMG_Load(const char* f)
{
    SDL_Surface* s;
	s = (SDL_Surface*) malloc(sizeof(SDL_Surface));
	if(!s) return NULL;
	s->texture = sfil_load_PNG_file(f, SF2D_PLACE_RAM);

	if (s->texture) {
		s->w = s->texture->width;
		s->h = s->texture->height;
		sf2d_texture_set_params(s->texture, GPU_TEXTURE_MAG_FILTER(GPU_LINEAR) | GPU_TEXTURE_MIN_FILTER(GPU_LINEAR)); 
		s->flags = 0;
		s->format = 0;
		return s;
	} else {
		free(s);
		return NULL;
	} 
}

SDL_Surface* SDL_CreateRGBSurface(int type, int width, int height, int bpp, int a, int b, int c, int d)
{
    SDL_Surface* s;
	s = (SDL_Surface*) malloc(sizeof(SDL_Surface));
	if(!s) return NULL;
	s->texture = sf2d_create_texture (width, height,  TEXFMT_RGBA8, SF2D_PLACE_RAM);
	if (s->texture) {
		s->w = s->texture->width;
		s->h = s->texture->height;
		s->flags = 0;
		sf2d_texture_set_params(s->texture, GPU_TEXTURE_MAG_FILTER(GPU_LINEAR) | GPU_TEXTURE_MIN_FILTER(GPU_LINEAR)); 
		s->format = 0;
		sf2d_texture_tile32(s->texture);
		return s;
	} else {
		free(s);
		return NULL;
	} 
}


void SDL_FreeSurface(SDL_Surface* s)
{
    if(s)
	{
		if((s==(SDL_Surface*) 1) || (s==(SDL_Surface*) 2)) s=NULL;
		else
		{
			if(s->texture) sf2d_free_texture(s->texture);
			free(s);
		}
	}
}


void SDL_SetColorKey(SDL_Surface* s, int flag, u32 color)
{
 //   s->colorKey= color;
}

u32 SDL_MapRGB(int flag , u8 r, u8 g, u8 b)
{
    return RGBA8(r, g, b, 0xff);
}


void SDL_FillRect(SDL_Surface* s, SDL_Rect* rect, u32 color)
{
    if (s==(SDL_Surface*) 1) 
	{

		if(!drawing)
		{
			sf2d_start_frame(GFX_TOP, GFX_LEFT);
			drawing=true;
		} 
		if(rect) sf2d_draw_rectangle(rect->x+40*offset, rect->y, rect->w, rect->h, color);
		else  sf2d_draw_rectangle(40*offset, 0, 320 *scalepos, 240, color);
	} else if (s==(SDL_Surface*) 2)
	{
		if(drawing) endTopFrame();
		if(!drawing2)
		{
			sf2d_start_frame(GFX_BOTTOM, GFX_LEFT);
			drawing2=true;
		} 
		if(rect) sf2d_draw_rectangle(rect->x, rect->y, rect->w, rect->h, color);
		else  sf2d_draw_rectangle(0, 0, 320, 240, color);
	} 
}

void filledEllipseRGBA(SDL_Surface* s, int x, int y, int rx, int ry, u8 r, u8 g, u8 b, u8 a)
{
    if (s==(SDL_Surface*) 1) 
	{
		if(!drawing)
		{
			sf2d_start_frame(GFX_TOP, GFX_LEFT);
			drawing=true;
		} 
		sf2d_draw_fill_circle(x*scalepos+40*offset, y, rx*scalewidth, RGBA8(r, g, b, a));
	} else if (s==(SDL_Surface*) 3) 
	{
		if(drawing) endTopFrame();
		if(!drawing2)
		{
			sf2d_start_frame(GFX_BOTTOM, GFX_LEFT);
			drawing2=true;
		} 
		sf2d_draw_fill_circle(x, y, rx, RGBA8(r, g, b, a));
	}
}

void SDL_BlitSurface(SDL_Surface* s, SDL_Rect * src, SDL_Surface* d, SDL_Rect * dst)
{
	u32 pixel;
    if(d==(SDL_Surface*) 1)
	{
		if(!drawing)
		{
			sf2d_start_frame(GFX_TOP, GFX_LEFT);
			drawing=true;
		} 

		if(src) {
			if(dst) 
				sf2d_draw_texture_part_scale(s->texture, (int)(dst->x*scalepos+40*offset),dst->y, src->x, src->y, src->w, src->h, scalewidth,1);
			else
				sf2d_draw_texture_part_scale(s->texture, 40*offset,0, src->x, src->y, src->w, src->h, scalewidth,1);
		} else {
			if(dst) 
				sf2d_draw_texture_scale(s->texture, (int)(dst->x*scalepos+40*offset),dst->y, scalewidth,1);
			else
				sf2d_draw_texture_scale(s->texture, 40*offset,0, scalewidth,1);
		}
	} else if(d==(SDL_Surface*) 2) 
	{
		if(drawing) endTopFrame();
		if(!drawing2)
		{
			sf2d_start_frame(GFX_BOTTOM, GFX_LEFT);
			drawing2=true;
		} 

		if(src) {
			if(dst) 
				sf2d_draw_texture_part(s->texture, dst->x,dst->y, src->x, src->y, src->w, src->h);
			else
				sf2d_draw_texture_part(s->texture, 0,0, src->x, src->y, src->w, src->h);
		} else {
			if(dst) 
				sf2d_draw_texture(s->texture, dst->x,dst->y);
			else
				sf2d_draw_texture(s->texture, 0,0);
		}
	} else {
		if(s && d) 
			if(s->texture && d->texture){ 
				int i,j;

				int sx = (src)?src->x:0;
				int sy = (src)?src->y:0;
				int dx = (dst)?dst->x:0;
				int dy = (dst)?dst->y:0;
				int sw = (src)?src->w:s->texture->width;
				int sh = (src)?src->h:s->texture->height;
				if (sx+sw > s->texture->width) sw = s->texture->width - sx;
				if (sy+sh > s->texture->height) sh = s->texture->height - sy;
				for (i=0;i< sw; i++)
					for (j=0;j< sh; j++) {
						pixel = sf2d_get_pixel ( s->texture,i+sx,j+sy);
						if (pixel & 0xff) sf2d_set_pixel(d->texture,i+dx,j+dy,pixel); 		
					}
				sf2d_texture_tile32(d->texture);
			}
	}
}


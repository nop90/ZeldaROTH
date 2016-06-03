/*

    Zelda Return of the Hylian

    Copyright (C) 2005-2008  Vincent Jouillat

    Please send bugreports with examples or suggestions to www.zeldaroth.fr

*/

#ifndef __ZONERECT_H__
#define __ZONERECT_H__

#include "3ds/gfx_3ds.h"

#include "Listable.h"

class ZoneRect : public Listable {
    public :
        ZoneRect(SDL_Rect* zn);
        ZoneRect(int x, int y, int w, int h);
        ~ZoneRect();
        void setZone(SDL_Rect* zn);
        SDL_Rect* getZone();
        ZoneRect* getSuivant();
    private :
        SDL_Rect* zone;
};

#endif  // ZoneRect.h

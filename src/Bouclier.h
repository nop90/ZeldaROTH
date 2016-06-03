/*

    Zelda Return of the Hylian

    Copyright (C) 2005-2008  Vincent Jouillat

    Please send bugreports with examples or suggestions to www.zeldaroth.fr

*/

#ifndef __BOUCLIER_H__
#define __BOUCLIER_H__

#include "3ds/gfx_3ds.h"


#include "ZoneRect.h"

class Bouclier {
    public :
        Bouclier(SDL_Surface* img, int id = 0);
        ~Bouclier();
        void draw(SDL_Surface* screen, int x, int y, ZoneRect* z, int direction = 0);
        void setZone(ZoneRect* z);
    private :
        ZoneRect* zone;
        SDL_Surface* image;
        int type;
};

#endif  // Bouclier.h

/*

    Zelda Return of the Hylian

    Copyright (C) 2005-2008  Vincent Jouillat

    Please send bugreports with examples or suggestions to www.zeldaroth.fr

*/

#include "ZoneRect.h"
#include "3ds/gfx_3ds.h"


ZoneRect::ZoneRect(SDL_Rect* zn) : Listable(), zone(zn) {
}

ZoneRect::ZoneRect(int x, int y, int w, int h) : Listable() {
    zone = new SDL_Rect;
    zone->x=x; zone->y=y; zone->w=w; zone->h=h;
}

ZoneRect::~ZoneRect() {
    delete zone;
}

void ZoneRect::setZone(SDL_Rect* zn) {
    delete zone;
    zone = zn;
}
        
SDL_Rect* ZoneRect::getZone() {
    return zone;
}

ZoneRect* ZoneRect::getSuivant() {
    return (ZoneRect*)suivant;
}

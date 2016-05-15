/*

    Zelda Return of the Hylian

    Copyright (C) 2005-2008  Vincent Jouillat

    Please send bugreports with examples or suggestions to www.zeldaroth.fr

*/

#include "Personnage.h"


Personnage::Personnage() : Listable(), x(0), y(0), xdep(0), ydep(0), vie(1), 
viemax(1), invincible(0), type(0), direction(S), h(0), w(0), anim(0), animmax(0) {
    image = NULL;
    lastAnimTime = SDL_GetTicks();
    gpZoneBase = new ZoneRect(new SDL_Rect());
}

Personnage::~Personnage() {
    SDL_FreeSurface(image);
    delete gpZoneBase;
}

void Personnage::saveEmpl() {
    xdep = x;
    ydep = y;
    dirdep = direction;
}

int Personnage::getX() { return x;}
int Personnage::getY() { return y;}
int Personnage::getW() { return w;}
int Personnage::getH() { return h;}

int Personnage::getXDep() {return xdep;}
int Personnage::getYDep() {return ydep;}
int Personnage::getDirDep() {return dirdep;}

void Personnage::setX(int i) {x=i;}
void Personnage::setY(int i) {y=i;}
void Personnage::setXDep(int i) {xdep=i;}
void Personnage::setYDep(int i) {ydep=i;}
void Personnage::setDirDep(Direction d) {dirdep=d;}

int Personnage::getVie() { return vie;}
int Personnage::getVieMax() { return viemax;}

int Personnage::getInvincible() { return invincible;}

ZoneRect* Personnage::getZoneBase() {
    return gpZoneBase;
}

void Personnage::setDirection(Direction d) {
    direction = d;
}

Direction Personnage::getDirection() { return direction;}

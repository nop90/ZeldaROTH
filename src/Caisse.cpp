/*

    Zelda Return of the Hylian

    Copyright (C) 2005-2008  Vincent Jouillat

    Please send bugreports with examples or suggestions to www.zeldaroth.fr

*/

//#include <SDL/SDL.h>
//#include <SDL/SDL_image.h>
#include "3ds/os_3ds.h"

#include "Menu.h"
#include "Joueur.h"
#include "Monde.h"
#include "Texte.h"
#include "Projectile.h"
#include "Caisse.h"
#include "Jeu.h"

Caisse::Caisse(Jeu* jeu, int type, int i, int j) : gpJeu(jeu), id(type),
pousseX(0), pousseY(0), direction(N) {
    x=i; y=j; w=16; h=16; xdep=x; ydep=y;
    image = IMG_Load("romfs:/images/objet/caisse.png");
    sx = ((type-1)%3)*w;
    sy = ((type-1)/3)*h;
}

Caisse::~Caisse() {
	if(suivant)	{
		Caisse* temp;
		temp = (Caisse*)suivant;
		suivant=NULL;
		delete temp;
	}
}

void Caisse::draw(SDL_Surface* gpScreen) {
    int phg[2];
    phg[0] = gpJeu->getPhg(0);
    phg[1] = gpJeu->getPhg(1);
    
    if (vie && id && x+w > phg[0] && x <= phg[0]+320 && y+h > phg[1] && y <= phg[1]+240) {
        
        bouge();
    
        SDL_Rect src;
        SDL_Rect dst;
        
        src.x=sx; src.y=sy; src.w=w; src.h=h; dst.x=x-phg[0]; dst.y=y-phg[1];
        
        SDL_BlitSurface(image, &src, gpScreen, &dst);
    }
    
    if (suivant != NULL) ((Caisse*)suivant)->draw(gpScreen);
}

bool Caisse::pousse(Direction dir) {
    if (pousseX!=0 || pousseY!=0) return false;
    int phg[2];
    phg[0] = gpJeu->getPhg(0);
    phg[1] = gpJeu->getPhg(1);
    switch (dir) {
        case N : 
            if (y > phg[1]+32
            && gpJeu->isMarchable(x, y-16, 1, 1, 0)
            && gpJeu->isMarchable(x+15, y-16, 1, 1, 0) 
            && gpJeu->isMarchable(x, y-1, 1, 1, 0)
            && gpJeu->isMarchable(x+15, y-1, 1, 1, 0)) pousseY=-16; 
            break;
        case S : 
            if (y < phg[1]+240-32
            && gpJeu->isMarchable(x, y+16, 1, 1, 0)
            && gpJeu->isMarchable(x+15, y+16, 1, 1, 0) 
            && gpJeu->isMarchable(x, y+31, 1, 1, 0)
            && gpJeu->isMarchable(x+15, y+31, 1, 1, 0)) pousseY=16; 
            break;
        case O : 
            if (x > phg[0]+32
            && gpJeu->isMarchable(x-16, y, 1, 1, 0)
            && gpJeu->isMarchable(x-1, y, 1, 1, 0) 
            && gpJeu->isMarchable(x-16, y+15, 1, 1, 0)
            && gpJeu->isMarchable(x-1, y+15, 1, 1, 0)) pousseX=-16; 
            break;
        case E : 
            if (x < phg[0]+320-32
            && gpJeu->isMarchable(x+16, y, 1, 1, 0)
            && gpJeu->isMarchable(x+31, y, 1, 1, 0) 
            && gpJeu->isMarchable(x+16, y+15, 1, 1, 0)
            && gpJeu->isMarchable(x+31, y+15, 1, 1, 0)) pousseX=16; 
            break;
    }
    if (pousseX!=0 || pousseY!=0) {
        direction = dir;
        gpJeu->getAudio()->playSound(39);
    }
    return true;
}

void Caisse::moveX(int i) {
    x+=i;
}

void Caisse::moveY(int i) {
    y+=i;
}

void Caisse::bouge() {
    if (pousseX==0 && pousseY==0) return;
    if (pousseX>0) {
        pousseX-=2;
        moveX(2);
    }
    if (pousseX<0) {
        pousseX+=2;
        moveX(-2);
    }
    if (pousseY>0) {
        pousseY-=2;
        moveY(2);
    }
    if (pousseY<0) {
        pousseY+=2;
        moveY(-2);
    }
    if (pousseX==0 && pousseY==0) {
        if (gpJeu->isVide(x, y, 16, 16)) {
            vie = 0;
            gpJeu->getAudio()->playSound(6);
        }
    }
    if (vie) gpJeu->testEpee(x, y, w, h, CAISSE, 999, direction);
}

void Caisse::revie() {
    vie = 1;
    x = xdep;
    y = ydep;
    if (suivant != NULL) getSuivant()->revie();
}

Caisse* Caisse::getSuivant() {
    return (Caisse*)suivant;
}

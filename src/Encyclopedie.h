/*

    Zelda Return of the Hylian

    Copyright (C) 2005-2008  Vincent Jouillat

    Please send bugreports with examples or suggestions to www.zeldaroth.fr

*/

#ifndef __ENCYCLOPEDIE_H__
#define __ENCYCLOPEDIE_H__

#include "3ds/os_3ds.h"

#include "Jeu.h"

class Encyclopedie {
    public :
        Encyclopedie(Jeu* jeu);
        ~Encyclopedie();
        void draw(SDL_Surface* gpScreen);
        void init();
        void moveL();
        void moveR();
        void setPage(int i);
    private :
        void fond();
        void placeCadres();
        void cadre(int x, int y, int w, int h, int id);
        
        Jeu* gpJeu;
        int page;
        Ennemi* gpEnnemi;
        SDL_Surface* image;
        SDL_Surface* imageCadre;
};

#endif  // Encyclopedie.h

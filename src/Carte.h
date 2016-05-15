/*

    Zelda Return of the Hylian

    Copyright (C) 2005-2008  Vincent Jouillat

    Please send bugreports with examples or suggestions to www.zeldaroth.fr

*/

#ifndef __CARTE_H__
#define __CARTE_H__

#include "Jeu.h"

class Carte {
    public :
        Carte(Jeu* jeu);
        ~Carte();
        void init();
        void draw(SDL_Surface* gpScreen);
        void moveLevel(Direction dir);
    private :
        void initDonjon();
        void initEtage();
        void initItems();
        void initName();
        void initCarte();
        void initBoussole();
        Jeu* gpJeu;
        int levely;
        SDL_Surface* image;
};

#endif  // Carte.h

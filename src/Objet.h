/*

    Zelda Return of the Hylian

    Copyright (C) 2005-2008  Vincent Jouillat

    Please send bugreports with examples or suggestions to www.zeldaroth.fr

*/

#ifndef __OBJET_H__
#define __OBJET_H__

#include "Listable.h"
#include "Common.h"

class Jeu;

class Objet : public Listable {
    public :
        Objet(Jeu* jeu, int i, int j, int typ, int num=0, int persistant=0);
        ~Objet();
        void draw(SDL_Surface* gpScreen);
        void ramasse();
        int getX();
        int getY();
        int getW();
        int getH();
        int getVie();
        void revie();
        Objet* getSuivant();
    private :
        
        Jeu* gpJeu;
        int vie;
        int x;
        int y;
        int type;
        int id;
        int vanim;
        int anim;
        int w;
        int h;
        int max;
        int persist;
        Uint32 lastAnimTime;
        SDL_Surface* image;
};

#endif  // Objet.h

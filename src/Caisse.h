/*

    Zelda Return of the Hylian

    Copyright (C) 2005-2008  Vincent Jouillat

    Please send bugreports with examples or suggestions to www.zeldaroth.fr

*/

#ifndef __CAISSE_H__
#define __CAISSE_H__

#include "Personnage.h"

class Jeu;

class Caisse : public Personnage {
    public :
        Caisse(Jeu* jeu, int type, int i, int j);
        ~Caisse();
        void draw(SDL_Surface* gpScreen);
        void moveX(int i);
        void moveY(int i);
        void revie();
        Caisse* getSuivant();
        bool pousse(Direction dir);
    private :
        void bouge();
        
        Jeu* gpJeu;
        int id;
        int pousseX;
        int pousseY;
        Direction direction;
        int sx;
        int sy;
};

#endif  // Caisse.h

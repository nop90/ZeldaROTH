/*

    Zelda Return of the Hylian

    Copyright (C) 2005-2008  Vincent Jouillat

    Please send bugreports with examples or suggestions to www.zeldaroth.fr

*/

#ifndef __SNIPE_H__
#define __SNIPE_H__

#include "Listable.h"
#include "Common.h"
#include "3ds/gfx_3ds.h"

class Jeu;

class Snipe : public Listable {
    public :
        Snipe(Jeu* jeu, int type, double a, double b, double da, double db);
        ~Snipe();
        void draw(SDL_Surface* gpScreen);
        bool renvoit(Direction dir);
        Snipe* getSuivant();
        int getX();
        int getY();
        int getW();
        int getH();
    private :
        void depl();
        void attaque();
        void dessin(SDL_Surface* gpScreen);
        void fin();
        
        Jeu* gpJeu;
        int id;
        double x;
        double y;
        double anglx;
        double angly;
        int anim;
        int max;
        int vanim;
        int vie;
        int hauteur;
        SDL_Surface* image;
        int w;
        int h;
        bool renvoie;
        int force;
        bool renvoyable;
        bool rebond;
        int element;
        double historique[8][2];
        Uint32 lastAnimTime;
};

#endif  // Snipe.h

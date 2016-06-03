/*

    Zelda Return of the Hylian

    Copyright (C) 2005-2008  Vincent Jouillat

    Please send bugreports with examples or suggestions to www.zeldaroth.fr

*/

#ifndef __STATUT_H__
#define __STATUT_H__

#include "3ds/gfx_3ds.h"

class Jeu;

class Statut {
    public :
        Statut(Jeu* jeu);
        ~Statut();
        void draw(SDL_Surface* gpScreen);
        void setJauge(bool b);
        void setValJauge(int i);
        void setMaxJauge(int i);
    private :
        void drawBoss(SDL_Surface* gpScreen);
        void drawVie(SDL_Surface* gpScreen);
        void drawItems(SDL_Surface* gpScreen);
        void drawMagie(SDL_Surface* gpScreen);
        void drawNombre(SDL_Surface* gpScreen, int val, int max, int x, int y, int nb);
        SDL_Surface* imageStatut;
        SDL_Surface* imageChiffre;
        SDL_Surface* imageInventaire;
        Jeu* gpJeu;
        bool jauge; // jauge activée
        int valjauge;
        int maxjauge;
        int nivjauge; // hauteur 
};

#endif  // Statut.h

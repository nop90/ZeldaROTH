/*

    Zelda Return of the Hylian

    Copyright (C) 2005-2008  Vincent Jouillat

    Please send bugreports with examples or suggestions to www.zeldaroth.fr

*/

#ifndef __GENERIQUE_H__
#define __GENERIQUE_H__

//#include <SDL/SDL.h>
#include "3ds/os_3ds.h"
#include "3ds/gfx_3ds.h"

class Jeu;

class Generique {
    public :
        Generique(Jeu* jeu);
        ~Generique();
        void drawFin(SDL_Surface* gpScreen);
        void initDisclamer();
        void initLogo();
        void initTitre();
        void initScore();
        void initSelection();
        void initOption();
        void initRecord();
        void initEffacer();
        void initCharger();
        void initIntro();
        void initNuit();
        void initAide1();
        void initAide2();
        void initRang(int i);
        void draw(SDL_Surface* gpScreen);
        void drawTitre(SDL_Surface* gpScreen);
        void drawSelection(SDL_Surface* gpScreen, int ligne, int colonne);
        void drawOption(SDL_Surface* gpScreen, int ligne, int opt1, int opt2);
        void drawRecord(SDL_Surface* gpScreen, int ligne, int colonne);
        void drawEffacer(SDL_Surface* gpScreen, int ligne);
        void drawCharger(SDL_Surface* gpScreen, int ligne, int ligne2);
        void drawEffacerSave(SDL_Surface* gpScreen, int ligne, int ligne2);
        void drawIntro(SDL_Surface* gpScreen, int etape);
        void drawDebut(SDL_Surface* gpScreen);
		void drawBackground(SDL_Surface* gpScreen);
    private :
        void cadre(int x, int y, int w, int h);
        Jeu* gpJeu;
        SDL_Surface* imageFin;
        SDL_Surface* image;
        SDL_Surface* imageArbre;
        SDL_Surface* imageTitre;
        SDL_Surface* imageCurseur;
        SDL_Surface* imageNiveau;
        SDL_Surface* imageFee;
        SDL_Surface* imageCadre;
        SDL_Surface* imageFond1;
        SDL_Surface* imageFond2;
		SDL_Surface* imageBackground;
        SDL_Surface* imageNuit;
        SDL_Surface* imageIntro[5];
        int anim;
        Uint32 lastAnimTime;
};

#endif  // Generique.h

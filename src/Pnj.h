/*

    Zelda Return of the Hylian

    Copyright (C) 2005-2008  Vincent Jouillat

    Please send bugreports with examples or suggestions to www.zeldaroth.fr

*/

#ifndef __PNJ_H__
#define __PNJ_H__

#include "Personnage.h"
#include "ZoneRect.h"

class Jeu;

class Pnj : public Personnage {
    public :
        Pnj(Jeu* jeu, int type, int i, int j, int msg);
        ~Pnj();
        void draw(SDL_Surface* gpScreen);
        void moveX(int i);
        void moveY(int i);
        Pnj* getSuivant();
        int getMessage();
        void setMessage(int i);
        void setTypeIA(int i);
        void setImmo(bool b);
        void revie();
    private :
        void init();
        void IA();
        void bouge();
        void suit();
        bool parle();
        
        Jeu* gpJeu;
        int id;
        int message;
        int typePnj;
        bool immo;
        int typeIA;
};

#endif  // Pnj.h

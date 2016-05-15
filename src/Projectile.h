/*

    Zelda Return of the Hylian

    Copyright (C) 2005-2008  Vincent Jouillat

    Please send bugreports with examples or suggestions to www.zeldaroth.fr

*/

#ifndef __PROJECTILE_H__
#define __PROJECTILE_H__

#include "Listable.h"
#include "Common.h"
#include "3ds/gfx_3ds.h"

class Jeu;

class Projectile : public Listable {
    public :
        Projectile(Jeu* jeu, int type, Direction dir, int a, int b, int vit);
        ~Projectile();
        void draw(SDL_Surface* gpScreen);
    private :
        void move(int dx, int dy);
        void attaque();
        void dessinChaine(SDL_Surface* gpScreen);
        void dessinTrainee(SDL_Surface* gpScreen);
        void testAgrippe();
        void changeType(int type);
        void fin();
        
        Jeu* gpJeu;
        int id;
        Direction direction;
        int x;
        int y;
        int anim;
        int max;
        int vanim;
        int vie;
        int hauteur;
        int vitesse;
        SDL_Surface* image;
        int etape; //pour les objets revenant
        int chemin; //chemin parcourru
        int chmax; //chemin max
        int w;
        int h;
        int zx; //zone de la flèche
        int zy;
        int zw;
        int zh;
        Uint32 lastAnimTime;
};

#endif  // Projectile.h

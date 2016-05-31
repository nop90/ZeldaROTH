/*

    Zelda Return of the Hylian

    Copyright (C) 2005-2008  Vincent Jouillat

    Please send bugreports with examples or suggestions to www.zeldaroth.fr

*/

#include "Snipe.h"
#include "Projectile.h"
#include "Statut.h"
#include "Joueur.h"
#include "Monde.h"
#include "Menu.h"
#include "Texte.h"
#include "Jeu.h"
#include "3ds/gfx_3ds.h"

#include <cmath>
using std::fabs;

Snipe::Snipe(Jeu* jeu, int type, double a, double b, double da, double db) : Listable(),
gpJeu(jeu), id(type), x(a), y(b), anglx(da), angly(db), anim(0), max(0), vanim(120),
vie(1), hauteur(0), image(NULL), w(0), h(0), renvoie(false), force(0), renvoyable(false),
rebond(false), element(FEU) {

    for (int i = 0; i < 8; i++) {historique[i][0]=x; historique[i][1]=y;}

    switch (id) {
        case 1 : // boule de feu petite
            w=7; h=7; force=2; renvoyable=true;
            image = IMG_Load("romfs:/images/objet/projectiles2.png");
            break;
        case 2 : // boule de feu petite droite
            if (angly!=0) {h=16;w=7;}
            if (anglx!=0) {h=7;w=16;}
            force=4; max=1;
            image = IMG_Load("romfs:/images/objet/projectiles3.png");
            break;
        case 3 : // arc magique
            if (angly!=0) {h=8;w=16;}
            if (anglx!=0) {h=16;w=8;}
            force=4; max=1; renvoyable=true; element=ENNEMI;
            image = IMG_Load("romfs:/images/objet/projectiles4.png");
            break;
        case 4 : // boulet
            w=8; h=13; force=2; element=PROJECTILE;
            image = IMG_Load("romfs:/images/objet/projectiles5.png");
            break;
        case 5 : // boule magique
            w=16; h=16; force=2; renvoyable=true; element=ENNEMI; max=1;
            image = IMG_Load("romfs:/images/objet/projectiles6.png");
            break;
        case 6 : // eclair
            w=83; h=148; force=6; element=ENNEMI; max=4; anglx=0; angly=0;
            image = IMG_Load("romfs:/images/objet/projectiles8.png");
            break;
        case 7 : // lapin
            w=22; h=22; force=0; element=LAPIN; max=1;
            image = IMG_Load("romfs:/images/objet/projectiles7.png");
            break;
        case 8 : // boule de feu violette
            w=7; h=7; force=0; renvoyable=true; element=DEMI;
            image = IMG_Load("romfs:/images/objet/projectiles9.png");
            break;
    }

    if (image != NULL)
        SDL_SetColorKey(image,SDL_SRCCOLORKEY,SDL_MapRGB(image->format,0,0,255));

    lastAnimTime = SDL_GetTicks();
}

Snipe::~Snipe() {
    SDL_FreeSurface(image);
	if(suivant)	{
		Snipe* temp;
		temp = (Snipe*)suivant;
		suivant=NULL;
		delete temp;
	}

}

void Snipe::draw(SDL_Surface* gpScreen) {
    if (id > 0 && vie) {

        // déplacement
        if (vie && !gpJeu->getStop()) depl();

        //dessin
        if (vie) {

            dessin(gpScreen);

            attaque();

            //anim
            if(SDL_GetTicks() > lastAnimTime + vanim && !gpJeu->getStop()) {
                lastAnimTime = SDL_GetTicks();
                anim++;
                if (anim > max) anim = 0;
                if (anim==0 && id==6) vie = 0;
            }
        }
    }



    if (suivant != NULL) if (((Snipe*)suivant)->vie == 0) enleve(suivant);
    if (suivant != NULL) ((Snipe*)suivant)->draw(gpScreen);
}

void Snipe::depl() {
    Monde* gpMonde = gpJeu->getMonde();

    if (x+anglx <= gpMonde->regionValue(0) || x+anglx >= gpMonde->regionValue(2)
    || y+angly <= gpMonde->regionValue(1) || y+angly >= gpMonde->regionValue(3)) {
        fin(); return;
    }

    int valeur = gpMonde->murValue((int)(x+anglx), (int)(y+angly));
    bool obstacle = (valeur == PLEIN || valeur == BOIS);

    if (angly < 0) {
        if (valeur == BAS || valeur == BAS_GAUCHE || valeur == BAS_DROITE) hauteur--;
        else if (valeur == HAUT || valeur == HAUT_GAUCHE || valeur == HAUT_DROITE) hauteur++;
    }
    if (angly > 0) {
        if (valeur == BAS || valeur == BAS_GAUCHE || valeur == BAS_DROITE) hauteur++;
        else if (valeur == HAUT || valeur == HAUT_GAUCHE || valeur == HAUT_DROITE) hauteur--;
    }
    if (anglx < 0) {
        if (valeur == DROITE || valeur == HAUT_DROITE || valeur == BAS_DROITE) hauteur--;
        else if (valeur == GAUCHE || valeur == HAUT_GAUCHE || valeur == BAS_GAUCHE) hauteur++;
    }
    if (anglx > 0) {
        if (valeur == DROITE || valeur == HAUT_DROITE || valeur == BAS_DROITE) hauteur++;
        else if (valeur == GAUCHE || valeur == HAUT_GAUCHE || valeur == BAS_GAUCHE) hauteur--;
    }

    if ((obstacle && !hauteur) || hauteur < 0 ) fin();
    else {
        for (int i = 0; i < 7; i++) {
            historique[7-i][0]=historique[6-i][0];
            historique[7-i][1]=historique[6-i][1];
        }
        historique[0][0]=x;
        historique[0][1]=y;
        x+=anglx; y+=angly;
    }
}

void Snipe::attaque() {
    if (hauteur != 0) return;

    Direction dir = S;
    if (abs(anglx)-abs(angly) > 0) {
        if (anglx<0) dir = O;
        else dir = E;
    }
    else {
        if (angly<0) dir = N;
        else dir = S;
    }

    if (id==6) {
        gpJeu->testDegat((int)x-(w/2), (int)y, w, h, element, force, dir);
        return;
    }

    if (gpJeu->testDegat((int)x-(w/2), (int)y-(h/2), w, h, element, force, dir)) vie=0;

    if (renvoie)
        if (gpJeu->testEpee((int)x-(w/2), (int)y-(h/2), w, h, element, force, dir)) vie=0;
}


void Snipe::dessin(SDL_Surface* gpScreen) {
    int phg[2];
    phg[0] = gpJeu->getPhg(0);
    phg[1] = gpJeu->getPhg(1);

    SDL_Rect src;
    SDL_Rect dst;

    if (id == 1 || id == 8) {
        src.h = 3; src.w = 3; src.x=11; src.y=0;
        dst.x = (int)(historique[4][0]-1.5)-phg[0];
        dst.y = (int)(historique[4][1]-1.5)-phg[1];
        SDL_BlitSurface(image, &src, gpScreen, &dst);
        src.h = 4; src.w = 4; src.x=7; src.y=0;
        dst.x = (int)(historique[2][0]-2)-phg[0];
        dst.y = (int)(historique[2][1]-2)-phg[1];
        SDL_BlitSurface(image, &src, gpScreen, &dst);
        src.h = h; src.w = w; src.x = 0; src.y = 0;
        dst.x = (int)(x - (w/2)) - phg[0];
        dst.y = (int)(y - (h/2)) - phg[1];
        SDL_BlitSurface(image, &src, gpScreen, &dst);
    }

    if (id == 2) {
        src.h = h; src.w = w;
        if (angly < 0) {src.x = w*anim; src.y = 0;}
        if (angly > 0) {src.x = w*(anim+2); src.y = 0;}
        if (anglx < 0) {src.x = 28; src.y = h*anim;}
        if (anglx > 0) {src.x = 44; src.y = h*anim;}
        dst.x = (int)(x - (w/2)) - phg[0];
        dst.y = (int)(y - (h/2)) - phg[1];
        SDL_BlitSurface(image, &src, gpScreen, &dst);
    }

    if (id == 3) {
        src.h = h; src.w = w;
        if (angly < 0) {src.x = 0; src.y = h*anim;}
        if (angly > 0) {src.x = w; src.y = h*anim;;}
        if (anglx < 0) {src.x = 32+w*anim; src.y = 0;}
        if (anglx > 0) {src.x = 48+w*anim; src.y = 0;}
        dst.x = (int)(x - (w/2)) - phg[0];
        dst.y = (int)(y - (h/2)) - phg[1];
        SDL_BlitSurface(image, &src, gpScreen, &dst);
    }

    if (id == 4) {
        src.h = h; src.w = w; src.x = 0; src.y = 0;
        dst.x = (int)(x - (w/2)) - phg[0];
        dst.y = (int)(y - (h/2)) - phg[1];
        SDL_BlitSurface(image, &src, gpScreen, &dst);
    }

    if (id == 5) {
        src.h = 8; src.w = 8; src.x=28; src.y=16*anim;
        dst.x = (int)(historique[6][0]-4)-phg[0];
        dst.y = (int)(historique[6][1]-4)-phg[1];
        SDL_BlitSurface(image, &src, gpScreen, &dst);
        src.h = 12; src.w = 12; src.x=16; src.y=16*anim;
        dst.x = (int)(historique[3][0]-6)-phg[0];
        dst.y = (int)(historique[3][1]-6)-phg[1];
        SDL_BlitSurface(image, &src, gpScreen, &dst);
        src.h = h; src.w = w; src.x = 0; src.y = 16*anim;
        dst.x = (int)(x - (w/2)) - phg[0];
        dst.y = (int)(y - (h/2)) - phg[1];
        SDL_BlitSurface(image, &src, gpScreen, &dst);
    }

    if (id == 6) {
        src.h = h; src.w = w; src.x = w*anim; src.y = 0;
        dst.x = (int)(x - (w/2)) - phg[0];
        dst.y = (int)y - phg[1];
        SDL_BlitSurface(image, &src, gpScreen, &dst);
    }

    if (id == 7) {
        src.h = h; src.w = w; src.x = 0; src.y = h*anim;
        dst.x = (int)(x - (w/2)) - phg[0];
        dst.y = (int)(y - (h/2)) - phg[1];
        SDL_BlitSurface(image, &src, gpScreen, &dst);
    }


}

int Snipe::getX() { return (int)x;}
int Snipe::getY() { return (int)y;}
int Snipe::getW() { return (int)w;}
int Snipe::getH() { return (int)h;}

Snipe* Snipe::getSuivant() {return (Snipe*)suivant;}

bool Snipe::renvoit(Direction dir) {

    if (renvoie || !renvoyable || gpJeu->getJoueur()->getEpee()<3) return false;

    if (dir==N || dir==S) {
        angly=fabs(angly);
        if (angly==0) {
            angly=fabsf(anglx);
            anglx=0;
        }
        if (dir==N) angly=-angly;
    }
    if (dir==O || dir==E) {
        anglx=fabsf(anglx);
        if (anglx==0) {
            anglx=fabsf(angly);
            angly=0;
        }
        if (dir==O) anglx=-anglx;
    }

    if (id == 3) {
        if (angly!=0) {h=8;w=16;}
        if (anglx!=0) {h=16;w=8;}
    }

    renvoie=true;

    return true;
}

void Snipe::fin() {
    if (id == 7) {
        Monde* gpMonde = gpJeu->getMonde();

        int valeur = gpMonde->murValue((int)(x+anglx), (int)y);
        bool obstacleX = (valeur == PLEIN || valeur == BOIS);
        valeur = gpMonde->murValue((int)x, (int)(y+angly));
        bool obstacleY = (valeur == PLEIN || valeur == BOIS);

        if (obstacleX && obstacleY) {anglx=-anglx; angly=-angly; return;}
        if (x+anglx <= gpMonde->regionValue(0) || x+anglx >= gpMonde->regionValue(2)
        || obstacleX) {
            anglx=-anglx;
            return;
        }
        if (y+angly <= gpMonde->regionValue(1) || y+angly >= gpMonde->regionValue(3)
        || obstacleY) {
            angly=-angly;
            return;
        }
        return;
    }
    vie = 0;
}

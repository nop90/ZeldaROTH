/*

    Zelda Return of the Hylian

    Copyright (C) 2005-2008  Vincent Jouillat

    Please send bugreports with examples or suggestions to www.zeldaroth.fr

*/

#include "Statut.h"
#include "Joueur.h"
#include "Monde.h"
#include "Menu.h"
#include "Texte.h"
#include "Projectile.h"
#include "Jeu.h"
#include "3ds/gfx_3ds.h"

#include <iostream>

Statut::Statut(Jeu* jeu) : gpJeu(jeu), jauge(false), valjauge(0), maxjauge(10), nivjauge(50) {
    
    imageStatut = IMG_Load("romfs:/images/statut/statut.png");
    imageChiffre = IMG_Load("romfs:/images/statut/chiffres.png");
    imageInventaire = IMG_Load("romfs:/images/statut/inventaire.png");
    
    SDL_SetColorKey(imageStatut,SDL_SRCCOLORKEY,SDL_MapRGB(imageStatut->format,0,0,255));
    SDL_SetColorKey(imageChiffre,SDL_SRCCOLORKEY,SDL_MapRGB(imageChiffre->format,0,0,255));
}

Statut::~Statut() {
    SDL_FreeSurface(imageStatut);
    SDL_FreeSurface(imageChiffre);
    SDL_FreeSurface(imageInventaire);
}

void Statut::draw(SDL_Surface* gpScreen) {
    drawBoss(gpScreen);
    drawItems(gpScreen);
    drawVie(gpScreen);
    drawMagie(gpScreen);
}

void Statut::drawBoss(SDL_Surface* gpScreen) {
    SDL_Rect src;
    SDL_Rect dst;

    //jauge vie boss
    if (valjauge<0) valjauge=0;
    if (jauge==false) valjauge=0;
    if (jauge==true || nivjauge<50) {
        if (jauge==true && valjauge>0 && nivjauge>0) nivjauge--;
        if (jauge==false && nivjauge<50) nivjauge++;
        
        src.h = 10;src.w = 100;
        dst.x = 110;dst.y = 220+nivjauge;
        src.x=100;src.y=45;
        SDL_BlitSurface(imageStatut, &src, gpScreen, &dst);
        
        if (maxjauge==0) maxjauge=1;
        if(valjauge>maxjauge) valjauge = maxjauge;
        src.w = (valjauge*90)/maxjauge;src.h = 10;
        dst.x = 115;dst.y = 220+nivjauge;
        src.x=5+90-src.w;src.y=45;
        SDL_BlitSurface(imageStatut, &src, gpScreen, &dst);
    }
}

void Statut::drawMagie(SDL_Surface* gpScreen) {
    SDL_Rect src;
    SDL_Rect dst;
    
    Joueur* gpJoueur = gpJeu->getJoueur();
    bool oni = gpJoueur->getOnilink();
    
    //jauge oni
    if (oni) {
        src.h = 45;src.w = 10;
        dst.x = 10;dst.y = 10;
        src.x=212;src.y=0;
        SDL_BlitSurface(imageStatut, &src, gpScreen, &dst);
        src.h = (gpJoueur->getOnijauge()*32)/gpJoueur->getOnimax();src.w = 8;
        dst.x = 10;dst.y = 18+32-((gpJoueur->getOnijauge()*32)/gpJoueur->getOnimax());
        src.x=202;src.y=8;
        SDL_BlitSurface(imageStatut, &src, gpScreen, &dst);
    }
    
    //jauge magie
    if (gpJoueur->hasObjet(O_LANTERNE)) {
        src.h = 45; src.w = 16; src.x = 0; src.y = 0;
        if (oni) dst.x=20; else dst.x = 10; dst.y = 10;
        SDL_BlitSurface(imageStatut, &src, gpScreen, &dst);
        //demi magie
        if (gpJoueur->getMagieMax()>32) {
            src.h = 8; src.w = 16; src.x = 186; src.y = 37;
            SDL_BlitSurface(imageStatut, &src, gpScreen, &dst);
        }
        src.h = (gpJoueur->getMagie()/(gpJoueur->getMagieMax()/32));src.w = 8;
        if (oni) dst.x=24; else dst.x = 14;
        dst.y = 50-(gpJoueur->getMagie()/(gpJoueur->getMagieMax()/32));
        src.x=124;src.y=8;
        SDL_BlitSurface(imageStatut, &src, gpScreen, &dst);
    }
    
    //emplacement objet
    if (gpJoueur->hasObjet(O_SAC_BOMBES) || gpJoueur->hasBouteille(0)) {
        src.h = 22;src.w = 22; src.x=17; src.y=5;
        if (oni) dst.x=37; else dst.x = 27; dst.y = 15;
        SDL_BlitSurface(imageStatut, &src, gpScreen, &dst);
    }
    
    src.h = 17;src.w = 16;
    if (oni) dst.x=40; else dst.x = 30; dst.y = 15;;dst.y=18;
    //bool test=false;
    switch (gpJoueur->getObjet()) {
        case 0 : src.x=0; src.y=0; if (gpJoueur->hasObjet(O_ARC)==2)src.y=85; break;
        case 1 : src.x=16; src.y=0; break;
        case 2 : src.x=32; src.y=0; break;
        case 3 : src.x=0; src.y=17; break;
        case 4 : src.x=16; src.y=17; break;
        case 5 : src.x=32; src.y=17; break;
        case 6 : src.x=0; src.y=34; break;
        case 7 : src.x=16; src.y=34; break;
        case 8 : src.x=32; src.y=34; if (gpJoueur->hasObjet(O_GANTS)==1)src.y=85; break;
        default :
            src.x=0;
            if(gpJoueur->hasBouteille(gpJoueur->getObjet()-9)==3)src.x=16;
            if(gpJoueur->hasBouteille(gpJoueur->getObjet()-9)==4)src.x=32;
            if(gpJoueur->hasBouteille(gpJoueur->getObjet()-9)>1)src.y=51;
            if(gpJoueur->hasBouteille(gpJoueur->getObjet()-9)==1)src.y=68;
            break;
    }    
    if(gpJoueur->getObjet() < 9) {
        if (gpJoueur->hasObjet(gpJoueur->getObjet())) 
            if (gpJoueur->getObjet() != 2 || gpJoueur->getBombe()) 
                SDL_BlitSurface(imageInventaire, &src, gpScreen, &dst);
    }
    else {
        if(gpJoueur->hasBouteille(gpJoueur->getObjet()-9))
            SDL_BlitSurface(imageInventaire, &src, gpScreen, &dst);
    }
    
}

void Statut::drawVie(SDL_Surface* gpScreen) {
    SDL_Rect src;
    SDL_Rect dst;
    
    Joueur* gpJoueur = gpJeu->getJoueur();
    
    //life
    src.h = 7;src.w = 44;
    dst.x = 250;dst.y = 10;
    src.x=158;src.y=0;
    SDL_BlitSurface(imageStatut, &src, gpScreen, &dst);
    
    for(int i=0; i < gpJoueur->getVieMax()/2; i++) {
        src.h = 7; src.w = 7;
        dst.x = 233+((i%10)*8);dst.y = 19+8*((int)(i/10));
        src.y=9;
        if (gpJoueur->getVie()-(i*2)>1)src.x=141;
        if (gpJoueur->getVie()-(i*2)==1)src.x=149;
        if (gpJoueur->getVie()-(i*2)<1)src.x=157;
        SDL_BlitSurface(imageStatut, &src, gpScreen, &dst);
    }    
}

void Statut::drawItems(SDL_Surface* gpScreen) {
    SDL_Rect src;
    SDL_Rect dst;
    
    Joueur* gpJoueur = gpJeu->getJoueur();
    
    //rubis
    src.h = 8;src.w = 8;
    dst.x =129;dst.y = 10;
    src.x=52;src.y=0;
    SDL_BlitSurface(imageStatut, &src, gpScreen, &dst);
    if (gpJoueur->getRubisMax() > 99)
        drawNombre(gpScreen, gpJoueur->getRubis(), gpJoueur->getRubisMax(),121,20,3); 
    else
        drawNombre(gpScreen, gpJoueur->getRubis(), gpJoueur->getRubisMax(),121,20,2); 
    //bombes
    if (gpJoueur->hasObjet(O_SAC_BOMBES)) {
        src.h = 8;src.w = 8;
        dst.x =157;dst.y = 10;
        src.x=80;src.y=0;
        SDL_BlitSurface(imageStatut, &src, gpScreen, &dst);
        drawNombre(gpScreen, gpJoueur->getBombe(), gpJoueur->getBombeMax(),153,20,2);
    }
    //fleches
    if (gpJoueur->hasObjet(O_ARC)) {
        src.h = 8;src.w = 14;
        dst.x =178;dst.y = 10;
        src.x=101;
        if (gpJoueur->hasObjet(O_ARC) == 5) src.y=21; else src.y=0;
        SDL_BlitSurface(imageStatut, &src, gpScreen, &dst);
        drawNombre(gpScreen, gpJoueur->getFleche(), gpJoueur->getFlecheMax(),177,20,2);
    }
    
    //clées
    if (gpJeu->isDonjon()) {
        src.h = 8;src.w = 8;
        dst.x =89;dst.y = 10;
        src.x=50;src.y=28;
        SDL_BlitSurface(imageStatut, &src, gpScreen, &dst);
        if (gpJoueur->getCle() > 9)
            drawNombre(gpScreen, gpJoueur->getCle(), 99,81,20,2);
        else
            drawNombre(gpScreen, gpJoueur->getCle(), 99,89,20,1);
    }
}

void Statut::drawNombre(SDL_Surface* gpScreen, int val, int max, int x, int y, int nb) {
    int val2;
    
    SDL_Rect src;
    SDL_Rect dst;
    
    src.h = 8;src.w = 8;
    
    dst.x = x;dst.y = y;
    
    //si on commence par les centaines
    if (nb >= 3) {
        val2 = val/100;
        src.x = (val2%5)*8;
        src.y = ((int)(val2/5))*8;
        if (val == max) src.y += 16;
        SDL_BlitSurface(imageChiffre, &src, gpScreen, &dst);
        dst.x += 8;
    }
    
    //les dizaines
    if (nb >= 2) {
        val2 = (val%100)/10;
        src.x = (val2%5)*8;
        src.y = ((int)(val2/5))*8;
        if (val == max) src.y += 16;
        SDL_BlitSurface(imageChiffre, &src, gpScreen, &dst);
        dst.x += 8;
    }
    
    //unitées
    val2 = val%10;
    src.x = (val2%5)*8;
    src.y = ((int)(val2/5))*8;
    if (val == max) src.y += 16;
    SDL_BlitSurface(imageChiffre, &src, gpScreen, &dst);
    
}

void Statut::setJauge(bool b) {jauge=b;}
void Statut::setValJauge(int i) {valjauge=i;}
void Statut::setMaxJauge(int i) {maxjauge=i;}

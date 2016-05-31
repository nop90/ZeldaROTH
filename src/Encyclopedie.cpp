/*

    Zelda Return of the Hylian

    Copyright (C) 2005-2008  Vincent Jouillat

    Please send bugreports with examples or suggestions to www.zeldaroth.fr

*/

#include "Encyclopedie.h"

Encyclopedie::Encyclopedie(Jeu* jeu) : gpJeu(jeu), page(0), gpEnnemi(0) {
    image = SDL_CreateRGBSurface(SDL_HWSURFACE, 320, 240, 32, 0, 0, 0, 0);
    imageCadre = IMG_Load("romfs:/images/logos/cadres.png");
    gpEnnemi = new Ennemi(gpJeu, 0, 0, 0, true);
}

Encyclopedie::~Encyclopedie() {
    SDL_FreeSurface(image);
    SDL_FreeSurface(imageCadre);
    delete gpEnnemi;
}

void Encyclopedie::draw(SDL_Surface* gpScreen) {
    SDL_Rect dst; dst.x = 0; dst.y = 0;
    SDL_BlitSurface(image, NULL, gpScreen, &dst);
}

void Encyclopedie::init() {
    fond();
}

void Encyclopedie::fond() {
    SDL_Rect src; src.w = 16; src.h = 16;
    SDL_Rect dst;
    
    src.x = 16; src.y = 16;
    for (int j = 0; j < 240; j+=16)
        for (int i = 0; i < 320; i+=16) {
            dst.x = i; dst.y = j; SDL_BlitSurface(imageCadre, &src, image, &dst);
        }
        
    src.x = 0; src.y = 0; dst.x = 16; dst.y = 0; 
    SDL_BlitSurface(imageCadre, &src, image, &dst);
    src.x = 0; src.y = 16; dst.x = 16; dst.y = 16; 
    SDL_BlitSurface(imageCadre, &src, image, &dst);
    src.x = 0; src.y = 32; dst.x = 16; dst.y = 32; 
    SDL_BlitSurface(imageCadre, &src, image, &dst);
    
    for (int i = 0; i < 5; i++) {
        src.x = 16; src.y = 0; dst.x = 32+16*i; dst.y = 0; 
        SDL_BlitSurface(imageCadre, &src, image, &dst);
        src.x = 16; src.y = 64; dst.x = 32+16*i; dst.y = 16; 
        SDL_BlitSurface(imageCadre, &src, image, &dst);
        src.x = 16; src.y = 32; dst.x = 32+16*i; dst.y = 32; 
        SDL_BlitSurface(imageCadre, &src, image, &dst);
    }
    
    src.x = 32; src.y = 0; dst.x = 112; dst.y = 0; 
    SDL_BlitSurface(imageCadre, &src, image, &dst);
    src.x = 32; src.y = 16; dst.x = 112; dst.y = 16; 
    SDL_BlitSurface(imageCadre, &src, image, &dst);
    src.x = 32; src.y = 32; dst.x = 112; dst.y = 32; 
    SDL_BlitSurface(imageCadre, &src, image, &dst);
    
    
    ostringstream os; os << (page+1);
    gpJeu->affiche(image, "MONSTERS "+os.str(), 40, 17);
    
    placeCadres();
    
    gpJeu->setStop(true);
    gpEnnemi->draw(image);
    gpJeu->setStop(false);
    
    delete gpEnnemi;
    gpEnnemi = new Ennemi(gpJeu, 0, 0, 0, true);
}

void Encyclopedie::placeCadres() {
    switch (page) {
        case 0 :
            cadre(152,8,32,32,4);
            cadre(208,8,32,32,7);
            cadre(264,8,32,32,11);
            cadre(16,48,48,48,12);
            cadre(96,48,48,48,10);
            cadre(176,48,48,48,9);
            cadre(256,48,48,48,8);
            cadre(16,112,48,48,1);
            cadre(96,112,48,48,6);
            cadre(176,112,48,48,5);
            cadre(256,112,48,48,16);
            cadre(16,176,48,48,15);
            cadre(96,176,48,48,3);
            cadre(176,176,48,48,13);
            cadre(252,176,56,48,14);
            break;
        case 1 :
            cadre(16,48,48,48,18);
            cadre(96,48,48,48,26);
            cadre(176,48,48,48,21);
            cadre(256,48,48,48,27);
            cadre(16,112,48,48,32);
            cadre(96,112,48,48,22);
            cadre(176,112,48,48,30);
            cadre(256,112,48,48,23);
            cadre(16,176,48,48,42);
            cadre(96,176,48,48,37);
            cadre(176,176,48,48,41);
            cadre(256,176,48,48,29);
            break;
        case 2 :
            cadre(16,48,48,48,2);
            cadre(96,48,48,48,25);
            cadre(176,48,48,48,33);
            cadre(256,48,48,48,34);
            cadre(16,112,48,48,38);
            cadre(96,112,48,48,39);
            cadre(176,112,48,48,44);
            cadre(256,112,48,48,46);
            cadre(16,176,48,48,47);
            cadre(96,176,48,48,48);
            break;
        case 3 :
            cadre(16,48,80,48,19);
            cadre(112,44,48,56,24);
            cadre(176,48,48,48,28);
            cadre(240,48,48,48,31);
            cadre(16,112,48,48,35);
            cadre(80,108,64,56,40);
            cadre(160,112,48,48,43);
            cadre(224,104,80,88,45);
            cadre(16,176,64,48,50);
            break;
    }
}

void Encyclopedie::cadre(int x, int y, int w, int h, int id) {
    SDL_Rect src; src.w = 16; src.h = 16;
    SDL_Rect dst;
    
    for (int j = y+16; j < y+h-16; j+=16)
        for (int i = x+16; i < x+w-16; i+=16) {
            src.x = 16; src.y = 64; dst.x = i; dst.y = j; 
            SDL_BlitSurface(imageCadre, &src, image, &dst);
        }
    
    //haut et bas
    for (int i = x+16; i < x+w-16; i+=16) {
        src.x = 16; src.y = 48; dst.x = i; dst.y = y; 
        SDL_BlitSurface(imageCadre, &src, image, &dst);
        src.x = 16; src.y = 80; dst.x = i; dst.y = y+h-16; 
        SDL_BlitSurface(imageCadre, &src, image, &dst);
    }
    
    //gauche et droite
    for (int j = y+16; j < y+h-16; j+=16) {
        src.x = 0; src.y = 64; dst.x = x; dst.y = j; 
        SDL_BlitSurface(imageCadre, &src, image, &dst);
        src.x = 32; src.y = 64; dst.x = x+w-16; dst.y = j;
        SDL_BlitSurface(imageCadre, &src, image, &dst);
    }
    
    //haut gauche
    src.x = 0; src.y = 48; dst.x = x; dst.y = y; 
    SDL_BlitSurface(imageCadre, &src, image, &dst);
    
    //haut droite
    src.x = 32; src.y = 48; dst.x = x+w-16; dst.y = y; 
    SDL_BlitSurface(imageCadre, &src, image, &dst);
    
    //bas gauche
    src.x = 0; src.y = 80; dst.x = x; dst.y = y+h-16; 
    SDL_BlitSurface(imageCadre, &src, image, &dst);
    
    //bas droite
    src.x = 32; src.y = 80; dst.x = x+w-16; dst.y = y+h-16; 
    SDL_BlitSurface(imageCadre, &src, image, &dst);
    
    if (!gpJeu->getJoueur()->getEnnemi(id)) return;
    Ennemi* enn = new Ennemi(gpJeu, id, 0, 0, true);
    enn->setX(gpJeu->getPhg(0)+x+((w-enn->getW())/2));
    enn->setY(gpJeu->getPhg(1)+y+((h-enn->getH())/2));
    gpEnnemi->ajout(enn);
}

void Encyclopedie::moveL() {page--; if (page < 0) page = 3; init();}

void Encyclopedie::moveR() {page++; if (page > 3) page = 0; init();}

void Encyclopedie::setPage(int i) {page = i; if (page < 0) page = 0; if (page > 3) page = 3;}

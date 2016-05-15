/*

    Zelda Return of the Hylian

    Copyright (C) 2005-2008  Vincent Jouillat

    Please send bugreports with examples or suggestions to www.zeldaroth.fr

*/

#include "Carte.h"

Carte::Carte(Jeu* jeu) : gpJeu(jeu), levely(0) {
    image = SDL_CreateRGBSurface(SDL_HWSURFACE, 320, 240, 32, 0, 0, 0, 0);
}

Carte::~Carte() {
    SDL_FreeSurface(image);
}

void Carte::init() {
    SDL_Rect src;
    SDL_Rect dst;
    Joueur* gpJoueur = gpJeu->getJoueur();
    int zone = gpJeu->getZone();
    
    if (gpJeu->isDehors()) {
        SDL_Surface* tmp = IMG_Load("romfs:/images/carte/monde.png");
        src.w = 320; src.h = 240; src.x = 0; src.y = 0; dst.y = 0; dst.x = 0;
        SDL_BlitSurface(tmp, &src, image, &dst);
        SDL_FreeSurface(tmp);
        
        tmp = IMG_Load("romfs:/images/carte/link.png");
        SDL_SetColorKey(tmp,SDL_SRCCOLORKEY,SDL_MapRGB(tmp->format,0,0,255));
        src.w = 14; src.h = 12; src.x = 0; src.y = 0; 
        
        dst.x = gpJoueur->getX()/16; dst.y = gpJoueur->getY()/16;
        dst.y+=30;
                    
        switch (zone) {
            case 1 : dst.x+=249; break;
            case 2 : dst.x+=160; dst.y+=45; break;
            case 3 : dst.x+=120; dst.y+=105; break;
            case 4 : dst.x+=140; break;
            case 5 : dst.x+=15; break;
            case 6 : dst.x+=253; dst.y+=75; break;
            case 7 : dst.x+=5; dst.y+=105; break;
            case 8 : dst.x+=102; dst.y+=45; break;
            case 9 : dst.x+=10; dst.y+=45; break;
            case 10: dst.x+=2; dst.y+=165; break;
            case 11 : dst.x+=160; dst.y+=165; break;
        }
                    
        dst.x-=7; dst.y-=6;
        
        SDL_BlitSurface(tmp, &src, image, &dst);
        SDL_FreeSurface(tmp);
    }
    
    if (gpJeu->isDonjon()) {
        levely = gpJeu->getMonde()->etage();
        initDonjon();
    }
}

void Carte::initDonjon() {
    SDL_Rect src;
    SDL_Rect dst;
    Joueur* gpJoueur = gpJeu->getJoueur();
    
    SDL_Surface* tmp = IMG_Load("romfs:/images/carte/donjon.png");
    src.w = 320; src.h = 240; src.x = 0; src.y = 0; dst.y = 0; dst.x = 0;
    SDL_BlitSurface(tmp, &src, image, &dst);
    SDL_FreeSurface(tmp);
        
    initName();
    initItems();
    if (gpJoueur->getCle(gpJeu->getZone()-12,0)) initEtage();
}

void Carte::initEtage() {
    SDL_Rect src;
    SDL_Rect dst;
    int zone = gpJeu->getZone();
    
    src.h = 16; src.w = 32;src.x = 0;src.y=32;dst.x = 40; dst.y = 96;
    switch (zone-12) {
        case 0 : src.h=32; break;
        case 2 : case 5 : case 6 : src.h=32; src.y=16; dst.y-=16; break;
        case 3 : src.h=48; src.y=0; dst.y-=32; break;
        case 4 : src.h=48; break;
        case 8 : src.h=64; src.y=16; dst.y-=16; break;
    }
    
    SDL_Surface* tmp = IMG_Load("romfs:/images/carte/level.png");
    SDL_BlitSurface(tmp, &src, image, &dst);
    SDL_FreeSurface(tmp);
    
    switch (zone-12) {
        case 0 : if (levely<-1) levely=0; if (levely>0) levely=-1; break;
        case 1 : case 7 : levely=0; break;
        case 2 : case 5 : case 6 : if (levely<0) levely=1; if (levely>1) levely=0; break;
        case 3 : if (levely<0) levely=2; if (levely>2) levely=0; break;
        case 4 : if (levely<-2) levely=0; if (levely>0) levely=-2; break;
        case 8 : if (levely<-2) levely=1; if (levely>1) levely=-2; break;
    }
    src.h = 16; src.w = 32;src.x = 0;src.y=32-levely*16;dst.x = 40; dst.y = 96-levely*16;
    
    tmp = IMG_Load("romfs:/images/carte/level2.png");
    SDL_BlitSurface(tmp, &src, image, &dst);
    SDL_FreeSurface(tmp);
    
    //étage de link
    tmp = IMG_Load("romfs:/images/carte/link.png");
    SDL_SetColorKey(tmp,SDL_SRCCOLORKEY,SDL_MapRGB(tmp->format,0,0,255));
    src.h = 12; src.w = 14;src.x = 0;src.y=0;
    dst.x = 25; dst.y = 98-gpJeu->getMonde()->etage()*16;
    SDL_BlitSurface(tmp, &src, image, &dst);
    SDL_FreeSurface(tmp);
    
    initCarte();
}

void Carte::initCarte() {
    SDL_Rect src;
    SDL_Rect dst;
    int zone = gpJeu->getZone();
    Joueur* gpJoueur = gpJeu->getJoueur();
    
    int dx=0;
    int dy=0;
    
    ostringstream os;
    ostringstream os2;
    
    switch (zone-12) {
        case 0 : dx=3; dy=3; break;
        case 1 : dx=1; dy=2; break;
        case 2 : dx=1; dy=1; break;
        case 3 : dx=2; dy=3; break;
        case 4 : dx=4; dy=2; if (levely==-2) dx=2; break;
        case 5 : dx=1; dy=0; if (levely==1) {dx=4; dy=6;} break;
        case 6 : dx=2; dy=3; break;
        case 7 : dx=3; dy=0; break;
        case 8 : dx=2; dy=2; if (levely==1) dx=4; if (levely==-1) {dx=4; dy=7;}
            if (levely==-2) dy=3; break;
    }
    
    
    os << (zone-11);
    os2 << levely;
    
    SDL_Surface* tmp = IMG_Load(("romfs:/images/carte/d"+os.str()+"e"+os2.str()+".png").c_str());
    src.x=0; src.y=0; src.w=tmp->w; src.h=tmp->h; dst.y = 48+dy*16; dst.x = 128+dx*16;
    SDL_BlitSurface(tmp, &src, image, &dst);
    SDL_FreeSurface(tmp);
    
    tmp = IMG_Load(("romfs:/images/carte/nd"+os.str()+"e"+os2.str()+".png").c_str());
    
    int di=0;
    int dj=0;
    
    switch (zone-12) {
        case 0 : if (levely==-1)di=4; break;
        case 2 : if (levely==1)di=8; break;
        case 3 : di=levely*5; break;
        case 4 : if (levely==-1)di=1; if (levely==-2) di=3; break;
        case 5 : if (levely==1) {di=1;dj=0;} break;
        case 6 : if (levely==1)di=5; break;
        case 8 : if (levely==1) {di=2; dj=5;} if (levely==-1) {di=6; dj=3;}
            if (levely==-2) {di=5; dj=1;} break;
    }
    
    for (int i = 0; i < tmp->w/16; i++) 
        for (int j = 0; j < tmp->h/16; j++) 
            if (!gpJoueur->getExplore(zone-12,i+di,j+dj)) {
                src.x=i*16; src.y=j*16; src.w=16; src.h=16; 
                dst.x = 128+(dx+i)*16; dst.y = 48+(dy+j)*16;
                SDL_BlitSurface(tmp, &src, image, &dst);
            }
        
    SDL_FreeSurface(tmp);
    
    if (gpJoueur->getCle(zone-12,1)) initBoussole();
    
    //étage de link
    if (gpJeu->getMonde()->etage() == levely) {
        tmp = IMG_Load("romfs:/images/carte/link.png");
        SDL_SetColorKey(tmp,SDL_SRCCOLORKEY,SDL_MapRGB(tmp->format,0,0,255));
        src.h = 12; src.w = 14;src.x = 0;src.y=0;
        dst.x = 128+(dx-di)*16+(gpJoueur->getX()/20)-6; 
        dst.y = 48+(dy-dj)*16+(gpJoueur->getY()/15)-7;
        SDL_BlitSurface(tmp, &src, image, &dst);
        SDL_FreeSurface(tmp);
    }
}

void Carte::initBoussole() {
    SDL_Rect src;
    SDL_Rect dst;
    int zone = gpJeu->getZone();
    Joueur* gpJoueur = gpJeu->getJoueur();
    
    SDL_Surface* tmp = IMG_Load("romfs:/images/carte/boussole.png");
    SDL_SetColorKey(tmp,SDL_SRCCOLORKEY,SDL_MapRGB(tmp->format,0,0,255));
    
    switch (zone-12) {
        case 0 :
            if (levely==0) {
                src.h = 2; src.w = 2;src.x = 0;src.y=0;
                if(!gpJoueur->getCle(0,2)) {
                    dst.x = 127+16*3+8; dst.y = 48+16*4+7;
                    SDL_BlitSurface(tmp, &src, image, &dst);
                }
                if(!gpJoueur->getCoffre(1,0)) {
                    dst.x = 127+16*6+8; dst.y = 48+16*7+7;
                    SDL_BlitSurface(tmp, &src, image, &dst);
                }
                if(!gpJoueur->getCoffre(1,1)) {
                    dst.x = 127+16*5; dst.y = 48+16*3+7;
                    SDL_BlitSurface(tmp, &src, image, &dst);
                }
                if(!gpJoueur->getCoffre(1,2)) {
                    dst.x = 127+16*3+3; dst.y = 48+16*6+10;
                    SDL_BlitSurface(tmp, &src, image, &dst);
                }
                if (!gpJoueur->hasObjet(O_GANTS)) {
                    dst.x = 127+16*6+8; dst.y = 48+16*4+7;
                    SDL_BlitSurface(tmp, &src, image, &dst);
                }
            }
            if (levely==-1) {
                if (!gpJoueur->hasCristal(0)) {
                    src.h = 8; src.w = 8;src.x = 2;src.y=0;
                    dst.x = 127+16*5-3; dst.y = 48+16*3+4;
                    SDL_BlitSurface(tmp, &src, image, &dst);
                }
            }
            if (!gpJoueur->hasCristal(0)) {
                src.h = 8; src.w = 8;src.x = 2;src.y=0;
                dst.x = 24+48+4; dst.y = 96+16+4;
                SDL_BlitSurface(tmp, &src, image, &dst);
            }
            break;
        case 1 :
            src.h = 2; src.w = 2;src.x = 0;src.y=0;
            if(!gpJoueur->getCle(1,2)) {
                dst.x = 127+16*4+8; dst.y = 48+16*3+3;
                SDL_BlitSurface(tmp, &src, image, &dst);
            }
            if(!gpJoueur->getCoffre(2,6)) {
                dst.x = 127+16*7+8; dst.y = 48+16*3+7;
                SDL_BlitSurface(tmp, &src, image, &dst);
            }
            if(!gpJoueur->getCoffre(2,7)) {
                dst.x = 127+16+8; dst.y = 48+16*3+7;
                SDL_BlitSurface(tmp, &src, image, &dst);
            }
            if(!gpJoueur->getCoffre(2,8)) {
                dst.x = 127+16+8; dst.y = 48+16*7+7;
                SDL_BlitSurface(tmp, &src, image, &dst);
            }
            if(!gpJoueur->getCoffre(2,9)) {
                dst.x = 127+16*6+3; dst.y = 48+16*5+4;
                SDL_BlitSurface(tmp, &src, image, &dst);
            }
            if(!gpJoueur->getCoffre(2,10)) {
                dst.x = 127+16*4+8; dst.y = 48+16*5+7;
                SDL_BlitSurface(tmp, &src, image, &dst);
            }
            if (!gpJoueur->hasObjet(O_GRAPPIN)) {
                dst.x = 127+16*4+3; dst.y = 48+16*7+5;
                SDL_BlitSurface(tmp, &src, image, &dst);
            }
            if (!gpJoueur->hasCristal(1)) {
                src.h = 8; src.w = 8;src.x = 2;src.y=0;
                dst.x = 127+16*4+5; dst.y = 48+16*6+4;
                SDL_BlitSurface(tmp, &src, image, &dst);
                dst.x = 24+48+4; dst.y = 96+4;
                SDL_BlitSurface(tmp, &src, image, &dst);
            }
            break;
        case 2 :
            if (levely==0) {
                src.h = 2; src.w = 2;src.x = 0;src.y=0;
                if(!gpJoueur->getCoffre(3,3)) {
                    dst.x = 127+16*7+8; dst.y = 48+16+7;
                    SDL_BlitSurface(tmp, &src, image, &dst);
                }
                if (!gpJoueur->hasObjet(O_LANTERNE)) {
                    dst.x = 127+16*4+8; dst.y = 48+16*3+7;
                    SDL_BlitSurface(tmp, &src, image, &dst);
                }
                if (!gpJoueur->hasCristal(2)) {
                    src.h = 8; src.w = 8;src.x = 2;src.y=0;
                    dst.x = 127+16*5+5; dst.y = 48+16*7+4;
                    SDL_BlitSurface(tmp, &src, image, &dst);
                }
            }
            if (levely==1) {
                src.h = 2; src.w = 2;src.x = 0;src.y=0;
                if(!gpJoueur->getCle(2,2)) {
                    dst.x = 127+16*4+8; dst.y = 48+16*2+7;
                    SDL_BlitSurface(tmp, &src, image, &dst);
                }
                if(!gpJoueur->getCoffre(3,0)) {
                    dst.x = 127+16*4+8; dst.y = 48+16*3+7;
                    SDL_BlitSurface(tmp, &src, image, &dst);
                }
                if(!gpJoueur->getCoffre(3,1)) {
                    dst.x = 127+16*5+8; dst.y = 48+16*2+7;
                    SDL_BlitSurface(tmp, &src, image, &dst);
                }
                if(!gpJoueur->getCoffre(3,2)) {
                    dst.x = 127+16*4+8; dst.y = 48+16+7;
                    SDL_BlitSurface(tmp, &src, image, &dst);
                }
            }
            if (!gpJoueur->hasCristal(2)) {
                src.h = 8; src.w = 8;src.x = 2;src.y=0;
                dst.x = 24+48+4; dst.y = 96+4;
                SDL_BlitSurface(tmp, &src, image, &dst);
            }
            break;
        case 3 :
            if (levely==0) {
                src.h = 2; src.w = 2;src.x = 0;src.y=0;
                if(!gpJoueur->getCle(3,2)) {
                    dst.x = 127+16*5+8; dst.y = 48+16*5+7;
                    SDL_BlitSurface(tmp, &src, image, &dst);
                }
                if(!gpJoueur->getCoffre(4,0)) {
                    dst.x = 127+16*2+8; dst.y = 48+16*6+7;
                    SDL_BlitSurface(tmp, &src, image, &dst);
                }
                if (!gpJoueur->hasObjet(O_PALMES)) {
                    dst.x = 127+16*2+8; dst.y = 48+16*5+7;
                    SDL_BlitSurface(tmp, &src, image, &dst);
                }
                if (!gpJoueur->hasCristal(3)) {
                    src.h = 8; src.w = 8;src.x = 2;src.y=0;
                    dst.x = 127+16*4+5; dst.y = 48+16*5+4;
                    SDL_BlitSurface(tmp, &src, image, &dst);
                }
            }
            if (levely==1) {
                src.h = 2; src.w = 2;src.x = 0;src.y=0;
                if(!gpJoueur->getCoffre(4,1)) {
                    dst.x = 127+16*3+8; dst.y = 48+16*6+7;
                    SDL_BlitSurface(tmp, &src, image, &dst);
                }
                if(!gpJoueur->getCoffre(4,2)) {
                    dst.x = 127+16*3+8; dst.y = 48+16*4+7;
                    SDL_BlitSurface(tmp, &src, image, &dst);
                }
            }
            if (!gpJoueur->hasCristal(3)) {
                src.h = 8; src.w = 8;src.x = 2;src.y=0;
                dst.x = 24+48+4; dst.y = 96+4;
                SDL_BlitSurface(tmp, &src, image, &dst);
            }
            break;
        case 4 :
            if (levely==-2) {
                src.h = 2; src.w = 2;src.x = 0;src.y=0;
                if(!gpJoueur->getCle(4,2)) {
                    dst.x = 127+16*7+8; dst.y = 48+16*2+7;
                    SDL_BlitSurface(tmp, &src, image, &dst);
                }
                if(!gpJoueur->getCoffre(5,0)) {
                    dst.x = 127+16*4+8; dst.y = 48+16*2+7;
                    SDL_BlitSurface(tmp, &src, image, &dst);
                }
                if (!gpJoueur->hasObjet(O_MARTEAU)) {
                    dst.x = 127+16*3+8; dst.y = 48+16*3+7;
                    SDL_BlitSurface(tmp, &src, image, &dst);
                }
                if (!gpJoueur->hasCristal(4)) {
                    src.h = 8; src.w = 8;src.x = 2;src.y=0;
                    dst.x = 127+16*7+5; dst.y = 48+16*5+4;
                    SDL_BlitSurface(tmp, &src, image, &dst);
                }
            }
            if (!gpJoueur->hasCristal(4)) {
                src.h = 8; src.w = 8;src.x = 2;src.y=0;
                dst.x = 24+48+4; dst.y = 96+16*2+4;
                SDL_BlitSurface(tmp, &src, image, &dst);
            }
            break;
        case 5 :
            if (levely==0) {
                src.h = 2; src.w = 2;src.x = 0;src.y=0;
                if(!gpJoueur->getCle(5,2)) {
                    dst.x = 127+16*5+8; dst.y = 48+16*7+7;
                    SDL_BlitSurface(tmp, &src, image, &dst);
                }
                if(!gpJoueur->getCoffre(6,0)) {
                    dst.x = 127+16+8; dst.y = 48+16*2+7;
                    SDL_BlitSurface(tmp, &src, image, &dst);
                }
                if(!gpJoueur->getCoffre(6,1)) {
                    dst.x = 127+16*3+8; dst.y = 48+16*7+7;
                    SDL_BlitSurface(tmp, &src, image, &dst);
                }
                if(!gpJoueur->getCoffre(6,2)) {
                    dst.x = 127+16*7+8; dst.y = 48+16*2+7;
                    SDL_BlitSurface(tmp, &src, image, &dst);
                }
                if (!gpJoueur->hasObjet(O_BFEU)) {
                    dst.x = 127+16*4+8; dst.y = 48+7;
                    SDL_BlitSurface(tmp, &src, image, &dst);
                }
            }
            if (levely==1) {
                if (!gpJoueur->hasCristal(5)) {
                    src.h = 8; src.w = 8;src.x = 2;src.y=0;
                    dst.x = 127+16*4+5; dst.y = 48+16*6+4;
                    SDL_BlitSurface(tmp, &src, image, &dst);
                }
            }
            if (!gpJoueur->hasCristal(5)) {
                src.h = 8; src.w = 8;src.x = 2;src.y=0;
                dst.x = 24+48+4; dst.y = 96-16+4;
                SDL_BlitSurface(tmp, &src, image, &dst);
            }
            break;
        case 6 :
            if (levely==0) {
                src.h = 2; src.w = 2;src.x = 0;src.y=0;
                if(!gpJoueur->getCoffre(7,0)) {
                    dst.x = 127+16*5+8; dst.y = 48+16*5+7;
                    SDL_BlitSurface(tmp, &src, image, &dst);
                }
                if (!gpJoueur->hasObjet(O_BGLACE)) {
                    dst.x = 127+16*2+8; dst.y = 48+16*7+7;
                    SDL_BlitSurface(tmp, &src, image, &dst);
                }
            }
            if (levely==1) {
                src.h = 2; src.w = 2;src.x = 0;src.y=0;
                if(!gpJoueur->getCle(6,2)) {
                    dst.x = 127+16*2+8; dst.y = 48+16*7+7;
                    SDL_BlitSurface(tmp, &src, image, &dst);
                }
                if(!gpJoueur->getCoffre(7,1)) {
                    dst.x = 127+16*3+8; dst.y = 48+16*7+7;
                    SDL_BlitSurface(tmp, &src, image, &dst);
                }
                if (!gpJoueur->hasCristal(6)) {
                    src.h = 8; src.w = 8;src.x = 2;src.y=0;
                    dst.x = 127+16*4+5; dst.y = 48+16*4+4;
                    SDL_BlitSurface(tmp, &src, image, &dst);
                }
            }
            if (!gpJoueur->hasCristal(6)) {
                src.h = 8; src.w = 8;src.x = 2;src.y=0;
                dst.x = 24+48+4; dst.y = 96-16+4;
                SDL_BlitSurface(tmp, &src, image, &dst);
            }
            break;
        case 7 :
            src.h = 2; src.w = 2;src.x = 0;src.y=0;
            if(!gpJoueur->getCle(7,2)) {
                dst.x = 127+16*5+8; dst.y = 48+7;
                SDL_BlitSurface(tmp, &src, image, &dst);
            }
            if(!gpJoueur->getCoffre(8,0)) {
                dst.x = 127+16*6+8; dst.y = 48+16*6+7;
                SDL_BlitSurface(tmp, &src, image, &dst);
            }
            if(!gpJoueur->getCoffre(8,1)) {
                dst.x = 127+16*3+8; dst.y = 48+16*7+7;
                SDL_BlitSurface(tmp, &src, image, &dst);
            }
            if (gpJoueur->getEpee()<5) {
                src.h = 8; src.w = 8;src.x = 2;src.y=0;
                dst.x = 127+16*4+5; dst.y = 48+16*3+4;
                SDL_BlitSurface(tmp, &src, image, &dst);
                dst.x = 24+48+4; dst.y = 96+4;
                SDL_BlitSurface(tmp, &src, image, &dst);
            }
            break;
        case 8 :
            if (levely==0) {
                src.h = 2; src.w = 2;src.x = 0;src.y=0;
                if(!gpJoueur->getCoffre(9,0)) {
                    dst.x = 127+16*3+8; dst.y = 48+16*2+7;
                    SDL_BlitSurface(tmp, &src, image, &dst);
                }
                if(!gpJoueur->getCoffre(9,1)) {
                    dst.x = 127+16*2+8; dst.y = 48+16*2+7;
                    SDL_BlitSurface(tmp, &src, image, &dst);
                }
            }
            if (levely==-2) {
                src.h = 2; src.w = 2;src.x = 0;src.y=0;
                if(!gpJoueur->getCle(8,2)) {
                    dst.x = 127+16*5+8; dst.y = 48+16*8+7;
                    SDL_BlitSurface(tmp, &src, image, &dst);
                }
                if(!gpJoueur->getCoffre(9,2)) {
                    dst.x = 127+16*3+8; dst.y = 48+16*3+7;
                    SDL_BlitSurface(tmp, &src, image, &dst);
                }
            }
            if (levely==1) {
                src.h = 8; src.w = 8;src.x = 2;src.y=0;
                dst.x = 127+16*4+5; dst.y = 48+16*2+4;
                SDL_BlitSurface(tmp, &src, image, &dst);
            }
            src.h = 8; src.w = 8;src.x = 2;src.y=0;
            dst.x = 24+48+4; dst.y = 96-16+4;
            SDL_BlitSurface(tmp, &src, image, &dst);
            break;
    }
    SDL_FreeSurface(tmp);
}

void Carte::initItems() {
    SDL_Rect src;
    SDL_Rect dst;
    Joueur* gpJoueur = gpJeu->getJoueur();
    if (gpJoueur->getCle(gpJeu->getZone()-12,0)) { //carte
        src.h = 16; src.w = 16;src.x = 48;src.y=0;dst.x = 28; dst.y = 192;
        SDL_BlitSurface(gpJeu->getImageObjets(), &src, image, &dst);
    }
    if (gpJoueur->getCle(gpJeu->getZone()-12,1)) { //boussole
        src.h = 16; src.w = 16;src.x = 96;src.y=0;dst.x = 48; dst.y = 192;
        SDL_BlitSurface(gpJeu->getImageObjets(), &src, image, &dst);
    }
    if (gpJoueur->getCle(gpJeu->getZone()-12,2)) { //cle boss
        src.h = 16; src.w = 16;src.x = 16;src.y=0;dst.x = 68; dst.y = 192;
        SDL_BlitSurface(gpJeu->getImageObjets(), &src, image, &dst);
    }
}

void Carte::initName() {
    int zone = gpJeu->getZone();
    //message
    switch (zone-12) {
        case 0 : gpJeu->affiche(image, "Forest Temple",128,25); break;
        case 1 : gpJeu->affiche(image, "Water Temple",128,25); break;
        case 2 : gpJeu->affiche(image, "Mountain Temple",128,25); break;
        case 3 : gpJeu->affiche(image, "Desert Temple",128,25); break;
        case 4 : gpJeu->affiche(image, "Dark Temple",128,25); break;
        case 5 : gpJeu->affiche(image, "Fire Temple",128,25); break;
        case 6 : gpJeu->affiche(image, "Ice Temple",128,25); break;
        case 7 : gpJeu->affiche(image, "Hidden Temple",128,25); break;
        case 8 : gpJeu->affiche(image, "Hyrule Castle",128,25); break;
    }
}

void Carte::moveLevel(Direction dir) {
    if (!gpJeu->isDonjon()) return;
    if (!gpJeu->getJoueur()->getCle(gpJeu->getZone()-12,0)) return;
    if (gpJeu->getZone()!=13 && gpJeu->getZone()!=19) gpJeu->getAudio()->playSound(3);
    int old = levely;
    if (dir == N) levely++;
    if (dir == S) levely--;
    initDonjon();
    if (old != levely) gpJeu->getAudio()->playSound(3);
}

void Carte::draw(SDL_Surface* gpScreen) {
    SDL_Rect src;
    SDL_Rect dst;
    
    src.w = 320; src.h = 240; src.x = 0; src.y = 0; dst.y = 0; dst.x = 0;
    
    SDL_BlitSurface(image, &src, gpScreen, &dst);
}

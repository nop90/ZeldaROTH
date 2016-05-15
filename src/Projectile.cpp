/*

    Zelda Return of the Hylian

    Copyright (C) 2005-2008  Vincent Jouillat

    Please send bugreports with examples or suggestions to www.zeldaroth.fr

*/

#include "Projectile.h"
#include "Statut.h"
#include "Joueur.h"
#include "Monde.h"
#include "Menu.h"
#include "Texte.h"
#include "Jeu.h"

#include "3ds/gfx_3ds.h"


Projectile::Projectile(Jeu* jeu, int type, Direction dir, int a, int b, int vit) : 
Listable(), gpJeu(jeu), id(type), direction(dir), x(a), y(b), anim(0), max(0), 
vanim(120), vie(1), hauteur(0), vitesse(vit), image(NULL), etape(1), chemin(0), 
chmax(0) {
    
    changeType(id);
    
    lastAnimTime = SDL_GetTicks();
}

Projectile::~Projectile() {
    SDL_FreeSurface(image);
}

void Projectile::draw(SDL_Surface* gpScreen) {
    if (id == 3 && (gpJeu->getJoueur()->getTypeAnim()==TOUCHE 
    || gpJeu->getJoueur()->getTypeAnim()==CHUTE)) vie = 0;
    if (id > 0 && vie) {
    
        int phg[2];
        phg[0] = gpJeu->getPhg(0);
        phg[1] = gpJeu->getPhg(1);
    
        // déplacement
        if (vie && !gpJeu->getStop() && vitesse) {
            for (int i=0; i<vitesse; i++) {
                if (etape == 1) {
                    switch (direction) {
                        case N : move(0,-1); break;
                        case S : move(0,1); break;
                        case O : move(-1,0); break;
                        case E : move(1,0); break;
                    }
                }else {
                    switch (direction) {
                        case S : move(0,-1); break;
                        case N : move(0,1); break;
                        case E : move(-1,0); break;
                        case O : move(1,0); break;
                    }
                }
                if (vie <= 0) break;
                if (hauteur==0 || (id>=21 && id<=26)) attaque();
            }
        }
    
        //dessin
        if (vie) {
            
            if ((hauteur==0 || (id>=21 && id<=26)) && !vitesse) attaque();
            
            SDL_Rect src;
            SDL_Rect dst;
    
            src.h = h;src.w = w;
            dst.x = x-phg[0];dst.y = y-phg[1];
            src.x=w*direction;src.y=h*anim;
        
            if ((id == 1 || id == 2) && anim > 3) src.y=h*(anim-2);
            if (id == 4) {
                src.y=8; if (anim < 22) src.x=0; else src.x=(anim-21)*16;
                if (id == 4 && anim == 30) src.y=0;
            }
            if (id == 5) src.x+=51;
            if (id == 6) src.x=34;
            if (id == 7) {src.y=0; if (anim<4) src.x=w*(anim%2); else src.x=32;}
            
            if (id == 8) {
                src.y = 0; dst.y = y - phg[1];
                src.x = 0; dst.x = x - phg[0]-anim*4-8;
                SDL_BlitSurface(image, &src, gpScreen, &dst);
                src.x = 16; dst.x = x - phg[0]+anim*4+8;
            }
            
            if (id >=9 && id <=12) {
                src.x=w*anim; src.y=h*(id-9);
            }
            
            if (id ==15 || id == 16) {src.x=w*anim; src.y=0;}
            
            if (id >=21 && id <=26) {
                if (!anim) { src.x=112; src.y=(id-21)*16;}
                if (anim==1) { src.x=64; src.y=74;}
                if (anim==2) { src.x=56; src.y=90;}
                if (anim>2) { src.x=80; src.y=74;}
            }
            
            SDL_BlitSurface(image, &src, gpScreen, &dst);
            
            if (id == 3) dessinChaine(gpScreen);
            if (id == 6) dessinTrainee(gpScreen);
        
            //anim
            if(SDL_GetTicks() > lastAnimTime + vanim && !gpJeu->getStop()) {
                lastAnimTime = SDL_GetTicks();
                anim++;
                if (anim > max) {
                    anim = 0;
                    if ((id == 1 || id == 2 || id == 4 || id >= 7/* || id == 8*/) && max > 0) vie = 0;
                    if (id == 7 && gpJeu->getJoueur()->getTypeAnim()==LANTERNE) 
                        gpJeu->getJoueur()->setTypeAnim(AUCUNE);
                }
                if (id == 4 && anim == 30) {
                    w=32; h=32; zx=0; zy=0; zw=32; zh=32; x-=8; y-=8;
                    gpJeu->getAudio()->playSound(16);
                }
                if (id >= 21 && id <=26 && anim == 2) {
                    w=24; h=24; zx=0; zy=0; zw=24; zh=24; x-=4; y-=4;
                }
                if (id >= 21 && id <=26 && anim == 3) {
                    w=32; h=32; zx=0; zy=0; zw=32; zh=32; x-=4; y-=4;
                }
            }
        }
    }
    
    
    
    if (suivant != NULL) if (((Projectile*)suivant)->vie == 0) enleve(suivant);
    if (suivant != NULL) ((Projectile*)suivant)->draw(gpScreen);
}

void Projectile::attaque() {
    switch (id) {
        case 1 :
        case 2 : 
            //if (gpJeu->testEpee(x+zx, y+zy, zw, zh, FLECHE, 2+3*(id-1), direction)) vie=0; 
            switch (direction) {
                case N:
                    if (gpJeu->testEpee(x+zx+(zw/2), y+zy+4, 1, 1, FLECHE, 2+3*(id-1), direction)) vie=0;
                    break;
                case S:
                    if (gpJeu->testEpee(x+zx+(zw/2), y+zy+zh-1-4, 1, 1, FLECHE, 2+3*(id-1), direction)) vie=0;
                    break;
                case O:
                    if (gpJeu->testEpee(x+zx+4, y+zy+(zh/2), 1, 1, FLECHE, 2+3*(id-1), direction)) vie=0;
                    break;
                case E:
                    if (gpJeu->testEpee(x+zx+zw-1-4, y+zy+(zh/2), 1, 1, FLECHE, 2+3*(id-1), direction)) vie=0;
                    break;
            }
            break;
        case 3 : 
            if (etape == 1) 
            switch (direction) {
                case N:
                    if (gpJeu->testEpee(x+zx+(zw/2), y+zy+4, 1, 1, COUP_GRAPPIN, 1, direction)) 
                        etape=2; break;
                case S:
                    if (gpJeu->testEpee(x+zx+(zw/2), y+zy+zh-1-4, 1, 1, COUP_GRAPPIN, 1, direction)) 
                        etape=2; break;
                case O:
                    if (gpJeu->testEpee(x+zx+4, y+zy+(zh/2), 1, 1, COUP_GRAPPIN, 1, direction)) 
                        etape=2; break;
                case E:
                    if (gpJeu->testEpee(x+zx+zw-1-4, y+zy+(zh/2), 1, 1, COUP_GRAPPIN, 1, direction)) 
                        etape=2; break;
            }
            break;
        case 4 : 
            if (anim == 30) {
                gpJeu->testEpee(x+zx, y+zy, zw, zh, EXPLOSION, 5, direction);
                gpJeu->testDegat(x+zx, y+zy, zw, zh, EXPLOSION, 5, direction);
            }
            break;
        case 5 : 
            if (gpJeu->testEpee(x+zx, y+zy, zw, zh, FEU, 3, direction)) {
                changeType(7);
                vitesse=0;
                gpJeu->getAudio()->playSound(29);
            } break;
        case 6 : if (gpJeu->testEpee(x+zx, y+zy, zw, zh, COUP_GLACE, 3, direction)) vie=0; break;
        case 7 : gpJeu->testEpee(x+zx, y+zy, zw, zh, FEU, 1, direction); break;
        case 21 :
        case 22 :
        case 23 :
        case 24 :
        case 25 : 
            if (gpJeu->testEpee(x+zx, y+zy, zw, zh, POT, 2, direction) && vitesse) {
                vitesse=0;anim=1;max=4;gpJeu->getAudio()->playSound(36);
                lastAnimTime = SDL_GetTicks();
            } 
            break;
        case 26 : if (gpJeu->testEpee(x+zx, y+zy, zw, zh, POT, 4, direction) && vitesse) {
                vitesse=0;anim=1;max=4;gpJeu->getAudio()->playSound(36);
                lastAnimTime = SDL_GetTicks();
            } 
            break;
    }
}

void Projectile::move(int dx, int dy) {
    
    if ((id == 1 || id == 2) && max > 0) return;
    
    Monde* gpMonde = gpJeu->getMonde();
    
    if (x+dx+zx <= gpMonde->regionValue(0) || x+dx+zx+zw >= gpMonde->regionValue(2)
    || y+dy+zy <= gpMonde->regionValue(1) || y+dy+zy+zh >= gpMonde->regionValue(3)) {
        fin(); return;
    }
    
    if (id == 3) testAgrippe();
    
    int valeur=0;
    bool obstacle = false;
    
    switch (direction) {
        case N:
            valeur = gpMonde->murValue(x+dx+zx+(zw/2), y+dy+zy+4);
            if (valeur == PLEIN || valeur == BOIS) obstacle = true;
            else if (valeur == BAS || valeur == BAS_GAUCHE || valeur == BAS_DROITE) hauteur--;
            else if (valeur == HAUT || valeur == HAUT_GAUCHE || valeur == HAUT_DROITE) hauteur++;
            break;
        case S:
            valeur = gpMonde->murValue(x+dx+zx+(zw/2), y+dy+zy+zh-1-4);
            if (valeur == PLEIN || valeur == BOIS) obstacle = true;
            else if (valeur == BAS || valeur == BAS_GAUCHE || valeur == BAS_DROITE) hauteur++;
            else if (valeur == HAUT || valeur == HAUT_GAUCHE || valeur == HAUT_DROITE) hauteur--;
            break;
        case O:
            valeur = gpMonde->murValue(x+dx+zx+4, y+dy+zy+(zh/2));
            if (valeur == PLEIN || valeur == BOIS) obstacle = true;
            else if (valeur == DROITE || valeur == HAUT_DROITE || valeur == BAS_DROITE) hauteur--;
            else if (valeur == GAUCHE || valeur == HAUT_GAUCHE || valeur == BAS_GAUCHE) hauteur++;
            break;
        case E:
            valeur = gpMonde->murValue(x+dx+zx+zw-1-4, y+dy+zy+(zh/2));
            if (valeur == PLEIN || valeur == BOIS) obstacle = true;
            else if (valeur == DROITE || valeur == HAUT_DROITE || valeur == BAS_DROITE) hauteur++;
            else if (valeur == GAUCHE || valeur == HAUT_GAUCHE || valeur == BAS_GAUCHE) hauteur--;
            break;
    }
    
    if (((obstacle && !hauteur) || hauteur < 0 || (chmax>0 && chemin>=chmax)) && etape == 1) {
        fin();
    }else {
        x+=dx; y+=dy; 
        if (etape==1) chemin++;
        else chemin--;
        if (id == 3 && chemin <= 0) { vie = 0; gpJeu->getJoueur()->setTypeAnim(AUCUNE);}
    }
}

void Projectile::testAgrippe() {
    if (hauteur>0) return;
    
    Monde* gpMonde = gpJeu->getMonde();
    Joueur* gpJoueur = gpJeu->getJoueur();
    
    //SDL_Rect* z = gpJoueur->getZoneBase()->getZone();
    
    switch (direction) {
        case N:
            if(gpMonde->murValue(x+zx+(zw/2), y+zy) == BOIS) {
                vie=0;
                gpJoueur->setY(y+zy-3-4-4);
                while (!gpJoueur->verifZone()) gpJoueur->moveY(2);
                gpJoueur->setTypeAnim(AUCUNE);
                return;
            }
            break;
        case S:
            if(gpMonde->murValue(x+zx+(zw/2), y+zy+zh-1) == BOIS) {
                vie=0;
                gpJoueur->setY(y+zy-17);
                while (!gpJoueur->verifZone()) gpJoueur->moveY(-2);
                gpJoueur->setTypeAnim(AUCUNE);
                return;
            }
            break;
        case O:
            if(gpMonde->murValue(x+zx, y+zy+(zh/2)) == BOIS) {
                vie=0;
                gpJoueur->setX(x+zx+1);
                while (!gpJoueur->verifZone()) gpJoueur->moveX(2);
                gpJoueur->setTypeAnim(AUCUNE);
                return;
            }
            break;
        case E:
            if(gpMonde->murValue(x+zx+zw-1, y+zy+(zh/2)) == BOIS) {
                vie=0;
                gpJoueur->setX(x+zx-9);
                while (!gpJoueur->verifZone()) gpJoueur->moveX(-2);
                gpJoueur->setTypeAnim(AUCUNE);
                return;
            }
            break;
    }
}

void Projectile::dessinChaine(SDL_Surface* gpScreen) {
    int phg[2];
    phg[0] = gpJeu->getPhg(0);
    phg[1] = gpJeu->getPhg(1);
    
    SDL_Rect src;
    SDL_Rect dst;
    
    src.h = 8; src.w = 8; src.x=48;
    
    int chemin2=chemin;
    int a=x-phg[0];
    int b=y-phg[1];
    int c=0;
    
    switch (direction) {
        case N :
            a+=2; b+=8;
            while (chemin2 > 16) {
                dst.x=a; dst.y=b; src.y=8*c;
                SDL_BlitSurface(image, &src, gpScreen, &dst);
                chemin2-=8; c=(c+1)%2; b+=8;
            }
            break;
        case S :
            a+=2; b-=8;
            while (chemin2 > 16) {
                dst.x=a; dst.y=b; src.y=8*c;
                SDL_BlitSurface(image, &src, gpScreen, &dst);
                chemin2-=8; c=(c+1)%2; b-=8;
            }
            break;
        case O :
            a+=8; b+=2;
            while (chemin2 > 16) {
                dst.x=a; dst.y=b; src.y=8*c;
                SDL_BlitSurface(image, &src, gpScreen, &dst);
                chemin2-=8; c=(c+1)%2; a+=8;
            }
            break;
        case E :
            a-=8; b+=2;
            while (chemin2 > 16) {
                dst.x=a; dst.y=b; src.y=8*c;
                SDL_BlitSurface(image, &src, gpScreen, &dst);
                chemin2-=8; c=(c+1)%2; a-=8;
            }
            break;
    }
}

void Projectile::dessinTrainee(SDL_Surface* gpScreen) {
    int phg[2];
    phg[0] = gpJeu->getPhg(0);
    phg[1] = gpJeu->getPhg(1);
    
    SDL_Rect src;
    SDL_Rect dst;
    
    src.h = h; src.w = w; src.y = 0;
    
    int a=x-phg[0];
    int b=y-phg[1];
    
    if(chemin>=17) {
        src.x=17;
        switch (direction) {
            case N : dst.x=a; dst.y=b+17; break;
            case S : dst.x=a; dst.y=b-17; break;
            case O : dst.x=a+17; dst.y=b; break;
            case E : dst.x=a-17; dst.y=b; break;
        }
        SDL_BlitSurface(image, &src, gpScreen, &dst);
    }
    
    if(chemin>=34) {
        src.x=0;
        switch (direction) {
            case N : dst.x=a; dst.y=b+34; break;
            case S : dst.x=a; dst.y=b-34; break;
            case O : dst.x=a+34; dst.y=b; break;
            case E : dst.x=a-34; dst.y=b; break;
        }
        SDL_BlitSurface(image, &src, gpScreen, &dst);
    }
}

void Projectile::changeType(int type) {
    id = type;
    SDL_FreeSurface(image);
    switch (type) {
        case 1 : //flèche
        case 2 : //flèche argent
            if (id == 1) image = IMG_Load("romfs:/images/objet/projectiles.png"); 
            else image = IMG_Load("romfs:/images/objet/projectilesarg.png"); 
            w=16; h=16;
            if (direction == N || direction == S) {zx=4; zy=0; zw=7; zh=16;} 
            else {zx=0; zy=4; zw=16; zh=7;}
            break;
        case 3 : //grappin
            image = IMG_Load("romfs:/images/objet/grappin.png");
            w=12; h=12;
            if (direction == N || direction == S) {zx=0; zy=0; zw=12; zh=8;} 
            else {zx=0; zy=0; zw=8; zh=12;}
            chmax=208;
            break;
        case 4 : //bombe
            image = IMG_Load("romfs:/images/objet/bombe.png");
            w=16; h=16;
            zx=0; zy=0; zw=16; zh=16;
            max=30;
            break;
        case 5 : //baguette de feu
        case 6 : //baguette de glace
            image = IMG_Load("romfs:/images/objet/magie.png");
            w=17; h=17;
            zx=0; zy=0; zw=17; zh=17;
            break;
        case 7 : //flamme
            image = IMG_Load("romfs:/images/objet/feu.png");
            w=16; h=16;
            zx=0; zy=0; zw=16; zh=16;
            max=5;
            break;
        case 8 : //plouf
            image = IMG_Load("romfs:/images/objet/plouf.png");
            w=16; h=16;
            zx=0; zy=0; zw=16; zh=16;
            vanim=40;
            max=3;
            gpJeu->getAudio()->playSound(31);
            break;
        case 9 : // buisson
        case 10 :
        case 11 :
        case 12 :
            image = IMG_Load("romfs:/images/objet/buisson.png");
            w=27; h=43;
            zx=0; zy=0; zw=27; zh=43;
            max=6;
            gpJeu->getAudio()->playSound(38);
            break;
        case 15 : //explosion fumée blanche
            image = IMG_Load("romfs:/images/ennemi/explo.png");
            w=28; h=28;
            zx=0; zy=0; zw=w; zh=h;
            max=6; vanim=60;
            break;
        case 16 : //explosion fumée rose
            image = IMG_Load("romfs:/images/link/exploLapin.png");
            w=28; h=28;
            zx=0; zy=0; zw=w; zh=h;
            max=3; vanim=60;
            break;
        case 21 : //pots
        case 22 :
        case 23 :
        case 24 :
        case 25 :
        case 26 :
            image = IMG_Load("romfs:/images/link/objets.png");
            w=16; h=16;
            zx=0; zy=0; zw=16; zh=16;
            chmax=64;
            if (direction==N) chmax-=8;
            if(vitesse==0){anim=1;max=4;gpJeu->getAudio()->playSound(36);}
            break;
    }
    if (image != NULL) SDL_SetColorKey(image,SDL_SRCCOLORKEY,SDL_MapRGB(image->format,0,0,255));
}

void Projectile::fin() {
    if (id == 1 || id == 2) {anim=1; max = 5;}
    else if (id == 3) etape=2;
    else if (id == 5) {changeType(7); vitesse=0; gpJeu->getAudio()->playSound(29);}
    else if (id >= 21 && id <= 26) {
        if (gpJeu->isWater(x, y, w, h) && chemin>=chmax) {
            vie = 0;
            gpJeu->ajouteProjectile(8, direction, x, y, 0);
        }
        else if (gpJeu->isVide(x, y, w, h) && chemin>=chmax) {
            vie = 0;
            gpJeu->getAudio()->playSound(6);
        }
        else {
            vitesse=0;anim=1;max=4;gpJeu->getAudio()->playSound(36);
            lastAnimTime = SDL_GetTicks();
        }
    }
    else vie=0;
}

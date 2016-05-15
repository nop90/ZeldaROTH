/*

    Zelda Return of the Hylian

    Copyright (C) 2005-2008  Vincent Jouillat

    Please send bugreports with examples or suggestions to www.zeldaroth.fr

*/

#include <cmath>

#include "Menu.h"
#include "Joueur.h"
#include "Monde.h"
#include "Texte.h"
#include "Projectile.h"
#include "Ennemi.h"
#include "Jeu.h"

Ennemi::Ennemi(Jeu* jeu, int type, int i, int j, bool p) : Personnage(), gpJeu(jeu), 
id(type), vanim(120), vol(0), nage(0), spectre(0), porte(0), typeIA(IA_RIEN), 
vitesse(1), force(0), recul(16), glace(0), maxglace(60), typeEnn(E_SIMPLE), 
zonesAttaques(NULL), zonesVulnerables(NULL), chargeIm(false), arme(false),
special1(0), special2(0), persistent(p) {
    if (id) {
        Ennemi* gpEnnemi = gpJeu->existEnnemi(id);
        if (gpEnnemi != NULL) {
            image = gpEnnemi->image;
        }
        else {
            ostringstream im;
            im << id;
            image = IMG_Load(("romfs:/images/ennemi/ennemi" + im.str() + ".png").c_str());
            SDL_SetColorKey(image,SDL_SRCCOLORKEY,SDL_MapRGB(image->format,0,0,255));
            chargeIm = true;
        }
        x=i; y=j; saveEmpl();
        special1=0; // à sortir pour l'ennemi 20
        init();
    }
}

Ennemi::~Ennemi() {
    if (!chargeIm) image = NULL;
    delete zonesAttaques;
    delete zonesVulnerables;
}

void Ennemi::drawSpecial(SDL_Surface* gpScreen) {
    int phg[2];
    phg[0] = gpJeu->getPhg(0);
    phg[1] = gpJeu->getPhg(1);
    
    Monde* gpMonde = gpJeu->getMonde();
    
    if (id && vie>0 && x+w > gpMonde->regionValue(0) && x < gpMonde->regionValue(2)
    && y+h > gpMonde->regionValue(1) && y < gpMonde->regionValue(3)) {
        
        SDL_Rect src;
        SDL_Rect dst;
        
        if (id == 51) {
            if (anim > 10+10) {
                dst.x = x-phg[0]+2;
                dst.y = y-phg[1]+18;
                src.y=22; src.x=2; src.h=6; src.w=20;
                SDL_BlitSurface(image, &src, gpScreen, &dst);
            }
        }
        
    }
    
    if (suivant != NULL) ((Ennemi*)suivant)->drawSpecial(gpScreen);
}

void Ennemi::draw(SDL_Surface* gpScreen) {
    int phg[2];
    phg[0] = gpJeu->getPhg(0);
    phg[1] = gpJeu->getPhg(1);
    
    Monde* gpMonde = gpJeu->getMonde();
    
    if (isBoss()) {
        gpJeu->getStatut()->setValJauge(vie); gpJeu->getStatut()->setMaxJauge(viemax);}
    
    if (id && vie>0 && x+w > gpMonde->regionValue(0) && x < gpMonde->regionValue(2)
    && y+h > gpMonde->regionValue(1) && y < gpMonde->regionValue(3)) {
        
        if (!gpJeu->getStop() && (!invincible || isBoss()) && !glace) IA();
        
        if (gpJeu->getTransition()) return;
        
        SDL_Rect src;
        SDL_Rect dst;
        
        int anim2 = anim;
        
        if (id == 15) {
            if (anim < 18) anim2 = anim2%2;
            else anim2 = 2;
        }
        
        if (id == 16 || id == 30) anim2 = anim/5;
        
        if (id == 27) {
            if (anim < 6) anim2 = anim2%2;
            else if (anim < 8) anim2 = 2;
            else anim2 = 3;
        }
        
        if (id == 31 && anim > 3) anim2=3;
        
        if (id == 51) {
            if (anim > 30 && special1 == 0) anim2=1;
            else anim2=0;
        }
        
        if (typeIA==IA_STATUE) anim2=0;
        
        switch (typeEnn) {
            case E_SIMPLE : src.x=direction*w; src.y=h*anim2; break;
            case E_PROFIL : src.x=(direction%2)*w; src.y=h*anim2; break;
            case E_MONO_DIR : src.x=w*anim2; src.y=0; break;
        }
        dst.x=x-phg[0]; dst.y=y-phg[1]; src.w=w; src.h=h;
        
        if (id == 20 && special1) src.x+=16;
        if (id == 51) dst.y-=special1;
        if (id == 52 && special1 >= 3) src.y+=34;
        
        if (direction==N && arme) drawArme(gpScreen);
        
        if (id !=51 || anim>=20) SDL_BlitSurface(image, &src, gpScreen, &dst);
        
        if (direction!=N && arme) drawArme(gpScreen);
        
        //if(animmax) {    
            if(SDL_GetTicks() > lastAnimTime + vanim && !gpJeu->getStop()) {
            lastAnimTime = SDL_GetTicks();
                if (!invincible && !glace) {
                    anim++;
                    if (anim > animmax) anim = 0;
                    if (id == 10 && anim == 1) snipe();
                    if (id == 15 && anim == 20) tire();
                    if (id == 16 && anim > 4 && anim%2==1) tire();
                    if (id == 27 && anim == 8) tire();
                    if (id == 30 && anim == 5) tire();
                    if (id == 31 && anim == 8) snipe();
                    if ((id == 36 || id == 49) && anim == 8) snipe();
                    if (id == 51 && anim == 10+10) {
                        x=gpJeu->getJoueur()->getX()-4;y=gpJeu->getJoueur()->getY();}
                    if (id == 51 && anim == 20) {special1=240;gpJeu->getAudio()->playSound(6);}
                    if (id == 51 && anim == 30) if (special1>0) anim--;
                    if (id == 51 && anim == 40) if (special1<240) anim--;
                }
                if (invincible > 0) invincible++;
                if (invincible > 3/*>= 1200/vanim*/) invincible=0;
                if (glace > 0) glace++;
                if (glace >= (maxglace*40)/vanim) glace=0;
            }
        //}
    }
    if (suivant != NULL) ((Ennemi*)suivant)->draw(gpScreen);
}

void Ennemi::drawArme(SDL_Surface* gpScreen) {
    int phg[2];
    phg[0] = gpJeu->getPhg(0);
    phg[1] = gpJeu->getPhg(1);
    
    SDL_Rect src;
    SDL_Rect dst;
    
    dst.x=x-phg[0]+zonesAttaques->getSuivant()->getZone()->x;
    dst.y=y-phg[1]+zonesAttaques->getSuivant()->getZone()->y;
    src.w = zonesAttaques->getSuivant()->getZone()->w;
    src.h = zonesAttaques->getSuivant()->getZone()->h;
    
    switch (id) {
        case 5 :
            switch (direction) {
                case N : src.x=64; src.y=10; break;
                case S : src.x=69; src.y=10; break;
                case O : src.x=64; src.y=0; break;
                case E : src.x=64; src.y=5; break;
            }
            SDL_BlitSurface(image, &src, gpScreen, &dst);
            break;
        case 44 :
            switch (direction) {
                case N : src.x=88; src.y=14; break;
                case S : src.x=95; src.y=14; break;
                case O : src.x=88; src.y=0; break;
                case E : src.x=88; src.y=7; break;
            }
            SDL_BlitSurface(image, &src, gpScreen, &dst);
            break;
        case 46 :
        case 47 :
        case 48 :
            switch (direction) {
                case N : src.x=88; src.y=10; break;
                case S : src.x=93; src.y=10; break;
                case O : src.x=88; src.y=0; break;
                case E : src.x=88; src.y=5; break;
            }
            SDL_BlitSurface(image, &src, gpScreen, &dst);
            break;
    }
}

void Ennemi::moveX(int i) {
    moveX(i, vol, nage, spectre);
}

void Ennemi::moveY(int i) {
    moveY(i, vol, nage, spectre);
}

void Ennemi::moveX(int i, int v, int n, int s) {
    if (id==21 && !gpJeu->isWater(x + gpZoneBase->getZone()->x + i, 
        y + gpZoneBase->getZone()->y, gpZoneBase->getZone()->w, 
        gpZoneBase->getZone()->h)) return;
    if (gpJeu->isEnnemi(x + gpZoneBase->getZone()->x + i, y + gpZoneBase->getZone()->y, 
        gpZoneBase->getZone()->w, gpZoneBase->getZone()->h, getListId())) return;
    if (gpJeu->isPnj(x + gpZoneBase->getZone()->x + i, y + gpZoneBase->getZone()->y, 
        gpZoneBase->getZone()->w, gpZoneBase->getZone()->h, getListId())) return;
    if (!gpJeu->isNotFull(x + gpZoneBase->getZone()->x + i, y + gpZoneBase->getZone()->y, 
        gpZoneBase->getZone()->w, gpZoneBase->getZone()->h, v, n, s)) return;
    x+=i;
}

void Ennemi::moveY(int i, int v, int n, int s) {
    if (id==21 && !gpJeu->isWater(x + gpZoneBase->getZone()->x, 
        y + gpZoneBase->getZone()->y + i, gpZoneBase->getZone()->w, 
        gpZoneBase->getZone()->h)) return;
    if (gpJeu->isEnnemi(x + gpZoneBase->getZone()->x, y + gpZoneBase->getZone()->y + i, 
        gpZoneBase->getZone()->w, gpZoneBase->getZone()->h, getListId())) return;
    if (gpJeu->isPnj(x + gpZoneBase->getZone()->x, y + gpZoneBase->getZone()->y + i, 
        gpZoneBase->getZone()->w, gpZoneBase->getZone()->h)) return;
    if (!gpJeu->isNotFull(x + gpZoneBase->getZone()->x, y + gpZoneBase->getZone()->y + i, 
        gpZoneBase->getZone()->w, gpZoneBase->getZone()->h, v, n, s)) return;
    y+=i;
}

void Ennemi::pousse(Direction dir) {
    switch (dir) {
        case N : for (int i = 0; i < recul; i++) moveY(-1, 1, 1, spectre); break;
        case S : for (int i = 0; i < recul; i++) moveY(1, 1, 1, spectre); break;
        case O : for (int i = 0; i < recul; i++) moveX(-1, 1, 1, spectre); break;
        case E : for (int i = 0; i < recul; i++) moveX(1, 1, 1, spectre); break;
    }
    //rajouter vérif eau/vide
    if (gpJeu->isVide(x+gpZoneBase->getZone()->x,y+gpZoneBase->getZone()->y,
    gpZoneBase->getZone()->w,gpZoneBase->getZone()->h) && !vol && !spectre) {
        vie = 0; gpJeu->getAudio()->playSound(6);
        gpJeu->ajouteProjectile(15, direction, x+(w/2)-14, y+(h/2)-14, 0);
        gpJeu->testAnim();
    }
    if (gpJeu->isWater(x+gpZoneBase->getZone()->x,y+gpZoneBase->getZone()->y,
    gpZoneBase->getZone()->w,gpZoneBase->getZone()->h) && !vol && !nage && !spectre) {
        vie = 0; gpJeu->ajouteProjectile(8, direction, x, y, 0);
    }
}

void Ennemi::revie() {
    if (!isBoss()) init();
    if (suivant != NULL) getSuivant()->revie();
}

void Ennemi::permute(int val) {
    if (id == 20) {
        special1=val;
    }
    if (suivant != NULL) getSuivant()->permute(val);
}

int Ennemi::getEffet(int i) {return effets[i];}

int Ennemi::getObjets(int i) {return objets[i];}

ZoneRect* Ennemi::getZonesVulnerables() {return zonesVulnerables;}

Ennemi* Ennemi::getSuivant() {return (Ennemi*)suivant;}

void Ennemi::setInvincible(int i) {invincible=i;}

void Ennemi::gele() {glace=1;}

void Ennemi::degele() {glace=0;}

void Ennemi::enerve() {
    if (id == 52) {
        for (int k = 0; k < 16; k++) effets[k]=0;
        typeIA = IA_FONCE;
        special1=3;
        vitesse=2;
        vanim = 120;
    }
}

void Ennemi::setVie(int i) {
    if (id == 52 && special1 < 3) {
        special1++;
        if (special1 == 3) gpJeu->poule();
    }
    if (id == 31) {
        special2++;
        special1 = 1 + special2;
    }
    vie=i;
    if (vie < 0) vie = 0;
    if (vie > viemax) vie = viemax;
}

void Ennemi::ajuste() {
    switch(id) {
        case 2 :
            if (direction==N || direction==S) {
                zonesAttaques->getZone()->x=0;
                zonesAttaques->getZone()->w=21;
            }
            else {
                zonesAttaques->getZone()->x=3;
                zonesAttaques->getZone()->w=16;
            }
            break;
        case 5 :
            switch(direction) {
                case N : 
                    zonesAttaques->getSuivant()->getZone()->w=5;
                    zonesAttaques->getSuivant()->getZone()->h=13;
                    zonesAttaques->getSuivant()->getZone()->x=12;
                    zonesAttaques->getSuivant()->getZone()->y=4-3*anim;
                    break;
                case S : 
                    zonesAttaques->getSuivant()->getZone()->w=5;
                    zonesAttaques->getSuivant()->getZone()->h=13;
                    zonesAttaques->getSuivant()->getZone()->x=0;
                    zonesAttaques->getSuivant()->getZone()->y=15+4*anim;
                    break;
                case O : 
                    zonesAttaques->getSuivant()->getZone()->w=13;
                    zonesAttaques->getSuivant()->getZone()->h=5;
                    zonesAttaques->getSuivant()->getZone()->x=-4-5*anim;
                    zonesAttaques->getSuivant()->getZone()->y=19;
                    break;
                case E : 
                    zonesAttaques->getSuivant()->getZone()->w=13;
                    zonesAttaques->getSuivant()->getZone()->h=5;
                    zonesAttaques->getSuivant()->getZone()->x=7+5*anim;
                    zonesAttaques->getSuivant()->getZone()->y=19;
                    break;
            }
            break;
        case 13 :
            switch (anim) {
                case 0 : zonesAttaques->getZone()->w=24; zonesAttaques->getZone()->x=4; break;
                case 1 : zonesAttaques->getZone()->w=32; zonesAttaques->getZone()->x=0; break;
                case 2 : zonesAttaques->getZone()->w=18; zonesAttaques->getZone()->x=7; break;
            }
            break;
        case 15 :
            if (direction!=N) {zonesAttaques->getZone()->y=5; zonesAttaques->getZone()->h=26;}
            else {zonesAttaques->getZone()->y=0; zonesAttaques->getZone()->h=31;}
            break;
        case 19 :
            switch (anim) {
                case 0 : gpZoneBase->getZone()->w=64; gpZoneBase->getZone()->x=0; break;
                case 1 : gpZoneBase->getZone()->w=44; gpZoneBase->getZone()->x=10; break;
                case 2 : gpZoneBase->getZone()->w=22; gpZoneBase->getZone()->x=21; break;
            }
            break;
        case 25 :
            if (direction==N || direction==S) {
                zonesAttaques->getZone()->x=0; zonesAttaques->getZone()->w=28;}
            else {zonesAttaques->getZone()->x=6; zonesAttaques->getZone()->w=16;}
            break;
        case 33 :
        case 38 :
            if (direction==N || direction==S) {
                zonesAttaques->getZone()->x=0; zonesAttaques->getZone()->w=22;}
            else {zonesAttaques->getZone()->x=2; zonesAttaques->getZone()->w=18;}
            break;
        case 34 :
        case 39 :
            if (direction==N || direction==S) {
                zonesAttaques->getZone()->x=0; zonesAttaques->getZone()->w=24;}
            else {zonesAttaques->getZone()->x=4; zonesAttaques->getZone()->w=16;}
            break;
        case 44 :
            if (direction==N || direction==S) {
                zonesAttaques->getZone()->x=0; zonesAttaques->getZone()->w=22;}
            else {zonesAttaques->getZone()->x=3; zonesAttaques->getZone()->w=16;}
            switch(direction) {
                case N : 
                    zonesAttaques->getSuivant()->getZone()->w=7;
                    zonesAttaques->getSuivant()->getZone()->h=15;
                    zonesAttaques->getSuivant()->getZone()->x=15;
                    zonesAttaques->getSuivant()->getZone()->y=-6+7*anim;
                    break;
                case S : 
                    zonesAttaques->getSuivant()->getZone()->w=7;
                    zonesAttaques->getSuivant()->getZone()->h=15;
                    zonesAttaques->getSuivant()->getZone()->x=0;
                    zonesAttaques->getSuivant()->getZone()->y=17+3*anim;
                    break;
                case O : 
                    zonesAttaques->getSuivant()->getZone()->w=15;
                    zonesAttaques->getSuivant()->getZone()->h=7;
                    zonesAttaques->getSuivant()->getZone()->x=-12+5*anim;
                    zonesAttaques->getSuivant()->getZone()->y=14+2*anim;
                    break;
                case E : 
                    zonesAttaques->getSuivant()->getZone()->w=15;
                    zonesAttaques->getSuivant()->getZone()->h=7;
                    zonesAttaques->getSuivant()->getZone()->x=19-5*anim;
                    zonesAttaques->getSuivant()->getZone()->y=14+2*anim;
                    break;
            }
            break;
        case 45 :
            switch(anim%2) {
                case 0 : 
                    zonesAttaques->getSuivant()->getZone()->y=67;
                    zonesAttaques->getSuivant()->getSuivant()->getZone()->y=59;
                    break;
                case 1 : 
                    zonesAttaques->getSuivant()->getZone()->y=59;
                    zonesAttaques->getSuivant()->getSuivant()->getZone()->y=67;
                    break;
            }
            break;
        case 46 :
        case 47 :
        case 48 :
            if (direction==N || direction==S) {
                zonesAttaques->getZone()->x=0; zonesAttaques->getZone()->w=22;}
            else {zonesAttaques->getZone()->x=2; zonesAttaques->getZone()->w=18;}
            switch(direction) {
                case N : 
                    zonesAttaques->getSuivant()->getZone()->w=5;
                    zonesAttaques->getSuivant()->getZone()->h=16+((int)(id/48));
                    zonesAttaques->getSuivant()->getZone()->x=16;
                    zonesAttaques->getSuivant()->getZone()->y=-6+7*anim;
                    break;
                case S : 
                    zonesAttaques->getSuivant()->getZone()->w=5;
                    zonesAttaques->getSuivant()->getZone()->h=16+((int)(id/48));
                    zonesAttaques->getSuivant()->getZone()->x=0;
                    zonesAttaques->getSuivant()->getZone()->y=21+anim-((int)(id/48));
                    break;
                case O : 
                    zonesAttaques->getSuivant()->getZone()->w=16+((int)(id/48));
                    zonesAttaques->getSuivant()->getZone()->h=5;
                    zonesAttaques->getSuivant()->getZone()->x=-12+3*anim-((int)(id/48));
                    zonesAttaques->getSuivant()->getZone()->y=16+2*anim;
                    break;
                case E : 
                    zonesAttaques->getSuivant()->getZone()->w=16+((int)(id/48));
                    zonesAttaques->getSuivant()->getZone()->h=5;
                    zonesAttaques->getSuivant()->getZone()->x=18-3*anim;
                    zonesAttaques->getSuivant()->getZone()->y=16+2*anim;
                    break;
            }
            break;
        case 50 :
            switch(direction) {
                case N : zonesAttaques->getZone()->x=7; zonesAttaques->getZone()->w=38; break;
                case S : zonesAttaques->getZone()->x=4*anim; zonesAttaques->getZone()->w=48; 
                    break;
                default : break;
            }
            break;
    }
}

void Ennemi::regard() {
    //if (typeEnn == E_MONO_DIR) return;
    Joueur* gpJoueur = gpJeu->getJoueur();
    int ennCX = x + gpZoneBase->getZone()->x + (gpZoneBase->getZone()->w/2);
    int ennCY = y + gpZoneBase->getZone()->y + (gpZoneBase->getZone()->h);
    int linkCX = gpJoueur->getX() + 8;
    int linkCY = gpJoueur->getY() + 24;
    int difx = ennCX - linkCX;
    int dify = ennCY - linkCY;
    
    if (abs(difx) > abs(dify) || typeEnn == E_PROFIL) {
        if (difx>0) direction=O;
        else direction=E;
    }
    else {
        if (dify>0) direction=N;
        else direction=S;
    }
    
    if (id == 50) {if (dify>0) direction = N; else direction = S;}
    
    ajuste();
}

void Ennemi::hautbas() {
    int phg[2];
    phg[0] = gpJeu->getPhg(0);
    phg[1] = gpJeu->getPhg(1);
    
    int old = y;
    switch (direction) {
        case N :
            moveY(-1);
            if (old == y) {
                direction = S;
                if (x+w > phg[0] && x < phg[0] + 320 && y+h > phg[1] && y < phg[1] + 240)
                    gpJeu->getAudio()->playSound(21);
            }
            break;
        case S :
            moveY(1);
            if (old == y) {
                direction = N;
                if (x+w > phg[0] && x < phg[0] + 320 && y+h > phg[1] && y < phg[1] + 240)
                    gpJeu->getAudio()->playSound(21);
            }
            break;
        default : break;
    }
}

void Ennemi::special() {
    if (id==51) {
        if (special1>0) special1-=4;
        if (anim>31 && anim <36)
        gpJeu->testDegat(x + zonesAttaques->getZone()->x, 
            y + zonesAttaques->getZone()->y , 
            zonesAttaques->getZone()->w, 
            zonesAttaques->getZone()->h, 
            RETOUR, force, direction);
        if (anim>35) special1+=8;
    }
}

void Ennemi::IA() {
    switch (typeIA) {
        case IA_FONCE : for (int i = 0; i < vitesse; i++) bouge(); break;
        case IA_ATTEND : attend(); break;
        case IA_RANDOM : deplace(); break;
        case IA_SNIPE : regard(); break;
        case IA_HAUT_BAS : for (int i = 0; i < vitesse; i++) hautbas(); break;
        case IA_SPECIAL : special(); break;
        default : break;
    }
    
    if ((id == 52 && special1 < 3) || id == 20 || id == 51 || force==0) return;
    
    ZoneRect* zonesAttaques2 = zonesAttaques;
    while (zonesAttaques2 != NULL) {
        gpJeu->testDegat(x + zonesAttaques2->getZone()->x, 
            y + zonesAttaques2->getZone()->y , 
            zonesAttaques2->getZone()->w, 
            zonesAttaques2->getZone()->h, 
            ENNEMI, force, direction);
        zonesAttaques2 = zonesAttaques2->getSuivant();
    }
}

void Ennemi::bouge() {
    if (!distance()) {
        if (id == 13 || id == 18) typeIA=IA_ATTEND;
        return;
    }
    if (id == 15 && anim>=18) return;
    if (id == 27 && anim>=6) return;
    
    Joueur* gpJoueur = gpJeu->getJoueur();
    
    int x1 = x + gpZoneBase->getZone()->x + (gpZoneBase->getZone()->w/2);
    int y1 = y + gpZoneBase->getZone()->y + gpZoneBase->getZone()->h;
    
    int x2 = gpJoueur->getX()+8;
    int y2 = gpJoueur->getY()+24-vol*7;
    
    
    if (x1 < x2) moveX(1);
    else if (x1 > x2) moveX(-1);
    
    if (y1 < y2) moveY(1);
    else if (y1 > y2) moveY(-1);
    
    regard();
}

void Ennemi::attend() {
    anim=animmax+1;
    if (!distance()) return;
    typeIA = IA_FONCE; 
    if (id != 13 && id !=18) porte+=130;
    for (int k = 0; k < 12; k++) if (effets[k]>=1) effets[k]=2; else effets[k]=1;
    effets[COUP_MARTEAU]=2;
    anim=0;
}

void Ennemi::deplace() {
    int randomValue = (int)((float)rand() / RAND_MAX * (100));    
    switch (randomValue) {
        case 1 : moveX(-1);direction=O; break;
        case 2 : moveX(1); direction=E; break;
        case 3 : moveY(-1);direction=N; break;
        case 4 : moveY(1); direction=S; break;
        case 5 :
        case 6 :
        case 7 :
        case 8 : break;
        default :
            switch (direction) {
                case N : moveY(-1); break;
                case S : moveY(1); break;
                case O : moveX(-1); break;
                case E : moveX(1); break;
            }
            break;    
    }
}

bool Ennemi::distance() {
    Joueur* gpJoueur = gpJeu->getJoueur();
    /*int x1 = x + gpZoneBase->getZone()->x + (gpZoneBase->getZone()->w/2);
    int y1 = y + gpZoneBase->getZone()->y + gpZoneBase->getZone()->h;
    if (typeIA==IA_ATTEND) y1 -= gpZoneBase->getZone()->h/2;
    int tmp1 = abs(x1-(gpJoueur->getX()+8));
    int tmp2 = abs(y1-(gpJoueur->getY()+24-vol*7));
    if (tmp1 > tmp2) return tmp1;
    else return tmp2;*/
    
    int gauche = x+gpZoneBase->getZone()->x;
    int droite = gauche + gpZoneBase->getZone()->w;
    int haut = y+gpZoneBase->getZone()->y;
    int bas = haut + gpZoneBase->getZone()->h;
    
    if (  ((gpJoueur->getX()<gauche-porte && gpJoueur->getX()+16>gauche-porte)
    || (gpJoueur->getX()>=gauche-porte && gpJoueur->getX()+16<=droite+porte)
    || (gpJoueur->getX()<droite+porte && gpJoueur->getX()+16>droite+porte))
    &&
    ((gpJoueur->getY()+8<haut-porte && gpJoueur->getY()+24>haut-porte)
    || (gpJoueur->getY()+8>=haut-porte && gpJoueur->getY()+24<=bas+porte)
    || (gpJoueur->getY()+8<bas+porte && gpJoueur->getY()+24>bas+porte))) {
        return true;
    }
    return false;
}

void Ennemi::snipe() {
    if(!distance()) return;
    
    Joueur* gpJoueur = gpJeu->getJoueur();
    
    int high= 16;
    /*if (gpJoueur->getTypeAnim()==FLOTTE || gpJoueur->getTypeAnim()==NAGE) high=10;
    else high = 16;*/
    
    double anglx = 0;
    double angly = 0;
    int origx = x + (w/2);
    int origy = y + (h/2);
    int destx = gpJoueur->getX() + 8;
    int desty = gpJoueur->getY() + high;;
    
    double coef1 = 0;
    double coef2 = 0;
    
    if ((destx-origx) == 0) {anglx=0; angly=12;}
    else if ((desty-origy) == 0) {anglx=12; angly=0;}
    else { 
        coef1=((double)(desty-origy))/((double)(destx-origx));
        coef2=((double)(destx-origx))/((double)(desty-origy));
        anglx=(sqrt(12/(1+(coef1*coef1))));
        angly=(sqrt(12/(1+(coef2*coef2))));
    }
    if (destx - origx < 0) anglx = -anglx;
    if (desty - origy < 0) angly = -angly;
    
    int type = 0;
    switch(id) {
        case 31 : 
            origy+=16;
            if (special1==0) {type = 5;gpJeu->getAudio()->playSound(35);}
            else if (special1 == 1 + special2) {
                type=7; special1--;gpJeu->getAudio()->playSound(28);}
            else {
                type=6; special1--; gpJeu->getAudio()->playSound(28);
            }
            break;
        case 49 : type = 8; gpJeu->getAudio()->playSound(35); break;
        default : type = 1; gpJeu->getAudio()->playSound(35); break;
    }
    
    if (anglx>4) anglx=4;
    if (angly>4) angly=4;
    if (anglx<-4) anglx=-4;
    if (angly<-4) angly=-4;
    
    gpJeu->ajouteSnipe(type, origx, origy, anglx, angly);
}

void Ennemi::tire() {
    if(!distance()) return;
    
    double anglx = 0;
    double angly = 0;
    int origx = x + (w/2);
    int origy = y + (h/2);
    
    int type = 0;
    switch(id) {
        case 15 : type = 2; gpJeu->getAudio()->playSound(29); break;
        case 16 : case 30 : type = 3; gpJeu->getAudio()->playSound(28); origy+=4; break;
        case 27 : type = 4; gpJeu->getAudio()->playSound(35); break;
        default : type = 2; break;
    }
    
    if (direction==E) anglx=4;
    if (direction==S) angly=4;
    if (direction==O) anglx=-4;
    if (direction==N) angly=-4;
    
    gpJeu->ajouteSnipe(type, origx, origy, anglx, angly);
}

int Ennemi::getEnnId() {return id;}

bool Ennemi::isBoss() {
    return (id == 19 || id == 24 || id == 28 || id == 31 || id == 35 || id == 40
    || id == 43 || id == 45 || id == 50);
}

void Ennemi::init() {
    x=xdep; y=ydep; direction=S; anim=0; if (id!=20) special1=0; special2=0;
    vanim=240; vol=0; nage=0; spectre=0; porte=0; typeIA=IA_RIEN; 
    vitesse=1; force=0; recul=16; glace=0; maxglace=60; typeEnn=E_SIMPLE;
    for (int k = 0; k < 16; k++) effets[k]=2;
    for (int k = 0; k < 3; k++) objets[k]=0;
    switch (id) {
        case 1 :
            viemax=3; w=16; h=26; animmax=1; typeIA=IA_FONCE; porte=160; force = 2;
            objets[0]=I_BOMBE;objets[1]=I_FLECHE;objets[2]=I_RUBIS_ROUGE;
            gpZoneBase->getZone()->w=16; gpZoneBase->getZone()->h=16;
            gpZoneBase->getZone()->x=0; gpZoneBase->getZone()->y=10; 
            zonesAttaques = new ZoneRect(0,0,w,h);
            zonesVulnerables = new ZoneRect(0,0,w,h);
            break;
        case 2 :
            viemax=12; w=21; h=26; animmax=1; typeIA=IA_FONCE; porte=300; force = 8;
            objets[0]=I_RUBIS_BLEU;objets[1]=I_RUBIS_ROUGE;objets[2]=I_MAGIE_PEU;
            gpZoneBase->getZone()->w=16; gpZoneBase->getZone()->h=16;
            gpZoneBase->getZone()->x=2; gpZoneBase->getZone()->y=10; 
            zonesAttaques = new ZoneRect(0,0,w,h);
            zonesVulnerables = new ZoneRect(0,0,w,h);
            recul = 8; maxglace=30;
            break;
        case 3 :
            viemax=2; w=28; h=17; animmax=2; typeIA=IA_FONCE; porte=200; force = 1;
            objets[0]=I_PETIT_COEUR;objets[1]=I_RUBIS_VERT;objets[2]=I_RUBIS_BLEU;
            gpZoneBase->getZone()->w=28; gpZoneBase->getZone()->h=16;
            gpZoneBase->getZone()->x=0; gpZoneBase->getZone()->y=1; 
            zonesAttaques = new ZoneRect(0,0,w,h);
            zonesVulnerables = new ZoneRect(0,0,w,h);
            typeEnn=E_MONO_DIR;
            break;
        case 4 :
            viemax=1; w=16; h=17; animmax=2; typeIA=IA_FONCE; porte=100; force = 1;
            objets[0]=I_PETIT_COEUR;objets[1]=I_RUBIS_VERT;objets[2]=I_RUBIS_BLEU;
            gpZoneBase->getZone()->w=16; gpZoneBase->getZone()->h=16;
            gpZoneBase->getZone()->x=0; gpZoneBase->getZone()->y=1; 
            zonesAttaques = new ZoneRect(0,0,w,h);
            zonesVulnerables = new ZoneRect(0,0,w,h);
            typeEnn=E_MONO_DIR;
            break;
        case 5 :
            viemax=3; w=16; h=26; animmax=1; typeIA=IA_FONCE; porte=200; force = 2;
            objets[0]=I_FLECHE;objets[1]=I_RUBIS_BLEU;objets[2]=I_PETIT_COEUR;
            gpZoneBase->getZone()->w=16; gpZoneBase->getZone()->h=16;
            gpZoneBase->getZone()->x=0; gpZoneBase->getZone()->y=10; 
            zonesAttaques = new ZoneRect(0,0,w,h);
            zonesAttaques->ajout(new ZoneRect(0,15,5,13));
            zonesVulnerables = new ZoneRect(0,0,w,h); arme = true;
            break;
        case 6 :
            viemax=5; w=16; h=37; animmax=9; typeIA=IA_ATTEND; porte=30; force = 2;
            objets[0]=I_FLECHE;objets[1]=I_RUBIS_BLEU;objets[2]=I_PETIT_COEUR;
            gpZoneBase->getZone()->w=16; gpZoneBase->getZone()->h=16;
            gpZoneBase->getZone()->x=0; gpZoneBase->getZone()->y=21; 
            zonesAttaques = new ZoneRect(0,5,w,32);
            zonesVulnerables = new ZoneRect(0,5,w,32); recul = 8; vanim = 30; typeEnn=E_MONO_DIR;
            for (int k = 0; k < 16; k++) effets[k]=1;
            break;
        case 7 :
            viemax=1; w=16; h=16; animmax=1; typeIA=IA_FONCE; porte=100; force = 1;
            objets[0]=I_PETIT_COEUR;objets[1]=I_RUBIS_VERT;objets[2]=I_RUBIS_BLEU;
            gpZoneBase->getZone()->w=16; gpZoneBase->getZone()->h=16;
            gpZoneBase->getZone()->x=0; gpZoneBase->getZone()->y=1; 
            zonesAttaques = new ZoneRect(0,0,w,h);
            zonesVulnerables = new ZoneRect(0,0,w,h);
            break;
        case 8 :
            viemax=4; w=16; h=24; animmax=1; typeIA=IA_FONCE; porte=160; force = 2;
            objets[0]=I_RUBIS_BLEU;objets[1]=I_RUBIS_BLEU;objets[2]=I_RUBIS_ROUGE;
            gpZoneBase->getZone()->w=16; gpZoneBase->getZone()->h=16;
            gpZoneBase->getZone()->x=0; gpZoneBase->getZone()->y=8; 
            zonesAttaques = new ZoneRect(0,0,w,h);
            zonesVulnerables = new ZoneRect(0,0,w,h);
            maxglace=100;
            break;
        case 9 :
            viemax=3; w=16; h=24; animmax=1; typeIA=IA_FONCE; porte=160; force = 2;
            objets[0]=I_FLECHE;objets[1]=I_PETIT_COEUR;objets[2]=I_RUBIS_ROUGE;
            gpZoneBase->getZone()->w=16; gpZoneBase->getZone()->h=16;
            gpZoneBase->getZone()->x=0; gpZoneBase->getZone()->y=8; 
            zonesAttaques = new ZoneRect(0,0,w,h);
            zonesVulnerables = new ZoneRect(0,0,w,h);
            maxglace=100;
            break;
        case 10 :
            viemax=1; w=16; h=16; animmax=1; typeIA=IA_SNIPE; porte=200; force = 2;
            objets[0]=I_FLECHE;objets[1]=I_PETIT_COEUR;objets[2]=I_RUBIS_ROUGE;
            gpZoneBase->getZone()->w=16; gpZoneBase->getZone()->h=16;
            gpZoneBase->getZone()->x=0; gpZoneBase->getZone()->y=0; 
            zonesAttaques = new ZoneRect(0,0,w,h);
            zonesVulnerables = new ZoneRect(0,0,w,h);
            maxglace=100; recul=0; vanim=1240; direction=N; typeEnn=E_MONO_DIR;
            break;
        case 11 :
            viemax=1; w=16; h=21; animmax=2; typeIA=IA_ATTEND; porte=60; force = 2;
            objets[0]=I_BOMBE;objets[1]=I_RUBIS_BLEU;objets[2]=I_PETIT_COEUR;
            gpZoneBase->getZone()->w=16; gpZoneBase->getZone()->h=16;
            gpZoneBase->getZone()->x=0; gpZoneBase->getZone()->y=0; 
            zonesAttaques = new ZoneRect(0,0,w,16);
            zonesVulnerables = new ZoneRect(0,0,w,16); recul = 8; typeEnn=E_MONO_DIR;
            for (int k = 0; k < 16; k++) effets[k]=0;
            break;
        case 12 :
            viemax=10; w=17; h=17; animmax=2; typeIA=IA_FONCE; porte=150; force = 3;
            objets[0]=I_RUBIS_BLEU;objets[1]=I_MAGIE_PEU;objets[2]=I_PETIT_COEUR;
            gpZoneBase->getZone()->w=17; gpZoneBase->getZone()->h=17;
            gpZoneBase->getZone()->x=0; gpZoneBase->getZone()->y=0; 
            zonesAttaques = new ZoneRect(0,0,w,h);
            zonesVulnerables = new ZoneRect(0,0,w,h);
            maxglace=30; typeEnn=E_PROFIL; spectre=1; direction=E;
            for (int k = 0; k < 16; k++) effets[k]=1;
            if (gpJeu->getJoueur()->getEpee()==5) {effets[COUP_EPEE]=2; effets[EPEE_SPIN]=2;}
            if (gpJeu->getJoueur()->hasObjet(O_ARC)==5) effets[FLECHE]=2; 
            break;
        case 13 :
            viemax=5; w=32; h=15; animmax=1; typeIA=IA_ATTEND; porte=100; force = 2;
            objets[0]=I_RUBIS_VERT;objets[1]=I_RUBIS_BLEU;objets[2]=I_PETIT_COEUR;
            gpZoneBase->getZone()->w=18; gpZoneBase->getZone()->h=15;
            gpZoneBase->getZone()->x=7; gpZoneBase->getZone()->y=0; 
            zonesAttaques = new ZoneRect(7,0,18,15);
            zonesVulnerables = new ZoneRect(7,0,18,15);
            maxglace=100; direction=N; typeEnn=E_MONO_DIR; spectre=1; vol=1; vanim=150;
            break;
        case 14 :
            viemax=5; w=41; h=16; animmax=1; typeIA=IA_FONCE; porte=160; force = 4;
            objets[0]=I_BOMBE;objets[1]=I_FLECHE;objets[2]=I_RUBIS_ROUGE;
            gpZoneBase->getZone()->w=41; gpZoneBase->getZone()->h=16;
            gpZoneBase->getZone()->x=0; gpZoneBase->getZone()->y=0; 
            zonesAttaques = new ZoneRect(0,0,w,h);
            zonesVulnerables = new ZoneRect(0,0,w,h);
            maxglace=100; direction=N; typeEnn=E_MONO_DIR;
            break;
        case 15 :
            viemax=10; w=24; h=31; animmax=20; typeIA=IA_FONCE; porte=200; force = 8;
            objets[0]=I_RUBIS_BLEU;objets[1]=I_FLECHE;objets[2]=I_PETIT_COEUR;
            gpZoneBase->getZone()->w=16; gpZoneBase->getZone()->h=16;
            gpZoneBase->getZone()->x=4; gpZoneBase->getZone()->y=15; 
            zonesAttaques = new ZoneRect(0,0,w,h);
            zonesVulnerables = new ZoneRect(0,0,w,h); vanim=120;
            break;
        case 16 :
            viemax=6; w=16; h=24; animmax=9; typeIA=IA_SNIPE; porte=200; force = 5;
            objets[0]=I_FLECHE;objets[1]=I_PETIT_COEUR;objets[2]=I_RUBIS_ROUGE;
            gpZoneBase->getZone()->w=16; gpZoneBase->getZone()->h=16;
            gpZoneBase->getZone()->x=0; gpZoneBase->getZone()->y=8; 
            zonesAttaques = new ZoneRect(0,0,w,h);
            zonesVulnerables = new ZoneRect(0,0,w,h);
            maxglace=40; recul=0;
            for (int k = 0; k < 16; k++) effets[k]=1;
            effets[ENNEMI]=2;
            break;
        case 17 :
            viemax=1; w=16; h=16; animmax=0; typeIA=IA_HAUT_BAS; porte=0; force = 1;
            objets[0]=I_RIEN;objets[1]=I_RIEN;objets[2]=I_RIEN;
            gpZoneBase->getZone()->w=16; gpZoneBase->getZone()->h=16;
            gpZoneBase->getZone()->x=0; gpZoneBase->getZone()->y=0; 
            zonesAttaques = new ZoneRect(0,0,w,h);
            zonesVulnerables = new ZoneRect(0,0,w,h);
            maxglace=0; recul=0; vol=1; vitesse=2; typeEnn=E_MONO_DIR;
            for (int k = 0; k < 16; k++) effets[k]=0;
            break;
        case 18 :
            viemax=1; w=16; h=14; animmax=1; typeIA=IA_ATTEND; porte=70; force = 1;
            objets[0]=I_PETIT_COEUR;objets[1]=I_RUBIS_VERT;objets[2]=I_RUBIS_BLEU;
            gpZoneBase->getZone()->w=16; gpZoneBase->getZone()->h=14;
            gpZoneBase->getZone()->x=0; gpZoneBase->getZone()->y=0; 
            zonesAttaques = new ZoneRect(0,0,w,h);
            zonesVulnerables = new ZoneRect(0,0,w,h);
            maxglace=60; direction=N; typeEnn=E_MONO_DIR; spectre=1; vol=1;
            break;
        case 19 :
            viemax=8; w=64; h=32; animmax=2; typeIA=IA_FONCE; porte=300; force = 2;
            objets[0]=I_RIEN;objets[1]=I_RIEN;objets[2]=I_RIEN;
            gpZoneBase->getZone()->w=w; gpZoneBase->getZone()->h=h;
            gpZoneBase->getZone()->x=0; gpZoneBase->getZone()->y=0; 
            zonesAttaques = new ZoneRect(0,0,w,h);
            zonesVulnerables = new ZoneRect(0,0,w,h);
            typeEnn=E_MONO_DIR; recul=4; spectre=1;
            break;
        case 20 :
            viemax=1; w=16; h=16; animmax=0; typeIA=IA_RIEN; porte=200; force = 1;
            objets[0]=I_RIEN;objets[1]=I_RIEN;objets[2]=I_RIEN;
            gpZoneBase->getZone()->w=w; gpZoneBase->getZone()->h=h;
            gpZoneBase->getZone()->x=0; gpZoneBase->getZone()->y=0; 
            zonesAttaques = new ZoneRect(0,0,w,h);
            zonesVulnerables = new ZoneRect(0,0,w,h);
            typeEnn=E_MONO_DIR; recul=0; maxglace=0;
            break;
        case 21 :
            viemax=2; w=16; h=15; animmax=1; typeIA=IA_FONCE; porte=200; force = 2;
            objets[0]=I_FLECHE;objets[1]=I_RUBIS_BLEU;objets[2]=I_PETIT_COEUR;
            gpZoneBase->getZone()->w=16; gpZoneBase->getZone()->h=15;
            gpZoneBase->getZone()->x=0; gpZoneBase->getZone()->y=0; 
            zonesAttaques = new ZoneRect(0,0,w,h);
            zonesVulnerables = new ZoneRect(0,0,w,h);
            maxglace=30; typeEnn=E_PROFIL; direction=E; nage=1;
            break;
        case 22 :
            viemax=2; w=16; h=23; animmax=1; typeIA=IA_FONCE; porte=200; force = 2;
            objets[0]=I_FLECHE;objets[1]=I_RUBIS_BLEU;objets[2]=I_PETIT_COEUR;
            gpZoneBase->getZone()->w=16; gpZoneBase->getZone()->h=23;
            gpZoneBase->getZone()->x=0; gpZoneBase->getZone()->y=0; 
            zonesAttaques = new ZoneRect(0,0,w,h);
            zonesVulnerables = new ZoneRect(0,0,w,h);
            maxglace=30; direction=N; typeEnn=E_MONO_DIR; spectre=1;
            break;
        case 23 :
            viemax=2; w=26; h=17; animmax=1; typeIA=IA_FONCE; porte=200; force = 2;
            objets[0]=I_FLECHE;objets[1]=I_RUBIS_BLEU;objets[2]=I_PETIT_COEUR;
            gpZoneBase->getZone()->w=26; gpZoneBase->getZone()->h=16;
            gpZoneBase->getZone()->x=0; gpZoneBase->getZone()->y=1; 
            zonesAttaques = new ZoneRect(0,0,w,h);
            zonesVulnerables = new ZoneRect(0,0,w,h);
            maxglace=30; direction=N; typeEnn=E_MONO_DIR;
            break;
        case 24 :
            viemax=16; w=32; h=53; animmax=1; typeIA=IA_FONCE; porte=300; force = 2;
            objets[0]=I_RIEN;objets[1]=I_RIEN;objets[2]=I_RIEN;
            gpZoneBase->getZone()->w=w; gpZoneBase->getZone()->h=h;
            gpZoneBase->getZone()->x=0; gpZoneBase->getZone()->y=0; 
            zonesAttaques = new ZoneRect(0,0,w,h);
            zonesVulnerables = new ZoneRect(0,0,w,h);
            typeEnn=E_MONO_DIR; recul=4; spectre=1;
            break;
        case 25 :
            viemax=7; w=28; h=30; animmax=1; typeIA=IA_FONCE; porte=160; force = 2;
            objets[0]=I_RUBIS_BLEU;objets[1]=I_RUBIS_ROUGE;objets[2]=I_PETIT_COEUR;
            gpZoneBase->getZone()->w=16; gpZoneBase->getZone()->h=16;
            gpZoneBase->getZone()->x=6; gpZoneBase->getZone()->y=14; 
            zonesAttaques = new ZoneRect(0,0,w,h);
            zonesVulnerables = new ZoneRect(0,0,w,h);
            recul=8; maxglace=100;
            break;
        case 26 :
            viemax=2; w=14; h=16; animmax=1; typeIA=IA_ATTEND; porte=100; force = 2;
            objets[0]=I_RUBIS_VERT;objets[1]=I_MAGIE_PEU;objets[2]=I_PETIT_COEUR;
            gpZoneBase->getZone()->w=w; gpZoneBase->getZone()->h=h;
            gpZoneBase->getZone()->x=0; gpZoneBase->getZone()->y=0; 
            zonesAttaques = new ZoneRect(0,0,w,h);
            zonesVulnerables = new ZoneRect(0,0,w,h);
            maxglace=60; direction=N; typeEnn=E_MONO_DIR; spectre=1; vol=1;
            break;
        case 27 :
            viemax=3; w=16; h=17; animmax=10; typeIA=IA_FONCE; porte=300; force = 2;
            objets[0]=I_FLECHE;objets[1]=I_MAGIE_PEU;objets[2]=I_BOMBE;
            gpZoneBase->getZone()->w=16; gpZoneBase->getZone()->h=16;
            gpZoneBase->getZone()->x=0; gpZoneBase->getZone()->y=1; 
            zonesAttaques = new ZoneRect(0,0,w,h);
            zonesVulnerables = new ZoneRect(0,0,w,h);
            break;
        case 28 :
            viemax=16; w=32; h=38; animmax=1; typeIA=IA_FONCE; porte=300; force = 2;
            objets[0]=I_RIEN;objets[1]=I_RIEN;objets[2]=I_RIEN;
            gpZoneBase->getZone()->w=16; gpZoneBase->getZone()->h=16;
            gpZoneBase->getZone()->x=8; gpZoneBase->getZone()->y=22; 
            zonesAttaques = new ZoneRect(0,0,w,h);
            zonesVulnerables = new ZoneRect(0,0,w,h);
            typeEnn=E_MONO_DIR; recul=4;
            break;
        case 29 :
            viemax=1; w=16; h=26; animmax=1; typeIA=IA_FONCE; porte=300; force = 4;
            objets[0]=I_RUBIS_BLEU;objets[1]=I_MAGIE_PEU;objets[2]=I_MAGIE_BCP;
            gpZoneBase->getZone()->w=16; gpZoneBase->getZone()->h=16;
            gpZoneBase->getZone()->x=0; gpZoneBase->getZone()->y=10; 
            zonesAttaques = new ZoneRect(0,0,w,h);
            zonesVulnerables = new ZoneRect(0,0,w,h);
            for (int k = 0; k < 16; k++) effets[k]=1; effets[FEU]=2;
            maxglace=100;
            break;
        case 30 :
            viemax=3; w=16; h=24; animmax=9; typeIA=IA_SNIPE; porte=200; force = 4;
            objets[0]=I_RUBIS_BLEU;objets[1]=I_PETIT_COEUR;objets[2]=I_MAGIE_PEU;
            gpZoneBase->getZone()->w=16; gpZoneBase->getZone()->h=16;
            gpZoneBase->getZone()->x=0; gpZoneBase->getZone()->y=8; 
            zonesAttaques = new ZoneRect(0,0,w,h);
            zonesVulnerables = new ZoneRect(0,0,w,h);
            maxglace=40; recul=0;
            for (int k = 0; k < 16; k++) effets[k]=1;
            effets[ENNEMI]=2;
            break;
        case 31 :
            viemax=16; w=32; h=32; animmax=9; typeIA=IA_SNIPE; porte=300; force = 4;
            objets[0]=I_RIEN;objets[1]=I_RIEN;objets[2]=I_RIEN;
            gpZoneBase->getZone()->w=w; gpZoneBase->getZone()->h=h;
            gpZoneBase->getZone()->x=0; gpZoneBase->getZone()->y=0; 
            zonesAttaques = new ZoneRect(0,0,w,h);
            zonesVulnerables = new ZoneRect(0,0,w,h);
            maxglace=40; recul=0; typeEnn=E_MONO_DIR; vanim=120;
            for (int k = 0; k < 16; k++) effets[k]=1; effets[ENNEMI]=2; 
            break;
        case 32 :
            viemax=5; w=16; h=23; animmax=2; typeIA=IA_FONCE; porte=200; force = 4;
            objets[0]=I_RUBIS_VERT;objets[1]=I_RUBIS_BLEU;objets[2]=I_RUBIS_ROUGE;
            gpZoneBase->getZone()->w=16; gpZoneBase->getZone()->h=16;
            gpZoneBase->getZone()->x=0; gpZoneBase->getZone()->y=7; 
            zonesAttaques = new ZoneRect(0,0,w,h);
            zonesVulnerables = new ZoneRect(0,0,w,h);
            typeEnn=E_MONO_DIR; maxglace=100;
            break;
        case 33 :
            viemax=10; w=22; h=25; animmax=1; typeIA=IA_FONCE; porte=300; force = 6;
            objets[0]=I_RUBIS_BLEU;objets[1]=I_MAGIE_PEU;objets[2]=I_FLECHE;
            gpZoneBase->getZone()->w=16; gpZoneBase->getZone()->h=16;
            gpZoneBase->getZone()->x=3; gpZoneBase->getZone()->y=9; 
            zonesAttaques = new ZoneRect(0,0,w,h);
            zonesVulnerables = new ZoneRect(0,0,w,h);
            maxglace=100;
            break;
        case 34 :
            viemax=15; w=24; h=25; animmax=1; typeIA=IA_FONCE; porte=300; force = 8;
            objets[0]=I_RUBIS_ROUGE;objets[1]=I_RUBIS_ROUGE;objets[2]=I_RUBIS_ROUGE;
            gpZoneBase->getZone()->w=16; gpZoneBase->getZone()->h=16;
            gpZoneBase->getZone()->x=4; gpZoneBase->getZone()->y=9; 
            zonesAttaques = new ZoneRect(0,0,w,h);
            zonesVulnerables = new ZoneRect(0,0,w,h);
            recul=8; maxglace=100;
            break;
        case 35 :
            viemax=24; w=32; h=32; animmax=2; typeIA=IA_FONCE; porte=300; force = 8;
            objets[0]=I_RIEN;objets[1]=I_RIEN;objets[2]=I_RIEN;
            gpZoneBase->getZone()->w=w; gpZoneBase->getZone()->h=h;
            gpZoneBase->getZone()->x=0; gpZoneBase->getZone()->y=0; 
            zonesAttaques = new ZoneRect(0,0,w,h);
            zonesVulnerables = new ZoneRect(0,0,w,h);
            typeEnn=E_MONO_DIR; recul=4; spectre=1; maxglace=40;
            break;
        case 36 :
            viemax=1; w=16; h=16; animmax=9; typeIA=IA_STATUE; porte=300; force = 0;
            objets[0]=I_RIEN;objets[1]=I_RIEN;objets[2]=I_RIEN;
            gpZoneBase->getZone()->w=16; gpZoneBase->getZone()->h=16;
            gpZoneBase->getZone()->x=0; gpZoneBase->getZone()->y=0; 
            zonesAttaques = new ZoneRect(0,0,w,h);
            zonesVulnerables = new ZoneRect(0,0,w,h);
            maxglace=0; recul=0; vanim=120; direction=N; typeEnn=E_MONO_DIR;
            for (int k = 0; k < 16; k++) effets[k]=0;
            break;
        case 37 :
            viemax=9; w=16; h=26; animmax=1; typeIA=IA_FONCE; porte=160; force = 5;
            objets[0]=I_BOMBE;objets[1]=I_FLECHE;objets[2]=I_RUBIS_ROUGE;
            gpZoneBase->getZone()->w=16; gpZoneBase->getZone()->h=16;
            gpZoneBase->getZone()->x=0; gpZoneBase->getZone()->y=10; 
            zonesAttaques = new ZoneRect(0,0,w,h);
            zonesVulnerables = new ZoneRect(0,0,w,h);
            break;
        case 38 :
            viemax=15; w=22; h=25; animmax=1; typeIA=IA_FONCE; porte=300; force = 8;
            objets[0]=I_RUBIS_ROUGE;objets[1]=I_MAGIE_PEU;objets[2]=I_MAGIE_BCP;
            gpZoneBase->getZone()->w=16; gpZoneBase->getZone()->h=16;
            gpZoneBase->getZone()->x=3; gpZoneBase->getZone()->y=9; 
            zonesAttaques = new ZoneRect(0,0,w,h);
            zonesVulnerables = new ZoneRect(0,0,w,h);
            maxglace=100;
            break;
        case 39 :
            viemax=20; w=24; h=25; animmax=1; typeIA=IA_FONCE; porte=300; force = 10;
            objets[0]=I_RUBIS_ROUGE;objets[1]=I_RUBIS_ROUGE;objets[2]=I_RUBIS_ROUGE;
            gpZoneBase->getZone()->w=16; gpZoneBase->getZone()->h=16;
            gpZoneBase->getZone()->x=4; gpZoneBase->getZone()->y=9; 
            zonesAttaques = new ZoneRect(0,0,w,h);
            zonesVulnerables = new ZoneRect(0,0,w,h);
            recul=8; maxglace=100;
            break;
        case 40 :
            viemax=24; w=54; h=42; animmax=2; typeIA=IA_FONCE; porte=300; force = 10;
            objets[0]=I_RIEN;objets[1]=I_RIEN;objets[2]=I_RIEN;
            gpZoneBase->getZone()->w=w; gpZoneBase->getZone()->h=h;
            gpZoneBase->getZone()->x=0; gpZoneBase->getZone()->y=0; 
            zonesAttaques = new ZoneRect(0,0,w,h);
            zonesVulnerables = new ZoneRect(0,0,w,h);
            typeEnn=E_MONO_DIR; recul=4; spectre=1; maxglace=100;
            break;
        case 41 :
            viemax=9; w=16; h=24; animmax=1; typeIA=IA_FONCE; porte=300; force = 6;
            objets[0]=I_RUBIS_BLEU;objets[1]=I_MAGIE_PEU;objets[2]=I_MAGIE_BCP;
            gpZoneBase->getZone()->w=16; gpZoneBase->getZone()->h=16;
            gpZoneBase->getZone()->x=0; gpZoneBase->getZone()->y=8; 
            zonesAttaques = new ZoneRect(0,0,w,h);
            zonesVulnerables = new ZoneRect(0,0,w,h);
            maxglace=30;
            break;
        case 42 :
            viemax=3; w=26; h=19; animmax=1; typeIA=IA_FONCE; porte=300; force = 10;
            objets[0]=I_MAGIE_PEU;objets[1]=I_MAGIE_PEU;objets[2]=I_MAGIE_BCP;
            gpZoneBase->getZone()->w=26; gpZoneBase->getZone()->h=19;
            gpZoneBase->getZone()->x=0; gpZoneBase->getZone()->y=0; 
            zonesAttaques = new ZoneRect(0,0,w,h);
            zonesVulnerables = new ZoneRect(0,0,w,h);
            maxglace=10; direction=N; typeEnn=E_MONO_DIR;
            for (int k = 0; k < 16; k++) effets[k]=1; effets[FEU]=2;
            break;
        case 43 :
            viemax=24; w=32; h=32; animmax=2; typeIA=IA_FONCE; porte=300; force = 10;
            objets[0]=I_RIEN;objets[1]=I_RIEN;objets[2]=I_RIEN;
            gpZoneBase->getZone()->w=w; gpZoneBase->getZone()->h=h;
            gpZoneBase->getZone()->x=0; gpZoneBase->getZone()->y=0; 
            zonesAttaques = new ZoneRect(0,0,w,h);
            zonesVulnerables = new ZoneRect(0,0,w,h);
            recul=4; spectre=1; maxglace=30;
            break;
        case 44 :
            viemax=9; w=22; h=26; animmax=1; typeIA=IA_FONCE; porte=300; force = 8;
            objets[0]=I_RUBIS_BLEU;objets[1]=I_FLECHE;objets[2]=I_PETIT_COEUR;
            gpZoneBase->getZone()->w=16; gpZoneBase->getZone()->h=16;
            gpZoneBase->getZone()->x=3; gpZoneBase->getZone()->y=10; 
            zonesAttaques = new ZoneRect(0,0,w,h);
            zonesAttaques->ajout(new ZoneRect(0,20,7,15));
            zonesVulnerables = new ZoneRect(0,0,w,h); arme = true;
            break;
        case 45 :
            viemax=30; w=66; h=76; animmax=3; typeIA=IA_FONCE; porte=300; force = 10;
            objets[0]=I_RIEN;objets[1]=I_RIEN;objets[2]=I_RIEN;
            gpZoneBase->getZone()->w=w; gpZoneBase->getZone()->h=h;
            gpZoneBase->getZone()->x=0; gpZoneBase->getZone()->y=0; 
            zonesAttaques = new ZoneRect(0,0,66,64);
            zonesAttaques->ajout(new ZoneRect(0,67,16,9));
            zonesAttaques->ajout(new ZoneRect(50,59,16,9));
            zonesVulnerables = new ZoneRect(27,39,12,17);
            typeEnn=E_MONO_DIR; recul=4; spectre=1; maxglace=40;
            break;
        case 46 :
            viemax=10; w=22; h=27; animmax=1; typeIA=IA_FONCE; porte=300; force = 9;
            objets[0]=I_RUBIS_VERT;objets[1]=I_MAGIE_PEU;objets[2]=I_MAGIE_BCP;
            gpZoneBase->getZone()->w=16; gpZoneBase->getZone()->h=16;
            gpZoneBase->getZone()->x=3; gpZoneBase->getZone()->y=11; 
            zonesAttaques = new ZoneRect(0,0,w,h);
            zonesAttaques->ajout(new ZoneRect(0,21,5,16));
            zonesVulnerables = new ZoneRect(0,0,w,h); arme = true;
            break;
        case 47 :
            viemax=15; w=22; h=27; animmax=1; typeIA=IA_FONCE; porte=300; force = 14;
            objets[0]=I_RUBIS_BLEU;objets[1]=I_FLECHE;objets[2]=I_BOMBE;
            gpZoneBase->getZone()->w=16; gpZoneBase->getZone()->h=16;
            gpZoneBase->getZone()->x=3; gpZoneBase->getZone()->y=11; 
            zonesAttaques = new ZoneRect(0,0,w,h);
            zonesAttaques->ajout(new ZoneRect(0,21,5,16));
            zonesVulnerables = new ZoneRect(0,0,w,h); arme = true;
            break;
        case 48 :
            viemax=20; w=22; h=27; animmax=1; typeIA=IA_FONCE; porte=300; force = 19;
            objets[0]=I_RUBIS_ROUGE;objets[1]=I_PETIT_COEUR;objets[2]=I_PETIT_COEUR;
            gpZoneBase->getZone()->w=16; gpZoneBase->getZone()->h=16;
            gpZoneBase->getZone()->x=3; gpZoneBase->getZone()->y=11; 
            zonesAttaques = new ZoneRect(0,0,w,h);
            zonesAttaques->ajout(new ZoneRect(0,21,5,17));
            zonesVulnerables = new ZoneRect(0,0,w,h); arme = true;
            break;
        case 49 :
            viemax=1; w=16; h=16; animmax=9; typeIA=IA_STATUE; porte=300; force = 0;
            objets[0]=I_RIEN;objets[1]=I_RIEN;objets[2]=I_RIEN;
            gpZoneBase->getZone()->w=16; gpZoneBase->getZone()->h=16;
            gpZoneBase->getZone()->x=0; gpZoneBase->getZone()->y=0; 
            zonesAttaques = new ZoneRect(0,0,w,h);
            zonesVulnerables = new ZoneRect(0,0,w,h);
            maxglace=0; recul=0; vanim=120; direction=N; typeEnn=E_MONO_DIR;
            for (int k = 0; k < 16; k++) effets[k]=0;
            break;
        case 50 :
            viemax=100; w=52; h=36; animmax=1; typeIA=IA_FONCE; porte=300; force = 20;
            objets[0]=I_RIEN;objets[1]=I_RIEN;objets[2]=I_RIEN;
            gpZoneBase->getZone()->w=w; gpZoneBase->getZone()->h=h;
            gpZoneBase->getZone()->x=0; gpZoneBase->getZone()->y=0; 
            zonesAttaques = new ZoneRect(0,0,w,h);
            zonesVulnerables = new ZoneRect(0,0,w,h);
            typeEnn=E_SIMPLE; recul=4; spectre=1; maxglace=0;
            for (int k = 0; k < 16; k++) effets[k]=1;
            if (gpJeu->getJoueur()->getEpee()==5) {effets[COUP_EPEE]=2; effets[EPEE_SPIN]=2;}
            if (gpJeu->getJoueur()->hasObjet(O_ARC)==5) effets[FLECHE]=2; 
            break;
        case 51 :
            viemax=1; w=24; h=22; animmax=40; typeIA=IA_SPECIAL; porte=300; force = 0;
            objets[0]=I_RIEN;objets[1]=I_RIEN;objets[2]=I_RIEN;
            gpZoneBase->getZone()->w=w; gpZoneBase->getZone()->h=h;
            gpZoneBase->getZone()->x=0; gpZoneBase->getZone()->y=0; 
            zonesAttaques = new ZoneRect(0,0,w,h);
            zonesVulnerables = new ZoneRect(0,0,w,h);
            typeEnn=E_MONO_DIR; recul=0; spectre=1; maxglace=0; vanim=120;
            for (int k = 0; k < 16; k++) effets[k]=0;
            break;
        case 52 :
            viemax=100; w=16; h=17; animmax=1; typeIA=IA_RANDOM; porte=1000; force = 1;
            objets[0]=I_RIEN;objets[1]=I_RIEN;objets[2]=I_RIEN;
            gpZoneBase->getZone()->w=16; gpZoneBase->getZone()->h=16;
            gpZoneBase->getZone()->x=0; gpZoneBase->getZone()->y=1; 
            zonesAttaques = new ZoneRect(0,0,w,h);
            zonesVulnerables = new ZoneRect(0,0,w,h);
            maxglace=0; typeEnn=E_PROFIL;
            break;
    }
    vie=viemax;
    lastAnimTime = SDL_GetTicks();
}

/*

    Zelda Return of the Hylian

    Copyright (C) 2005-2008  Vincent Jouillat

    Please send bugreports with examples or suggestions to www.zeldaroth.fr

*/

#include "Joueur.h"
#include "Monde.h"
#include "Menu.h"
#include "Texte.h"
#include "Projectile.h"
#include "Jeu.h"
#include "Keyboard.h"

#include <cstdlib>
#include <iostream>

Jeu::Jeu(Audio* audio) : gpAudio(audio), gpGenerique(0), gpKeyboard(0),
zone(21), stop(false), transition(false),
menu(false), text(false), vueHorz(0), vueVert(0) {
    phg[0] = 0;
    phg[1] = 0;
    
    imageObjets = IMG_Load("romfs:/images/objet/objets.png");
    SDL_SetColorKey(imageObjets,SDL_SRCCOLORKEY,SDL_MapRGB(imageObjets->format,0,0,255));
    
    gpProjectile = NULL; gpObjet = NULL; gpSnipe = NULL; gpCaisse = NULL; gpEnnemi = NULL;
    gpPiege = NULL; gpPnj = NULL; gpJoueur = NULL; gpMonde = NULL; gpMenu = NULL;
    gpStatut = NULL;
    gpTexte = new Texte(this);
}

Jeu::~Jeu() {
    delete gpMonde;
    delete gpJoueur;
    delete gpMenu;
    delete gpTexte;
    delete gpStatut;
    delete gpProjectile;
    delete gpObjet;
    delete gpSnipe;
    delete gpCaisse;
    delete gpEnnemi;
    delete gpPiege;
    delete gpPnj;
}

void Jeu::init(int save) {
    zone=21;
    gpProjectile = new Projectile(this, 0, N, 0, 0, 0);
    gpObjet = new Objet(this, 0, 0, 0, 0);
    gpSnipe = new Snipe(this, 0, 0, 0, 0, 0);
    gpCaisse = new Caisse(this, 0, 0, 0);
    gpEnnemi = new Ennemi(this, 0, 0, 0, true);
    gpPiege = new Ennemi(this, 0, 0, 0, true);
    gpPnj = new Pnj(this, 0, 0, 0, 0);
    gpJoueur = new Joueur(this, save);
    gpMonde = new Monde(this);
    gpMenu = new Menu(this);
    //gpTexte = new Texte(this);
    gpStatut = new Statut(this);
    gpAudio->playMusic(zone);
    if (isDonjon()) ecrit(206+zone-12);
}

void Jeu::setKeyboard(Keyboard* kb) {
    gpKeyboard = kb;
}

void Jeu::setGenerique(Generique* gen) {
    gpGenerique = gen;
}

void Jeu::videListes() {
    delete gpProjectile;
    gpProjectile = new Projectile(this, 0, N, 0, 0, 0);
    delete gpObjet;
    gpObjet = new Objet(this, 0, 0, 0, 0);
    delete gpSnipe;
    gpSnipe = new Snipe(this, 0, 0, 0, 0, 0);
    delete gpCaisse;
    gpCaisse = new Caisse(this, 0, 0, 0);
    delete gpEnnemi;
    gpEnnemi = new Ennemi(this, 0, 0, 0, true);
    delete gpPiege;
    gpPiege = new Ennemi(this, 0, 0, 0, true);
    delete gpPnj;
    gpPnj = new Pnj(this, 0, 0, 0, 0);
}

void Jeu::scrolling() {
    phg[0] = gpJoueur->getX() - 320/2 + 8 + vueHorz;
    phg[1] = gpJoueur->getY() - 240/2 + 16 + vueVert;
    if (phg[0] < gpMonde->regionValue(0)) phg[0] = gpMonde->regionValue(0);
    if (phg[1] < gpMonde->regionValue(1)) phg[1] = gpMonde->regionValue(1);
    if (phg[0] > gpMonde->regionValue(2) - 320) phg[0] = gpMonde->regionValue(2) - 320;
    if (phg[1] > gpMonde->regionValue(3) - 240) phg[1] = gpMonde->regionValue(3) - 240;
}

void Jeu::replace() {
    gpEnnemi->revie();
    gpPiege->revie();
    gpPnj->revie();
    gpCaisse->revie();
    gpObjet->revie();
    delete gpSnipe;
    gpSnipe = new Snipe(this, 0, 0, 0, 0, 0);
    delete gpProjectile;
    gpProjectile = new Projectile(this, 0, N, 0, 0, 0);
    gpMonde->replace();
}

int Jeu::nbEnnemis() {
    int t=0;
    Ennemi* gpEnnemi2 = gpEnnemi->getSuivant();
    while (gpEnnemi2 != NULL) {
        if (gpEnnemi2->getVie()>0 && gpEnnemi2->getX()>gpMonde->regionValue(0) 
        && gpEnnemi2->getX()<gpMonde->regionValue(2) && gpEnnemi2->getY()>gpMonde->regionValue(1) 
        && gpEnnemi2->getY()<gpMonde->regionValue(3)) t++;
        gpEnnemi2 = gpEnnemi2->getSuivant();
    }
    return t;
}

void Jeu::draw(SDL_Surface* screen) {
    
    if (menu || gpMenu->getVal()) {
		drawFreeze(screen);
		return;
	}

    if (!transition) scrolling();
    
    gpMonde->drawSol(screen, phg);
    
    gpPiege->drawSpecial(screen);
    
    if (!transition) {
        gpCaisse->draw(screen);
        gpObjet->draw(screen);
        gpPnj->draw(screen);
        gpEnnemi->draw(screen);
        gpSnipe->draw(screen);
        gpProjectile->draw(screen);
    }
    
    gpJoueur->draw(screen);
    gpPiege->draw(screen);
    gpMonde->drawAir(screen, phg);
    
    if (transition) gpMonde->defilTransit();
    
    gpStatut->draw(screen);
    
    if (text && !transition) gpTexte->draw(screen);
}

void Jeu::drawFreeze(SDL_Surface* screen) {
    
    if (!transition) scrolling();
    
    gpMonde->drawSol(screen, phg);
    
    gpPiege->drawSpecial(screen);
    
    if (!transition) {
        gpCaisse->draw(screen);
        gpObjet->draw(screen);
        gpPnj->draw(screen);
//        gpEnnemi->draw(screen);
        gpSnipe->draw(screen);
        gpProjectile->draw(screen);
    }
    
    gpJoueur->draw(screen);
    gpPiege->draw(screen);
    gpMonde->drawAir(screen, phg);
    
    if (transition) gpMonde->defilTransit();
    
    gpStatut->draw(screen);
	SDL_FillRect(screen, NULL, RGBA8(0, 0, 0, 0x80));
}

void Jeu::drawMenu(SDL_Surface* screen) {
	if (menu || gpMenu->getVal()) gpMenu->draw(screen);
}

Joueur* Jeu::getJoueur() {
    return gpJoueur;
}

Monde* Jeu::getMonde() {
    return gpMonde;
}

Audio* Jeu::getAudio() {
    return gpAudio;
}

Texte* Jeu::getTexte() {
    return gpTexte;
}

Statut* Jeu::getStatut() {
    return gpStatut;
}

Generique* Jeu::getGenerique() {
    return gpGenerique;
}

Keyboard* Jeu::getKeyboard() {
    return gpKeyboard;
}

void Jeu::ajouteProjectile(int type, Direction dir, int a, int b, int v) {
    Projectile* proj = new Projectile(this, type, dir, a, b, v);
    if (gpProjectile == NULL) gpProjectile = proj;
    else gpProjectile->ajout(proj);
}

void Jeu::ajouteObjet(int type, int i, int j, int num, int p) {
    Objet* obj = new Objet(this, i, j, type, num, p);
    if (gpObjet == NULL) gpObjet = obj;
    else gpObjet->ajout(obj);
}

void Jeu::ajouteSnipe(int type, double i, double j, double dirx, double diry) {
    Snipe* snp = new Snipe(this, type, i, j, dirx, diry);
    if (gpSnipe == NULL) gpSnipe = snp;
    else gpSnipe->ajout(snp);
}

void Jeu::ajouteCaisse(int type, int i, int j) {
    Caisse* css = new Caisse(this, type, i, j);
    if (gpCaisse == NULL) gpCaisse = css;
    else gpCaisse->ajout(css);
}

void Jeu::ajouteEnnemi(int type, int i, int j, bool p) {
    Ennemi* enn = new Ennemi(this, type, i, j, p);
    if (gpEnnemi == NULL) gpEnnemi = enn;
    else gpEnnemi->ajout(enn);
}

void Jeu::ajoutePiege(int type, int i, int j, bool p) {
    Ennemi* enn = new Ennemi(this, type, i, j, p);
    if (gpPiege == NULL) gpPiege = enn;
    else gpPiege->ajout(enn);
}

void Jeu::ajoutePnj(int type, int i, int j, int m) {
    Pnj* pnj = new Pnj(this, type, i, j, m);
    if (gpPnj == NULL) gpPnj = pnj;
    else gpPnj->ajout(pnj);
}

bool Jeu::isDonjon() {
    return (zone >=12 && zone<=20);
}

bool Jeu::isDehors() {
    return (zone < 12);
}

bool Jeu::isGrotte() {
    return (zone > 21 && zone < 28);
}

void Jeu::poule() {
    Ennemi* gpEnnemi2 = gpEnnemi->getSuivant();
    while (gpEnnemi2 != NULL) {
        if (gpEnnemi2->getEnnId()==52) gpEnnemi2->enerve();
        gpEnnemi2 = gpEnnemi2->getSuivant();
    }
}

void Jeu::lire() {
    int i=0;
    int j=0;
    switch(gpJoueur->getDirection()) {
        case N : i=gpJoueur->getX()+8; j=gpJoueur->getY()+7-2-2; break;
        case S : i=gpJoueur->getX()+8; j=gpJoueur->getY()+24+2; break;
        case O : i=gpJoueur->getX()-1-2; j=gpJoueur->getY()+16; break;
        case E : i=gpJoueur->getX()+16+2; j=gpJoueur->getY()+16; break;
    }
    int id = gpMonde->contenuValue(i, j);
    int val = gpMonde->motifValue(i, j);
    //lit pancarte ou achete
    if (gpMonde->murValue(i, j) && id && gpJoueur->getDirection()==N && 
    (val == 33 || val == 34 || val == 36 || val == 2435 || val == 2437 || val == 2439)) {
        ecrit(id);
        return;
    }
    //ouvre coffre
    if (gpMonde->murValue(i, j) && val==303 && gpJoueur->getDirection()==N) {
        switch (id) {
            case C_RIEN : 
            case C_QUART_COEUR :
                switch(zone) {
                    case 33:
                        gpMonde->setValeur(4*16,5*16,304,1); 
                        gpMonde->setValeur(9*16,5*16,304,1); 
                        gpMonde->setValeur(14*16,5*16,304,1);
                        gpMonde->setContenu(4,5,0);
                        gpMonde->setContenu(9,5,0);
                        gpMonde->setContenu(14,5,0);
                        break;
                    case 34:
                        gpMonde->setValeur(4*16,4*16,304,1); 
                        gpMonde->setValeur(9*16,4*16,304,1); 
                        gpMonde->setValeur(14*16,4*16,304,1);
                        gpMonde->setContenu(4,4,0);
                        gpMonde->setContenu(9,4,0);
                        gpMonde->setContenu(14,4,0);
                        gpMonde->setValeur(4*16,7*16,304,1);
                        gpMonde->setValeur(9*16,7*16,304,1); 
                        gpMonde->setValeur(14*16,7*16,304,1);
                        gpMonde->setContenu(4,7,0);
                        gpMonde->setContenu(9,7,0);
                        gpMonde->setContenu(14,7,0);
                        break;
                    case 39:
                        gpMonde->setValeur(4*16,3*16,304,1); 
                        gpMonde->setValeur(9*16,3*16,304,1); 
                        gpMonde->setValeur(14*16,3*16,304,1);
                        gpMonde->setContenu(4,3,0);
                        gpMonde->setContenu(9,3,0);
                        gpMonde->setContenu(14,3,0);
                        gpMonde->setValeur(4*16,6*16,304,1); 
                        gpMonde->setValeur(9*16,6*16,304,1); 
                        gpMonde->setValeur(14*16,6*16,304,1);
                        gpMonde->setContenu(4,6,0);
                        gpMonde->setContenu(9,6,0);
                        gpMonde->setContenu(14,6,0);
                        gpMonde->setValeur(4*16,9*16,304,1); 
                        gpMonde->setValeur(9*16,9*16,304,1); 
                        gpMonde->setValeur(14*16,9*16,304,1);
                        gpMonde->setContenu(4,9,0);
                        gpMonde->setContenu(9,9,0);
                        gpMonde->setContenu(14,9,0);
                        break;
                    default : return;
                }
                if (id == C_RIEN) {gpPnj->getSuivant()->setMessage(153); ecrit(157);}
                if (id == C_QUART_COEUR) {
                    int idcoeur = zone+2;
                    if (idcoeur==41) idcoeur=37;
                    gpPnj->getSuivant()->setMessage(156);
                    gpJoueur->setCoeur(idcoeur); trouve(3);
                }
                break;
            case C_PAIE : ecrit(id); break;
            default : gpMonde->setValeur(i, j, 304, -1, PLEIN, C_RIEN); trouve(id);
        }
        return;
    }
    //parle pnj
    Pnj* gpPnj2 = gpPnj->getSuivant();
    while (gpPnj2 != NULL) {
        int x1 = gpPnj2->getX();
        int y1 = gpPnj2->getY();
        int w1 = x1+gpPnj2->getZoneBase()->getZone()->x+gpPnj2->getZoneBase()->getZone()->w;
        int h1 = y1+gpPnj2->getZoneBase()->getZone()->y+gpPnj2->getZoneBase()->getZone()->h;
        if (i>=x1 && i<w1 && j>=y1 && j<h1) {
            ecrit(gpPnj2->getMessage());
            return;
        }
        gpPnj2 = gpPnj2->getSuivant();
    }
}

void Jeu::trouve(int id) {
    ecrit(id);
    gpJoueur->setTrouve(id);
    gpAudio->playSound(19,2);
}

bool Jeu::finTexte(int id) {
    if (gpJoueur) gpJoueur->setTrouve(0);
    switch (id) {
        case 1 :
            gpJoueur->setBouclier(1);
            break;
        case 2 :
            gpJoueur->setEpee(1);
            break;
        case 30 :
            if (zone == 8) gpJoueur->setBouteille(0,1);
            if (zone == 5) gpJoueur->setBouteille(1,1);
            if (zone == 6) gpJoueur->setBouteille(2,1);
            gpJoueur->setBoostRubis(-100);
            trouve(C_BOUTEILLE);
            return true;
            break;
        case 38 : gpJoueur->setAvancement(4); gpPnj->getSuivant()->setMessage(39); break;
        case 67 : gpJoueur->setCle(zone-12,0); break;
        case 68 : gpJoueur->setCle(zone-12,1); break;
        case 69 : gpJoueur->setCle(zone-12,2); break;
        case 70 : 
            gpJoueur->setCle(zone-12,3,gpJoueur->getCle(zone-12,3)+1);
            switch (zone) {
                case 12 :
                    if (gpMonde->regionValue(0) == 60*16) gpJoueur->setCoffre(1,0);
                    if (gpMonde->regionValue(0) == 20*16) gpJoueur->setCoffre(1,1);
                    if (gpMonde->regionValue(0) == 0) gpJoueur->setCoffre(1,2);
                    break;
                case 13 :
                    if (gpMonde->regionValue(0) == 120*16) gpJoueur->setCoffre(2,6);
                    if (gpMonde->regionValue(0) == 0 && gpMonde->regionValue(1) == 15*16) 
                        gpJoueur->setCoffre(2,7);
                    if (gpMonde->regionValue(0) == 0 && gpMonde->regionValue(1) == 75*16)
                        gpJoueur->setCoffre(2,8);
                    if (gpMonde->regionValue(0) == 100*16) gpJoueur->setCoffre(2,9);
                    if (gpMonde->regionValue(0) == 60*16) gpJoueur->setCoffre(2,10);
                    break;
                case 14 :
                    if (gpMonde->regionValue(0) == 220*16 && gpMonde->regionValue(1) == 30*16) 
                        gpJoueur->setCoffre(3,0);
                    if (gpMonde->regionValue(0) == 240*16 && gpMonde->regionValue(1) == 15*16) 
                        gpJoueur->setCoffre(3,1);
                    if (gpMonde->regionValue(0) == 220*16 && gpMonde->regionValue(1) == 0) 
                        gpJoueur->setCoffre(3,2);
                    if (gpMonde->regionValue(0) == 100*16 && gpMonde->regionValue(1) == 0) 
                        gpJoueur->setCoffre(3,3);
                    break;
                case 15 :
                    if (gpMonde->regionValue(0) == 0) gpJoueur->setCoffre(4,0);
                    if (gpMonde->regionValue(0) == 120*16 && gpMonde->regionValue(1) == 45*16) 
                        gpJoueur->setCoffre(4,1);
                    if (gpMonde->regionValue(0) == 120*16 && gpMonde->regionValue(1) == 15*16) 
                        gpJoueur->setCoffre(4,2);
                    break;
                case 16 :
                    gpJoueur->setCoffre(5,0);
                    break;
                case 17 :
                    if (gpMonde->regionValue(0) == 0) gpJoueur->setCoffre(6,0);
                    if (gpMonde->regionValue(0) == 40*16) gpJoueur->setCoffre(6,1);
                    if (gpMonde->regionValue(0) == 120*16) gpJoueur->setCoffre(6,2);
                    break;
                case 18 :
                    if (gpMonde->regionValue(0) == 60*16) gpJoueur->setCoffre(7,0);
                    if (gpMonde->regionValue(0) == 120*16) gpJoueur->setCoffre(7,1);
                    break;
                case 19 :
                    if (gpMonde->regionValue(0) == 60*16) gpJoueur->setCoffre(8,0);
                    if (gpMonde->regionValue(0) == 0) gpJoueur->setCoffre(8,1);
                    break;
                case 20 :
                    if (gpMonde->regionValue(0) == 20*16) gpJoueur->setCoffre(9,0);
                    if (gpMonde->regionValue(0) == 0) gpJoueur->setCoffre(9,1);
                    if (gpMonde->regionValue(0) == 120*16) gpJoueur->setCoffre(9,2);
                    break;
            }
            break;
        case 71 : gpJoueur->trouveObjet(O_GANTS); break;
        case 72 : ecrit(225); return true; break;
        case 80 : gpJoueur->trouveObjet(O_GRAPPIN); break;
        case 81 : gpJoueur->trouveObjet(O_LANTERNE); 
            gpJoueur->setMagie(gpJoueur->getMagieMax()); break;
        case 82 : gpJoueur->trouveObjet(O_PALMES); break;
        case 83 : gpJoueur->trouveObjet(O_MARTEAU); break;
        case 84 : gpJoueur->trouveObjet(O_BFEU); break;
        case 85 : gpJoueur->trouveObjet(O_BGLACE); break;
        case 86 : gpJoueur->setEpee(5); break;
        case 87 : gpJoueur->setAvancement(5); 
            gpPnj->getSuivant()->setMessage(89);
            gpPnj->getSuivant()->setImmo(false);
            gpPnj->getSuivant()->setTypeIA(IA_SUIT); break;
        case 90 : 
            gpJoueur->setImmo(true);
            gpPnj->getSuivant()->setImmo(false);break;
        case 92 : gpJoueur->setBoostVie(gpJoueur->getVieMax()-gpJoueur->getVie()); break;
        case 97 :
            gpAudio->playSpecial(5);
            gpKeyboard->setMode(11);
            ecrit(223, false, true, 0, 0, 322, 240);
            break;
        case 98 : gpJoueur->trouveObjet(O_LIVRE); break;
        case 99 : case 100 : case 224 : trouve(C_MEDAILLON); return true; break;
        case 102 : 
            gpPnj->getSuivant()->setMessage(101);
            gpJoueur->trouveMedaillon(zone-24); break;
        case 103 : trouve(C_MIMAGIE); return true; break;
        case 104 : 
            gpPnj->getSuivant()->setMessage(101);
            gpJoueur->setMagieMax(64); break;
        case 108 : 
            int i;
            i=0;
            while(gpJoueur->hasBouteille(i)!=1) i++;
            switch(zone) {
                case 29 : 
                    trouve(C_POTION_ROUGE); 
                    gpJoueur->setBouteille(i,2); 
                    gpJoueur->setBoostRubis(-60);
                    return true; break;
                case 37 : 
                    trouve(C_POTION_VERTE);
                    gpJoueur->setBouteille(i,3);
                    gpJoueur->setBoostRubis(-40);
                    return true; break;
                case 40 : 
                    trouve(C_POTION_BLEUE); 
                    gpJoueur->setBouteille(i,4);
                    gpJoueur->setBoostRubis(-160);
                    return true; break;
            }break;
        case 129 : //achète objet
            Objet* obj;
            obj = NULL;
            switch(gpMonde->contenuValue(gpJoueur->getX()+8, gpJoueur->getY()+7-2)) {
                case 114 : 
                    gpJoueur->setBoostRubis(-10);
                    obj = new Objet(this, 0, 0, I_PETIT_COEUR); break;
                case 116 : 
                    gpJoueur->setBoostRubis(-20);
                    obj = new Objet(this, 0, 0, I_MAGIE_PEU); break;
                case 118 : 
                    gpJoueur->setBoostRubis(-30);
                    obj = new Objet(this, 0, 0, I_MAGIE_BCP); break;
                case 120 : 
                    gpJoueur->setBoostRubis(-30);
                    obj = new Objet(this, 0, 0, I_FLECHE); break;
                case 122 : 
                    gpJoueur->setBoostRubis(-30);
                    obj = new Objet(this, 0, 0, I_BOMBE); break;
                case 126 : 
                    gpJoueur->setBoostRubis(-50);
                    gpMonde->setContenu(gpJoueur->getX()+8, gpJoueur->getY()+7-2,0);
                    gpObjet->getSuivant()->ramasse(); return true; break;
                default : return false;
            }
            obj->ramasse();
            delete obj;
            break;
        case 130 : 
            gpJoueur->setFleche(gpJoueur->getFlecheMax());
            gpJoueur->trouveObjet(O_ARC,2); break;
        case 132 :
            gpJoueur->setAvancement(2); gpMonde->setContenu(17,4,126);
            gpPnj->getSuivant()->setMessage(131);
            break;
        case 144 :
            gpJoueur->setAvancement(1);
            gpPnj->getSuivant()->setMessage(145);
            break;
        case 153 :
            int hasard;
            hasard=0;
            switch(zone) {
                case 33:
                    gpMonde->setValeur(4*16,5*16,303,1); 
                    gpMonde->setValeur(9*16,5*16,303,1); 
                    gpMonde->setValeur(14*16,5*16,303,1);
                    gpMonde->setContenu(4,5,0);
                    gpMonde->setContenu(9,5,0);
                    gpMonde->setContenu(14,5,0);
                    hasard = rand()%3;
                    gpMonde->setContenu(4+hasard*5,5,C_QUART_COEUR);
                    break;
                case 34:
                    gpMonde->setValeur(4*16,4*16,303,1); 
                    gpMonde->setValeur(9*16,4*16,303,1); 
                    gpMonde->setValeur(14*16,4*16,303,1);
                    gpMonde->setContenu(4,4,0);
                    gpMonde->setContenu(9,4,0);
                    gpMonde->setContenu(14,4,0);
                    gpMonde->setValeur(4*16,7*16,303,1);
                    gpMonde->setValeur(9*16,7*16,303,1); 
                    gpMonde->setValeur(14*16,7*16,303,1);
                    gpMonde->setContenu(4,7,0);
                    gpMonde->setContenu(9,7,0);
                    gpMonde->setContenu(14,7,0);
                    hasard = rand()%6;
                    gpMonde->setContenu(4+(hasard%3)*5,4+(hasard/3)*3,C_QUART_COEUR);
                    break;
                case 39:
                    gpMonde->setValeur(4*16,3*16,303,1); 
                    gpMonde->setValeur(9*16,3*16,303,1); 
                    gpMonde->setValeur(14*16,3*16,303,1);
                    gpMonde->setContenu(4,3,0);
                    gpMonde->setContenu(9,3,0);
                    gpMonde->setContenu(14,3,0);
                    gpMonde->setValeur(4*16,6*16,303,1); 
                    gpMonde->setValeur(9*16,6*16,303,1); 
                    gpMonde->setValeur(14*16,6*16,303,1);
                    gpMonde->setContenu(4,6,0);
                    gpMonde->setContenu(9,6,0);
                    gpMonde->setContenu(14,6,0);
                    gpMonde->setValeur(4*16,9*16,303,1); 
                    gpMonde->setValeur(9*16,9*16,303,1); 
                    gpMonde->setValeur(14*16,9*16,303,1);
                    gpMonde->setContenu(4,9,0);
                    gpMonde->setContenu(9,9,0);
                    gpMonde->setContenu(14,9,0);
                    hasard = rand()%9;
                    gpMonde->setContenu(4+(hasard%3)*5,3+(hasard/3)*3,C_QUART_COEUR);
                    break;
            }
            gpJoueur->setBoostRubis(-10); gpPnj->getSuivant()->setMessage(155);
            ecrit(129); return true; break;
        case 166 : 
            gpJoueur->setEpee(3); trouve(C_EPEE_R); return true; break;
        case 169 : 
            gpJoueur->trouveObjet(O_GANTS, 2); trouve(C_GANTS_R); return true; break;
        case 172 : 
            gpJoueur->trouveObjet(O_ARC, 5); trouve(C_ARC_FEE); return true; break;
        case 179 : gpJoueur->setAvancement(3); gpPnj->getSuivant()->setMessage(181); break;
        case 195 : gpJoueur->trouveObjet(O_ENCYCL); gpPnj->getSuivant()->setMessage(196); break;
        case 203 : 
            int id; id=38;
            while (gpJoueur->getCoeur(id)) {id++; if (id>43) return false;}
            gpJoueur->setCoeur(id); trouve(C_QUART_COEUR); 
            gpPnj->getSuivant()->setMessage(196); return true; break;
        case 223 : 
            gpAudio->stopSpecial();
            gpAudio->stopMusic(); 
            gpGenerique->initScore();
            zone = 21;
            gpJoueur->setVie(gpJoueur->getVieMax());
            gpJoueur->setXDep(86);
            gpJoueur->setYDep(24);
            gpJoueur->setDirDep(S);
            gpJoueur->save();
            gpKeyboard->setMode(14); break;
        case 216 : gpJoueur->save();
        case 217 :
            gpKeyboard->setMode(2); gpGenerique->initLogo();
            gpAudio->stopSpecial();
            gpAudio->stopMusic();
            videListes();
            break;
        case 225 : gpJoueur->save(); ecrit(227); return true; break;
        case 232 : case 235 : return false; break;
    }
    testAnim();
    return false;
}

void Jeu::testRamasse(int x, int y, int w, int h) {
    Objet* gpObjet2 = gpObjet->getSuivant();
    while (gpObjet2 != NULL) {
        if (gpObjet2->getVie()) {
            if (x<gpObjet2->getX()+gpObjet2->getW()-2 && x+w>gpObjet2->getX()+2
            && y<gpObjet2->getY()+gpObjet2->getH()-2 && y+h-4>gpObjet2->getY()+2) {
                gpObjet2->ramasse();
            }
        }
        gpObjet2 = gpObjet2->getSuivant();
    }
}

bool Jeu::testEpee(int x, int y, int w, int h, int type, int force, Direction direction) {
    bool retour = false;
    //if (type==COUP_EPEE || type==EPEE_SPIN) testRamasse(x, y, w, h);
    for (int i = x; i < x + w; i++) {
        for (int j = y; j < y + h; j++) {
            if (i >= 0 && j >=0) {
                retour = (retour || coupeBuisson(i, j, type));
                if (type==COUP_MARTEAU) retour = (retour || applatitBloc(i, j));
                if (type==FEU) retour = (retour || allumeTorche(i, j));
                if (type==COUP_GLACE) retour = (retour || etteintTorche(i, j));
                if (type==EXPLOSION) retour = (retour || exploseMur(i, j));
                if (type==COUP_EPEE || type==EPEE_SPIN) {
                    retour = (retour || renvoieProj(i, j, direction));
                    retour = (retour || coupeRideau(i, j));
                }
                retour = (retour || tueEnnemi(i, j, type, force, direction));
            }
        }
    }
    return retour;
}

bool Jeu::renvoieProj(int i, int j, Direction dir) {
    
    Snipe* gpSnipe2 = gpSnipe->getSuivant();
    while (gpSnipe2 != NULL) {
        if (i >= gpSnipe2->getX() && i < gpSnipe2->getX() + gpSnipe2->getW()
        && j >= gpSnipe2->getY() && j < gpSnipe2->getY() + gpSnipe2->getH()) {
            if (gpSnipe2->renvoit(dir)) {
                gpAudio->playSound(7);
                return true;
            }
        }
        gpSnipe2 = gpSnipe2->getSuivant();
    }
    
    return false;
}

bool Jeu::coupeRideau(int i, int j) {
    if (gpMonde->motifValue(i,j)==2276) {
        if (gpMonde->motifValue(i-16,j)!=2276) gpMonde->passage(i/16,(j/16)-1,0);
        else gpMonde->passage((i/16)-1,(j/16)-1,0);
        return true;
    }
    return false;
}

bool Jeu::coupeBuisson(int i, int j, int type) {
    int donne = 0;
    bool retour = false;
    if (type==COUP_EPEE || type==EPEE_SPIN || type==FEU 
    || type==COUP_GLACE || type==EXPLOSION) {
        if (gpMonde->motifValue(i,j)==26) {
            gpMonde->setValeur(i, j, 29, -1, 0, 0);
            donne=1;
            ajouteProjectile(9, N, i-(i%16)-5,j-(j%16)-13, 0);
        }    
        if (gpMonde->motifValue(i,j)==385) {
            gpMonde->setValeur(i, j, 386, -1, 0, 0);
            donne=2;
            ajouteProjectile(10, N, i-(i%16)-5,j-(j%16)-13, 0);
        }
        if (gpMonde->motifValue(i,j)==48) {
            gpMonde->setValeur(i, j, 49, -1, 0, 0);
            donne=3;
            ajouteProjectile(11, N, i-(i%16)-5,j-(j%16)-13, 0);
        }
        if (donne) {
            retour=true;
            //nombreMystere = (std::rand() % (MAX - MIN + 1)) + MIN;
            int ob=0;
            int randomV = (int)((float)std::rand() / RAND_MAX * (15*donne));
            //int randomV = (int) ((std::rand() % (15*donne)) + 1);
            switch (randomV) {                        
                case 1: if (gpJoueur->getRubis()<gpJoueur->getRubisMax()) 
                    ob=I_RUBIS_VERT; break;
                case 2: if (gpJoueur->getVie()<gpJoueur->getVieMax()) 
                    ob=I_PETIT_COEUR; break;
                case 3: if (gpJoueur->getFleche()<gpJoueur->getFlecheMax()
                    && gpJoueur->hasObjet(O_ARC)) 
                    ob=I_FLECHE; break;
                case 4: if (gpJoueur->getBombe()<gpJoueur->getBombeMax()
                    && gpJoueur->hasObjet(O_SAC_BOMBES)) 
                    ob=I_BOMBE; break;
                case 5: if (gpJoueur->getMagie()<gpJoueur->getMagieMax()
                    && gpJoueur->hasObjet(O_LANTERNE)) 
                    ob=I_MAGIE_PEU; break; 
            }
            if(ob>0) ajouteObjet(ob, i-(i%16), j-(j%16));
        }
    }
    return retour;
}

bool Jeu::applatitBloc(int i, int j) {
    int val = gpMonde->motifValue(i,j);
    if (val==126 || val==393 || val==987 || val==1761) {
        gpMonde->setValeur(i, j, val+1, -1, 0, 0);
        return true;
    }
    if (val>=960 && val<=962) { 
        gpMonde->passage(((int)(i/16))-(val-960),((int)(j/16))-2,2); 
        gpAudio->playSound(9);
        if (zone==7) gpJoueur->setCoffre(0,6);
        return true;
    }
    return false;
}

bool Jeu::allumeTorche(int i, int j) {
    int val = gpMonde->motifValue(i,j);
    if (val==1455) {
        gpMonde->setValeur(i, j, 1181, PLEIN);
        testAnim();
        return true;
    }
    return false;
}

bool Jeu::etteintTorche(int i, int j) {
    int val = gpMonde->motifValue(i,j);
    if (val==1181) {
        gpMonde->setValeur(i, j, 1455, PLEIN);
        testAnim();
        return true;
    }
    //gel eau
    if (val==28 || val==463 || val==466 || (val>=1305 && val<=1317))
        if (i>gpMonde->regionValue(0)+31 && j>gpMonde->regionValue(1)+31 
        && i<gpMonde->regionValue(2)+-20 && j<gpMonde->regionValue(3)+-20)
            gpMonde->setValeur(i,j,27,-1,GLACE,C_RIEN);
    if (val==540) {
        gpMonde->setValeur(i,j,980,-1,GLACE,C_RIEN);
        gpMonde->setMurs(((int)(i/16))*2,((int)(j/16))*2,2);
    }
    if (val==543) {
        gpMonde->setValeur(i,j,981,-1,GLACE,C_RIEN);
        gpMonde->setMurs(((int)(i/16))*2+1,((int)(j/16))*2,2);
    }
    return false;
}

bool Jeu::exploseMur(int i, int j) {
    int val = gpMonde->motifValue(i,j);
    
    if (val==532 || val==533 || val ==790 || val==791) {
        switch (val) {
            case 532 : gpMonde->passage(((int)(i/16)),((int)(j/16))-1,0); break;
            case 533 : gpMonde->passage(((int)(i/16))-1,((int)(j/16))-1,0); break;
            case 790 : gpMonde->passage(((int)(i/16)),((int)(j/16))-1,1); break;
            case 791 : gpMonde->passage(((int)(i/16))-1,((int)(j/16))-1,1); break;
        }
        if (zone==1) gpJoueur->setCoffre(0,0);
        if (zone==3) gpJoueur->setCoffre(0,1);
        if (zone==3 && (val==790 || val==791)) gpJoueur->setCoffre(0,2);
        if (zone==5) gpJoueur->setCoffre(0,3);
        if (zone==7) gpJoueur->setCoffre(0,4);
        if (zone==11) gpJoueur->setCoffre(0,5);
        gpAudio->playSound(9);
        return true;
    }
    //donjon
    //h et b
    if (val==1092 || val==1093 || val==1094 || val==1095) {
        if (zone == 12) gpJoueur->setCoffre(1,8);
        int x1=0, y1=0;
        switch(val) {
            case 1092 :
                x1=((int)(i/16)); y1=((int)(j/16))-1; break;
            case 1093 :
                x1=((int)(i/16))-1; y1=((int)(j/16))-1; break;
            case 1094 :
                x1=((int)(i/16)); y1=((int)(j/16))+2; break;
            case 1095 :
                x1=((int)(i/16))-1; y1=((int)(j/16))+2; break;
        }
        for (int jj=y1*2-3;jj<y1*2+3; jj++)
            for (int ii=x1*2+1; ii<x1*2+3; ii++)
                gpMonde->setMurs(ii,jj,0);
        gpMonde->passage(x1,y1,4);
        gpAudio->playSound(9);
        return true;
    }
    //g et d
    if (val==1099 || val==1101 || val==1102 || val==1104
    || val==1457 || val==1459 || val==1460 || val==1462
    || val==2388 || val==2390 || val==2391 || val==2393) {
        if (zone == 12) gpJoueur->setCoffre(1,7);
        if (zone == 14 && gpMonde->regionValue(1) == 105*16) gpJoueur->setCoffre(3,8);
        if (zone == 14 && gpMonde->regionValue(0) > 160*16 && gpJoueur->getX() < 230*16) 
            gpJoueur->setCoffre(3,9);
        if (zone == 14 && gpMonde->regionValue(0) > 160*16 && gpJoueur->getX() > 230*16) 
            gpJoueur->setCoffre(3,10);
        if (zone == 20) gpJoueur->setCoffre(9,7);
        int x1=0, y1=0;
        switch(val) {
            case 1099 : case 1457 : case 2388 :
                x1=((int)(i/16))-1; y1=((int)(j/16)); break;
            case 1101 : case 1459 : case 2390 :
                x1=((int)(i/16))-1; y1=((int)(j/16))-1; break;
            case 1102 : case 1460 : case 2391 :
                x1=((int)(i/16))+2; y1=((int)(j/16)); break;
            case 1104 : case 1462 : case 2393 :
                x1=((int)(i/16))+2; y1=((int)(j/16))-1; break;
        }
        for (int jj=y1*2+1;jj<y1*2+3; jj++)
            for (int ii=x1*2-3; ii<x1*2+3; ii++)
                gpMonde->setMurs(ii,jj,0);
        gpMonde->passage(x1,y1,3);
        gpAudio->playSound(9);
        return true;
    }
    //sol fissuré
    if (val==1894) {
        if (zone == 18) gpJoueur->setCoffre(7,4);
        int x1, y1;
        x1=((int)(i/16)); y1=((int)(j/16));
        while(gpMonde->motifValue((x1-1)*16,y1*16)==1894) x1--;
        while(gpMonde->motifValue(x1*16,(y1-1)*16)==1894) y1--;
        gpMonde->setValeur(x1*16,y1*16,1997,-1,VIDE,0);
        gpMonde->setValeur((x1+1)*16,y1*16,1999,-1,VIDE,0);
        gpMonde->setValeur(x1*16,(y1+1)*16,2003,-1,VIDE,0);
        gpMonde->setValeur((x1+1)*16,(y1+1)*16,2005,-1,VIDE,0);
        gpAudio->playSound(9);
        return true;
    }
    
    return false;
}

bool Jeu::tueEnnemi(int i, int j, int type, int force, Direction direction) {
    Ennemi* gpEnnemi2 = gpEnnemi->getSuivant();
    ZoneRect* z;
    SDL_Rect* r;
    int degats;
    bool retour = false;
    while (gpEnnemi2 != NULL) {
        if (gpEnnemi2->getVie()) {
            if (type == CAISSE) z = gpEnnemi2->getZoneBase();
            else z = gpEnnemi2->getZonesVulnerables();
            while (z != NULL && gpEnnemi2->getVie()) {
                r = z->getZone();
                if (i >= gpEnnemi2->getX() + r->x && i < gpEnnemi2->getX() + r->x + r->w
                && j >= gpEnnemi2->getY() + r->y && j < gpEnnemi2->getY() + r->y + r->h) {
                    
                    if (gpEnnemi2->getEffet(type)) {
                    
                        if ((gpEnnemi2->getEffet(type)==2 && !gpEnnemi2->getInvincible()) 
                        || type == CAISSE) {
                            degats = force;
                            if (gpEnnemi2->getEnnId()!=20) 
                                gpEnnemi2->setVie(gpEnnemi2->getVie()-degats);
                            else {
                                gpMonde->permuteBlocs();
                                if (!verifZoneJoueur(gpJoueur->getX(), 
                                gpJoueur->getY()+8, 16, 16, 2)) 
                                    gpMonde->permuteBlocs();
                            }
                        }
                        if (gpEnnemi2->getVie()) {
                            if (type == COUP_GLACE && !gpEnnemi2->getInvincible()) 
                                gpEnnemi2->gele();
                            if (type == FEU && !gpEnnemi2->getInvincible()) 
                                gpEnnemi2->degele();
                            if (gpEnnemi2->getEffet(type)==2 && !gpEnnemi2->getInvincible()) {
                                gpAudio->playSound(7);
                            }
                            if (gpEnnemi2->getEffet(type)>=1) {
                                if (!gpEnnemi2->isBoss() || !gpEnnemi2->getInvincible()) 
                                    gpEnnemi2->pousse(direction);
                                if (!gpEnnemi2->getInvincible()) gpEnnemi2->setInvincible(1);
                            }
                        }
                        else {
                            gpJoueur->setEnnemi(gpEnnemi2->getEnnId());
                            gpAudio->playSound(8);
                            ajouteProjectile(15, direction, 
                                gpEnnemi2->getX()+(gpEnnemi2->getW()/2)-14, 
                                gpEnnemi2->getY()+(gpEnnemi2->getH()/2)-14, 0);
                            donneObjet(gpEnnemi2);
                            testAnim();
                        }
                        
                    }
                    retour = true;
                    
                    if (type == COUP_GRAPPIN) retour = true;
                }
                z = z->getSuivant();
            }
        }
        gpEnnemi2 = gpEnnemi2->getSuivant();
    }
    return retour;;
}

void Jeu::donneObjet(Ennemi* ennemi) {
    int obj;
    
    if (ennemi->isBoss()) {
        if (zone < 19) {
            obj = I_CRISTAL; 
            gpAudio->stopSpecial();
            gpStatut->setJauge(false);
        }
        else {
            if (zone==20) obj = I_TRIFORCE; else obj=0;
            gpAudio->playSpecial(3);
            gpStatut->setJauge(false);
        }
    }else {
    
        int randomValue = (int)(((float)rand() / RAND_MAX) * 12);
        switch (randomValue){
            case 0: case 1: case 2: case 3: obj=ennemi->getObjets(0); break;
            case 4: case 5: case 6: obj=ennemi->getObjets(1); break;
            case 7: case 8: obj=ennemi->getObjets(2); break;                        
            default:obj=0;break;
        }
        if (
        ((obj == I_RUBIS_VERT || obj == I_RUBIS_BLEU || obj == I_RUBIS_ROUGE) 
            && gpJoueur->getRubis() >= gpJoueur->getRubisMax())
        || (obj == I_PETIT_COEUR && gpJoueur->getVie() >= gpJoueur->getVieMax())
        || (obj == I_FLECHE && (!gpJoueur->hasObjet(O_ARC)
            || gpJoueur->getFleche() >= gpJoueur->getFlecheMax()))
        || (obj == I_BOMBE && (!gpJoueur->hasObjet(O_SAC_BOMBES) 
            || gpJoueur->getBombe() >= gpJoueur->getBombeMax()))
        || ((obj == I_MAGIE_PEU || obj == I_MAGIE_BCP) 
            && (gpJoueur->getMagie() >= gpJoueur->getMagieMax() 
            || !gpJoueur->hasObjet(O_LANTERNE)))
        ) obj = 0;
    }
    
    //spécial
    if (zone==20 && gpMonde->regionValue(0)==140*16 
    && gpMonde->regionValue(1)==30*16 && gpJoueur->getCoffre(9,9)==0) obj=I_PETITE_CLE;
    
    if (obj) ajouteObjet(obj, ennemi->getX()+(ennemi->getW()/2)-8, 
        ennemi->getY()+(ennemi->getH()/2)-8);
}

bool Jeu::testDegat(int x, int y, int w, int h, int type, int force, Direction direction) {
    if (gpJoueur->getInvincible() || !gpJoueur->getVie()) return false;
    int degats=0;
    SDL_Rect* z = gpJoueur->getZoneBase()->getZone();
    
    int a = z->x+z->w+gpJoueur->getX();
    int b = z->x+gpJoueur->getX();
    int c = z->y+z->h+gpJoueur->getY();
    int d = z->y+gpJoueur->getY();
    
    for (int i = x; i < x + w; i++)
        for (int j = y; j < y + h; j++)
            if (i >= 0 && j >=0)
                if (type==FEU) allumeTorche(i, j);
    
    if (gpJoueur->getTypeAnim() == FLOTTE || gpJoueur->getTypeAnim() == NAGE) c-=4;
    
    if (x < a && x + w > b && y < c && y + h > d) {
        
        if (type!=LAPIN && type!=RETOUR) gpAudio->playSound(33);
        degats=force;
        if (!gpJoueur->getDefense()) degats=degats*2;
        else degats=degats/gpJoueur->getDefense();
        if (type==DEMI) degats=(gpJoueur->getVie()+1)/2;
        if (type!=LAPIN && type!=RETOUR) if (degats < 1) degats = 1;
        gpJoueur->setVie(gpJoueur->getVie()-degats);
        if (type!=RETOUR) gpJoueur->setTypeAnim(TOUCHE);
        if (type==LAPIN) gpJoueur->setLapin();
        if (type == RETOUR) {
            gpAudio->playSound(5);
            gpMonde->transitFull(zone, gpJoueur->getXDep(), gpJoueur->getYDep());
        }
        
        if (type==EXPLOSION) {
            int dfx = (x+(w/2)) - (gpJoueur->getX()+8);
            int dfy = (y+(h/2)) - (gpJoueur->getY()+16);
            if (std::abs(dfx) > std::abs(dfy)) { 
                if (dfx > 0) direction = O; else direction = E;}
            else { if (dfy > 0) direction = N; else direction = S;}
        }
        
        if (type!=LAPIN && type!=RETOUR)
        for (int i = 0; i < 16; i+=2) {
            switch (direction) {
                case N : gpJoueur->setDirection(S); gpJoueur->moveY(-2,2);break;
                case S : gpJoueur->setDirection(N); gpJoueur->moveY(2,2);break;
                case O : gpJoueur->setDirection(E); gpJoueur->moveX(-2,2);break;
                case E : gpJoueur->setDirection(O); gpJoueur->moveX(2,2);break;
            }
        }
        testFin();
        return true;
    }
    
    return false;
}

bool Jeu::pousse(int i, int j, Direction dir) {
    Caisse* gpCaisse2 = gpCaisse->getSuivant();
    while (gpCaisse2!=NULL) {
        if (i>=gpCaisse2->getX() && i<gpCaisse2->getX()+16 
        && j>=gpCaisse2->getY() && j<gpCaisse2->getY()+16
        && gpCaisse2->getVie()) {
            return gpCaisse2->pousse(dir);
        }
        gpCaisse2=gpCaisse2->getSuivant();
    }
    return false;
}

void Jeu::ouvrePorteDonjon(int x, int y) {
    //porte boss
    int val = gpMonde->motifValue(x+7,y+8);
    if ((val==1170 || val==1171 || val==1303 || val==1304 || val==1445 || val==1446
    || val==1591 || val==1592 || val==1708 || val==1709 || val==1868 || val==1869
    || val==1987 || val==1988 || val==2133 || val==2134 || val==2265 || val==2266) 
    && gpJoueur->getCle(zone-12, 2)>0) {
        if (zone==12) gpJoueur->setCoffre(1, 6);
        if (zone==13) gpJoueur->setCoffre(2, 5);
        if (zone==14) gpJoueur->setCoffre(3, 11);
        if (zone==15) gpJoueur->setCoffre(4, 6);
        if (zone==16) gpJoueur->setCoffre(5, 2);
        if (zone==17) gpJoueur->setCoffre(6, 6);
        if (zone==18) gpJoueur->setCoffre(7, 5);
        if (zone==19) gpJoueur->setCoffre(8, 4);
        if (zone==20) gpJoueur->setCoffre(9, 6);
        gpAudio->playSound(20);
        int x1,y1;
        x1=(int)((x+7)/16); y1=(int)((y+8)/16)-1;
        switch(val) {
            case 1171 : case 1304 : case 1446 : case 1592 : case 1709 : case 1869 : 
            case 1988 : case 2134 : case 2266 : 
                x1--; break;
        }
        gpMonde->passage(x1,y1,0);
    }
    //porte vérouillée h b
    if ((val==1132 || val==1133 || val==1134 || val==1135 || val==1265 || val==1266 
    || val==1267 || val==1268 || val==1425 || val==1426 || val==1427 || val==1428
    || val==1571 || val==1572 || val==1573 || val==1574 || val==1959 || val==1960 
    || val==1961 || val==1962 || val==2093 || val==2094 || val==2095 || val==2096
    || val==2225 || val==2226 || val==2227 || val==2228) 
    && gpJoueur->getCle(zone-12, 3)>0) {
        if (zone==12) gpJoueur->setCoffre(1, 3);
        if (zone==13 && gpMonde->regionValue(0)==20*16) gpJoueur->setCoffre(2, 1);
        if (zone==13 && gpMonde->regionValue(0)==80*16) gpJoueur->setCoffre(2, 3);
        if (zone==13 && gpMonde->regionValue(0)==40*16) gpJoueur->setCoffre(2, 4);
        if (zone==14 && gpMonde->regionValue(0)==40*16) gpJoueur->setCoffre(3, 5);
        if (zone==14 && gpMonde->regionValue(0)==60*16) gpJoueur->setCoffre(3, 6);
        if (zone==14 && gpMonde->regionValue(0)==260*16) gpJoueur->setCoffre(3, 7);
        if (zone==15 && gpMonde->regionValue(0)==160*16) gpJoueur->setCoffre(4, 4);
        if (zone==15 && gpMonde->regionValue(0)==140*16) gpJoueur->setCoffre(4, 5);
        if (zone==18 && gpMonde->regionValue(0)==80*16) gpJoueur->setCoffre(7, 2);
        if (zone==18 && gpMonde->regionValue(0)==100*16) gpJoueur->setCoffre(7, 3);
        if (zone==19) gpJoueur->setCoffre(8, 2);
        if (zone==20) gpJoueur->setCoffre(9, 4);
        gpAudio->playSound(20);
        
        int x1,y1;
        x1=(int)((x+7)/16); y1=(int)((y+8)/16)-1;
        switch(val) {
            case 1133 : case 1266 : case 1426 : case 1572 : case 1960 : case 2094 : 
            case 2226 : 
                x1--; break;
        }
        switch(val) {
            case 1134 : case 1267 : case 1427 : case 1573 : case 1961 : case 2095 : 
            case 2227 : 
                y1=(int)((y+10+8)/16)+2;; break;
            case 1135 : case 1268 : case 1428 : case 1574 : case 1962 : case 2096 : 
            case 2228 : 
                x1--; y1=(int)((y+10+8)/16)+2;; break;
        }
        gpMonde->passage(x1,y1,0);
        gpJoueur->setCle(zone-12, 3, gpJoueur->getCle(zone-12, 3)-1);
    }
    //porte vérouillée g d
    if ((val==1141 || val==1146 || val==1274 || val==1279
    || val==1434 || val==1439 || val==1580 || val==1585
    || val==1689 || val==1694 || val==1837 || val==1842
    || val==2102 || val==2107 || val==2234 || val==2239
    || val==2350 || val==2355) 
    && gpJoueur->getCle(zone-12, 3)>0) {        
        if (zone==12 && gpJoueur->getY()>40*16) gpJoueur->setCoffre(1, 4);
        if (zone==12 && gpJoueur->getY()<40*16) gpJoueur->setCoffre(1, 5);
        if (zone==13 && gpMonde->regionValue(3)==45*16) gpJoueur->setCoffre(2, 0);
        if (zone==13 && gpMonde->regionValue(3)==90*16) gpJoueur->setCoffre(2, 2);
        if (zone==14) gpJoueur->setCoffre(3, 4);
        if (zone==15) gpJoueur->setCoffre(4, 3);
        if (zone==16) gpJoueur->setCoffre(5, 1);
        if (zone==17 && gpMonde->regionValue(0)==80*16) gpJoueur->setCoffre(6, 3);
        if (zone==17 && gpMonde->regionValue(0)==100*16) gpJoueur->setCoffre(6, 4);
        if (zone==17 && gpMonde->regionValue(0)==60*16) gpJoueur->setCoffre(6, 5);
        if (zone==19) gpJoueur->setCoffre(8, 3);
        if (zone==20 && gpMonde->regionValue(0)<100*16) gpJoueur->setCoffre(9, 3);
        if (zone==20 && gpMonde->regionValue(0)>100*16) gpJoueur->setCoffre(9, 5);
        gpAudio->playSound(20);
        int x1,y1;
        x1=(int)((x+7)/16)-1; y1=(int)((y+8)/16)-1;
        switch(val) {
            case 1146 : case 1279 : case 1439 : case 1585 : case 1694 : 
            case 1842 : case 2107 : case 2239 : case 2355 : 
                x1=(int)((x+7+10)/16)+2; break;
        }
        gpMonde->passage(x1,y1,1);
        gpJoueur->setCle(zone-12, 3, gpJoueur->getCle(zone-12, 3)-1);
    }
    //porte prison
    val = gpMonde->motifValue(x+7,y);
    if (val==2416 && gpJoueur->getCle(zone-12, 3)>0) {
        if (zone==20) gpJoueur->setCoffre(9, 8);
        gpAudio->playSound(20);
        int x1,y1;
        x1=(int)((x+7)/16); y1=(int)((y-8+8)/16);
        gpMonde->setValeur(x1*16,y1*16,2304,-1,LIBRE,0);
        gpJoueur->setCle(zone-12, 3, gpJoueur->getCle(zone-12, 3)-1);
    }
}

void Jeu::ouvrePorteMaison(int x, int y) {
    int val = gpMonde->motifValue(x,y);
    x=x/16; y=y/16;
    if (val==610 || val==611 || val==705 || val==706 || val==918 || val==919) {
        gpAudio->playSound(20);
        switch (val) {
            case 610 :
                gpMonde->setMap(x,y,602);//setValeur(x, y, 602, -1, LIBRE, 0);
                gpMonde->setMap(x+1,y,603);//gpMonde->setValeur(x+16, y, 603, -1, LIBRE, 0);
                break;
            case 611 :
                gpMonde->setMap(x-1,y,602);//gpMonde->setValeur(x-16, y, 602, -1, LIBRE, 0);
                gpMonde->setMap(x,y,603);//gpMonde->setValeur(x, y, 603, -1, LIBRE, 0);
                break;
            case 705 :
                gpMonde->setMap(x,y,711);//gpMonde->setValeur(x, y, 711, -1, LIBRE, 0);
                gpMonde->setMap(x+1,y,712);//gpMonde->setValeur(x+16, y, 712, -1, LIBRE, 0);
                break;
            case 706 :
                gpMonde->setMap(x-1,y,711);//gpMonde->setValeur(x-16, y, 711, -1, LIBRE, 0);
                gpMonde->setMap(x,y,712);//gpMonde->setValeur(x, y, 712, -1, LIBRE, 0);
                break;
            case 918 :
                gpMonde->setMap(x,y-1,923);
                gpMonde->setMap(x+1,y-1,924);
                gpMonde->setMap(x,y,925);
                gpMonde->setMap(x+1,y,926);
                break;
            case 919 :
                gpMonde->setMap(x-1,y-1,923);
                gpMonde->setMap(x,y-1,924);
                gpMonde->setMap(x-1,y,925);
                gpMonde->setMap(x,y,926);
                break;
        }
    }
}

bool Jeu::isDangereux(int x, int y) {
    if (gpJoueur->getInvincible() || !gpJoueur->getVie()) return false;
    //motif dangereux
    if (gpMonde->motifValue(x,y)==830 || gpMonde->motifValue(x+15,y)==830
    || gpMonde->motifValue(x,y+15)==830 || gpMonde->motifValue(x+15,y+15)==830
    || gpMonde->motifValue(x,y)==1892 || gpMonde->motifValue(x+15,y)==1892
    || gpMonde->motifValue(x,y+15)==1892 || gpMonde->motifValue(x+15,y+15)==1892) {
    
        gpAudio->playSound(33);
        int degats=2;
        if (!gpJoueur->getDefense()) degats=degats*2;
        else degats=degats/gpJoueur->getDefense();
        if (degats < 1) degats = 1;
        gpJoueur->setVie(gpJoueur->getVie()-degats);
        gpJoueur->setTypeAnim(TOUCHE);
        
        for (int i = 0; i < 16; i+=2) {
            switch (gpJoueur->getDirection()) {
                case S : gpJoueur->setDirection(S); gpJoueur->moveY(-2,2);break;
                case N : gpJoueur->setDirection(N); gpJoueur->moveY(2,2);break;
                case E : gpJoueur->setDirection(E); gpJoueur->moveX(-2,2);break;
                case O : gpJoueur->setDirection(O); gpJoueur->moveX(2,2);break;
            }
        }
        testFin();
        return true;
    }
    
    return false;
}

void Jeu::testInterrupteur(int x, int y) {
    if (gpMonde->motifValue(x,y)==1759) {
        switch (zone) {
            case 16 :
                gpAudio->playSound(20);
                gpMonde->passage(129,15,0);
                gpMonde->setValeur(x, y, 1760, -1, LIBRE, 0);
                break;
            case 17 :
                gpAudio->playSound(20);
                gpMonde->passage(69,75,0);
                gpMonde->setValeur(x, y, 1760, -1, LIBRE, 51);
                break;
            case 20 :
                gpAudio->playSound(20);
                if (gpMonde->regionValue(0)==0)gpMonde->passage(20,51,1);
                else gpMonde->passage(80,51,1);
                gpMonde->setValeur(x, y, 1760, -1, LIBRE, 51);
                break;
        }
    }
}

bool Jeu::verifZoneJoueur(int x, int y, int w, int h, int nbdir) {
    //ouvre porte
    ouvrePorteMaison(x+8, y);
    
    //portes verrouillées
    ouvrePorteDonjon(x/*+8*/, y/*-2*/);
    
    testInterrupteur(x+8, y+4);
    
    if (isDangereux(x,y)) return false;
    
    if (!isNotFull(x,y,w,h)) {
        if ((x-(gpJoueur->getX()+gpJoueur->getZoneBase()->getZone()->x)==0 
        || y-(gpJoueur->getY()+gpJoueur->getZoneBase()->getZone()->y)==0) && nbdir==1) {
        //en fonction de la direction
            if (y-(gpJoueur->getY()+gpJoueur->getZoneBase()->getZone()->y)<0 
            && gpJoueur->getDirection()==N) { 
                if ( isMarchable(x-2,y, 1, 1, 0)
                && isMarchable(x-2,y+h-1, 1, 1, 0)
                && isMarchable(x,y, 1, 1, 0)
                && isMarchable(x+7,y, 1, 1, 0)) gpJoueur->moveX(-2);
                if ( isMarchable(x+8,y, 1, 1, 0)
                && isMarchable(x+w-1,y, 1, 1, 0)
                && isMarchable(x+w+1,y, 1, 1, 0)
                && isMarchable(x+w+1,y+h-1, 1, 1, 0)) gpJoueur->moveX(2);
            }
            if (y-(gpJoueur->getY()+gpJoueur->getZoneBase()->getZone()->y)>0 
            && gpJoueur->getDirection()==S) {
                if ( isMarchable(x-2,y, 1, 1, 0)
                && isMarchable(x-2,y+h-1, 1, 1, 0)
                && isMarchable(x,y+h-1, 1, 1, 0)
                && isMarchable(x+7,y+h-1, 1, 1, 0)) gpJoueur->moveX(-2);
                if ( isMarchable(x+8,y+h-1, 1, 1, 0)
                && isMarchable(x+w-1,y+h-1, 1, 1, 0)
                && isMarchable(x+w+1,y, 1, 1, 0)
                && isMarchable(x+w+1,y+h-1, 1, 1, 0)) gpJoueur->moveX(2);
            }
            if (x-(gpJoueur->getX()+gpJoueur->getZoneBase()->getZone()->x)<0 
            && gpJoueur->getDirection()==O) {
                if ( isMarchable(x,y-2, 1, 1, 0)
                && isMarchable(x+w-1,y-2, 1, 1, 0)
                && isMarchable(x,y, 1, 1, 0)
                && isMarchable(x,y+7, 1, 1, 0)) gpJoueur->moveY(-2);
                if ( isMarchable(x,y+8, 1, 1, 0)
                && isMarchable(x,y+h-1, 1, 1, 0)
                && isMarchable(x,y+h+1, 1, 1, 0)
                && isMarchable(x+w+1,y+h+1, 1, 1, 0)) gpJoueur->moveY(2);
            }
            if (x-(gpJoueur->getX()+gpJoueur->getZoneBase()->getZone()->x)>0 
            && gpJoueur->getDirection()==E) {
                if ( isMarchable(x,y-2, 1, 1, 0)
                && isMarchable(x+w-1,y-2, 1, 1, 0)
                && isMarchable(x+w-1,y, 1, 1, 0)
                && isMarchable(x+w-1,y+7, 1, 1, 0)) gpJoueur->moveY(-2);
                if ( isMarchable(x+w-1,y+8, 1, 1, 0)
                && isMarchable(x+w-1,y+h-1, 1, 1, 0)
                && isMarchable(x,y+h+1, 1, 1, 0)
                && isMarchable(x+w+1,y+h+1, 1, 1, 0)) gpJoueur->moveY(2);
            }
        }
        return false;
    }
        
    return true;
}

// renvoit vrai si Link peut y aller
bool Jeu::isNotFull(int x, int y, int w, int h) {
    for (int i = x; i < x + w; i++)
        for (int j = y; j < y + h; j++) {
            if (!isMarchable(i, j, 1, 1, 0)) return false;
        }
    return true;
}

//cas général
bool Jeu::isNotFull(int x, int y, int w, int h, int vol, int nage, int spectre) {
    for (int i = x; i < x + w; i++)
        for (int j = y; j < y + h; j++) {
            if (!isMarchable(i, j, 1, 1, spectre)) return false;
        }
    if (isVide(x,y,w,h) && !vol && !spectre) return false;
    if (isWater(x,y,w,h) && !vol && !nage && !spectre) return false;
    return true;
}

//renvoit vrai si plein d'eau, lol
bool Jeu::isWater(int x, int y, int w, int h) {
    for (int i = x; i < x + w; i++)
        for (int j = y; j < y + h; j++) {
            if (i>0 && j>0 && i<gpMonde->regionValue(2) && j<gpMonde->regionValue(3)) 
                if (gpMonde->murValue(i,j)!=EAU) return false;
        }
    return true;
}

//renvoit vrai si plein de vide, super lol
bool Jeu::isVide(int x, int y, int w, int h) {
    for (int i = x; i < x + w; i++)
        for (int j = y; j < y + h; j++) {
            if (i>0 && j>0 && i<gpMonde->regionValue(2) && j<gpMonde->regionValue(3)) 
                if (gpMonde->murValue(i,j)!=VIDE) return false;
        }
    return true;
}

//renvoit vrai présence de glace
bool Jeu::isGlace(int x, int y, int w, int h) {
    for (int i = x; i < x + w; i++)
        for (int j = y; j < y + h; j++) {
            if (i>0 && j>0 && i<gpMonde->regionValue(2) && j<gpMonde->regionValue(3)) 
                if (gpMonde->murValue(i,j)==GLACE) return true;
        }
    return false;
}

//renvoit vrai si ramassable
bool Jeu::isRamassable(int x, int y, int g) {
    int val = gpMonde->motifValue(x,y);
    if (val>=38 && val <=39 && g==2) return true;
    if (val>=30 && val <=37 && g>=1) return true;
    return false;
}

bool Jeu::isMarchable(int i, int j, int vol, int nage, int spectre) {
    if (i < gpMonde->regionValue(0) || i>= gpMonde->regionValue(2) 
    || j < gpMonde->regionValue(1) || j>= gpMonde->regionValue(3)) return false;
    if (spectre) return true;
    //vérif caisses
    Caisse* gpCaisse2 = gpCaisse->getSuivant();
    while (gpCaisse2!=NULL) {
        if (abs(i-gpCaisse2->getX()<16) || abs(j-gpCaisse2->getY()<16)) {
            if (i >= gpCaisse2->getX() && i < gpCaisse2->getX()+16
                && j >= gpCaisse2->getY() && j < gpCaisse2->getY()+16
                && gpCaisse2->getVie()) return false;
        }
        gpCaisse2 = gpCaisse2->getSuivant();
    }
    int val = gpMonde->murValue(i, j);
    if (val == LIBRE || val == GLACE) return true;
    if (val == VIDE && vol) return true;
    if (val == EAU && (vol || nage)) return true;
    return false;
}

bool Jeu::isEnnemi(int x, int y, int w, int h, int id) {
    Ennemi* gpEnnemi2 = gpEnnemi->getSuivant();
    SDL_Rect* r;
    int x1;
    int y1;
    while (gpEnnemi2 != NULL) {
        if (gpEnnemi2->getListId() != id && gpEnnemi2->getVie()>0) {
            r = gpEnnemi2->getZoneBase()->getZone();
            x1 = gpEnnemi2->getX();
            y1 = gpEnnemi2->getY();
            if ((x1 + r->x + r->w > x && x1 + r->x < x+w)
            && (y1 + r->y + r->h > y && y1 + r->y < y+h)) 
                return true;
        }
        gpEnnemi2 = gpEnnemi2->getSuivant();
    }
    return false;
}

bool Jeu::isPnj(int x, int y, int w, int h, int id) {
    Pnj* gpPnj2 = gpPnj->getSuivant();
    SDL_Rect* r;
    int x1;
    int y1;
    while (gpPnj2 != NULL) {
        if (gpPnj2->getListId() != id && gpPnj2->getVie()>0) {
            r = gpPnj2->getZoneBase()->getZone();
            x1 = gpPnj2->getX();
            y1 = gpPnj2->getY();
            if ((x1 + r->x + r->w > x && x1 + r->x < x+w)
            && (y1 + r->y + r->h > y && y1 + r->y < y+h)) 
                return true;
        }
        gpPnj2 = gpPnj2->getSuivant();
    }
    return false;
}

bool Jeu::isLink(int x, int y, int w, int h) {
    SDL_Rect* r = gpJoueur->getZoneBase()->getZone();
    int x1 = gpJoueur->getX();
    int y1 = gpJoueur->getY();
    
    if (gpJoueur->getVie()>0) {
        if ((x1 + r->x + r->w > x && x1 + r->x < x+w)
        && (y1 + r->y + r->h > y && y1 + r->y < y+h)) 
            return true;
    }
    return false;
}

//ramasse objet rammassable (pancarte, pierre, etc.)
void Jeu::ramasse() {
    int i=0;
    int j=0;
    int g = gpJoueur->hasObjet(O_GANTS);
    switch(gpJoueur->getDirection()) {
        case N : i=gpJoueur->getX()+8; j=gpJoueur->getY()+7-2; break;
        case S : i=gpJoueur->getX()+8; j=gpJoueur->getY()+24+2; break;
        case O : i=gpJoueur->getX()-1-2; j=gpJoueur->getY()+16; break;
        case E : i=gpJoueur->getX()+16+2; j=gpJoueur->getY()+16; break;
    }
    if (isRamassable(i,j,g)) {gpJoueur->setTypeAnim(SOULEVE); return;}
    if (g < 2) if (isRamassable(i,j,2)) gpJoueur->setTypeAnim(LOURD);
}

int Jeu::enleve() {
    int i=0;
    int j=0;
    int g = gpJoueur->hasObjet(O_GANTS);
    switch(gpJoueur->getDirection()) {
        case N : i=gpJoueur->getX()+8; j=gpJoueur->getY()+7-2; break;
        case S : i=gpJoueur->getX()+8; j=gpJoueur->getY()+24+2; break;
        case O : i=gpJoueur->getX()-1-2; j=gpJoueur->getY()+16; break;
        case E : i=gpJoueur->getX()+16+2; j=gpJoueur->getY()+16; break;
    }
    if (!isRamassable(i,j, g)) return 0;
    int val = gpMonde->motifValue(i,j);
    int new_val = val+9;
    int obj=0;
    
    switch(val) {
        case 30 : obj = 1; new_val = 40; break;
        case 31 : obj = 2; new_val = 40; break;
        case 32 : obj = 3; new_val = 41; break;
        case 33 : obj = 4; new_val = 42; break;
        case 34 : obj = 4; new_val = 43; break;
        case 35 : obj = 3; new_val = 44; if (zone == 5) new_val = 46; break;
        case 36 : obj = 4; new_val = 45; if (zone == 5) new_val = 47; break;
        case 37 : obj = 5; new_val = 44; break;
        case 38 : obj = 6; new_val = 41; break;
        case 39 : obj = 6; new_val = 44; if (zone == 5) new_val = 46; break;
    }
    gpMonde->setValeur(i,j,new_val,0);
    if (isDonjon()) {
        if(gpMonde->contenuValue(i, j)==51) gpMonde->setValeur(i,j,1760,-1,0,51);
        if(gpMonde->contenuValue(i, j)==50) gpMonde->setValeur(i,j,1759,-1,0,50);
    }
    
    gpAudio->playSound(34);
    
    return obj;
}

void Jeu::setMenu(bool b) {
    menu = b;
    if (!menu) gpMenu->menuOut();
    else {stop = true; gpMenu->menuIn();}
}

void Jeu::ecrit(int id, bool anim, bool cadre, int x, int y, int w, int h) {
    setText(true);
    gpTexte->setTexte(id, x, y, w, h, anim, cadre);
}

void Jeu::affiche(SDL_Surface* gpScreen, std::string s, int x, int y) {
    gpTexte->affiche(gpScreen, s, x, y);
}

Ennemi* Jeu::existEnnemi(int type) {
    Ennemi* gpEnnemi2 = gpEnnemi->getSuivant();
    while (gpEnnemi2 != NULL) {
        if (gpEnnemi2->getEnnId() == type) return gpEnnemi2;
        gpEnnemi2 = gpEnnemi2->getSuivant();
    }
    return NULL;
}

void Jeu::setStop(bool b) { stop = b;}

void Jeu::setTransition(bool b) { transition = b;}

void Jeu::setText(bool b) { text = b; stop = text;}

void Jeu::setPhg(int i, int j) {phg[0]=i;phg[1]=j;}

void Jeu::setZone(int i) {zone=i;}

bool Jeu::getStop() { return stop;}
bool Jeu::getTransition() { return transition;}
int Jeu::getZone() { return zone;}
int Jeu::getPhg(int i) { return phg[i];}
bool Jeu::getMenu() { return menu;}
bool Jeu::getText() { return text;}
SDL_Surface* Jeu::getImageObjets() { return imageObjets;}

int Jeu::getVueHorz() {return vueHorz;}
int Jeu::getVueVert() {return vueVert;}
void Jeu::setVueHorz(int i) {vueHorz = i;}
void Jeu::setVueVert(int i) {vueVert = i;}

void Jeu::setNewZone(int nz) { newzone = nz;}
void Jeu::setNewX(int nx) { newx = nx;}
void Jeu::setNewY(int ny) { newy = ny;}

void Jeu::permuteBloc(int val) {
    gpEnnemi->permute(val);
}

void Jeu::testAnim() {
    switch (zone) {
        case 12 :
            if (gpMonde->regionValue(0)==110*16 && gpJoueur->hasCristal(0) 
            && gpMonde->motifValue(119*16,13*16)==1188) {
                gpAudio->playSound(20); gpMonde->passage(119,15,0); break;
            }
            if (gpMonde->regionValue(0)==110*16 && !gpJoueur->hasCristal(0) 
            && gpMonde->motifValue(119*16,13*16)==1071) {
                gpAudio->playSound(20);
                gpAudio->playSpecial(1);
                gpStatut->setJauge(true);
                gpJoueur->moveY(-16);
                gpMonde->setValeur(119*16, 13*16, 1188, -1, LIBRE, 0);
                gpMonde->setValeur(120*16, 13*16, 1189, -1, LIBRE, 0);
                gpMonde->setValeur(119*16, 14*16, 1190, 0, LIBRE, 0);
                gpMonde->setValeur(120*16, 14*16, 1191, 0, LIBRE, 0);
                for (int i = 0; i < 4; i++) gpMonde->setMurs(119*2+i,13*2+1,PLEIN);
                for (int i = 0; i < 4; i++) gpMonde->setMurs(119*2+i,13*2+2,HAUT);
                for (int i = 0; i < 4; i++) gpMonde->setMurs(119*2+i,13*2+3,SEMI_PLEIN);
            }
            break;
        case 13 :
            if (gpMonde->regionValue(0)==60*16 && gpMonde->regionValue(1)==60*16 
            && gpJoueur->hasCristal(1) && gpMonde->motifValue(69*16,73*16)==1349) {
                gpAudio->playSound(20); gpMonde->passage(69,75,0); break;
            }
            if (gpMonde->regionValue(0)==60*16 && gpMonde->regionValue(1)==60*16 
            && !gpJoueur->hasCristal(1) && gpMonde->motifValue(69*16,73*16)==1244) {
                gpAudio->playSound(20);
                gpAudio->playSpecial(1);
                gpStatut->setJauge(true);
                gpJoueur->moveY(-16);
                gpMonde->setValeur(69*16, 73*16, 1349, -1, LIBRE, 0);
                gpMonde->setValeur(70*16, 73*16, 1350, -1, LIBRE, 0);
                gpMonde->setValeur(69*16, 74*16, 1351, 0, LIBRE, 0);
                gpMonde->setValeur(70*16, 74*16, 1352, 0, LIBRE, 0);
                for (int i = 0; i < 4; i++) gpMonde->setMurs(69*2+i,73*2+1,PLEIN);
                for (int i = 0; i < 4; i++) gpMonde->setMurs(69*2+i,73*2+2,HAUT);
                for (int i = 0; i < 4; i++) gpMonde->setMurs(69*2+i,73*2+3,SEMI_PLEIN);
            }
            break;
        case 14 :
            if (gpMonde->regionValue(0)==80*16 && gpMonde->regionValue(1)==90*16 
            && gpJoueur->hasCristal(2) && gpMonde->motifValue(89*16,103*16)==1451) {
                gpAudio->playSound(20);
                gpMonde->passage(89,105,0);
                break;
            }
            if (gpMonde->regionValue(0)==80*16 && gpMonde->regionValue(1)==90*16 
            && !gpJoueur->hasCristal(2) && gpMonde->motifValue(89*16,103*16)==1403) {
                gpAudio->playSound(20);
                gpAudio->playSpecial(1);
                gpStatut->setJauge(true);
                gpJoueur->moveY(-16);
                gpMonde->setValeur(89*16, 103*16, 1451, -1, LIBRE, 0);
                gpMonde->setValeur(90*16, 103*16, 1452, -1, LIBRE, 0);
                gpMonde->setValeur(89*16, 104*16, 1453, 0, LIBRE, 0);
                gpMonde->setValeur(90*16, 104*16, 1454, 0, LIBRE, 0);
                for (int i = 0; i < 4; i++) gpMonde->setMurs(89*2+i,103*2+1,PLEIN);
                for (int i = 0; i < 4; i++) gpMonde->setMurs(89*2+i,103*2+2,HAUT);
                for (int i = 0; i < 4; i++) gpMonde->setMurs(89*2+i,103*2+3,SEMI_PLEIN);
            }
            if (gpMonde->motifValue(67*16,41*16)==1181 
            && gpMonde->motifValue(72*16,41*16)==1181 
            && gpMonde->motifValue(69*16,45*16)==1447) {
                gpAudio->playSound(20);
                gpMonde->passage(69,45,0);
            }
            if (gpMonde->motifValue(87*16,48*16)==1181 
            && gpMonde->motifValue(92*16,48*16)==1181 
            && gpMonde->motifValue(89*16,45*16)==1447) {
                gpAudio->playSound(20);
                gpMonde->passage(89,45,0);
            }
            if (gpMonde->motifValue(307*16,108*16)==1181 
            && gpMonde->motifValue(312*16,108*16)==1181 
            && gpMonde->motifValue(309*16,105*16)==1447) {
                gpAudio->playSound(20);
                gpMonde->passage(309,105,0);
            }
            if (gpMonde->motifValue(227*16,33*16)==1181 
            && gpMonde->motifValue(232*16,33*16)==1181 
            && gpMonde->motifValue(223*16,41*16)==1181 
            && gpMonde->motifValue(236*16,41*16)==1181 
            && gpMonde->motifValue(229*16,30*16)==1447) {
                gpAudio->playSound(20);
                gpMonde->passage(229,30,0);
            }
            break;
        case 15 :
            if (gpMonde->regionValue(0)==40*16 && gpMonde->regionValue(1)==30*16 
            && gpJoueur->hasCristal(3) && gpMonde->motifValue(49*16,43*16)==1597) {
                gpAudio->playSound(20);
                gpMonde->passage(49,45,0);
                break;
            }
            if (gpMonde->regionValue(0)==40*16 && gpMonde->regionValue(1)==30*16 
            && !gpJoueur->hasCristal(3) && gpMonde->motifValue(49*16,43*16)==1549) {
                gpAudio->playSound(20);
                gpAudio->playSpecial(1);
                gpStatut->setJauge(true);
                gpJoueur->moveY(-16);
                gpMonde->setValeur(49*16, 43*16, 1597, -1, LIBRE, 0);
                gpMonde->setValeur(50*16, 43*16, 1598, -1, LIBRE, 0);
                gpMonde->setValeur(49*16, 44*16, 1599, 0, LIBRE, 0);
                gpMonde->setValeur(50*16, 44*16, 1600, 0, LIBRE, 0);
                for (int i = 0; i < 4; i++) gpMonde->setMurs(49*2+i,43*2+1,PLEIN);
                for (int i = 0; i < 4; i++) gpMonde->setMurs(49*2+i,43*2+2,HAUT);
                for (int i = 0; i < 4; i++) gpMonde->setMurs(49*2+i,43*2+3,SEMI_PLEIN);
            }
            if (gpMonde->regionValue(0)==20*16 && gpMonde->regionValue(1)==60*16 
            && nbEnnemis()==0 && gpMonde->motifValue(20*16,66*16)==1601) {
                gpAudio->playSound(20);
                gpMonde->passage(20,66,1);
            }
            if (gpMonde->regionValue(0)==60*16 && gpMonde->regionValue(1)==60*16 
            && nbEnnemis()==0 && gpMonde->motifValue(80*16,66*16)==1601) {
                gpAudio->playSound(20);
                gpMonde->passage(80,66,1);
            }
            if (gpMonde->regionValue(0)<=20*16 && gpMonde->regionValue(1)==45*16 
            && nbEnnemis()==0 && gpMonde->motifValue(20*16,51*16)==1601) {
                gpAudio->playSound(20);
                gpMonde->passage(20,51,1);
            }
            if (gpMonde->regionValue(0)==140*16 && gpMonde->regionValue(1)==45*16 
            && nbEnnemis()==0 && gpMonde->motifValue(140*16,51*16)==1601) {
                gpAudio->playSound(20);
                gpMonde->passage(140,51,1);
            }
            if (gpMonde->regionValue(0)==140*16 && gpMonde->regionValue(1)==15*16 
            && nbEnnemis()==0 && gpMonde->motifValue(140*16,21*16)==1601) {
                gpAudio->playSound(20);
                gpMonde->passage(140,21,1);
            }
            if (gpMonde->regionValue(0)==80*16 && gpMonde->regionValue(1)==15*16 
            && nbEnnemis()==0 && gpMonde->motifValue(89*16,15*16)==1593) {
                gpAudio->playSound(20);
                gpMonde->passage(89,15,0);
            }
            if (gpMonde->regionValue(0)==160*16 && gpMonde->regionValue(1)==30*16 
            && nbEnnemis()==0 && gpMonde->motifValue(169*16,30*16)==1593) {
                gpAudio->playSound(20);
                gpMonde->passage(169,30,0);
            }
            break;
        case 16 :
            if (gpMonde->regionValue(0)==160*16 && gpMonde->regionValue(1)==45*16 
            && gpJoueur->hasCristal(4) && gpMonde->motifValue(169*16,58*16)==1718) {
                gpAudio->playSound(20);
                gpMonde->passage(169,60,0);
                break;
            }
            if (gpMonde->regionValue(0)==160*16 && gpMonde->regionValue(1)==45*16 
            && !gpJoueur->hasCristal(4) && gpMonde->motifValue(169*16,58*16)==1668) {
                gpAudio->playSound(20);
                gpAudio->playSpecial(1);
                gpStatut->setJauge(true);
                gpJoueur->moveY(-16);
                gpMonde->setValeur(169*16, 58*16, 1718, -1, LIBRE, 0);
                gpMonde->setValeur(170*16, 58*16, 1719, -1, LIBRE, 0);
                gpMonde->setValeur(169*16, 59*16, 1720, 0, LIBRE, 0);
                gpMonde->setValeur(170*16, 59*16, 1721, 0, LIBRE, 0);
                for (int i = 0; i < 4; i++) gpMonde->setMurs(169*2+i,58*2+1,PLEIN);
                for (int i = 0; i < 4; i++) gpMonde->setMurs(169*2+i,58*2+2,HAUT);
                for (int i = 0; i < 4; i++) gpMonde->setMurs(169*2+i,58*2+3,SEMI_PLEIN);
            }
            if (gpMonde->regionValue(0)==20*16 && gpMonde->regionValue(1)==0 
            && nbEnnemis()==0 && gpMonde->motifValue(40*16,6*16)==1722) {
                gpAudio->playSound(20);
                gpMonde->passage(40,6,1);
            }
            if (gpMonde->regionValue(0)==40*16 && gpMonde->regionValue(1)==0 
            && nbEnnemis()==0 && gpMonde->motifValue(40*16,6*16)==1722) {
                gpAudio->playSound(20);
                gpMonde->passage(40,6,1);
            }
            if (gpMonde->regionValue(0)==20*16 && gpMonde->regionValue(1)==60*16 
            && nbEnnemis()==0 && gpMonde->motifValue(40*16,66*16)==1722) {
                gpAudio->playSound(20);
                gpMonde->passage(40,66,1);
            }
            if (gpMonde->regionValue(0)==40*16 && gpMonde->regionValue(1)==60*16 
            && nbEnnemis()==0 && gpMonde->motifValue(40*16,66*16)==1722) {
                gpAudio->playSound(20);
                gpMonde->passage(40,66,1);
            }
            if (gpMonde->regionValue(0)==160*16 && gpMonde->regionValue(1)==30*16 
            && nbEnnemis()==0 && gpMonde->motifValue(169*16,30*16)==1714) {
                gpAudio->playSound(20);
                gpMonde->passage(169,30,0);
            }
            break;
        case 17 :
            if (gpMonde->regionValue(0)==20*16 && gpMonde->regionValue(1)==0 
            && gpJoueur->hasCristal(5) && gpMonde->motifValue(29*16,13*16)==1850) {
                gpAudio->playSound(20);
                gpMonde->passage(29,15,0);
                break;
            }
            if (gpMonde->regionValue(0)==20*16 && gpMonde->regionValue(1)==0 
            && !gpJoueur->hasCristal(5) && gpMonde->motifValue(29*16,13*16)==1808) {
                gpAudio->playSound(20);
                gpAudio->playSpecial(1);
                gpStatut->setJauge(true);
                gpJoueur->moveY(-16);
                gpMonde->setValeur(29*16, 13*16, 1850, -1, LIBRE, 0);
                gpMonde->setValeur(30*16, 13*16, 1851, -1, LIBRE, 0);
                gpMonde->setValeur(29*16, 14*16, 1852, 0, LIBRE, 0);
                gpMonde->setValeur(30*16, 14*16, 1853, 0, LIBRE, 0);
                for (int i = 0; i < 4; i++) gpMonde->setMurs(29*2+i,13*2+1,PLEIN);
                for (int i = 0; i < 4; i++) gpMonde->setMurs(29*2+i,13*2+2,HAUT);
                for (int i = 0; i < 4; i++) gpMonde->setMurs(29*2+i,13*2+3,SEMI_PLEIN);
            }
            if (gpMonde->motifValue(102*16,107*16)==1181 
            && gpMonde->motifValue(137*16,107*16)==1181 
            && gpMonde->motifValue(102*16,132*16)==1181 
            && gpMonde->motifValue(137*16,132*16)==1181 
            && gpMonde->motifValue(100*16,126*16)==1854) {
                gpAudio->playSound(20);
                gpMonde->passage(100,126,1);
            }
            if (gpMonde->motifValue(19*16,119*16)==1181 
            && gpMonde->motifValue(40*16,126*16)==1854) {
                gpAudio->playSound(20);
                gpMonde->passage(40,126,1);
            }
            if (gpMonde->motifValue(77*16,17*16)==1181 
            && gpMonde->motifValue(62*16,27*16)==1181 
            && gpMonde->motifValue(77*16,27*16)==1181 
            && gpMonde->motifValue(69*16,15*16)==1846) {
                gpAudio->playSound(20);
                gpMonde->passage(69,15,0);
            }
            if (gpMonde->motifValue(62*16,32*16)==1181 
            && gpMonde->motifValue(77*16,32*16)==1181 
            && gpMonde->motifValue(62*16,42*16)==1181 
            && gpMonde->motifValue(77*16,42*16)==1181 
            && gpMonde->motifValue(69*16,30*16)==1846) {
                gpAudio->playSound(20);
                gpMonde->passage(69,30,0);
            }
            if (gpMonde->motifValue(68*16,62*16)==1181 
            && gpMonde->motifValue(71*16,62*16)==1181 
            && gpMonde->motifValue(69*16,60*16)==1846) {
                gpAudio->playSound(20);
                gpMonde->passage(69,60,0);
            }
            if (gpMonde->motifValue(63*16,52*16)==1181 
            && gpMonde->motifValue(76*16,52*16)==1181 
            && gpMonde->motifValue(69*16,45*16)==1846) {
                gpAudio->playSound(20);
                gpMonde->passage(69,45,0);
            }
            if (gpMonde->motifValue(68*16,137*16)==1181 
            && gpMonde->motifValue(71*16,137*16)==1181 
            && gpMonde->motifValue(69*16,135*16)==1846) {
                gpAudio->playSound(20);
                gpMonde->passage(69,135,0);
            }
            if (gpMonde->motifValue(62*16,125*16)==1181 
            && gpMonde->motifValue(62*16,129*16)==1181 
            && gpMonde->motifValue(60*16,126*16)==1854) {
                gpAudio->playSound(20);
                gpMonde->passage(60,126,1);
            }
            if (gpMonde->motifValue(77*16,125*16)==1181 
            && gpMonde->motifValue(77*16,129*16)==1181 
            && gpMonde->motifValue(80*16,126*16)==1854) {
                gpAudio->playSound(20);
                gpMonde->passage(80,126,1);
            }
            if (gpMonde->motifValue(69*16,97*16)==1181 
            && gpMonde->motifValue(70*16,97*16)==1181 
            && gpMonde->motifValue(60*16,96*16)==1854) {
                gpAudio->playSound(20);
                gpMonde->passage(60,96,1);
                gpMonde->passage(80,96,1);
            }
            if (gpMonde->regionValue(0)==40*16 && gpMonde->regionValue(1)==135*16 
            && nbEnnemis()<=1 && gpMonde->motifValue(40*16,141*16)==1854) {
                gpAudio->playSound(20);
                gpMonde->passage(40,141,1);
            }
            if (gpMonde->regionValue(0)==80*16 && gpMonde->regionValue(1)==135*16 
            && nbEnnemis()<=1 && gpMonde->motifValue(100*16,141*16)==1854) {
                gpAudio->playSound(20);
                gpMonde->passage(100,141,1);
            }
            if (gpMonde->regionValue(0)==320*3 && gpMonde->regionValue(1)==240*7
            && gpMonde->motifValue(68*16,137*16)!=1181) {
                gpMonde->passage(69,135,0); 
                gpMonde->setValeur(68*16,137*16,1181,-1,PLEIN,C_RIEN); 
                gpMonde->setValeur(71*16,137*16,1181,-1,PLEIN,C_RIEN);
            }
            break;
        case 18 :
            if (gpMonde->regionValue(0)==140*16 && gpMonde->regionValue(1)==15*16 
            && gpJoueur->hasCristal(6) && gpMonde->motifValue(149*16,28*16)==1969) {
                gpAudio->playSound(20);
                gpMonde->passage(149,30,0);
                break;
            }
            if (gpMonde->regionValue(0)==140*16 && gpMonde->regionValue(1)==15*16 
            && !gpJoueur->hasCristal(6) && gpMonde->motifValue(149*16,28*16)==1939) {
                gpAudio->playSound(20);
                gpAudio->playSpecial(1);
                gpStatut->setJauge(true);
                gpJoueur->moveY(-16);
                gpMonde->setValeur(149*16, 28*16, 1969, -1, LIBRE, 0);
                gpMonde->setValeur(150*16, 28*16, 1970, -1, LIBRE, 0);
                gpMonde->setValeur(149*16, 29*16, 1971, 0, LIBRE, 0);
                gpMonde->setValeur(150*16, 29*16, 1972, 0, LIBRE, 0);
                for (int i = 0; i < 4; i++) gpMonde->setMurs(149*2+i,28*2+1,PLEIN);
                for (int i = 0; i < 4; i++) gpMonde->setMurs(149*2+i,28*2+2,HAUT);
                for (int i = 0; i < 4; i++) gpMonde->setMurs(149*2+i,28*2+3,SEMI_PLEIN);
            }
            if (gpMonde->motifValue(17*16,65*16)==1455 
            && gpMonde->motifValue(17*16,69*16)==1455 
            && gpMonde->motifValue(20*16,66*16)==1973) {
                gpAudio->playSound(20);
                gpMonde->passage(20,66,1);
            }
            if (gpMonde->motifValue(182*16,65*16)==1455 
            && gpMonde->motifValue(182*16,69*16)==1455 
            && gpMonde->motifValue(180*16,66*16)==1973) {
                gpAudio->playSound(20);
                gpMonde->passage(180,66,1);
            }
            if (gpMonde->motifValue(188*16,62*16)==1455 
            && gpMonde->motifValue(191*16,62*16)==1455 
            && gpMonde->motifValue(189*16,60*16)==1965) {
                gpAudio->playSound(20);
                gpMonde->passage(189,60,0);
            }
            if (gpMonde->regionValue(0)==20*16 && gpMonde->regionValue(1)==60*16 
            && nbEnnemis()==0 && gpMonde->motifValue(29*16,60*16)==1965) {
                gpAudio->playSound(20);
                gpMonde->passage(29,60,0);
            }
            if (gpMonde->regionValue(0)==60*16 && gpMonde->regionValue(1)==60*16 
            && nbEnnemis()==0 && gpMonde->motifValue(80*16,66*16)==1973) {
                gpAudio->playSound(20);
                gpMonde->passage(80,66,1);
            }
            break;
        case 19 :
            if (gpMonde->regionValue(0)==20*16 && gpMonde->regionValue(1)==45*16 
            && !nbEnnemis() && gpMonde->motifValue(29*16,43*16)==2115) {
                gpAudio->playSound(20);
                gpMonde->passage(29,45,0);
                break;
            }
            if (gpMonde->regionValue(0)==20*16 && gpMonde->regionValue(1)==45*16 
            && gpJoueur->getEpee()==5 && gpMonde->motifValue(29*16,58*16)==2115) {
                gpAudio->playSound(20);
                gpMonde->passage(29,60,0);
                break;
            }
            if (gpMonde->regionValue(0)==20*16 && gpMonde->regionValue(1)==45*16 
            && nbEnnemis() && gpMonde->motifValue(29*16,58*16)==2071) {
                gpAudio->playSound(20);
                gpAudio->playSpecial(1);
                gpStatut->setJauge(true);
                gpJoueur->moveY(-16);
                gpMonde->setValeur(29*16, 58*16, 2115, -1, LIBRE, 0);
                gpMonde->setValeur(30*16, 58*16, 2116, -1, LIBRE, 0);
                gpMonde->setValeur(29*16, 59*16, 2117, 0, LIBRE, 0);
                gpMonde->setValeur(30*16, 59*16, 2118, 0, LIBRE, 0);
                for (int i = 0; i < 4; i++) gpMonde->setMurs(29*2+i,58*2+1,PLEIN);
                for (int i = 0; i < 4; i++) gpMonde->setMurs(29*2+i,58*2+2,HAUT);
                for (int i = 0; i < 4; i++) gpMonde->setMurs(29*2+i,58*2+3,SEMI_PLEIN);
            }
            if (gpMonde->regionValue(0)==40*16 && gpMonde->regionValue(1)==135*16 
            && nbEnnemis()==0 && gpMonde->motifValue(49*16,135*16)==2111) {
                gpAudio->playSound(20);
                gpMonde->passage(49,135,0);
            }
            if (gpMonde->regionValue(0)==0 && gpMonde->regionValue(1)==90*16 
            && nbEnnemis()==0 && gpMonde->motifValue(9*16,105*16)==2111) {
                gpAudio->playSound(20);
                gpMonde->passage(9,105,0);
            }
            if (gpMonde->regionValue(0)==60*16 && gpMonde->regionValue(1)==105*16 
            && nbEnnemis()==0 && gpMonde->motifValue(69*16,105*16)==2111) {
                gpAudio->playSound(20);
                gpMonde->passage(69,105,0);
            }
            if (gpMonde->regionValue(0)==40*16 && gpMonde->regionValue(1)==15*16 
            && nbEnnemis()==0 && gpMonde->motifValue(49*16,15*16)==2111) {
                gpAudio->playSound(20);
                gpMonde->passage(49,15,0);
            }
            if (gpMonde->regionValue(0)==40*16 && gpMonde->regionValue(1)==105*16 
            && nbEnnemis()==0 && gpMonde->motifValue(60*16,111*16)==2119) {
                gpAudio->playSound(20);
                gpMonde->passage(60,111,1);
            }
            break;
        case 20 :
            if (gpMonde->regionValue(0)==40*16 && gpMonde->regionValue(1)==75*16 
            && gpMonde->motifValue(49*16,88*16)==2203) {
                gpAudio->playSound(20);
                gpAudio->playSpecial(4);
                gpStatut->setJauge(true);
                gpJoueur->moveY(-16);
                gpMonde->setValeur(49*16, 88*16, 2247, -1, LIBRE, 0);
                gpMonde->setValeur(50*16, 88*16, 2248, -1, LIBRE, 0);
                gpMonde->setValeur(49*16, 89*16, 2249, 0, LIBRE, 0);
                gpMonde->setValeur(50*16, 89*16, 2250, 0, LIBRE, 0);
                for (int i = 0; i < 4; i++) gpMonde->setMurs(49*2+i,88*2+1,PLEIN);
                for (int i = 0; i < 4; i++) gpMonde->setMurs(49*2+i,88*2+2,HAUT);
                for (int i = 0; i < 4; i++) gpMonde->setMurs(49*2+i,88*2+3,SEMI_PLEIN);
            }
            if (gpMonde->regionValue(0)==140*16 && gpMonde->regionValue(1)==15*16 
            && gpMonde->motifValue(149*16,28*16)==2327 && gpJoueur->getCle(8,3)>0) {
                gpAudio->playSound(20);
                gpJoueur->moveY(-16);
                gpMonde->setValeur(149*16, 28*16, 2363, -1, LIBRE, 0);
                gpMonde->setValeur(150*16, 28*16, 2364, -1, LIBRE, 0);
                gpMonde->setValeur(149*16, 29*16, 2365, 0, LIBRE, 0);
                gpMonde->setValeur(150*16, 29*16, 2366, 0, LIBRE, 0);
                gpMonde->setValeur(149*16, 30*16, 2359, -1, LIBRE, 0);
                gpMonde->setValeur(150*16, 30*16, 2360, -1, LIBRE, 0);
                gpMonde->setValeur(149*16, 31*16, 2361, 0, LIBRE, 0);
                gpMonde->setValeur(150*16, 31*16, 2362, 0, LIBRE, 0);
                for (int i = 0; i < 4; i++) gpMonde->setMurs(149*2+i,28*2+1,PLEIN);
                for (int i = 0; i < 4; i++) gpMonde->setMurs(149*2+i,28*2+2,HAUT);
                for (int i = 0; i < 4; i++) gpMonde->setMurs(149*2+i,28*2+3,SEMI_PLEIN);
                for (int i = 0; i < 4; i++) gpMonde->setMurs(149*2+i,28*2+4,SEMI_PLEIN);
                for (int i = 0; i < 4; i++) gpMonde->setMurs(149*2+i,28*2+5,BAS);
                for (int i = 0; i < 4; i++) gpMonde->setMurs(149*2+i,28*2+6,PLEIN);
            }
            if (gpMonde->regionValue(0)==0 && gpMonde->regionValue(1)==15*16 
            && nbEnnemis()==0 && gpMonde->motifValue(9*16,15*16)==2243) {
                gpAudio->playSound(20);
                gpMonde->passage(9,15,0);
            }
            if (gpMonde->regionValue(0)==20*16 && gpMonde->regionValue(1)==15*16 
            && nbEnnemis()==0 && gpMonde->motifValue(29*16,15*16)==2243) {
                gpAudio->playSound(20);
                gpMonde->passage(29,15,0);
            }
            if (gpMonde->regionValue(0)==120*16 && gpMonde->regionValue(1)==30*16 
            && nbEnnemis()==0 && gpMonde->motifValue(129*16,30*16)==2359) {
                gpAudio->playSound(20);
                gpMonde->passage(129,30,0);
            }
            if (gpMonde->regionValue(0)==160*16 && gpMonde->regionValue(1)==90*16 
            && gpJoueur->getCle(8,2) && gpMonde->motifValue(160*16,96*16)==2367) {
                gpAudio->playSound(20);
                gpMonde->passage(160,96,1);
            }
            if (gpMonde->regionValue(0)==320*2 && gpMonde->regionValue(1)==0
            && gpMonde->motifValue(49*16,16*16)==2276) {
                gpMonde->passage(49,15,0);
            }
            break;
        case 23 :
            if (gpMonde->regionValue(0)==20*16 && gpMonde->regionValue(1)==0
            && gpMonde->motifValue(19*16,6*16)==1861) {
                gpAudio->playSound(20);
                gpMonde->passage(20,6,1);
            }
            if (gpMonde->regionValue(0)==0 && gpMonde->regionValue(1)==0 
            && gpMonde->motifValue(18*16,6*16)!=1860) {
                gpAudio->playSound(20);
                gpJoueur->moveX(-16);
                gpMonde->setValeur(18*16, 6*16, 1860, -1, LIBRE, 0);
                gpMonde->setValeur(19*16, 6*16, 1861, -1, LIBRE, 0);
                gpMonde->setValeur(18*16, 7*16, 1862, 0, LIBRE, 0);
                gpMonde->setValeur(19*16, 7*16, 1863, 0, LIBRE, 0);
                gpMonde->setValeur(18*16, 8*16, 1864, 0, LIBRE, 0);
                gpMonde->setValeur(19*16, 8*16, 1865, 0, LIBRE, 0);
                for (int i = 0; i < 6; i++) gpMonde->setMurs(18*2+1,6*2+i,PLEIN);
                for (int i = 0; i < 6; i++) gpMonde->setMurs(18*2+2,6*2+i,GAUCHE);
                for (int i = 0; i < 6; i++) gpMonde->setMurs(18*2+3,6*2+i,SEMI_PLEIN);
            }
            break;
    }
}

void Jeu::finZone() {
    gpJoueur->setX(newx);
    gpJoueur->setY(newy);
    gpJoueur->saveSemiEmpl();
    
    if (zone != newzone) {
        gpJoueur->saveEmpl();
        videListes();
        zone = newzone;
        if (isDonjon() && !gpJoueur->getCharge() && (gpMonde->regionValue(1) || zone!=20)) 
            gpJoueur->setDirection(N);
        if (zone==4 && newx==16*42 && newy==16*5-8) gpJoueur->setDirection(E);
        gpMonde->changeZone(zone);
        if (isDonjon()) ecrit(206+zone-12);
        gpAudio->playMusic(zone);
    } else gpMonde->detRegion(zone);
    
    if (gpJoueur->getTypeAnim()==EMMENE) gpJoueur->setTypeAnim(MARCHE);
    gpJoueur->visible();
    scrolling();
}

void Jeu::testFin() {
    if (transition) return;
    //si peut ramasser un objet
    testRamasse(gpJoueur->getX(), gpJoueur->getY()+8, 16, 16);
    //si dans l'eau
    if (isWater(gpJoueur->getX(), gpJoueur->getY()+8, 16,16)) {
        gpJoueur->bigPlouf();
    }else {
        gpJoueur->antiPlouf();
        if (gpJoueur->getTypeAnim()==FLOTTE || gpJoueur->getTypeAnim()==NAGE) 
            gpJoueur->setTypeAnim(AUCUNE);
    }
    //si tombe dans le vide
    if (isVide(gpJoueur->getX(), gpJoueur->getY()+8, 16,16)) {
        gpAudio->playSound(12);
        gpJoueur->setTypeAnim(CHUTE);
    }
    switch(zone) {
        case 1 :
            if (gpJoueur->getX()<6 && gpJoueur->getY()<240) 
                gpMonde->transitZone(4, -6, 0, O);
            if (gpJoueur->getX()<6 && gpJoueur->getY()>240) 
                gpMonde->transitZone(2, -5, 3, O);
            if (gpJoueur->getY()>240*4+16*13+8-4) 
                gpMonde->transitZone(6, 0, 5, S);
            if (((int)(gpJoueur->getX()/16))==29 && ((int)((gpJoueur->getY()-8)/16))==34)
                gpMonde->transitFull(21, 152, 200);
            if (((int)(gpJoueur->getX()/16))==55 && ((int)((gpJoueur->getY()-8)/16))==0)
                gpMonde->transitFull(25, 152, 200);
            if (((int)(gpJoueur->getX()/16))==29 && ((int)(gpJoueur->getY()/16))==19)
                gpMonde->transitFull(12, 152, 1160);
            if (((int)(gpJoueur->getX()/16))==49 && ((int)(gpJoueur->getY()/16))==18)
                gpMonde->transitFull(12, 632, 1160);
            if (((int)(gpJoueur->getX()/16))==49 && ((int)(gpJoueur->getY()/16))==4)
                gpMonde->transitFull(16, 152, 200);
            break;
        case 2 :
            if (gpJoueur->getX()>320*4+16*18+8 && gpJoueur->getY()<240) 
                gpMonde->transitZone(1, 5, -3, E);
            if (gpJoueur->getX()>320*4+16*18+8 && gpJoueur->getY()>240) 
                gpMonde->transitZone(6, 5, 2, E);
            if (gpJoueur->getY()<6) 
                gpMonde->transitZone(4, -1, -3, N);
            if (gpJoueur->getY()>240*3+16*13+8-4) 
                gpMonde->transitZone(3, -2, 4, S);
            if (gpJoueur->getX()<6) 
                gpMonde->transitZone(8, -3, 0, O);
            if (((int)(gpJoueur->getX()/16))==72 && ((int)((gpJoueur->getY()-8)/16))==38)
                gpMonde->transitFull(44, 152, 200);
            break;
        case 3 :
            if (gpJoueur->getY()<6 && gpJoueur->getX()>320*2) 
                gpMonde->transitZone(2, 2, -4, N);
            if (gpJoueur->getY()<6 && gpJoueur->getX()<=320*2) 
                gpMonde->transitZone(8, -1, -4, N);
            if (gpJoueur->getX()>320*6+16*18+8) 
                gpMonde->transitZone(6, 7, -2, E);
            if (gpJoueur->getX()<6) 
                gpMonde->transitZone(7, -6, 0, O);
            if (((int)(gpJoueur->getX()/16))==88 && ((int)(gpJoueur->getY()/16))==34)
                gpMonde->transitFull(13, 1112, 1400);
            if (((int)(gpJoueur->getX()/16))==26 && ((int)(gpJoueur->getY()/16))==42)
                gpMonde->transitFull(18, 792, 1160);
            if (((int)(gpJoueur->getX()/16))==126 && ((int)((gpJoueur->getY()-8)/16))==8)
                gpMonde->transitFull(22, 792, 440);
            if (((int)(gpJoueur->getX()/16))==89 && ((int)((gpJoueur->getY()-8)/16))==44)
                gpMonde->transitFull(22, 792, 920);
            if (((int)(gpJoueur->getX()/16))==23 && ((int)((gpJoueur->getY()-8)/16))==50)
                gpMonde->transitFull(26, 152, 200);
            break;
        case 4 :
            if (gpJoueur->getY()>240*2+16*13+8-4) 
                gpMonde->transitZone(2, 1, 3, S);
            if (gpJoueur->getX()>320*5+16*18+8) 
                gpMonde->transitZone(1, 6, 0, E);
            if (gpJoueur->getX()<6) 
                gpMonde->transitZone(5, -7, 0, O);
            if (((int)(gpJoueur->getX()/16))==27 && ((int)(gpJoueur->getY()/16))==1)
                gpMonde->transitFull(14, 1432, 1880);
            if (((int)(gpJoueur->getX()/16))==40 && 
            (((int)(gpJoueur->getY()/16))==4 || ((int)(gpJoueur->getY()/16))==5))
                gpMonde->transitFull(14, 1752, 1880);
            break;
        case 5 :
            if (gpJoueur->getX()>320*6+16*18+8) 
                gpMonde->transitZone(4, 7, 0, E);
            if (gpJoueur->getY()>240*2+16*13+8-4 && gpJoueur->getX()>320*5) 
                gpMonde->transitZone(8, 5, 3, S);
            if (gpJoueur->getY()>240*2+16*13+8-4 && gpJoueur->getX()<320*5) 
                gpMonde->transitZone(9, 0, 3, S);
            if (((int)(gpJoueur->getX()/16))==14 && ((int)(gpJoueur->getY()/16))==6)
                gpMonde->transitFull(15, 792, 1160);
            if (((int)(gpJoueur->getX()/16))==131 && ((int)((gpJoueur->getY()-8)/16))==23)
                gpMonde->transitFull(24, 152, 200);
            if (((int)(gpJoueur->getX()/16))==121 && ((int)((gpJoueur->getY()-8)/16))==12)
                gpMonde->transitFull(34, 152, 200);
            if (((int)(gpJoueur->getX()/16))==121 && ((int)((gpJoueur->getY()-8)/16))==22)
                gpMonde->transitFull(35, 152, 200);
            if (((int)(gpJoueur->getX()/16))==112 && ((int)((gpJoueur->getY()-8)/16))==12)
                gpMonde->transitFull(36, 152, 200);
            if (((int)(gpJoueur->getX()/16))==103 && ((int)((gpJoueur->getY()-8)/16))==22)
                gpMonde->transitFull(37, 152, 200);
            if (((int)(gpJoueur->getX()/16))==103 && ((int)((gpJoueur->getY()-8)/16))==12)
                gpMonde->transitFull(38, 152, 200);
            break;
        case 6 :
            if (gpJoueur->getX()<6 && gpJoueur->getY()<240*2) 
                gpMonde->transitZone(2, -5, -2, O);
            if (gpJoueur->getX()<6 && gpJoueur->getY()>240*2) 
                gpMonde->transitZone(3, -7, 2, O);
            if (gpJoueur->getY()<6) 
                gpMonde->transitZone(1, 0, -5, N);
            if (((int)(gpJoueur->getX()/16))==49 && ((int)((gpJoueur->getY()-8)/16))==5)
                gpMonde->transitFull(16, 152, 1400);
            if (((int)(gpJoueur->getX()/16))==3 && ((int)((gpJoueur->getY()-8)/16))==30)
                gpMonde->transitFull(23, 152, 200);
            if (((int)(gpJoueur->getX()/16))==43 && ((int)((gpJoueur->getY()-8)/16))==39)
                gpMonde->transitFull(39, 152, 200);
            if (((int)(gpJoueur->getX()/16))==13 && ((int)((gpJoueur->getY()-8)/16))==63)
                gpMonde->transitFull(40, 152, 200);
            if (((int)(gpJoueur->getX()/16))==29 && ((int)((gpJoueur->getY()-8)/16))==51)
                gpMonde->transitFull(41, 152, 200);
            if (((int)(gpJoueur->getX()/16))==43 && ((int)((gpJoueur->getY()-8)/16))==63)
                gpMonde->transitFull(42, 152, 200);
            if (((int)(gpJoueur->getX()/16))==10 && ((int)((gpJoueur->getY()-8)/16))==47)
                gpMonde->transitFull(43, 152, 200);
            break;
        case 7 :
            if (gpJoueur->getY()<6) 
                gpMonde->transitZone(9, 0, -4, N);
            if (gpJoueur->getY()>240*3+16*13+8-4) 
                gpMonde->transitZone(10, 0, 4, S);
            if (gpJoueur->getX()>320*5+16*18+8) 
                gpMonde->transitZone(3, 6, 0, E);
            if (((int)(gpJoueur->getX()/16))==33 && ((int)(gpJoueur->getY()/16))==48)
                gpMonde->transitFull(17, 1112, 2360);
            if (((int)(gpJoueur->getX()/16))==35 && ((int)((gpJoueur->getY()-8)/16))==27)
                gpMonde->transitFull(17, 472, 440);
            if (((int)(gpJoueur->getX()/16))==28 && ((int)((gpJoueur->getY()-8)/16))==2)
                gpMonde->transitFull(27, 152, 200);
            break;
        case 8 :
            if (gpJoueur->getX()<6) 
                gpMonde->transitZone(9, -5, 0, O);
            if (gpJoueur->getY()<6) 
                gpMonde->transitZone(5, -5, -3, N);
            if (gpJoueur->getY()>240*3+16*13+8-4) 
                gpMonde->transitZone(3, 1, 4, S);
            if (gpJoueur->getX()>320*2+16*18+8) 
                gpMonde->transitZone(2, 3, 0, E);
            if (((int)(gpJoueur->getX()/16))==20 && ((int)((gpJoueur->getY()-8)/16))==27)
                gpMonde->transitFull(28, 152, 200);
            if (((int)(gpJoueur->getX()/16))==37 && ((int)((gpJoueur->getY()-8)/16))==27)
                gpMonde->transitFull(29, 152, 200);
            if (((int)(gpJoueur->getX()/16))==20 && ((int)((gpJoueur->getY()-8)/16))==35)
                gpMonde->transitFull(30, 152, 8);
            if (((int)(gpJoueur->getX()/16))==20 && ((int)((gpJoueur->getY()-8)/16))==40)
                gpMonde->transitFull(30, 152, 440);
            if (((int)(gpJoueur->getX()/16))==37 && ((int)((gpJoueur->getY()-8)/16))==37)
                gpMonde->transitFull(31, 152, 200);
            if (((int)(gpJoueur->getX()/16))==20 && ((int)((gpJoueur->getY()-8)/16))==15)
                gpMonde->transitFull(32, 472, 200);
            if (((int)(gpJoueur->getX()/16))==16 && ((int)((gpJoueur->getY()-8)/16))==15)
                gpMonde->transitFull(32, 152, 200);
            if (((int)(gpJoueur->getX()/16))==37 && ((int)((gpJoueur->getY()-8)/16))==15)
                gpMonde->transitFull(33, 152, 200);
            break;
        case 9 :
            if (gpJoueur->getY()<6) 
                gpMonde->transitZone(5, 0, -3, N);
            if (gpJoueur->getY()>240*3+16*13+8-4) 
                gpMonde->transitZone(7, 0, 4, S);
            if (gpJoueur->getX()>320*4+16*18+8) 
                gpMonde->transitZone(8, 5, 0, E);
            if ((((int)(gpJoueur->getX()/16))==64 || ((int)(gpJoueur->getX()/16))==65) 
            && ((int)(gpJoueur->getY()/16))==24)
                gpMonde->transitFull(20, 792, 920);
            if ((((int)(gpJoueur->getX()/16))==64 || ((int)(gpJoueur->getX()/16))==65) 
            && ((int)(gpJoueur->getY()/16))==19)
                gpMonde->transitFull(20, 792, 1640);
            //escalier vers b
            if (gpMonde->motifValue(gpJoueur->getX(),gpJoueur->getY()+16) == 932) {
                if (gpJoueur->getAvancement()==6) {
                    ecrit(96);
                    gpJoueur->setY(gpJoueur->getY()-4);
                    gpJoueur->setDirection(N);
                    break;
                }
                gpAudio->playSound(11);
                gpMonde->transitFull(20, 1432, 136);
            }
            if (((int)(gpJoueur->getX()/16))>=62 && 
            ((int)(gpJoueur->getX()/16))<=66 && ((int)(gpJoueur->getY()/16))==31
            && (gpJoueur->nbCristaux()<7)) {  //entre chateau sans cristaux
                gpAudio->playSound(5);
                SDL_Delay(1500);
                gpJoueur->restoreSemiEmpl();
            }
            break;
        case 10 :
            if (gpJoueur->getY()<6) 
                gpMonde->transitZone(7, 0, -4, N);
            if (gpJoueur->getX()>320*7+16*18+8) 
                gpMonde->transitZone(11, 8, 0, E);
            break;
        case 11 :
            if (gpJoueur->getX()<6) 
                gpMonde->transitZone(10, -8, 0, O);
            if (((int)(gpJoueur->getX()/16))==145 && ((int)((gpJoueur->getY()-8)/16))==2)
                gpMonde->transitFull(19, 472, 2360);
            if (((int)(gpJoueur->getX()/16))==156 && ((int)((gpJoueur->getY()-8)/16))==2)
                gpMonde->transitFull(23, 1112, 1160);
            break;
        case 12 :
            if (gpJoueur->getY()>gpMonde->regionValue(3)-28 
            && gpMonde->regionValue(0)==20*16 && gpMonde->regionValue(1)==60*16)
                gpMonde->transitFull(1, 16*49+8, 16*21-8);
            if (gpJoueur->getY()>gpMonde->regionValue(3)-28 
            && gpMonde->regionValue(0)==0 && gpMonde->regionValue(1)==60*16)
                gpMonde->transitFull(1, 16*29+8, 16*22-8);
            //escaliers vers h
            if (gpMonde->motifValue(gpJoueur->getX(),gpJoueur->getY()+15)==1172) {
                gpAudio->playSound(10);
                gpMonde->transitFull(12, gpJoueur->getX()-(16*80), gpJoueur->getY()-16);
                break;
            }
            //escalier vers b
            if (gpMonde->motifValue(gpJoueur->getX(),gpJoueur->getY()+16)==1178) {
                gpAudio->playSound(11);
                gpMonde->transitFull(12, gpJoueur->getX()+(16*80), gpJoueur->getY()+16);
                break;
            }
            break;
        case 13 :
            if (gpJoueur->getY()>gpMonde->regionValue(3)-28 
            && gpMonde->regionValue(0)==60*16 && gpMonde->regionValue(1)==75*16)
                gpMonde->transitFull(3, 16*88+8, 16*37-8);
            break;
        case 14 :
            if (gpJoueur->getY()>gpMonde->regionValue(3)-28 
            && gpMonde->regionValue(0)==80*16 && gpMonde->regionValue(1)==105*16)
                gpMonde->transitFull(4, 16*27, 16*3-8);
            if (gpJoueur->getY()>gpMonde->regionValue(3)-28 
            && gpMonde->regionValue(0)==100*16 && gpMonde->regionValue(1)==105*16)
                gpMonde->transitFull(4, 16*42, 16*5-8);
            //escaliers vers h
            if (gpMonde->motifValue(gpJoueur->getX(),gpJoueur->getY()+15)==1476) {
                gpAudio->playSound(10);
                gpMonde->transitFull(14, gpJoueur->getX()+(16*160), gpJoueur->getY()-16);
                break;
            }
            //escalier vers b
            if (gpMonde->motifValue(gpJoueur->getX(),gpJoueur->getY()+16)==1482) {
                gpAudio->playSound(11);
                gpMonde->transitFull(14, gpJoueur->getX()-(16*160), gpJoueur->getY()+16);
                break;
            }
            break;
        case 15 :
            if (gpJoueur->getY()>gpMonde->regionValue(3)-28 
            && gpMonde->regionValue(0)==40*16 && gpMonde->regionValue(1)==45*16)
                gpMonde->transitFull(5, 16*14+8, 16*8-8);
            //escaliers vers h
            if (gpMonde->motifValue(gpJoueur->getX(),gpJoueur->getY()+15)==1613) {
                gpAudio->playSound(10);
                gpMonde->transitFull(15, gpJoueur->getX()+(16*100), gpJoueur->getY()-16);
                break;
            }
            //escalier vers b
            if (gpMonde->motifValue(gpJoueur->getX(),gpJoueur->getY()+16)==1619) {
                gpAudio->playSound(11);
                gpMonde->transitFull(15, gpJoueur->getX()-(16*100), gpJoueur->getY()+16);
                break;
            }
            break;
        case 16 :
            if (gpJoueur->getY()>gpMonde->regionValue(3)-28 
            && gpMonde->regionValue(0)==0 && gpMonde->regionValue(1)==0)
                gpMonde->transitFull(1, 16*49+8, 16*6-8);
            if (gpJoueur->getY()>gpMonde->regionValue(3)-28 
            && gpMonde->regionValue(0)==0 && gpMonde->regionValue(1)==75*16)
                gpMonde->transitFull(6, 16*49+8, 16*8-8);
            //escaliers vers h
            if (gpMonde->motifValue(gpJoueur->getX(),gpJoueur->getY()+15)==1738) {
                gpAudio->playSound(10);
                int decal=20*16; if (gpMonde->regionValue(0)>80*16) decal=80*16;
                gpMonde->transitFull(16, gpJoueur->getX()-decal, gpJoueur->getY()-16);
                break;
            }
            //escalier vers b
            if (gpMonde->motifValue(gpJoueur->getX(),gpJoueur->getY()+16)==1744) {
                gpAudio->playSound(11);
                int decal=20*16; if (gpMonde->regionValue(0)>0) decal=80*16;
                gpMonde->transitFull(16, gpJoueur->getX()+decal, gpJoueur->getY()+16);
                break;
            }
            break;
        case 17 :
            if (gpJoueur->getY()>gpMonde->regionValue(3)-28 
            && gpMonde->regionValue(0)==320 && gpMonde->regionValue(1)==240)
                gpMonde->transitFull(7, 16*35+8, 16*30-8);
            if (gpJoueur->getY()>gpMonde->regionValue(3)-28 
            && gpMonde->regionValue(0)==320*3 && gpMonde->regionValue(1)==240*9)
                gpMonde->transitFull(7, 16*33, 16*51-8);
            //escaliers vers h
            if (gpMonde->motifValue(gpJoueur->getX(),gpJoueur->getY()+15)==1870) {
                gpAudio->playSound(10);
                gpMonde->transitFull(17, gpJoueur->getX()-(16*40), gpJoueur->getY()-(16*90)-16);
                break;
            }
            //escalier vers b
            if (gpMonde->motifValue(gpJoueur->getX(),gpJoueur->getY()+16)==1876) {
                gpAudio->playSound(11);
                int decal=20*16; if (gpMonde->regionValue(0)>0) decal=80*16;
                gpMonde->transitFull(17, gpJoueur->getX()+(16*40), gpJoueur->getY()+(16*90)+16);
                break;
            }
            break;
        case 18 :
            if (gpJoueur->getY()>gpMonde->regionValue(3)-28 
            && gpMonde->regionValue(0)==320*2 && gpMonde->regionValue(1)==240*4)
                gpMonde->transitFull(3, 16*26+8, 16*44);
            //escaliers vers h
            if (gpMonde->motifValue(gpJoueur->getX(),gpJoueur->getY()+15)==1989) {
                gpAudio->playSound(10);
                gpMonde->transitFull(18, gpJoueur->getX()+(16*100), gpJoueur->getY()-16);
                break;
            }
            //escalier vers b
            if (gpMonde->motifValue(gpJoueur->getX(),gpJoueur->getY()+16)==1995) {
                gpAudio->playSound(11);
                gpMonde->transitFull(18, gpJoueur->getX()-(16*100), gpJoueur->getY()+16);
                break;
            }
            break;
        case 19 :
            if (gpJoueur->getY()>gpMonde->regionValue(3)-28 
            && gpMonde->regionValue(0)==320 && gpMonde->regionValue(1)==240*9)
                gpMonde->transitFull(11, 16*145, 16*4);
            break;
        case 20 :
            if (gpJoueur->getY()>gpMonde->regionValue(3)-28 
            && gpMonde->regionValue(0)==320 && gpMonde->regionValue(1)==240*3) {
                if (gpJoueur->getAvancement()==5 || gpJoueur->getAvancement()==6) {
                    gpJoueur->moveY(-4);
                    gpJoueur->setDirection(N);
                    ecrit(93);
                }
                else gpMonde->transitFull(9, 16*64+8, 16*26+8);
            }
            if (gpJoueur->getY()>gpMonde->regionValue(3)-28 
            && gpMonde->regionValue(0)==320*2 && gpMonde->regionValue(1)==240*6)
                gpMonde->transitFull(9, 16*64+8, 16*21+8);
            if (gpJoueur->getX()>16*159-8 && gpMonde->regionValue(0)==140*16 
            && gpMonde->regionValue(1)==15*16 && gpJoueur->getAvancement()<5) { //oublie zelda
                ecrit(95);
                gpJoueur->moveX(-4);
                gpJoueur->setDirection(O);
            }
                
            if (gpMonde->motifValue(gpJoueur->getX(),gpJoueur->getY()+15)==2267) {
                if (gpJoueur->getAvancement()==5) {
                    gpJoueur->moveY(4);
                    gpJoueur->setDirection(S);
                    ecrit(94);
                }else {
                    gpAudio->playSound(10);
                    gpMonde->transitFull(9, 16*78+8, 16*16);
                }
                break;
            }
            //escaliers vers h
            if (gpMonde->motifValue(gpJoueur->getX(),gpJoueur->getY()+15)==2379) {
                gpAudio->playSound(10);
                int decalx=20*16; if (gpMonde->regionValue(1)==960) decalx=80*16;
                int decaly=30*16; if (gpMonde->regionValue(1)>960) decaly=-30*16;
                if (gpJoueur->getAvancement()==6) {
                    decaly-=16;
                    gpPnj->getSuivant()->setX(gpJoueur->getX()-decalx);
                    gpPnj->getSuivant()->setY(gpJoueur->getY()-16+decaly+24+8);
                }
                gpMonde->transitFull(20, gpJoueur->getX()-decalx, gpJoueur->getY()-16+decaly);
                break;
            }
            //escalier vers b vers ss1
            if (gpMonde->motifValue(gpJoueur->getX(),gpJoueur->getY()+16)==2273) {
                gpAudio->playSound(11);
                int decaly=-30*16; 
                if (gpJoueur->getAvancement()==6) {
                    decaly+=16;
                    gpPnj->getSuivant()->setX(gpJoueur->getX()+(80*16));
                    gpPnj->getSuivant()->setY(gpJoueur->getY()-32+16+decaly);
                }
                gpMonde->transitFull(20, gpJoueur->getX()+(80*16), gpJoueur->getY()+16+decaly);
                break;
            }
            //escalier vers b vers ss2
            if (gpMonde->motifValue(gpJoueur->getX(),gpJoueur->getY()+16)==2385) {
                gpAudio->playSound(11);
                int decaly=30*16; 
                if (gpJoueur->getAvancement()==6) {
                    decaly+=16;
                    gpPnj->getSuivant()->setX(gpJoueur->getX()+(20*16));
                    gpPnj->getSuivant()->setY(gpJoueur->getY()-32+16+decaly);
                }
                gpMonde->transitFull(20, gpJoueur->getX()+(20*16), gpJoueur->getY()+16+decaly);
                break;
            }
            break;
        case 21 :
            if (gpJoueur->getY()>240-24-4) gpMonde->transitFull(1, 472, 584);
            break;
        case 22 :
            if (gpJoueur->getY()>gpMonde->regionValue(3)-28 
            && gpMonde->regionValue(0)==320*2 && gpMonde->regionValue(1)==240*3)
                gpMonde->transitFull(3, 16*89+8, 16*46);
            if (gpJoueur->getY()>gpMonde->regionValue(3)-28 
            && gpMonde->regionValue(0)==320*2 && gpMonde->regionValue(1)==240)
                gpMonde->transitFull(3, 16*126+8, 16*10);
            //escaliers vers h
            if (gpMonde->motifValue(gpJoueur->getX(),gpJoueur->getY()+15)==1870) {
                gpAudio->playSound(10);
                int decalx=0; int decaly=15*16; 
                if (gpMonde->regionValue(1)>0) {decalx=40*16;decaly=0;}
                gpMonde->transitFull(22, gpJoueur->getX()+decalx, gpJoueur->getY()-16+decaly);
                break;
            }
            //escalier vers b
            if (gpMonde->motifValue(gpJoueur->getX(),gpJoueur->getY()+16)==1876) {
                gpAudio->playSound(11);
                int decalx=0; int decaly=15*16; 
                if (gpMonde->regionValue(1)!=240) {decalx=40*16;decaly=0;}
                gpMonde->transitFull(22, gpJoueur->getX()-decalx, gpJoueur->getY()+16-decaly);
                break;
            }
            break;
        case 23 :
            if (gpJoueur->getY()>gpMonde->regionValue(3)-28 
            && gpMonde->regionValue(0)==0 && gpMonde->regionValue(1)==0)
                gpMonde->transitFull(6, 16*3+8, 16*32);
            if (gpJoueur->getY()>gpMonde->regionValue(3)-28 
            && gpMonde->regionValue(0)==320*3 && gpMonde->regionValue(1)==240*4)
                gpMonde->transitFull(11, 16*156+8, 16*4);
            //escaliers vers h
            if (gpMonde->motifValue(gpJoueur->getX(),gpJoueur->getY()+15)==1870) {
                gpAudio->playSound(10);
                gpMonde->transitFull(23, gpJoueur->getX()-320, gpJoueur->getY()-16-240*3);
                break;
            }
            //escalier vers b
            if (gpMonde->motifValue(gpJoueur->getX(),gpJoueur->getY()+16)==1876) {
                gpAudio->playSound(11);
                gpMonde->transitFull(23, gpJoueur->getX()+320, gpJoueur->getY()+16+240*3);
                break;
            }
            break;
        case 24 :
            if (gpJoueur->getY()>gpMonde->regionValue(3)-28)
                gpMonde->transitFull(5, 16*131+8, 16*25);
            break;
        case 25 :
            if (gpJoueur->getY()>gpMonde->regionValue(3)-28)
                gpMonde->transitFull(1, 16*55+8, 16*2);
            break;
        case 26 :
            if (gpJoueur->getY()>gpMonde->regionValue(3)-28)
                gpMonde->transitFull(3, 16*23+8, 16*52);
            break;
        case 27 :
            if (gpJoueur->getY()>gpMonde->regionValue(3)-28)
                gpMonde->transitFull(7, 16*28+8, 16*4);
            break;
        case 28 :
            if (gpJoueur->getY()>gpMonde->regionValue(3)-28)
                gpMonde->transitFull(8, 16*20+8, 16*29+8);
            break;
        case 29 :
            if (gpJoueur->getY()>gpMonde->regionValue(3)-28)
                gpMonde->transitFull(8, 16*37+8, 16*29+8);
            break;
        case 30 :
            if (gpJoueur->getY()<gpMonde->regionValue(1)+6) 
                gpMonde->transitFull(8, 16*20+8, 16*33+8);
            if (gpJoueur->getY()>gpMonde->regionValue(3)-28)
                gpMonde->transitFull(8, 16*20+8, 16*42+8);
            break;
        case 31 :
            if (gpJoueur->getY()>gpMonde->regionValue(3)-28)
                gpMonde->transitFull(8, 16*37+8, 16*39+8);
            break;
        case 32 :
            if (gpJoueur->getY()>gpMonde->regionValue(3)-28 && gpJoueur->getX()<320) 
                gpMonde->transitFull(8, 16*16+8, 16*17+8);
            if (gpJoueur->getY()>gpMonde->regionValue(3)-28 && gpJoueur->getX()>=320) 
                gpMonde->transitFull(8, 16*20+8, 16*17+8);
            break;
        case 33 :
            if (gpJoueur->getY()>gpMonde->regionValue(3)-28)
                gpMonde->transitFull(8, 16*37+8, 16*17+8);
            break;
        case 34 :
            if (gpJoueur->getY()>gpMonde->regionValue(3)-28)
                gpMonde->transitFull(5, 16*121+8, 16*14+8);
            break;
        case 35 :
            if (gpJoueur->getY()>gpMonde->regionValue(3)-28)
                gpMonde->transitFull(5, 16*121+8, 16*24+8);
            break;
        case 36 :
            if (gpJoueur->getY()>gpMonde->regionValue(3)-28)
                gpMonde->transitFull(5, 16*112+8, 16*14+8);
            break;
        case 37 :
            if (gpJoueur->getY()>gpMonde->regionValue(3)-28)
                gpMonde->transitFull(5, 16*103+8, 16*24+8);
            break;
        case 38 :
            if (gpJoueur->getY()>gpMonde->regionValue(3)-28)
                gpMonde->transitFull(5, 16*103+8, 16*14+8);
            break;
        case 39 :
            if (gpJoueur->getY()>gpMonde->regionValue(3)-28)
                gpMonde->transitFull(6, 16*43+8, 16*41+8);
            break;
        case 40 :
            if (gpJoueur->getY()>gpMonde->regionValue(3)-28)
                gpMonde->transitFull(6, 16*13+8, 16*65+8);
            break;
        case 41 :
            if (gpJoueur->getY()>gpMonde->regionValue(3)-28)
                gpMonde->transitFull(6, 16*29+8, 16*53+8);
            break;
        case 42 :
            if (gpJoueur->getY()>gpMonde->regionValue(3)-28)
                gpMonde->transitFull(6, 16*43+8, 16*65+8);
            break;
        case 43 :
            if (gpJoueur->getY()>gpMonde->regionValue(3)-28)
                gpMonde->transitFull(6, 16*10+8, 16*49+8);
            break;
        case 44 :
            if (gpJoueur->getY()>gpMonde->regionValue(3)-28)
                gpMonde->transitFull(2, 16*72+8, 16*40+8);
            break;
    }
    
    if ((isDonjon() || isGrotte()) && !transition) {
        
        int dir=-1;
        
        if (gpJoueur->getY()<gpMonde->regionValue(1)+6 && !transition) {
            gpMonde->transitSalle(N);dir=N;}
        if (gpJoueur->getY()>gpMonde->regionValue(3)-28 && !transition) {
            gpMonde->transitSalle(S);dir=S;}
        if (gpJoueur->getX()<gpMonde->regionValue(0)+6 && !transition) {
            gpMonde->transitSalle(O);dir=O;}
        if (gpJoueur->getX()>gpMonde->regionValue(2)-24 && !transition) {
            gpMonde->transitSalle(E);dir=E;}
            
        //changement de zone de zelda
        if (gpJoueur->getAvancement()==6) {
           switch (dir) {
                case N :
                    gpJoueur->setY(gpJoueur->getY()-16);
                    gpPnj->getSuivant()->setX(gpJoueur->getX());
                    gpPnj->getSuivant()->setY(gpJoueur->getY()+24-32);
                    break;
                case S : 
                    gpJoueur->setY(gpJoueur->getY()+16);
                    gpPnj->getSuivant()->setX(gpJoueur->getX());
                    gpPnj->getSuivant()->setY(gpJoueur->getY()-13);
                    break;
               case O : 
                    gpJoueur->setX(gpJoueur->getX()-16);
                    gpPnj->getSuivant()->setX(gpJoueur->getX()-6);
                    gpPnj->getSuivant()->setY(gpJoueur->getY()+1);
                    break;
                case E : 
                    gpJoueur->setX(gpJoueur->getX()+16);
                    gpPnj->getSuivant()->setX(gpJoueur->getX()-10);
                    gpPnj->getSuivant()->setY(gpJoueur->getY()+1);
                    break;
            }
        }
    }
}

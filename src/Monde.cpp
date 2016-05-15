/*

    Zelda Return of the Hylian

    Copyright (C) 2005-2008  Vincent Jouillat

    Please send bugreports with examples or suggestions to www.zeldaroth.fr

*/

#include <string>
#include <sstream>
#include <fstream>

//#include <SDL/SDL.h>
//#include <SDL/SDL_gfxPrimitives.h>
#include "3ds/os_3ds.h"

#include "Menu.h"
#include "Texte.h"
#include "Joueur.h"
#include "Monde.h"
#include "Projectile.h"
#include "Jeu.h"
#include "Common.h"

Monde::Monde(Jeu* jeu) : largeur(0), hauteur(0), plafondmax(0), anim(0), 
musique(0), gpJeu(jeu), animtransX(0), animtransY(0), animtransC(0), niveau(0), charge(1) {
    
    for (int i = 0; i < 2; i++) {
        std::ostringstream im;
        im << (i+1);
        image[i] = IMG_Load(("romfs:/images/motifs/image" + im.str() + ".png").c_str());
        SDL_SetColorKey(image[i],SDL_SRCCOLORKEY,SDL_MapRGB(image[i]->format,0,0,255));
    }
    
    imagetransit = SDL_CreateRGBSurface(SDL_HWSURFACE, 320, 240, 32, 0, 0, 0, 0);
    
    imagelevel = IMG_Load("romfs:/images/menu/level.png");
    SDL_SetColorKey(imagelevel,SDL_SRCCOLORKEY,SDL_MapRGB(imagelevel->format,0,0,255));
    
    changeZone(gpJeu->getZone());
    
    charge=0;
}

Monde::~Monde() {
    for (int i = 0; i < 2; i++) SDL_FreeSurface(image[i]);
    SDL_FreeSurface(imagetransit);
    SDL_FreeSurface(imagelevel);
}

void Monde::changeZone(int newZone) {
    
    chargeMap(newZone);
    
    initMatrice(newZone);
    
    detRegion(newZone);
    
    lastAnimTime = SDL_GetTicks();
}

void Monde::corrigeMap(int zone) {
    Joueur* gpJoueur = gpJeu->getJoueur();
    switch(zone) {
        case 1 : if (gpJoueur->getCoffre(0, 0)) passage(55,1,0); break;
        case 3 :
            if (gpJoueur->getCoffre(0, 1)) passage(126,9,0);
            if (gpJoueur->getCoffre(0, 2)) passage(23,51,1); break;
        case 5 : if (gpJoueur->getCoffre(0, 3)) passage(131,24,1); break;
        case 7 :
            if (gpJoueur->getCoffre(0, 4)) passage(28,3,1);
            if (gpJoueur->getCoffre(0, 6)) passage(32,49,2); break;
        case 11 : if (gpJoueur->getCoffre(0, 5)) passage(156,3,1); break;
        case 12 :
            if(!gpJoueur->getCle(0,0)) setValeur(50*16,67*16,303,-1,PLEIN,C_CARTE); 
            else setValeur(50*16,67*16,304,-1,PLEIN,C_RIEN);
            if(!gpJoueur->getCle(0,1)) setValeur(70*16,52*16,303,-1,PLEIN,C_BOUSSOLE);
            else setValeur(70*16,52*16,304,-1,PLEIN,C_RIEN);
            if(!gpJoueur->getCle(0,2)) setValeur(9*16,22*16,303,-1,PLEIN,C_CLE_BOSS);
            else setValeur(9*16,22*16,304,-1,PLEIN,C_RIEN);
            if(!gpJoueur->getCoffre(1,0)) setValeur(70*16,67*16,303,-1,PLEIN,C_CLE);
            else setValeur(70*16,67*16,304,-1,PLEIN,C_RIEN);
            if(!gpJoueur->getCoffre(1,1)) setValeur(40*16,7*16,303,-1,PLEIN,C_CLE);
            else setValeur(40*16,7*16,304,-1,PLEIN,C_RIEN);
            if(!gpJoueur->getCoffre(1,2)) setValeur(3*16,55*16,303,-1,PLEIN,C_CLE);
            else setValeur(3*16,55*16,304,-1,PLEIN,C_RIEN);
            if (!gpJoueur->hasObjet(O_GANTS)) setValeur(70*16,22*16,303,-1,PLEIN,C_GANTS);
            else setValeur(70*16,22*16,304,-1,PLEIN,C_RIEN);
            //porte boss
            if (gpJoueur->getCoffre(1,6)) passage(119,15,0);
            //portes
            if (gpJoueur->getCoffre(1,3)) passage(29,60,0);
            if (gpJoueur->getCoffre(1,4)) passage(60,51,1);
            if (gpJoueur->getCoffre(1,5)) passage(60,21,1);
            //bombes
            if (gpJoueur->getCoffre(1,7)) passage(20,67,3);
            if (gpJoueur->getCoffre(1,8)) passage(9,60,4);
            break;
        case 13 :
            if(!gpJoueur->getCle(1,0)) setValeur(70*16,82*16,303,-1,PLEIN,C_CARTE); 
            else setValeur(70*16,82*16,304,-1,PLEIN,C_RIEN);
            if(!gpJoueur->getCle(1,1)) setValeur(130*16,82*16,303,-1,PLEIN,C_BOUSSOLE);
            else setValeur(130*16,82*16,304,-1,PLEIN,C_RIEN);
            if(!gpJoueur->getCle(1,2)) setValeur(70*16,18*16,303,-1,PLEIN,C_CLE_BOSS);
            else setValeur(70*16,18*16,304,-1,PLEIN,C_RIEN);
            if(!gpJoueur->getCoffre(2,6)) setValeur(130*16,22*16,303,-1,PLEIN,C_CLE);
            else setValeur(130*16,22*16,304,-1,PLEIN,C_RIEN);
            if(!gpJoueur->getCoffre(2,7)) setValeur(10*16,22*16,303,-1,PLEIN,C_CLE);
            else setValeur(10*16,22*16,304,-1,PLEIN,C_RIEN);
            if(!gpJoueur->getCoffre(2,8)) setValeur(10*16,82*16,303,-1,PLEIN,C_CLE);
            else setValeur(10*16,82*16,304,-1,PLEIN,C_RIEN);
            if(!gpJoueur->getCoffre(2,9)) setValeur(103*16,50*16,303,-1,PLEIN,C_CLE);
            else setValeur(103*16,50*16,304,-1,PLEIN,C_RIEN);
            if(!gpJoueur->getCoffre(2,10)) setValeur(70*16,52*16,303,-1,PLEIN,C_CLE);
            else setValeur(70*16,52*16,304,-1,PLEIN,C_RIEN);
            if (!gpJoueur->hasObjet(O_GRAPPIN)) setValeur(63*16,80*16,303,-1,PLEIN,C_GRAPPIN);
            else setValeur(63*16,80*16,304,-1,PLEIN,C_RIEN);
            //porte boss
            if (gpJoueur->getCoffre(2,5)) passage(69,75,0);
            //portes
            if (gpJoueur->getCoffre(2,0)) passage(40,21,1);
            if (gpJoueur->getCoffre(2,1)) passage(29,75,0);
            if (gpJoueur->getCoffre(2,2)) passage(60,81,1);
            if (gpJoueur->getCoffre(2,3)) passage(89,45,0);
            if (gpJoueur->getCoffre(2,4)) passage(49,75,0);
            break;
        case 14 :
            if(!gpJoueur->getCle(2,0)) setValeur(90*16,112*16,303,-1,PLEIN,C_CARTE);
            else setValeur(90*16,112*16,304,-1,PLEIN,C_RIEN);
            if(!gpJoueur->getCle(2,1)) setValeur(80*16,52*16,303,-1,PLEIN,C_BOUSSOLE);
            else setValeur(80*16,52*16,304,-1,PLEIN,C_RIEN);
            if(!gpJoueur->getCle(2,2)) setValeur(230*16,22*16,303,-1,PLEIN,C_CLE_BOSS);
            else setValeur(230*16,22*16,304,-1,PLEIN,C_RIEN);
            if(!gpJoueur->getCoffre(3,0)) setValeur(230*16,37*16,303,-1,PLEIN,C_CLE);
            else setValeur(230*16,37*16,304,-1,PLEIN,C_RIEN);
            if(!gpJoueur->getCoffre(3,1)) setValeur(250*16,22*16,303,-1,PLEIN,C_CLE);
            else setValeur(250*16,22*16,304,-1,PLEIN,C_RIEN);
            if(!gpJoueur->getCoffre(3,2)) setValeur(230*16,7*16,303,-1,PLEIN,C_CLE);
            else setValeur(230*16,7*16,304,-1,PLEIN,C_RIEN);
            if(!gpJoueur->getCoffre(3,3)) setValeur(130*16,7*16,303,-1,PLEIN,C_CLE);
            else setValeur(130*16,7*16,304,-1,PLEIN,C_RIEN);
            if (!gpJoueur->hasObjet(O_LANTERNE)) setValeur(70*16,37*16,303,-1,PLEIN,C_LANTERNE);
            else setValeur(70*16,37*16,304,-1,PLEIN,C_RIEN);
            //porte boss
            if (gpJoueur->getCoffre(3,11)) passage(89,105,0);
            //portes
            if (gpJoueur->getCoffre(3,4)) passage(60,111,1);
            if (gpJoueur->getCoffre(3,5)) passage(49,60,0);
            if (gpJoueur->getCoffre(3,6)) passage(69,30,0);
            if (gpJoueur->getCoffre(3,7)) passage(269,60,0);
            if (gpJoueur->getCoffre(3,8)) passage(140,112,3);
            if (gpJoueur->getCoffre(3,9)) passage(220,41,3);
            if (gpJoueur->getCoffre(3,10)) passage(240,41,3);
            break;
        case 15 :
            //carte
            if(!gpJoueur->getCle(3,0)) setValeur(10*16,67*16,303,-1,PLEIN,C_CARTE);
            else setValeur(10*16,67*16,304,-1,PLEIN,C_RIEN);
            //boussole
            if(!gpJoueur->getCle(3,1)) setValeur(90*16,67*16,303,-1,PLEIN,C_BOUSSOLE);
            else setValeur(90*16,67*16,304,-1,PLEIN,C_RIEN);
            //cle boss
            if(!gpJoueur->getCle(3,2)) setValeur(70*16,37*16,303,-1,PLEIN,C_CLE_BOSS);
            else setValeur(70*16,37*16,304,-1,PLEIN,C_RIEN);
            //cles
            if(!gpJoueur->getCoffre(4,0)) setValeur(10*16,52*16,303,-1,PLEIN,C_CLE);
            else setValeur(10*16,52*16,304,-1,PLEIN,C_RIEN);
            if(!gpJoueur->getCoffre(4,1)) setValeur(130*16,52*16,303,-1,PLEIN,C_CLE);
            else setValeur(130*16,52*16,304,-1,PLEIN,C_RIEN);
            if(!gpJoueur->getCoffre(4,2)) setValeur(130*16,22*16,303,-1,PLEIN,C_CLE);
            else setValeur(130*16,22*16,304,-1,PLEIN,C_RIEN);
            //palmes
            if (!gpJoueur->hasObjet(O_PALMES)) setValeur(10*16,33*16,303,-1,PLEIN,C_PALMES);
            else setValeur(10*16,33*16,304,-1,PLEIN,C_RIEN);
            //porte boss
            if (gpJoueur->getCoffre(4,6)) passage(49,45,0);
            //portes
            if (gpJoueur->getCoffre(4,3)) passage(60,51,1);
            if (gpJoueur->getCoffre(4,4)) passage(169,45,0);
            if (gpJoueur->getCoffre(4,5)) passage(149,30,0);
            break;
        case 16 :
            //carte
            if(!gpJoueur->getCle(4,0)) setValeur(150*16,67*16,303,-1,PLEIN,C_CARTE);
            else setValeur(150*16,67*16,304,-1,PLEIN,C_RIEN);
            //boussole
            if(!gpJoueur->getCle(4,1)) setValeur(130*16,52*16,303,-1,PLEIN,C_BOUSSOLE);
            else setValeur(130*16,52*16,304,-1,PLEIN,C_RIEN);
            //cle boss
            if(!gpJoueur->getCle(4,2)) setValeur(170*16,7*16,303,-1,PLEIN,C_CLE_BOSS);
            else setValeur(170*16,7*16,304,-1,PLEIN,C_RIEN);
            //cles
            if(!gpJoueur->getCoffre(5,0)) setValeur(110*16,7*16,303,-1,PLEIN,C_CLE);
            else setValeur(110*16,7*16,304,-1,PLEIN,C_RIEN);
            //marteau
            if (!gpJoueur->hasObjet(O_MARTEAU)) setValeur(90*16,22*16,303,-1,PLEIN,C_MARTEAU);
            else setValeur(90*16,22*16,304,-1,PLEIN,C_RIEN);
            //porte boss
            if (gpJoueur->getCoffre(5,2)) passage(169,60,0);
            //portes
            if (gpJoueur->getCoffre(5,1)) passage(160,6,1);
            break;
        case 17 :
            //carte
            if(!gpJoueur->getCle(5,0)) setValeur(30*16,142*16,303,-1,PLEIN,C_CARTE);
            else setValeur(30*16,142*16,304,-1,PLEIN,C_RIEN);
            //boussole
            if(!gpJoueur->getCle(5,1)) setValeur(110*16,142*16,303,-1,PLEIN,C_BOUSSOLE);
            else setValeur(110*16,142*16,304,-1,PLEIN,C_RIEN);
            //cle boss
            if(!gpJoueur->getCle(5,2)) setValeur(90*16,112*16,303,-1,PLEIN,C_CLE_BOSS);
            else setValeur(90*16,112*16,304,-1,PLEIN,C_RIEN);
            //cles
            if(!gpJoueur->getCoffre(6,0)) setValeur(10*16,37*16,303,-1,PLEIN,C_CLE);
            else setValeur(10*16,37*16,304,-1,PLEIN,C_RIEN);
            if(!gpJoueur->getCoffre(6,1)) setValeur(50*16,112*16,303,-1,PLEIN,C_CLE);
            else setValeur(50*16,112*16,304,-1,PLEIN,C_RIEN);
            if(!gpJoueur->getCoffre(6,2)) setValeur(130*16,37*16,303,-1,PLEIN,C_CLE);
            else setValeur(130*16,37*16,304,-1,PLEIN,C_RIEN);
            //baton de feu
            if (!gpJoueur->hasObjet(O_BFEU)) setValeur(70*16,7*16,303,-1,PLEIN,C_BFEU);
            else setValeur(70*16,7*16,304,-1,PLEIN,C_RIEN);
            //porte boss
            if (gpJoueur->getCoffre(6,6)) passage(29,15,0);
            //portes
            if (gpJoueur->getCoffre(6,3)) passage(100,96,1);
            if (gpJoueur->getCoffre(6,4)) passage(120,96,1);
            if (gpJoueur->getCoffre(6,5)) passage(80,111,1);
            break;
        case 18 :
            //carte
            if(!gpJoueur->getCle(6,0)) setValeur(150*16,52*16,303,-1,PLEIN,C_CARTE);
            else setValeur(150*16,52*16,304,-1,PLEIN,C_RIEN);
            //boussole
            if(!gpJoueur->getCle(6,1)) setValeur(170*16,48*16,303,-1,PLEIN,C_BOUSSOLE);
            else setValeur(170*16,48*16,304,-1,PLEIN,C_RIEN);
            //cle boss
            if(!gpJoueur->getCle(6,2)) setValeur(110*16,67*16,303,-1,PLEIN,C_CLE_BOSS);
            else setValeur(110*16,67*16,304,-1,PLEIN,C_RIEN);
            //cles
            if(!gpJoueur->getCoffre(7,0)) setValeur(70*16,36*16,303,-1,PLEIN,C_CLE);
            else setValeur(70*16,36*16,304,-1,PLEIN,C_RIEN);
            if(!gpJoueur->getCoffre(7,1)) setValeur(130*16,67*16,303,-1,PLEIN,C_CLE);
            else setValeur(130*16,67*16,304,-1,PLEIN,C_RIEN);
            //baton de glace
            if (!gpJoueur->hasObjet(O_BGLACE)) setValeur(10*16,67*16,303,-1,PLEIN,C_BGLACE);
            else setValeur(10*16,67*16,304,-1,PLEIN,C_RIEN);
            //porte boss
            if (gpJoueur->getCoffre(7,5)) passage(149,30,0);
            //portes
            if (gpJoueur->getCoffre(7,2)) passage(89,60,0);
            if (gpJoueur->getCoffre(7,3)) passage(109,60,0);
            if (gpJoueur->getCoffre(7,4)) {
                setValeur(168*16,37*16,1997,-1,VIDE,C_RIEN);
                setValeur(169*16,37*16,1999,-1,VIDE,C_RIEN);
                setValeur(168*16,38*16,2003,-1,VIDE,C_RIEN);
                setValeur(169*16,38*16,2005,-1,VIDE,C_RIEN);
            }
            break;
        case 19 :
            //carte
            if(!gpJoueur->getCle(7,0)) setValeur(50*16,142*16,303,-1,PLEIN,C_CARTE);
            else setValeur(50*16,142*16,304,-1,PLEIN,C_RIEN);
            //boussole
            if(!gpJoueur->getCle(7,1)) setValeur(30*16,97*16,303,-1,PLEIN,C_BOUSSOLE);
            else setValeur(30*16,97*16,304,-1,PLEIN,C_RIEN);
            //cle boss
            if(!gpJoueur->getCle(7,2)) setValeur(50*16,7*16,303,-1,PLEIN,C_CLE_BOSS);
            else setValeur(50*16,7*16,304,-1,PLEIN,C_RIEN);
            //cles
            if(!gpJoueur->getCoffre(8,0)) setValeur(70*16,97*16,303,-1,PLEIN,C_CLE);
            else setValeur(70*16,97*16,304,-1,PLEIN,C_RIEN);
            if(!gpJoueur->getCoffre(8,1)) setValeur(10*16,112*16,303,-1,PLEIN,C_CLE);
            else setValeur(10*16,112*16,304,-1,PLEIN,C_RIEN);
            //porte boss
            if (gpJoueur->getCoffre(8,4)) passage(29,60,0);
            //portes
            if (gpJoueur->getCoffre(8,3)) passage(40,96,1);
            if (gpJoueur->getCoffre(8,2)) passage(29,135,0);
            break;
        case 20 :
            //carte
            if(!gpJoueur->getCle(8,0)) setValeur(30*16,37*16,303,-1,PLEIN,C_CARTE);
            else setValeur(30*16,37*16,304,-1,PLEIN,C_RIEN);
            //boussole
            if(!gpJoueur->getCle(8,1)) setValeur(10*16,52*16,303,-1,PLEIN,C_BOUSSOLE);
            else setValeur(10*16,52*16,304,-1,PLEIN,C_RIEN);
            //cle boss
            if(!gpJoueur->getCle(8,2)) setValeur(170*16,97*16,303,-1,PLEIN,C_CLE_BOSS);
            else setValeur(170*16,97*16,304,-1,PLEIN,C_RIEN);
            //cles
            if(!gpJoueur->getCoffre(9,0)) setValeur(30*16,7*16,303,-1,PLEIN,C_CLE);
            else setValeur(30*16,7*16,304,-1,PLEIN,C_RIEN);
            if(!gpJoueur->getCoffre(9,1)) setValeur(10*16,7*16,303,-1,PLEIN,C_CLE);
            else setValeur(10*16,7*16,304,-1,PLEIN,C_RIEN);
            if(!gpJoueur->getCoffre(9,2)) setValeur(130*16,22*16,303,-1,PLEIN,C_CLE);
            else setValeur(130*16,22*16,304,-1,PLEIN,C_RIEN);
            //porte boss
            if (gpJoueur->getCoffre(9,6)) passage(49,90,0);
            //portes
            if (gpJoueur->getCoffre(9,3)) passage(20,36,1);
            if (gpJoueur->getCoffre(9,4)) passage(9,45,0);
            if (gpJoueur->getCoffre(9,5)) passage(140,36,1);
            if (gpJoueur->getCoffre(9,7)) passage(160,18,3);
            if (gpJoueur->getCoffre(9,8)) setValeur(150*16,23*16,2304,-1,LIBRE,C_RIEN);
            if (gpJoueur->getCle(8,2)) passage(160,96,1);
            passage(149,30,0);
            break;
        case 21 :
            if (!gpJoueur->getBouclier()) setValeur(15*16, 10*16, 303, 0, PLEIN, C_BOUCLIER);
            else setValeur(15*16, 10*16, 304, 0, PLEIN, C_RIEN);
            break;
        case 23 :
            //livre
            if (!gpJoueur->hasObjet(O_LIVRE)) setValeur(30*16,46*16,303,-1,PLEIN,C_LIVRE);
            else setValeur(30*16,46*16,304,-1,PLEIN,C_RIEN);
            break;
    }
}

void Monde::initMatrice(int zone) {
    Joueur* gpJoueur = gpJeu->getJoueur();
    switch(zone) {
        case 1 :
            if (!gpJoueur->getEpee()) gpJeu->ajouteObjet(I_EPEE, 50*16, 68*16-8);
            if (!gpJoueur->getCoeur(8)) gpJeu->ajouteObjet(I_QUART_COEUR,5*16,7*16,8);
            if (!gpJoueur->getCoeur(9)) gpJeu->ajouteObjet(I_QUART_COEUR,18*16,17*16,9);
            if (!gpJoueur->getCoeur(10)) gpJeu->ajouteObjet(I_QUART_COEUR,9*16,56*16,10);
            if (!gpJoueur->getCoeur(11)) gpJeu->ajouteObjet(I_QUART_COEUR,9*16,73*16,11);
            gpJeu->ajouteEnnemi(4,37*16,67*16-8); gpJeu->ajouteEnnemi(4,37*16,68*16-7); 
            gpJeu->ajouteEnnemi(4,8*16,61*16); gpJeu->ajouteEnnemi(4,1*16,40*16); 
            gpJeu->ajouteEnnemi(3,56*16,63*16); gpJeu->ajouteEnnemi(3,56*16,70*16);
            gpJeu->ajouteEnnemi(1,45*16,27*16); gpJeu->ajouteEnnemi(1,54*16,27*16); 
            gpJeu->ajouteEnnemi(3,56*16,52*16); gpJeu->ajouteEnnemi(1,30*16,5*16); 
            gpJeu->ajouteEnnemi(1,35*16,5*16);
            break;
        case 2 :
            if (!gpJoueur->getCoeur(12)) gpJeu->ajouteObjet(I_QUART_COEUR,55*16,54*16,12);
            if (!gpJoueur->getCoeur(13)) gpJeu->ajouteObjet(I_QUART_COEUR,18*16,25*16,13);
            if (!gpJoueur->getCoeur(14)) gpJeu->ajouteObjet(I_QUART_COEUR,29*16,12*16,14);
            if (!gpJoueur->getCoeur(15)) gpJeu->ajouteObjet(I_QUART_COEUR,5*16,5*16,15);
            gpJeu->ajouteEnnemi(7,70*16,52*16); gpJeu->ajouteEnnemi(7,55*16,45*16); 
            gpJeu->ajouteEnnemi(7,18*16,52*16); gpJeu->ajouteEnnemi(7,8*16,32*16); 
            gpJeu->ajouteEnnemi(7,70*16,15*16); gpJeu->ajouteEnnemi(8,70*16,25*16);
            gpJeu->ajouteEnnemi(8,33*16,17*16); gpJeu->ajouteEnnemi(8,10*16,18*16); 
            gpJeu->ajouteEnnemi(8,98*16,25*16);
            break;
        case 3 :
            if (!gpJoueur->getCoeur(16)) gpJeu->ajouteObjet(I_QUART_COEUR,91*16,24*16,16);
            if (!gpJoueur->getCoeur(17)) gpJeu->ajouteObjet(I_QUART_COEUR,53*16,30*16,17);
            gpJeu->ajouteEnnemi(9,30*16,6*16); gpJeu->ajouteEnnemi(9,35*16,7*16); 
            gpJeu->ajouteEnnemi(9,32*16,5*16); gpJeu->ajouteEnnemi(9,86*16,35*16); 
            gpJeu->ajouteEnnemi(10,50*16,48*16); gpJeu->ajouteEnnemi(10,30*16,18*16);
            gpJeu->ajouteEnnemi(10,70*16,18*16); gpJeu->ajouteEnnemi(10,120*16,30*16);
            break;
        case 4 :
            if (!gpJoueur->getCoeur(18)) gpJeu->ajouteObjet(I_QUART_COEUR,73*16,23*16,18);
            if (!gpJoueur->getCoeur(19)) gpJeu->ajouteObjet(I_QUART_COEUR,114*16,16*16,19);
            if (!gpJoueur->getCoeur(20)) gpJeu->ajouteObjet(I_QUART_COEUR,31*16,26*16,20);
            gpJeu->ajouteEnnemi(5,97*16,34*16-8); gpJeu->ajouteEnnemi(5,109*16,25*16-8); 
            gpJeu->ajouteEnnemi(5,111*16,25*16-8); gpJeu->ajouteEnnemi(5,68*16,6*16-8); 
            gpJeu->ajouteEnnemi(5,27*16,17*16-8); gpJeu->ajouteEnnemi(6,49*16,21*16-8);
            gpJeu->ajouteEnnemi(6,100*16,12*16-8); gpJeu->ajouteEnnemi(6,15*16,32*16-8); 
            gpJeu->ajouteEnnemi(6,14*16,7*16-8);
            break;
        case 5 :
            if (!gpJoueur->getCoeur(21)) gpJeu->ajouteObjet(I_QUART_COEUR,30*16,22*16,21);
            if (!gpJoueur->getCoeur(22)) gpJeu->ajouteObjet(I_QUART_COEUR,50*16,35*16,22);
            if (!gpJoueur->getCoeur(23)) gpJeu->ajouteObjet(I_QUART_COEUR,70*16,7*16,23);
            if (!gpJoueur->getCoeur(24)) gpJeu->ajouteObjet(I_QUART_COEUR,125*16,4*16,24);
            if (!gpJoueur->getCoeur(25)) gpJeu->ajouteObjet(I_QUART_COEUR,132*16,21*16,25);
            if (!gpJoueur->getCoeur(26)) gpJeu->ajouteObjet(I_QUART_COEUR,7*16+8,43*16,26);
            if (!gpJoueur->hasBouteille(1)) gpJeu->ajoutePnj(12,112*16+8,20*16,27);
            else gpJeu->ajoutePnj(12,112*16+8,20*16,32);
            gpJeu->ajoutePnj(14,104*16,16*16,59);
            gpJeu->ajoutePnj(13,123*16,33*16,60);
            gpJeu->ajouteEnnemi(13,87*16-9,36*16-4); gpJeu->ajouteEnnemi(13,65*16-9,16*16-4); 
            gpJeu->ajouteEnnemi(13,82*16-9,7*16-4); gpJeu->ajouteEnnemi(13,78*16-9,22*16-4); 
            gpJeu->ajouteEnnemi(13,57*16-9,24*16-4); gpJeu->ajouteEnnemi(13,40*16-9,15*16-4);
            gpJeu->ajouteEnnemi(13,8*16-9,17*16-4); gpJeu->ajouteEnnemi(14,14*16,15*16); 
            gpJeu->ajouteEnnemi(14,50*16,30*16); gpJeu->ajouteEnnemi(14,40*16,10*16);
            break;
        case 6 :
            if (!gpJoueur->getCoeur(27)) gpJeu->ajouteObjet(I_QUART_COEUR,39*16,54*16,27);
            if (gpJoueur->hasObjet(O_LIVRE)) {
                if (gpJoueur->getAvancement()>=4) gpJeu->ajoutePnj(3,28*16-8,30*16,39);
                if (gpJoueur->getAvancement()==3) gpJeu->ajoutePnj(3,30*16-8,29*16,38);
                if (gpJoueur->getAvancement()<=2) gpJeu->ajoutePnj(3,30*16-8,29*16,37);
                gpJeu->ajoutePnj(4,5*16,50*16,40);
                gpJeu->ajoutePnj(5,50*16,45*16,41);
                gpJeu->ajoutePnj(6,50*16,72*16,42);
            } else {
                gpJeu->ajoutePnj(3,30*16-8,29*16,36);
                gpJeu->ajoutePnj(4,5*16,50*16,36);
                gpJeu->ajoutePnj(5,50*16,45*16,36);
                gpJeu->ajoutePnj(6,50*16,72*16,36);
            }
            if (!gpJoueur->hasBouteille(2)) gpJeu->ajoutePnj(2,53*16,82*16,27);
            else gpJeu->ajoutePnj(2,53*16,82*16,32);
            gpJeu->ajoutePnj(1,15*16,82*16,35);
            gpJeu->ajouteEnnemi(11,57*16,27*16); gpJeu->ajouteEnnemi(11,21*16,27*16);
            gpJeu->ajouteEnnemi(11,21*16,58*16); gpJeu->ajouteEnnemi(11,7*16,46*16);
            gpJeu->ajouteEnnemi(11,50*16,55*16); gpJeu->ajouteEnnemi(11,29*16,87*16);
            gpJeu->ajouteEnnemi(12,29*16,20*16); gpJeu->ajouteEnnemi(12,36*16,20*16);
            gpJeu->ajouteEnnemi(12,43*16,20*16); gpJeu->ajouteEnnemi(12,50*16,20*16);
            break;
        case 7 :
            if (!gpJoueur->getCoeur(28)) gpJeu->ajouteObjet(I_QUART_COEUR,33*16,25*16,28);
            if (!gpJoueur->getCoeur(29)) gpJeu->ajouteObjet(I_QUART_COEUR,69*16,35*16,29);
            if (!gpJoueur->getCoeur(30)) gpJeu->ajouteObjet(I_QUART_COEUR,91*16,25*16,30);
            gpJeu->ajouteEnnemi(15,23*16,7*16); gpJeu->ajouteEnnemi(15,34*16,7*16);
            gpJeu->ajouteEnnemi(15,25*16,50*16); gpJeu->ajouteEnnemi(15,40*16,50*16);
            gpJeu->ajouteEnnemi(15,45*16,30*16); gpJeu->ajouteEnnemi(15,70*16,50*16);
            gpJeu->ajouteEnnemi(15,80*16,7*16); gpJeu->ajouteEnnemi(15,100*16,7*16);
            break;
        case 8 :
            gpJeu->ajouteCaisse(5,31*16,5*16);
            if (!gpJoueur->hasBouteille(0)) gpJeu->ajoutePnj(7,29*16,16*16,27);
            else gpJeu->ajoutePnj(7,29*16,16*16,32);
            gpJeu->ajoutePnj(9,15*16,28*16,47);
            gpJeu->ajoutePnj(10,45*16,36*16,48);
            gpJeu->ajoutePnj(11,29*16,34*16,49);
            gpJeu->ajouteEnnemi(52,45*16,40*16); gpJeu->ajouteEnnemi(52,46*16,38*16);
            gpJeu->ajouteEnnemi(52,47*16,40*16); gpJeu->ajouteEnnemi(52,49*16,36*16);
            gpJeu->ajouteEnnemi(52,47*16,36*16);
            break;
        case 9 :
            if (!gpJoueur->getCoeur(31)) gpJeu->ajouteObjet(I_QUART_COEUR,15*16,10*16,31);
            if (!gpJoueur->getCoeur(32)) gpJeu->ajouteObjet(I_QUART_COEUR,71*16,45*16,32);
            if (gpJoueur->getAvancement()==6) gpJeu->ajoutePnj(15,79*16-8,15*16,90);
            break;
        case 10 :
            gpJeu->ajouteEnnemi(16,40*16,5*16-8); gpJeu->ajouteEnnemi(16,60*16,12*16-8);
            gpJeu->ajouteEnnemi(16,90*16,12*16-8); gpJeu->ajouteEnnemi(16,91*16,11*16-8);
            gpJeu->ajouteEnnemi(16,92*16,10*16-8); gpJeu->ajouteEnnemi(16,150*16,5*16-8);
            gpJeu->ajouteEnnemi(16,151*16,6*16-8); gpJeu->ajouteEnnemi(16,152*16,7*16-8);
            gpJeu->ajouteEnnemi(16,153*16,8*16-8); gpJeu->ajouteEnnemi(16,154*16,9*16-8);
            break;
        case 11 :
            gpJeu->ajouteEnnemi(16,32*16,1*16-8); gpJeu->ajouteEnnemi(16,54*16,12*16-8);
            gpJeu->ajouteEnnemi(16,56*16,12*16-8); gpJeu->ajouteEnnemi(16,58*16,12*16-8);
            gpJeu->ajouteEnnemi(16,60*16,12*16-8); gpJeu->ajouteEnnemi(16,62*16,12*16-8);
            gpJeu->ajouteEnnemi(16,64*16,12*16-8); gpJeu->ajouteEnnemi(16,77*16,6*16-8);
            gpJeu->ajouteEnnemi(16,78*16,7*16-8); gpJeu->ajouteEnnemi(16,79*16,8*16-8);
            gpJeu->ajouteEnnemi(16,80*16,9*16-8); gpJeu->ajouteEnnemi(16,81*16,10*16-8);
            break;
        case 12 :
            //ennemis
            gpJeu->ajouteEnnemi(1,30*16,5*16); gpJeu->ajouteEnnemi(1,50*16,5*16);
            gpJeu->ajouteEnnemi(1,40*16-8,20*16);
            gpJeu->ajouteEnnemi(1,45*16,45*16);
            gpJeu->ajouteEnnemi(1,110*16,22*16); gpJeu->ajouteEnnemi(1,130*16,22*16);
            gpJeu->ajouteEnnemi(1,110*16,37*16); gpJeu->ajouteEnnemi(1,130*16,37*16);
            gpJeu->ajouteEnnemi(1,90*16,52*16); gpJeu->ajouteEnnemi(1,150*16,52*16); 
            gpJeu->ajouteEnnemi(1,120*16,50*16);
            gpJeu->ajouteEnnemi(18,50*16,66*16+8); gpJeu->ajouteEnnemi(18,70*16,66*16+8);
            gpJeu->ajouteEnnemi(18,36*16,30*16); gpJeu->ajouteEnnemi(18,43*16,30*16);
            gpJeu->ajouteEnnemi(4,4*16,64*16); gpJeu->ajouteEnnemi(4,15*16,64*16);
            gpJeu->ajouteEnnemi(4,40*16-8,50*16); gpJeu->ajouteEnnemi(4,4*16,49*16); 
            gpJeu->ajouteEnnemi(4,75*16,49*16); gpJeu->ajouteEnnemi(4,75*16,55*16);
            gpJeu->ajouteEnnemi(17,110*16,47*16); gpJeu->ajouteEnnemi(17,112*16,50*16);
            gpJeu->ajouteEnnemi(17,114*16,53*16); gpJeu->ajouteEnnemi(17,126*16,47*16);
            gpJeu->ajouteEnnemi(17,128*16,50*16); gpJeu->ajouteEnnemi(17,130*16,53*16);
            //boss
            if (!gpJoueur->hasCristal(0)) gpJeu->ajouteEnnemi(19,118*16,2*16, false);
            //objets
            gpJeu->ajouteObjet(I_PETIT_COEUR,16*112,16*2-4,0,1); 
            gpJeu->ajouteObjet(I_PETIT_COEUR,16*127,16*2-4,0,1);
            gpJeu->ajouteObjet(I_PETIT_COEUR,16*112,16*12-4,0,1);
            gpJeu->ajouteObjet(I_PETIT_COEUR,16*127,16*12-4,0,1);
            gpJeu->ajouteObjet(I_PETIT_COEUR,16*39,16*24-4,0,1);
            gpJeu->ajouteObjet(I_PETIT_COEUR,16*40,16*24-4,0,1);
            gpJeu->ajouteObjet(I_PETIT_COEUR,16*22,16*22-4,0,1);
            gpJeu->ajouteObjet(I_PETIT_COEUR,16*108,16*57-4,0,1);
            break;
        case 13 :
            //switch
            gpJeu->ajouteEnnemi(20,110*16-8,97*16);mur[110*2-1][97*2]=2;mur[110*2][97*2]=2;mur[110*2-1][97*2+1]=2;mur[110*2][97*2+1]=2;
            gpJeu->ajouteEnnemi(20,110*16-8,7*16);mur[110*2-1][7*2]=2;mur[110*2][7*2]=2;mur[110*2-1][7*2+1]=2;mur[110*2][7*2+1]=2;
            gpJeu->ajouteEnnemi(20,30*16-8,7*16);mur[30*2-1][7*2]=2;mur[30*2][7*2]=2;mur[30*2-1][7*2+1]=2;mur[30*2][7*2+1]=2;
            gpJeu->ajouteEnnemi(20,26*16,77*16);mur[26*2][77*2]=2;mur[26*2+1][77*2]=2;mur[26*2][77*2+1]=2;mur[26*2+1][77*2+1]=2;
            gpJeu->ajouteEnnemi(20,28*16,82*16);mur[28*2][82*2]=2;mur[28*2+1][82*2]=2;mur[28*2][82*2+1]=2;mur[28*2+1][82*2+1]=2;
            gpJeu->ajouteEnnemi(20,46*16,82*16);mur[46*2][82*2]=2;mur[46*2+1][82*2]=2;mur[46*2][82*2+1]=2;mur[46*2+1][82*2+1]=2;
            //pièges
            gpJeu->ajouteEnnemi(17,72*16,22*16); gpJeu->ajouteEnnemi(17,70*16,26*16); 
            gpJeu->ajouteEnnemi(17,68*16,30*16); gpJeu->ajouteEnnemi(17,66*16,34*16);
            //ennemis
            gpJeu->ajouteEnnemi(22,90*16,82*16); gpJeu->ajouteEnnemi(22,102*16,62*16);
            gpJeu->ajouteEnnemi(22,117*16,62*16); gpJeu->ajouteEnnemi(22,50*16,22*16);
            gpJeu->ajouteEnnemi(22,90*16,22*16); gpJeu->ajouteEnnemi(22,55*16,32*16);
            gpJeu->ajouteEnnemi(22,85*16,32*16); gpJeu->ajouteEnnemi(22,22*16,62*16);
            gpJeu->ajouteEnnemi(22,37*16,62*16);
            gpJeu->ajouteEnnemi(9,110*16-8,82*16); gpJeu->ajouteEnnemi(9,110*16-8,35*16);
            gpJeu->ajouteEnnemi(9,30*16-8,35*16); gpJeu->ajouteEnnemi(9,30*16-8,52*16);
            gpJeu->ajouteEnnemi(9,50*16-8,52*16); gpJeu->ajouteEnnemi(9,90*16-8,52*16);
            gpJeu->ajouteEnnemi(23,114*16,52*16); gpJeu->ajouteEnnemi(23,103*16,18*16);
            gpJeu->ajouteEnnemi(23,115*16,18*16); gpJeu->ajouteEnnemi(23,23*16,18*16);
            gpJeu->ajouteEnnemi(23,35*16,18*16);
            //boss
            if (!gpJoueur->hasCristal(1)) gpJeu->ajouteEnnemi(24,69*16,62*16);
            //objets
            gpJeu->ajouteObjet(I_FLECHE,16*77,16*87,0,1); 
            gpJeu->ajouteObjet(I_BOMBE,16*83,16*87,0,1); 
            gpJeu->ajouteObjet(I_PETIT_COEUR,16*117,16*87-4,0,1);
            gpJeu->ajouteObjet(I_FLECHE,16*109,16*67,0,1); 
            gpJeu->ajouteObjet(I_BOMBE,16*110,16*67,0,1);
            gpJeu->ajouteObjet(I_PETIT_COEUR,16*117,16*47-4,0,1);
            gpJeu->ajouteObjet(I_PETIT_COEUR,16*102,16*17-4,0,1);
            gpJeu->ajouteObjet(I_BOMBE,16*117,16*17,0,1);
            gpJeu->ajouteObjet(I_FLECHE,16*102,16*42,0,1);
            gpJeu->ajouteObjet(I_FLECHE,16*117,16*42,0,1);
            gpJeu->ajouteObjet(I_PETIT_COEUR,16*22,16*17-4,0,1);
            gpJeu->ajouteObjet(I_BOMBE,16*37,16*17,0,1);
            gpJeu->ajouteObjet(I_FLECHE,16*22,16*42,0,1);
            gpJeu->ajouteObjet(I_FLECHE,16*37,16*42,0,1);
            gpJeu->ajouteObjet(I_PETIT_COEUR,16*42,16*72-4,0,1);
            gpJeu->ajouteObjet(I_PETIT_COEUR,16*57,16*72-4,0,1);
            gpJeu->ajouteObjet(I_PETIT_COEUR,16*82,16*72-4,0,1);
            gpJeu->ajouteObjet(I_PETIT_COEUR,16*97,16*72-4,0,1);
            gpJeu->ajouteObjet(I_PETIT_COEUR,16*29,16*96-4,0,1);
            gpJeu->ajouteObjet(I_PETIT_COEUR,16*30,16*96-4,0,1);
            gpJeu->ajouteObjet(I_FLECHE,16*29,16*97,0,1);
            gpJeu->ajouteObjet(I_FLECHE,16*30,16*97,0,1);
            gpJeu->ajouteObjet(I_BOMBE,16*29,16*98,0,1);
            gpJeu->ajouteObjet(I_BOMBE,16*30,16*98,0,1);
            if (gpJoueur->hasObjet(O_GRAPPIN)) permuteBlocs();
            break;
        case 14 :
            //ennemis
            gpJeu->ajouteEnnemi(25,69*16,111*16); gpJeu->ajouteEnnemi(25,89*16,81*16); 
            gpJeu->ajouteEnnemi(25,109*16,66*16); gpJeu->ajouteEnnemi(25,109*16,51*16);
            gpJeu->ajouteEnnemi(25,49*16,66*16); gpJeu->ajouteEnnemi(25,69*16,6*16);
            gpJeu->ajouteEnnemi(25,89*16,6*16); gpJeu->ajouteEnnemi(25,249*16,6*16);
            gpJeu->ajouteEnnemi(25,209*16,36*16); gpJeu->ajouteEnnemi(25,249*16,36*16);
            gpJeu->ajouteEnnemi(25,169*16,66*16); gpJeu->ajouteEnnemi(25,309*16,66*16);
            gpJeu->ajouteEnnemi(26,29*16,108*16); gpJeu->ajouteEnnemi(26,39*16,108*16);
            gpJeu->ajouteEnnemi(26,49*16,108*16); gpJeu->ajouteEnnemi(26,29*16,116*16);
            gpJeu->ajouteEnnemi(26,39*16,116*16); gpJeu->ajouteEnnemi(26,49*16,116*16);
            gpJeu->ajouteEnnemi(26,7*16,96*16); gpJeu->ajouteEnnemi(26,10*16,89*16);
            gpJeu->ajouteEnnemi(26,7*16,83*16); gpJeu->ajouteEnnemi(26,4*16,29*16);
            gpJeu->ajouteEnnemi(26,15*16,29*16); gpJeu->ajouteEnnemi(26,36*16,10*16);
            gpJeu->ajouteEnnemi(26,44*16,10*16);
            gpJeu->ajouteEnnemi(27,10*16-8,66*16); gpJeu->ajouteEnnemi(27,30*16-8,66*16);
            gpJeu->ajouteEnnemi(27,10*16-8,51*16); gpJeu->ajouteEnnemi(27,30*16-8,51*16);
            gpJeu->ajouteEnnemi(27,130*16-8,66*16); gpJeu->ajouteEnnemi(27,150*16-8,66*16);
            gpJeu->ajouteEnnemi(27,130*16-8,51*16); gpJeu->ajouteEnnemi(27,150*16-8,51*16);
            gpJeu->ajouteEnnemi(27,230*16-8,66*16); gpJeu->ajouteEnnemi(27,250*16-8,66*16);
            gpJeu->ajouteEnnemi(27,230*16-8,51*16); gpJeu->ajouteEnnemi(27,250*16-8,51*16);
            gpJeu->ajouteEnnemi(27,190*16-8,66*16); gpJeu->ajouteEnnemi(27,210*16-8,66*16);
            gpJeu->ajouteEnnemi(27,270*16-8,66*16); gpJeu->ajouteEnnemi(27,290*16-8,66*16);
            //boss
            if (!gpJoueur->hasCristal(2)) gpJeu->ajouteEnnemi(28,89*16,94*16);
            //objets
            gpJeu->ajouteObjet(5,16*62,16*107,0,1); gpJeu->ajouteObjet(6,16*77,16*117,0,1);
            gpJeu->ajouteObjet(4,16*2,16*117-4,0,1); gpJeu->ajouteObjet(4,16*17,16*117-4,0,1);
            gpJeu->ajouteObjet(5,16*2,16*107,0,1); gpJeu->ajouteObjet(4,16*2,16*62-4,0,1);
            gpJeu->ajouteObjet(6,16*37,16*72,0,1); gpJeu->ajouteObjet(5,16*42,16*62,0,1);
            gpJeu->ajouteObjet(4,16*2,16*12-4,0,1); gpJeu->ajouteObjet(4,16*17,16*12-4,0,1);
            gpJeu->ajouteObjet(7,16*62,16*32-4,0,1); gpJeu->ajouteObjet(7,16*77,16*32-4,0,1);
            gpJeu->ajouteObjet(4,16*142,16*2-4,0,1); gpJeu->ajouteObjet(7,16*142,16*12-4,0,1);
            gpJeu->ajouteObjet(6,16*157,16*107,0,1); gpJeu->ajouteObjet(6,16*157,16*117,0,1);
            gpJeu->ajouteObjet(6,16*142,16*117,0,1); gpJeu->ajouteObjet(4,16*142,16*107-4,0,1);
            gpJeu->ajouteObjet(4,16*222,16*47-4,0,1); gpJeu->ajouteObjet(4,16*257,16*47-4,0,1);
            gpJeu->ajouteObjet(7,16*222,16*72-4,0,1); gpJeu->ajouteObjet(7,16*257,16*72-4,0,1);
            break;
        case 15 :
            //caisse
            gpJeu->ajouteCaisse(4,82*16,4*16); gpJeu->ajouteCaisse(4,82*16,5*16);
            gpJeu->ajouteCaisse(4,83*16,5*16); gpJeu->ajouteCaisse(4,83*16,6*16);
            gpJeu->ajouteCaisse(4,83*16,7*16); gpJeu->ajouteCaisse(4,83*16,8*16);
            gpJeu->ajouteCaisse(4,83*16,9*16); gpJeu->ajouteCaisse(4,82*16,9*16);
            gpJeu->ajouteCaisse(4,82*16,10*16);
            for (int j=0; j<2; j++) for (int i=0; i<7; i++) 
                gpJeu->ajouteCaisse(4,(42+i)*16,(32+j)*16);
            for (int j=0; j<2; j++) for (int i=0; i<7; i++) 
                gpJeu->ajouteCaisse(4,(51+i)*16,(32+j)*16);
            //ennemi
            gpJeu->ajouteEnnemi(21,85*16,21*16-8); gpJeu->ajouteEnnemi(21,94*16,21*16-8);
            gpJeu->ajouteEnnemi(21,85*16,32*16-8); gpJeu->ajouteEnnemi(21,94*16,32*16-8);
            gpJeu->ajouteEnnemi(21,30*16,19*16-8); gpJeu->ajouteEnnemi(21,40*16,25*16-8);
            gpJeu->ajouteEnnemi(21,50*16,19*16-8); gpJeu->ajouteEnnemi(21,60*16,25*16-8);
            gpJeu->ajouteEnnemi(21,70*16,19*16-8); gpJeu->ajouteEnnemi(29,30*16,4*16-8);
            gpJeu->ajouteEnnemi(29,40*16,10*16-8); gpJeu->ajouteEnnemi(29,50*16,4*16-8);
            gpJeu->ajouteEnnemi(29,60*16,10*16-8); gpJeu->ajouteEnnemi(29,70*16,4*16-8);
            gpJeu->ajouteEnnemi(29,89*16,7*16-8); gpJeu->ajouteEnnemi(29,29*16,67*16-8);
            gpJeu->ajouteEnnemi(29,69*16,67*16-8); gpJeu->ajouteEnnemi(29,164*16,52*16-8);
            gpJeu->ajouteEnnemi(29,175*16,52*16-8); gpJeu->ajouteEnnemi(29,169*16,37*16-8);
            gpJeu->ajouteEnnemi(29,149*16,22*16-8); gpJeu->ajouteEnnemi(30,89*16,52*16-8);
            gpJeu->ajouteEnnemi(30,9*16,7*16-8); gpJeu->ajouteEnnemi(30,9*16,22*16-8);
            gpJeu->ajouteEnnemi(30,29*16,52*16-8); gpJeu->ajouteEnnemi(30,149*16,52*16-8);
            gpJeu->ajouteEnnemi(30,151*16,50*16-8); gpJeu->ajouteEnnemi(30,147*16,54*16-8);
            gpJeu->ajouteEnnemi(30,169*16+8,22*16-8);
            //pièges
            gpJeu->ajouteEnnemi(17,25*16,2*16); gpJeu->ajouteEnnemi(17,35*16,2*16);
            gpJeu->ajouteEnnemi(17,45*16,2*16); gpJeu->ajouteEnnemi(17,55*16,2*16);
            gpJeu->ajouteEnnemi(17,65*16,2*16); gpJeu->ajouteEnnemi(17,75*16,2*16);
            //boss
            if (!gpJoueur->hasCristal(3)) gpJeu->ajouteEnnemi(31,49*16,32*16);
            //objets
            gpJeu->ajouteObjet(4,16*22,16*62-4,0,1); gpJeu->ajouteObjet(7,16*37,16*72-4,0,1);
            gpJeu->ajouteObjet(4,16*77,16*62-4,0,1); gpJeu->ajouteObjet(7,16*62,16*72-4,0,1);
            gpJeu->ajouteObjet(4,16*22,16*2-4,0,1); gpJeu->ajouteObjet(7,16*22,16*12-4,0,1);
            gpJeu->ajouteObjet(4,16*77,16*2-4,0,1); gpJeu->ajouteObjet(7,16*77,16*12-4,0,1);
            gpJeu->ajouteObjet(4,16*142,16*47-4,0,1); gpJeu->ajouteObjet(7,16*157,16*57-4,0,1);
            gpJeu->ajouteObjet(4,16*157,16*17-4,0,1); gpJeu->ajouteObjet(7,16*142,16*27-4,0,1);
            break;
        case 16 :
            //ennemi
            gpJeu->ajoutePiege(51,140*16,7*16); gpJeu->ajoutePiege(51,170*16,22*16);
            gpJeu->ajoutePiege(51,170*16,67*16); gpJeu->ajoutePiege(51,110*16,67*16);
            gpJeu->ajoutePiege(51,140*16,30*16);
            gpJeu->ajouteEnnemi(12,78*16,80*16); gpJeu->ajouteEnnemi(12,70*16,65*16); 
            gpJeu->ajouteEnnemi(12,84*16,56*16); gpJeu->ajouteEnnemi(12,68*16,35*16);
            gpJeu->ajouteEnnemi(12,70*16,20*16); gpJeu->ajouteEnnemi(12,84*16,11*16);
            gpJeu->ajouteEnnemi(32,23*16,63*16-8); gpJeu->ajouteEnnemi(32,36*16,63*16-8);
            gpJeu->ajouteEnnemi(32,43*16,3*16-8); gpJeu->ajouteEnnemi(32,56*16,3*16-8);
            gpJeu->ajouteEnnemi(32,163*16,33*16-8); gpJeu->ajouteEnnemi(32,176*16,33*16-8);
            gpJeu->ajouteEnnemi(32,163*16,41*16-8); gpJeu->ajouteEnnemi(32,176*16,41*16-8);
            gpJeu->ajouteEnnemi(32,109*16,52*16-8); gpJeu->ajouteEnnemi(32,149*16,52*16-8);
            gpJeu->ajouteEnnemi(32,149*16,82*16-8); gpJeu->ajouteEnnemi(33,29*16,67*16-8);
            gpJeu->ajouteEnnemi(33,49*16,67*16-8); gpJeu->ajouteEnnemi(33,49*16,7*16-8);
            gpJeu->ajouteEnnemi(33,149*16,37*16-8); gpJeu->ajouteEnnemi(33,92*16,37*16-8);
            gpJeu->ajouteEnnemi(33,129*16,67*16-8); gpJeu->ajouteEnnemi(34,169*16,37*16-8);
            gpJeu->ajouteEnnemi(34,49*16,22*16-8); gpJeu->ajouteEnnemi(34,155*16,7*16-8);
            //switch
            gpJeu->ajouteEnnemi(20,170*16-8,82*16);mur[170*2-1][82*2]=2;mur[170*2][82*2]=2;mur[170*2-1][82*2+1]=2;mur[170*2][82*2+1]=2;
            gpJeu->ajouteEnnemi(20,75*16,27*16);mur[75*2][27*2]=2;mur[75*2+1][27*2]=2;mur[75*2][27*2+1]=2;mur[75*2+1][27*2+1]=2;
            gpJeu->ajouteEnnemi(20,110*16-8,37*16);mur[110*2-1][37*2]=2;mur[110*2][37*2]=2;mur[110*2-1][37*2+1]=2;mur[110*2][37*2+1]=2;
            //boss
            if (!gpJoueur->hasCristal(4)) gpJeu->ajouteEnnemi(35,169*16,47*16);
            //objets
            gpJeu->ajouteObjet(4,16*37,16*62-4,0,1); gpJeu->ajouteObjet(4,16*142,16*87-4,0,1); 
            gpJeu->ajouteObjet(4,16*162,16*62-4,0,1); gpJeu->ajouteObjet(4,16*177,16*62-4,0,1);
            gpJeu->ajouteObjet(4,16*102,16*62-4,0,1); gpJeu->ajouteObjet(4,16*177,16*17-4,0,1);
            gpJeu->ajouteObjet(4,16*122,16*42-4,0,1); gpJeu->ajouteObjet(4,16*123,16*42-4,0,1);
            gpJeu->ajouteObjet(4,16*122,16*41-4,0,1); gpJeu->ajouteObjet(4,16*42,16*2-4,0,1);
            break;
        case 17 :
            //pièges
            gpJeu->ajouteEnnemi(17,16*16,117*16);
            gpJeu->ajouteEnnemi(36,69*16+8,37*16);mur[69*2+1][37*2]=2;mur[69*2+2][37*2]=2;mur[69*2+1][37*2+1]=2;mur[69*2+2][37*2+1]=2;
            gpJeu->ajouteEnnemi(36,62*16,17*16);mur[62*2][17*2]=2;mur[62*2+1][17*2]=2;mur[62*2][17*2+1]=2;mur[62*2+1][17*2+1]=2;
            gpJeu->ajouteEnnemi(36,117*16,116*16);
            gpJeu->ajouteEnnemi(36,117*16,119*16);
            gpJeu->ajouteEnnemi(36,117*16,122*16);
            gpJeu->ajouteEnnemi(36,120*16,116*16);
            gpJeu->ajouteEnnemi(36,120*16,119*16);
            gpJeu->ajouteEnnemi(36,120*16,122*16);
            gpJeu->ajouteEnnemi(36,123*16,116*16);
            gpJeu->ajouteEnnemi(36,123*16,119*16);
            gpJeu->ajouteEnnemi(36,123*16,122*16);
            gpJeu->ajouteEnnemi(36,4*16,49*16);
            gpJeu->ajouteEnnemi(36,4*16,56*16);
            gpJeu->ajouteEnnemi(36,4*16,63*16);
            gpJeu->ajouteEnnemi(36,4*16,70*16);
            gpJeu->ajouteEnnemi(36,15*16,49*16);
            gpJeu->ajouteEnnemi(36,15*16,56*16);
            gpJeu->ajouteEnnemi(36,15*16,63*16);
            gpJeu->ajouteEnnemi(36,15*16,70*16);
            gpJeu->ajouteEnnemi(36,124*16,49*16);
            gpJeu->ajouteEnnemi(36,124*16,56*16);
            gpJeu->ajouteEnnemi(36,124*16,63*16);
            gpJeu->ajouteEnnemi(36,124*16,70*16);
            gpJeu->ajouteEnnemi(36,135*16,49*16);
            gpJeu->ajouteEnnemi(36,135*16,56*16);
            gpJeu->ajouteEnnemi(36,135*16,63*16);
            gpJeu->ajouteEnnemi(36,135*16,70*16);
            gpJeu->ajouteEnnemi(36,42*16,137*16);mur[42*2][137*2]=2;mur[42*2+1][137*2]=2;mur[42*2][137*2+1]=2;mur[42*2+1][137*2+1]=2;
            gpJeu->ajouteEnnemi(36,97*16,137*16);mur[97*2][137*2]=2;mur[97*2+1][137*2]=2;mur[97*2][137*2+1]=2;mur[97*2+1][137*2+1]=2;
            gpJeu->ajouteEnnemi(36,57*16,122*16);mur[57*2][122*2]=2;mur[57*2+1][122*2]=2;mur[57*2][122*2+1]=2;mur[57*2+1][122*2+1]=2;
            gpJeu->ajouteEnnemi(36,57*16,132*16);mur[57*2][132*2]=2;mur[57*2+1][132*2]=2;mur[57*2][132*2+1]=2;mur[57*2+1][132*2+1]=2;
            gpJeu->ajouteEnnemi(36,82*16,122*16);mur[82*2][122*2]=2;mur[82*2+1][122*2]=2;mur[82*2][122*2+1]=2;mur[82*2+1][122*2+1]=2;
            gpJeu->ajouteEnnemi(36,82*16,132*16);mur[82*2][132*2]=2;mur[82*2+1][132*2]=2;mur[82*2][132*2+1]=2;mur[82*2+1][132*2+1]=2;
            gpJeu->ajouteEnnemi(36,37*16,92*16);mur[37*2][92*2]=2;mur[37*2+1][92*2]=2;mur[37*2][92*2+1]=2;mur[37*2+1][92*2+1]=2;
            gpJeu->ajouteEnnemi(36,137*16,92*16);mur[137*2][92*2]=2;mur[137*2+1][92*2]=2;mur[137*2][92*2+1]=2;mur[137*2+1][92*2+1]=2;
            gpJeu->ajouteEnnemi(36,42*16,47*16);mur[42*2][47*2]=2;mur[42*2+1][47*2]=2;mur[42*2][47*2+1]=2;mur[42*2+1][47*2+1]=2;
            gpJeu->ajouteEnnemi(36,97*16,47*16);mur[97*2][47*2]=2;mur[97*2+1][47*2]=2;mur[97*2][47*2+1]=2;mur[97*2+1][47*2+1]=2;
            gpJeu->ajouteEnnemi(36,62*16,72*16);mur[62*2][72*2]=2;mur[62*2+1][72*2]=2;mur[62*2][72*2+1]=2;mur[62*2+1][72*2+1]=2;
            gpJeu->ajouteEnnemi(36,77*16,72*16);mur[77*2][72*2]=2;mur[77*2+1][72*2]=2;mur[77*2][72*2+1]=2;mur[77*2+1][72*2+1]=2;
            gpJeu->ajouteEnnemi(36,22*16,2*16);mur[22*2][2*2]=2;mur[22*2+1][2*2]=2;mur[22*2][2*2+1]=2;mur[22*2+1][2*2+1]=2;
            gpJeu->ajouteEnnemi(36,37*16,2*16);mur[37*2][2*2]=2;mur[37*2+1][2*2]=2;mur[37*2][2*2+1]=2;mur[37*2+1][2*2+1]=2;
            //ennemi
            gpJeu->ajouteEnnemi(37,29*16,119*16); gpJeu->ajouteEnnemi(37,109*16,119*16); gpJeu->ajouteEnnemi(37,69*16+8,126*16);
            gpJeu->ajouteEnnemi(37,49*16+8,75*16); gpJeu->ajouteEnnemi(37,89*16+8,75*16); gpJeu->ajouteEnnemi(37,49*16+8,96*16+8);
            gpJeu->ajouteEnnemi(37,29*16+8,96*16+8); gpJeu->ajouteEnnemi(37,9*16+8,96*16+8); gpJeu->ajouteEnnemi(37,89*16+8,96*16+8);
            gpJeu->ajouteEnnemi(37,109*16+8,96*16+8); gpJeu->ajouteEnnemi(37,129*16+8,96*16+8); gpJeu->ajouteEnnemi(37,9*16+8,60*16+8);
            gpJeu->ajouteEnnemi(37,129*16+8,60*16+8);
            gpJeu->ajouteEnnemi(38,49*16+5,142*16-8); gpJeu->ajouteEnnemi(38,89*16+5,142*16-8); gpJeu->ajouteEnnemi(38,49*16+5,52*16-8);
            gpJeu->ajouteEnnemi(38,89*16+5,52*16-8); gpJeu->ajouteEnnemi(38,9*16+5,82*16-8); gpJeu->ajouteEnnemi(38,29*16+5,82*16-8);
            gpJeu->ajouteEnnemi(38,109*16+5,82*16-8); gpJeu->ajouteEnnemi(38,129*16+5,82*16-8);
            gpJeu->ajouteEnnemi(39,49*16,127*16-8); gpJeu->ajouteEnnemi(39,89*16,127*16-8); gpJeu->ajouteEnnemi(39,69*16+5,82*16-8);
            //boss
            if (!gpJoueur->hasCristal(5)) gpJeu->ajouteEnnemi(40,28*16-4,2*16);
            //objet
            gpJeu->ajouteObjet(7,16*62,16*77-4,0,1);
            gpJeu->ajouteObjet(7,16*77,16*77-4,0,1);
            for (int i=0; i<6; i++) gpJeu->ajouteObjet(7,16*(27+i),16*123-4,0,1);
            for (int i=0; i<6; i++) gpJeu->ajouteObjet(7,16*(107+i),16*123-4,0,1);
            break;
        case 18 :
            //caisse
            gpJeu->ajouteCaisse(3,82*16,7*16); gpJeu->ajouteCaisse(3,9*16+8,12*16); gpJeu->ajouteCaisse(3,109*16+8,12*16);
            gpJeu->ajouteCaisse(3,182*16,4*16); gpJeu->ajouteCaisse(3,182*16,5*16); gpJeu->ajouteCaisse(3,183*16,5*16);
            gpJeu->ajouteCaisse(3,183*16,6*16); gpJeu->ajouteCaisse(3,183*16,7*16); gpJeu->ajouteCaisse(3,183*16,8*16);
            gpJeu->ajouteCaisse(3,183*16,9*16); gpJeu->ajouteCaisse(3,182*16,9*16); gpJeu->ajouteCaisse(3,182*16,10*16);
            //ennemi
            gpJeu->ajouteEnnemi(2,29*16+6,44*16-8); gpJeu->ajouteEnnemi(2,85*16+6,44*16-8); gpJeu->ajouteEnnemi(2,93*16+6,44*16-8);
            gpJeu->ajouteEnnemi(2,85*16+6,29*16-8); gpJeu->ajouteEnnemi(2,93*16+6,29*16-8); gpJeu->ajouteEnnemi(2,5*16+6,44*16-8);
            gpJeu->ajouteEnnemi(2,13*16+6,44*16-8); gpJeu->ajouteEnnemi(2,5*16+6,29*16-8); gpJeu->ajouteEnnemi(2,13*16+6,29*16-8);
            gpJeu->ajouteEnnemi(2,159*16+6,67*16-8);
            gpJeu->ajouteEnnemi(41,29*16+7,67*16-8); gpJeu->ajouteEnnemi(41,69*16+7,67*16-8); gpJeu->ajouteEnnemi(41,29*16+7,7*16-8);
            gpJeu->ajouteEnnemi(41,69*16+7,7*16-8); gpJeu->ajouteEnnemi(41,129*16+7,44*16-8); gpJeu->ajouteEnnemi(41,109*16+7,29*16-8);
            gpJeu->ajouteEnnemi(41,109*16+7,35*16-8); gpJeu->ajouteEnnemi(41,114*16+7,45*16-8); gpJeu->ajouteEnnemi(41,109*16+7,53*16-8);
            gpJeu->ajouteEnnemi(42,89*16+3,3*16-8); gpJeu->ajouteEnnemi(42,9*16+3,3*16-8); gpJeu->ajouteEnnemi(42,189*16+3,3*16-8);
            gpJeu->ajouteEnnemi(42,109*16+3,3*16-8); gpJeu->ajouteEnnemi(42,169*16+3,37*16-8);
            //pieges
            for (int i=0; i<10; i++)gpJeu->ajouteEnnemi(17,(127+i*5)*16,3*16);
            //boss
            if (!gpJoueur->hasCristal(6)) gpJeu->ajouteEnnemi(43,148*16-4,17*16);
            break;
        case 19 :
            //ennemi
            gpJeu->ajouteEnnemi(44,56*16+6,146*16-8);
            gpJeu->ajouteEnnemi(44,56*16+6,137*16-8);
            gpJeu->ajouteEnnemi(44,21*16+6,63*16-8);
            gpJeu->ajouteEnnemi(44,36*16+6,63*16-8);
            gpJeu->ajouteEnnemi(44,41*16+6,63*16-8);
            gpJeu->ajouteEnnemi(44,56*16+6,63*16-8);
            gpJeu->ajouteEnnemi(44,48*16+6,110*16-8);
            gpJeu->ajouteEnnemi(44,68*16+6,110*16-8);
            gpJeu->ajouteEnnemi(44,28*16+6,113*16-8);
            gpJeu->ajouteEnnemi(44,8*16+6,96*16-8);
            gpJeu->ajouteEnnemi(44,48*16+6,96*16-8);
            gpJeu->ajouteEnnemi(44,41*16+6,36*16-8);
            gpJeu->ajouteEnnemi(44,44*16+6,38*16-8);
            gpJeu->ajouteEnnemi(44,48*16+6,40*16-8);
            gpJeu->ajouteEnnemi(44,53*16+6,38*16-8);
            gpJeu->ajouteEnnemi(44,56*16+6,36*16-8);
            //boss
            if (gpJoueur->getEpee()<5) gpJeu->ajouteEnnemi(45,28*16-4,47*16);
            if (gpJoueur->getEpee()<5) gpJeu->ajouteObjet(I_EXCALIBUR,29*16+8,4*16,0,1);
            break;
        case 20 :
            //switch
            gpJeu->ajouteEnnemi(20,30*16-8,97*16);mur[30*2-1][97*2]=2;mur[30*2][97*2]=2;mur[30*2-1][97*2+1]=2;mur[30*2][97*2+1]=2;
            //zelda
            if (gpJoueur->getAvancement()==7) {
                if (gpJoueur->getEpee()<5) gpJeu->ajoutePnj(15,49*16+8,97*16-8,91);
                else gpJeu->ajoutePnj(15,49*16+8,97*16-8,92);
            }
            else gpJeu->ajoutePnj(15,149*16+8,19*16,87);
            //ennemi
            gpJeu->ajouteEnnemi(46,46*16+2,47*16); gpJeu->ajouteEnnemi(46,52*16+2,47*16); gpJeu->ajouteEnnemi(46,29*16+2,51*16);
            gpJeu->ajouteEnnemi(46,69*16+2,51*16); gpJeu->ajouteEnnemi(46,24*16+2,17*16); gpJeu->ajouteEnnemi(46,34*16+2,17*16);
            gpJeu->ajouteEnnemi(46,4*16+2,17*16); gpJeu->ajouteEnnemi(46,9*16+2,20*16); gpJeu->ajouteEnnemi(46,14*16+2,17*16);
            gpJeu->ajouteEnnemi(46,4*16+2,77*16); gpJeu->ajouteEnnemi(46,14*16+2,77*16); gpJeu->ajouteEnnemi(46,69*16+2,21*16);
            gpJeu->ajouteEnnemi(46,69*16+2,36*16); gpJeu->ajouteEnnemi(46,84*16+2,77*16); gpJeu->ajouteEnnemi(46,94*16+2,77*16);
            gpJeu->ajouteEnnemi(47,49*16+2,25*16); gpJeu->ajouteEnnemi(47,9*16+2,36*16); gpJeu->ajouteEnnemi(47,9*16+2,74*16);
            gpJeu->ajouteEnnemi(47,9*16+2,96*16); gpJeu->ajouteEnnemi(47,89*16+2,29*16); gpJeu->ajouteEnnemi(47,89*16+2,74*16);
            gpJeu->ajouteEnnemi(47,89*16+2,96*16); gpJeu->ajouteEnnemi(47,129*16+2,96*16); gpJeu->ajouteEnnemi(47,104*16+2,44*16);
            gpJeu->ajouteEnnemi(47,114*16+2,44*16); gpJeu->ajouteEnnemi(47,109*16+2,47*16); gpJeu->ajouteEnnemi(47,129*16+2,36*16);
            gpJeu->ajouteEnnemi(48,151*16+2,37*16); gpJeu->ajouteEnnemi(48,109*16+2,96*16); gpJeu->ajouteEnnemi(48,139*16+2,51*16);
            gpJeu->ajouteEnnemi(48,109*16+2,74*16);
            gpJeu->ajouteEnnemi(49,137*16,92*16); setValeur(137*16,92*16,2304,2);
            gpJeu->ajouteEnnemi(49,102*16,102*16); setValeur(102*16,102*16,2304,2);
            gpJeu->ajouteEnnemi(49,122*16,47*16); setValeur(122*16,47*16,2304,2);
            gpJeu->ajouteEnnemi(49,157*16,47*16); setValeur(157*16,47*16,2304,2);
            gpJeu->ajouteEnnemi(49,157*16,42*16); setValeur(157*16,42*16,2304,2);
            gpJeu->ajouteEnnemi(49,177*16,17*16); setValeur(177*16,17*16,2304,2);
            gpJeu->ajouteEnnemi(49,162*16,42*16); setValeur(162*16,42*16,2304,2);
            gpJeu->ajouteEnnemi(49,182*16,32*16); setValeur(182*16,32*16,2304,2);
            gpJeu->ajouteEnnemi(49,197*16,57*16); setValeur(197*16,57*16,2304,2);
            gpJeu->ajouteEnnemi(49,42*16,77*16); setValeur(42*16,77*16,2294,2);
            gpJeu->ajouteEnnemi(49,57*16,77*16); setValeur(57*16,77*16,2294,2);
            //boss
            gpJeu->ajouteEnnemi(50,48*16-4,77*16);
            break;
        case 21 :
            gpJeu->ajouteObjet(I_PETIT_COEUR, 32, 28);
            gpJeu->ajouteObjet(I_PETIT_COEUR, 32, 44);
            gpJeu->ajouteObjet(I_PETIT_COEUR, 32, 60);
            break;
        case 22 :
            if (!gpJoueur->getCoeur(33)) gpJeu->ajouteObjet(I_QUART_COEUR,29*16+8,11*16-8,33,1);
            //switch
            gpJeu->ajouteEnnemi(20,3*16,37*16);mur[3*2][37*2]=2;mur[3*2+1][37*2]=2;mur[3*2][37*2+1]=2;mur[3*2+1][37*2+1]=2;
            //ennemis
            gpJeu->ajouteEnnemi(18,22*16,2*16-8);
            gpJeu->ajouteEnnemi(18,37*16,2*16-8);
            gpJeu->ajouteEnnemi(18,9*16,37*16-8);
            gpJeu->ajouteEnnemi(23,9*16+6,10*16-8);
            gpJeu->ajouteEnnemi(23,9*16+6,20*16-8);
            gpJeu->ajouteEnnemi(23,29*16+6,30*16-8);
            break;
        case 23 :
            //ennemis
            gpJeu->ajouteEnnemi(12,9*16,37*16); gpJeu->ajouteEnnemi(12,49*16,37*16); gpJeu->ajouteEnnemi(12,9*16,67*16);
            gpJeu->ajouteEnnemi(12,49*16,67*16); gpJeu->ajouteEnnemi(12,29*16,7*16); gpJeu->ajouteEnnemi(12,49*16,7*16);
            gpJeu->ajouteEnnemi(12,69*16,30*16); gpJeu->ajouteEnnemi(12,69*16,45*16);
            break;
        case 24 :
            if (gpJoueur->hasMedaillon(0)==0) gpJeu->ajoutePnj(16,9*16,3*16,99);
            break;
        case 25 :
            if (gpJoueur->hasMedaillon(1)==0) gpJeu->ajoutePnj(16,9*16,3*16,224);
            break;
        case 26 :
            if (gpJoueur->hasMedaillon(2)==0) gpJeu->ajoutePnj(16,9*16,3*16,100);
            break;
        case 27 :
            if (gpJoueur->getMagieMax()==32) gpJeu->ajoutePnj(16,9*16,3*16,103);
            break;
        case 28 :
            gpJeu->ajouteObjet(I_PETIT_COEUR,16*16,16*11-4,0,1);
            gpJeu->ajouteObjet(I_RUBIS_BLEU,16*17,16*11,0,1);
            gpJeu->ajouteObjet(I_PETIT_COEUR,16*16,16*12-4,0,1);
            gpJeu->ajouteObjet(I_RUBIS_BLEU,16*17,16*12,0,1);
            break;
        case 29 : gpJeu->ajoutePnj(17,9*16+7,7*16,106); break;
        case 30 :
            if (!gpJoueur->getCoeur(34)) gpJeu->ajouteObjet(I_QUART_COEUR,3*16+8,3*16-8,34,1);
            gpJeu->ajoutePnj(18,9*16+8,3*16,112);
            gpJeu->ajoutePnj(19,15*16,7*16-9,113);
            break;
        case 31 :
            if (!gpJoueur->hasObjet(O_ARC)) {
                gpJeu->ajouteObjet(I_ARC,16*17,16*3,0,1); contenu[17][4]=124;
                gpJeu->ajouteObjet(I_PETIT_COEUR,16*2,16*3,0,1); contenu[2][4]=114;
                gpJeu->ajouteObjet(I_MAGIE_PEU,16*5,16*3,0,1); contenu[5][4]=116;
                gpJeu->ajouteObjet(I_MAGIE_BCP,16*8,16*3,0,1); contenu[8][4]=118;
                gpJeu->ajouteObjet(I_FLECHE,16*11,16*3,0,1); contenu[11][4]=120;
                gpJeu->ajouteObjet(I_BOMBE,16*14,16*3,0,1); contenu[14][4]=122;
                if (gpJoueur->getAvancement()==2) contenu[17][4]=126;
            }else {
                gpJeu->ajouteObjet(I_PETIT_COEUR,16*3,16*3,0,1); contenu[3][4]=114;
                gpJeu->ajouteObjet(I_MAGIE_PEU,16*6,16*3,0,1); contenu[6][4]=116;
                gpJeu->ajouteObjet(I_MAGIE_BCP,16*9,16*3,0,1); contenu[9][4]=118;
                gpJeu->ajouteObjet(I_FLECHE,16*12,16*3,0,1); contenu[12][4]=120;
                gpJeu->ajouteObjet(I_BOMBE,16*15,16*3,0,1); contenu[15][4]=122;
            }
            if (gpJoueur->getAvancement()==1) gpJeu->ajoutePnj(20,15*16,10*16-9,132);
            else gpJeu->ajoutePnj(20,15*16,10*16-9,131);
            break;
        case 32 :
            if (gpJoueur->getAvancement()==0) gpJeu->ajoutePnj(21,19*16+8,7*16-9,133);
            if (gpJoueur->getAvancement()==1) gpJeu->ajoutePnj(21,19*16+8,7*16-9,145);
            if (gpJoueur->getAvancement()>=2) {
                if (gpJoueur->nbCristaux()<7) {
                    gpJeu->ajoutePnj(21,19*16+8,7*16-9,146);
                }else {
                    if (gpJoueur->getAvancement()<7) {
                        gpJeu->ajoutePnj(21,19*16+8,7*16-9,147);
                    }else {
                        if (gpJoueur->getEpee()<5) {
                            gpJeu->ajoutePnj(21,19*16+8,7*16-9,148);
                        }else {
                            gpJeu->ajoutePnj(21,19*16+8,7*16-9,151);
                        }
                    }
                }
            }
            break;
        case 33 :
            if (!gpJoueur->getCoeur(35)) {
                setValeur(4*16,5*16,303,1);
                setValeur(9*16,5*16,303,1);
                setValeur(14*16,5*16,303,1);
                contenu[4][5]=152; contenu[9][5]=152; contenu[14][5]=152;
                gpJeu->ajoutePnj(22,16*16+8,9*16,153);
            }else {
                setValeur(4*16,5*16,304,1);
                setValeur(9*16,5*16,304,1);
                setValeur(14*16,5*16,304,1);
                gpJeu->ajoutePnj(22,16*16,9*16,156);
            }
            break;
        case 34 :
            if (!gpJoueur->getCoeur(36)) {
                setValeur(4*16,4*16,303,1);
                setValeur(9*16,4*16,303,1);
                setValeur(14*16,4*16,303,1);
                setValeur(4*16,7*16,303,1);
                setValeur(9*16,7*16,303,1);
                setValeur(14*16,7*16,303,1);
                contenu[4][4]=152; contenu[9][4]=152; contenu[14][4]=152;
                contenu[4][7]=152; contenu[9][7]=152; contenu[14][7]=152;
                gpJeu->ajoutePnj(22,16*16+8,9*16,153);
            }else {
                setValeur(4*16,4*16,304,1);
                setValeur(9*16,4*16,304,1);
                setValeur(14*16,4*16,304,1);
                setValeur(4*16,7*16,304,1);
                setValeur(9*16,7*16,304,1);
                setValeur(14*16,7*16,304,1);
                gpJeu->ajoutePnj(22,16*16,9*16,156);
            }
            break;
        case 35 :
            gpJeu->ajouteObjet(4,16*16,16*12-4,0,1);
            gpJeu->ajouteObjet(4,16*17,16*12-4,0,1);
            gpJeu->ajoutePnj(23,9*16+8,3*16-9,158);
            break;
        case 36 :
            gpJeu->ajoutePnj(24,9*16+8,3*16-9,159);
            break;
        case 37 : gpJeu->ajoutePnj(25,9*16+7,7*16,161); break;
        case 38 :
            if (gpJoueur->nbCristaux()<2) gpJeu->ajoutePnj(26,3*16+6,2*16-6,164);
            else gpJeu->ajoutePnj(26,3*16+6,2*16-6,165);
            break;
        case 39 :
            if (!gpJoueur->getCoeur(37)) {
                setValeur(4*16,3*16,303,1);
                setValeur(9*16,3*16,303,1);
                setValeur(14*16,3*16,303,1);
                setValeur(4*16,6*16,303,1);
                setValeur(9*16,6*16,303,1);
                setValeur(14*16,6*16,303,1);
                setValeur(4*16,9*16,303,1);
                setValeur(9*16,9*16,303,1);
                setValeur(14*16,9*16,303,1);
                contenu[4][3]=152; contenu[9][3]=152; contenu[14][3]=152;
                contenu[4][6]=152; contenu[9][6]=152; contenu[14][6]=152;
                contenu[4][9]=152; contenu[9][9]=152; contenu[14][9]=152;
                if (gpJoueur->hasObjet(O_LIVRE)) gpJeu->ajoutePnj(27,16*16+8,11*16,153);
                else gpJeu->ajoutePnj(27,16*16+8,11*16,36);
            }else {
                setValeur(4*16,3*16,304,1);
                setValeur(9*16,3*16,304,1);
                setValeur(14*16,3*16,304,1);
                setValeur(4*16,6*16,304,1);
                setValeur(9*16,6*16,304,1);
                setValeur(14*16,6*16,304,1);
                setValeur(4*16,9*16,304,1);
                setValeur(9*16,9*16,304,1);
                setValeur(14*16,9*16,304,1);
                gpJeu->ajoutePnj(27,16*16,11*16,156);
            }
            break;
        case 40 : gpJeu->ajoutePnj(28,9*16+7,5*16,176);
            gpJeu->ajouteObjet(7,16*2,16*12-4,0,1);
            gpJeu->ajouteObjet(7,16*17,16*12-4,0,1);
            break;
        case 41 :
            if (gpJoueur->hasObjet(O_LIVRE)) {
                if (gpJoueur->nbCristaux()<4) gpJeu->ajoutePnj(29,9*16+8,4*16,178);
                else {
                    if (gpJoueur->getEpee()==5) gpJeu->ajoutePnj(29,9*16+8,4*16,184);
                    else {
                        if (gpJoueur->getAvancement()<=2) gpJeu->ajoutePnj(29,9*16+8,4*16,179);
                        if (gpJoueur->getAvancement()==3) gpJeu->ajoutePnj(29,9*16+8,4*16,181);
                        if (gpJoueur->getAvancement()>=4 && gpJoueur->getAvancement()<7) 
                            gpJeu->ajoutePnj(29,9*16+8,4*16,182);
                        if (gpJoueur->getAvancement()==7) gpJeu->ajoutePnj(29,9*16+8,4*16,183);
                    }
                }
            }
            else gpJeu->ajoutePnj(29,9*16+8,4*16,36);
            break;
        case 42 :
            if (gpJoueur->hasObjet(O_LIVRE)) gpJeu->ajoutePnj(30,9*16+8,4*16,186);
            else gpJeu->ajoutePnj(30,9*16+8,4*16,36);
            break;
        case 43 :
            if (gpJoueur->hasObjet(O_LIVRE)) gpJeu->ajoutePnj(31,9*16+8,4*16,187);
            else gpJeu->ajoutePnj(31,9*16+8,4*16,36);
            break;
        case 44 :
            if (!gpJoueur->hasObjet(O_ENCYCL)) gpJeu->ajoutePnj(32,9*16+8,4*16,189);
            else gpJeu->ajoutePnj(32,9*16+8,4*16,196);
            break;
    }
}

void Monde::detRegion(int newZone) {
    region[0]=0; region[1]=0;
    region[2]=largeur;region[3]=hauteur;
    
    
    
    if (gpJeu->isDonjon() || gpJeu->isGrotte()) {
        Joueur* gpJoueur = gpJeu->getJoueur();
        region[0]=((int)gpJoueur->getX()/320)*320; region[1]=((int)gpJoueur->getY()/240)*240;
        region[2]=region[0]+320;region[3]=region[1]+240;
        switch (newZone) {
            case 12 :
                if ((region[0]==320 || region[0]==640) && (region[1]==240*4 || region[1]==0)) {
                    region[0]=320; region[2]=320*3;}
                if (region[0]>=320 && region[0]<=640 && region[1]>=240 && region[1]<=240*3) {
                    region[0]=320; region[1]=240; region[2]=320*3; region[3]=240*4;}
                if (region[0]>=320*5 && region[0]<=320*6 && region[1]>=240 && region[1]<=240*2) {
                    region[0]=320*5; region[1]=240; region[2]=320*7; region[3]=240*3;}
                if (region[0]>=320*5 && region[0]<=320*6 && region[1]==240*3) {
                    region[0]=320*5; region[2]=320*7;}
                if (region[0]>=320*5 && region[0]<=320*6 && region[1]==0) {
                    region[0]=320*5+160; region[2]=320*6+160;}
                break;
            case 13 :
                if (region[0]>=320 && region[0]<=320*5 && (region[1]==240 || region[1]==480)) {
                    region[1]=240; region[3]=240*3;}
                if (region[0]>=320*2 && region[0]<=320*4 && (region[1]==240 || region[1]==480)) {
                    region[0]=320*2; region[2]=320*5;}
                if ((region[0]==320*2 || region[0]==320*4) && (region[1]==240*3 || region[1]==240*4)) {
                    region[1]=240*3; region[3]=240*5;}
                break;
            case 14 :
                if ((region[0]==0 || region[0]==320*4 || region[0]==320*7) 
                && (region[1]==240 || region[1]==240*2)) {
                    region[1]=240; region[3]=240*3;}
                if ((region[0]==0 || region[0]==320*3 || region[0]>=320*7) 
                && (region[1]==240*5 || region[1]==240*6)) {
                    region[1]=240*5; region[3]=240*7;}
                if ((region[0]==320 || region[0]==320*2) && (region[1]==0 || region[1]==240*7)) {
                    region[0]=320; region[2]=320*3;}
                if ((region[0]==320*5 || region[0]==320*6) && (region[1]==0 || region[1]==240*7)) {
                    region[0]=320*5; region[2]=320*7;}
                if ((region[0]==0 || region[0]==320) && (region[1]==240*3 || region[1]==240*4)) {
                    region[0]=0; region[2]=320*2;}
                if ((region[0]==320*6 || region[0]==320*7) && (region[1]==240*3 || region[1]==240*4)) {
                    region[0]=320*6; region[2]=320*8;}
                if ((region[0]==320*3 || region[0]==320*4) && (region[1]==240*3 || region[1]==240*4)) {
                    region[0]=320*3; region[1]=240*3; region[2]=320*5; region[3]=240*5;}
                if ((region[0]==320*9 || region[0]==320*10) && (region[1]==0 || region[1]==240*4)) {
                    region[0]=320*9; region[2]=320*11;}
                if ((region[0]==320*13 || region[0]==320*14) && (region[1]==0 || region[1]==240*4)) {
                    region[0]=320*13; region[2]=320*15;}
                if (region[0]==320*13 && (region[1]==240*2 || region[1]==240*3)) {
                    region[1]=240*2; region[3]=240*4;}
                if ((region[0]==320*11 || region[0]==320*12) && (region[1]==240*3 || region[1]==240*4)) {
                    region[0]=320*11; region[1]=240*3; region[2]=320*13; region[3]=240*5;}
                break;
            case 15 :
                if ((region[0]==320 || region[0]==320*2 || region[0]==320*3) 
                && (region[1]==0 || region[1]==240)) {
                    region[0]=320; region[2]=320*4;}
                if (region[0]==320*2 && (region[1]==240*3 || region[1]==240*4)) {
                    region[1]=240*3; region[3]=240*5;}
                if (region[0]==320*4 && (region[1]==240 || region[1]==240*2)) {
                    region[1]=240; region[3]=240*3;}
                break;
            case 16 :
                if ((region[0]==320*3 || region[0]==320*4) 
                && (region[1]==0 || region[1]==240 || region[1]==240*2)) {
                    region[0]=320*3; region[1]=0; region[2]=320*5; region[3]=240*3;}
                if ((region[0]==320*3 || region[0]==320*4) 
                && (region[1]==240*3 || region[1]==240*4 || region[1]==240*5)) {
                    region[0]=320*3; region[1]=240*3; region[2]=320*5; region[3]=240*6;}
                if ((region[0]==320*5 || region[0]==320*6 || region[0]==320*7) && region[1]==240*3) {
                    region[0]=320*5; region[2]=320*8;}
                if ((region[0]==320*6 || region[0]==320*7) && region[1]==0) {
                    region[0]=320*6; region[2]=320*8;}
                if ((region[0]==320*6 || region[0]==320*7) && (region[1]==240 || region[1]==240*2)) {
                    region[0]=320*6; region[1]=240; region[2]=320*8; region[3]=240*3;}
                break;
            case 17 :
                if ((region[0]==0 || region[0]==320*6) && (region[1]==240*3 || region[1]==240*4)) {
                    region[1]=240*3; region[3]=240*5;}
                if ((region[0]==320*2 || region[0]==320*4) && (region[1]==240*4 || region[1]==240*5)) {
                    region[1]=240*4; region[3]=240*6;}
                if ((region[0]==0 || region[0]==320) 
                && (region[1]>=240*5 && region[1]<=240*8)) {
                    region[0]=0; region[2]=320*2;}
                if ((region[0]==320*5 || region[0]==320*6) 
                && (region[1]==240*5 || region[1]==240*7 || region[1]==240*8)) {
                    region[0]=320*5; region[2]=320*7;}
                if ((region[0]!=320*2 && region[0]!=320*4) 
                && (region[1]==240*7 || region[1]==240*8)) {
                    region[1]=240*7; region[3]=240*9;}
                break;
            case 18 :
                if ((region[0]==0 || region[0]==320*4 || region[0]==320*5 || region[0]==320*9) 
                && (region[1]==240 || region[1]==240*2 || region[1]==240*3)) {
                    region[1]=240; region[3]=240*4;}
                if ((region[0]==320 || region[0]==320*2 || region[0]==320*6) 
                && (region[1]==240*2 || region[1]==240*3)) {
                    region[1]=240*2; region[3]=240*4;}
                if ((region[0]==320 || region[0]==320*2 || region[0]==320*3) && region[1]==0) {
                    region[0]=320; region[2]=320*4;}
                if ((region[0]==320*6 || region[0]==320*7 || region[0]==320*8) && region[1]==0) {
                    region[0]=320*6; region[2]=320*9;}
                if ((region[0]==320*7 || region[0]==320*8) && region[1]==240*4) {
                    region[0]=320*7; region[2]=320*9;}
                break;
            case 19 :
                if (region[0]==320 && (region[1]==240 || region[1]==240*2)) {
                    region[1]=240; region[3]=240*3;}
                if (region[0]==320*2 && (region[1]==240 || region[1]==240*2 || region[1]==240*3)) {
                    region[1]=240; region[3]=240*4;}
                if ((region[0]==320 || region[0]==320*2) && (region[1]==240*4 || region[1]==240*5)) {
                    region[1]=240*4; region[3]=240*6; gpJeu->getAudio()->stopSpecial();}
                if ((region[0]==320 || region[0]==320*2) && (region[1]==240*7 || region[1]==240*8)) {
                    region[1]=240*7; region[3]=240*9;}
                break;
            case 20 :
                if (region[1]==0 && (region[0]==40*16 || region[0]==60*16)) {
                    region[0]=40*16; region[2]=80*16;}
                if (region[1]==45*16 && (region[0]==20*16 || region[0]==40*16 || region[0]==60*16)) {
                    region[0]=20*16; region[2]=80*16;}
                if ((region[1]==15*16 || region[1]==30*16) 
                && (region[0]==40*16 || region[0]==60*16 || region[0]==80*16)) {
                    region[1]=15*16; region[3]=45*16;}
                if ((region[1]==60*16 || region[1]==75*16) 
                && (region[0]==0 || region[0]==80*16 || region[0]==100*16 || region[0]==180*16)) {
                    region[1]=60*16; region[3]=90*16;}
                if ((region[1]==30*16 || region[1]==45*16) 
                && (region[0]==100*16 || region[0]==180*16)) {
                    region[1]=30*16; region[3]=60*16;}
                if ((region[1]==15*16 || region[1]==30*16) && region[0]==160*16) {
                    region[1]=15*16; region[3]=45*16;}
                if (region[1]==90*16 && (region[0]==100*16 || region[0]==120*16)) {
                    region[0]=100*16; region[2]=140*16;}
                if (region[1]==45*16 && (region[0]==120*16 || region[0]==140*16)) {
                    region[0]=120*16; region[2]=160*16;}
                break;
            case 22 :
                if ((region[1]==0 || region[1]==15*16) && region[0]==0) {
                    region[1]=0; region[3]=30*16;}
                if ((region[1]==15*16 || region[1]==30*16) && region[0]==20*16) {
                    region[1]=15*16; region[3]=45*16;}
                break;
            case 23 :
                if ((region[1]==15*16 || region[1]==30*16 || region[1]==45*16) && region[0]==60*16) {
                    region[1]=15*16; region[3]=60*16;}
                if (region[1]==0 && (region[0]==20*16 || region[0]==40*16)) {
                    region[0]=20*16; region[2]=60*16;}
                if ((region[1]==30*16 || region[1]==45*16 || region[1]==60*16) 
                && (region[0]==0 || region[0]==20*16 || region[0]==40*16)) {
                    region[0]=0; region[2]=60*16; region[1]=30*16; region[3]=75*16;}
                break;
        }
        if (newZone<21) {
            for (int i = region[0]; i < region[2]; i+=320)
                for (int j = region[1]; j < region[3]; j+=240)
                    gpJoueur->setExplore(newZone-12,i/320, j/240);
            gpJoueur->setExplore(newZone-12,(region[2]-1)/320, (region[3]-1)/240);
        }
        if (charge) return;
        gpJeu->replace();
        gpJeu->testAnim();
    }
}

//replace crânes
void Monde::replace() {
    switch (gpJeu->getZone()) {
        case 12:
            setValeur(39*16,24*16,31,1); setValeur(40*16,24*16,31,1); setValeur(22*16,22*16,31,1);
            setValeur(112*16,2*16,31,1); setValeur(127*16,2*16,31,1); setValeur(112*16,12*16,31,1);
            setValeur(127*16,12*16,31,1); setValeur(108*16,57*16,31,1);
            break;
        case 13:
            setValeur(77*16,87*16,31,1); setValeur(82*16,87*16,31,1); setValeur(83*16,87*16,31,1);
            setValeur(102*16,87*16,31,1); setValeur(117*16,87*16,31,1); setValeur(109*16,67*16,31,1);
            setValeur(110*16,67*16,31,1); setValeur(117*16,47*16,31,1); setValeur(117*16,57*16,31,1);
            setValeur(102*16,17*16,31,1); setValeur(117*16,17*16,31,1); setValeur(102*16,42*16,31,1);
            setValeur(117*16,42*16,31,1); setValeur(22*16,17*16,31,1); setValeur(37*16,17*16,31,1);
            setValeur(22*16,42*16,31,1); setValeur(37*16,42*16,31,1); setValeur(42*16,72*16,31,1);
            setValeur(57*16,72*16,31,1); setValeur(82*16,72*16,31,1); setValeur(97*16,72*16,31,1);
            setValeur(29*16,96*16,31,1); setValeur(30*16,96*16,31,1); setValeur(29*16,97*16,31,1);
            setValeur(30*16,97*16,31,1); setValeur(29*16,98*16,31,1); setValeur(30*16,98*16,31,1);
            break;
        case 14:
            setValeur(62*16,107*16,31,1); setValeur(77*16,107*16,31,1); setValeur(62*16,117*16,31,1);
            setValeur(77*16,117*16,31,1); setValeur(2*16,107*16,31,1); setValeur(17*16,107*16,31,1);
            setValeur(2*16,117*16,31,1); setValeur(17*16,117*16,31,1); setValeur(2*16,62*16,31,1);
            setValeur(37*16,62*16,31,1); setValeur(2*16,72*16,31,1); setValeur(37*16,72*16,31,1);
            setValeur(42*16,62*16,31,1); setValeur(57*16,62*16,31,1); setValeur(2*16,2*16,31,1);
            setValeur(17*16,2*16,31,1); setValeur(2*16,12*16,31,1); setValeur(17*16,12*16,31,1);
            setValeur(62*16,32*16,31,1); setValeur(77*16,32*16,31,1); setValeur(142*16,2*16,31,1);
            setValeur(157*16,2*16,31,1); setValeur(142*16,12*16,31,1); setValeur(157*16,12*16,31,1);
            setValeur(142*16,107*16,31,1); setValeur(157*16,107*16,31,1); setValeur(142*16,117*16,31,1);
            setValeur(157*16,117*16,31,1); setValeur(222*16,47*16,31,1); setValeur(257*16,47*16,31,1);
            setValeur(222*16,72*16,31,1); setValeur(257*16,72*16,31,1);
            break;
        case 15 :
            setValeur(22*16,62*16,31,1); setValeur(22*16,72*16,31,1); setValeur(37*16,62*16,31,1);
            setValeur(37*16,72*16,31,1); setValeur(62*16,62*16,31,1); setValeur(62*16,72*16,31,1);
            setValeur(77*16,62*16,31,1); setValeur(77*16,72*16,31,1); setValeur(22*16,2*16,31,1);
            setValeur(22*16,12*16,31,1); setValeur(77*16,2*16,31,1); setValeur(77*16,12*16,31,1);
            setValeur(142*16,47*16,31,1); setValeur(142*16,57*16,31,1); setValeur(157*16,47*16,31,1);
            setValeur(157*16,57*16,31,1); setValeur(142*16,17*16,31,1); setValeur(142*16,27*16,31,1);
            setValeur(157*16,17*16,31,1); setValeur(157*16,27*16,31,1);
            break;
        case 16 :
            setValeur(22*16,62*16,31,1); setValeur(37*16,62*16,31,1); setValeur(142*16,87*16,31,1);
            setValeur(157*16,87*16,31,1); setValeur(162*16,62*16,31,1); setValeur(177*16,62*16,31,1);
            setValeur(162*16,72*16,31,1); setValeur(177*16,72*16,31,1); setValeur(102*16,62*16,31,1);
            setValeur(117*16,62*16,31,1); setValeur(102*16,72*16,31,1); setValeur(117*16,72*16,31,1);
            setValeur(162*16,17*16,31,1); setValeur(177*16,17*16,31,1); setValeur(122*16,41*16,31,1);
            setValeur(123*16,41*16,31,1); setValeur(122*16,42*16,31,1); setValeur(123*16,42*16,31,1);
            setValeur(42*16,2*16,31,1); setValeur(57*16,2*16,31,1);
            break;
        case 17 :
            setValeur(62*16,77*16,31,1); setValeur(77*16,77*16,31,1); setValeur(62*16,87*16,31,1);
            setValeur(77*16,87*16,31,1);
            for (int i=0; i<6; i++)setValeur((27+i)*16,123*16,31,1);
            for (int i=0; i<6; i++)setValeur((107+i)*16,123*16,31,1);
            break;
        case 20 :
            setValeur(2*16,47*16,30,1); setValeur(17*16,47*16,30,1); setValeur(2*16,57*16,30,1);
            setValeur(17*16,57*16,30,1); setValeur(82*16,47*16,30,1); setValeur(97*16,47*16,30,1);
            setValeur(82*16,57*16,30,1); setValeur(97*16,57*16,30,1);
            break;
    }
}

void Monde::permuteBlocs() {
    int temoin=0;
    switch (gpJeu->getZone()) {
        case 13 :
            if (map[28][42]==1348) {
                //bloc orange haut
                for (int i = 0; i < 9; i++) setValeur(73*16,(79+i)*16,1347,2);
                for (int i = 0; i < 7; i++) setValeur(93*16,(81+i)*16,1347,2);
                for (int i = 0; i < 2; i++) setValeur((27+i)*16,79*16,1347,2);
                for (int i = 0; i < 3; i++) setValeur(35*16,(81+i)*16,1347,2);
                for (int i = 0; i < 3; i++) setValeur(54*16,(81+i)*16,1347,2);
                for (int i = 0; i < 3; i++) setValeur(43*16,(21+i)*16,1347,2);
                for (int i = 0; i < 3; i++) setValeur(96*16,(21+i)*16,1347,2);
                //bloc bleu bas
                for (int i = 0; i < 2; i++) setValeur((109+i)*16,79*16,1346,0);
                for (int i = 0; i < 3; i++) setValeur(29*16,(81+i)*16,1346,0);
                for (int i = 0; i < 4; i++) setValeur((28+i)*16,41*16,1346,0);
                setValeur(28*16,42*16,1346,0);
                setValeur(31*16,42*16,1346,0);
                for (int i = 0; i < 2; i++) setValeur((49+i)*16,41*16,1346,0);
                for (int i = 0; i < 3; i++) setValeur(56*16,(81+i)*16,1346,0);
                temoin=1;
            }
            else {
                //bloc orange bas
                for (int i = 0; i < 9; i++) setValeur(73*16,(79+i)*16,1345,0);
                for (int i = 0; i < 7; i++) setValeur(93*16,(81+i)*16,1345,0);
                for (int i = 0; i < 2; i++) setValeur((27+i)*16,79*16,1345,0);
                for (int i = 0; i < 3; i++) setValeur(35*16,(81+i)*16,1345,0);
                for (int i = 0; i < 3; i++) setValeur(54*16,(81+i)*16,1345,0);
                for (int i = 0; i < 3; i++) setValeur(43*16,(21+i)*16,1345,0);
                for (int i = 0; i < 3; i++) setValeur(96*16,(21+i)*16,1345,0);
                //bloc bleu haut
                for (int i = 0; i < 2; i++) setValeur((109+i)*16,79*16,1348,2);
                for (int i = 0; i < 3; i++) setValeur(29*16,(81+i)*16,1348,2);
                for (int i = 0; i < 4; i++) setValeur((28+i)*16,41*16,1348,2);
                setValeur(28*16,42*16,1348,2);
                setValeur(31*16,42*16,1348,2);
                for (int i = 0; i < 2; i++) setValeur((49+i)*16,41*16,1348,2);
                for (int i = 0; i < 3; i++) setValeur(56*16,(81+i)*16,1348,2);
                temoin=0;
            }
            break;
        case 16 :
            if (map[147][78]==1348) {
                //bloc bleu bas
                for (int i = 0; i < 4; i++) setValeur((148+i)*16,79*16,1346,0);
                setValeur(147*16,78*16,1346,0);
                setValeur(146*16,77*16,1346,0);
                setValeur(152*16,78*16,1346,0);
                setValeur(153*16,77*16,1346,0);
                for (int i = 0; i < 4; i++) setValeur((108+i)*16,49*16,1346,0);
                setValeur(107*16,48*16,1346,0);
                setValeur(106*16,47*16,1346,0);
                setValeur(112*16,48*16,1346,0);
                setValeur(113*16,47*16,1346,0);
                setValeur(89*16,18*16,1346,0);
                setValeur(90*16,18*16,1346,0);
                setValeur(94*16,21*16,1346,0);
                setValeur(94*16,22*16,1346,0);
                setValeur(94*16,23*16,1346,0);
                //bloc orange haut
                for (int i = 0; i < 3; i++) setValeur(124*16,(66+i)*16,1347,2);
                setValeur(122*16,64*16,1347,2);
                setValeur(123*16,65*16,1347,2);
                setValeur(123*16,69*16,1347,2);
                setValeur(122*16,70*16,1347,2);
                for (int i = 0; i < 3; i++) setValeur(155*16,(36+i)*16,1347,2);
                setValeur(157*16,34*16,1347,2);
                setValeur(156*16,35*16,1347,2);
                setValeur(156*16,39*16,1347,2);
                setValeur(157*16,40*16,1347,2);
                for (int i = 0; i < 3; i++) setValeur(95*16,(36+i)*16,1347,2);
                setValeur(96*16,35*16,1347,2);
                setValeur(96*16,39*16,1347,2);
                temoin=1;
            }else {            
                //bloc bleu haut
                for (int i = 0; i < 4; i++) setValeur((148+i)*16,79*16,1348,2);
                setValeur(147*16,78*16,1348,2);
                setValeur(146*16,77*16,1348,2);
                setValeur(152*16,78*16,1348,2);
                setValeur(153*16,77*16,1348,2);
                for (int i = 0; i < 4; i++) setValeur((108+i)*16,49*16,1348,2);
                setValeur(107*16,48*16,1348,2);
                setValeur(106*16,47*16,1348,2);
                setValeur(112*16,48*16,1348,2);
                setValeur(113*16,47*16,1348,2);
                setValeur(89*16,18*16,1348,2);
                setValeur(90*16,18*16,1348,2);
                setValeur(94*16,21*16,1348,2);
                setValeur(94*16,22*16,1348,2);
                setValeur(94*16,23*16,1348,2);
                //bloc orange bas
                for (int i = 0; i < 3; i++) setValeur(124*16,(66+i)*16,1345,0);
                setValeur(122*16,64*16,1345,0);
                setValeur(123*16,65*16,1345,0);
                setValeur(123*16,69*16,1345,0);
                setValeur(122*16,70*16,1345,0);
                for (int i = 0; i < 3; i++) setValeur(155*16,(36+i)*16,1345,0);
                setValeur(157*16,34*16,1345,0);
                setValeur(156*16,35*16,1345,0);
                setValeur(156*16,39*16,1345,0);
                setValeur(157*16,40*16,1345,0);
                for (int i = 0; i < 3; i++) setValeur(95*16,(36+i)*16,1345,0);
                setValeur(96*16,35*16,1345,0);
                setValeur(96*16,39*16,1345,0);
                temoin=0;
            }
            break;
        case 20 :
            if (map[68][47]==1348) {
                //bloc orange haut
                for (int i = 0; i < 2; i++) setValeur((29+i)*16,48*16,1347,2);
                setValeur(28*16,47*16,1347,2);
                setValeur(31*16,47*16,1347,2);
                //bloc bleu bas
                for (int i = 0; i < 2; i++) setValeur((69+i)*16,48*16,1346,0);
                setValeur(68*16,47*16,1346,0);
                setValeur(71*16,47*16,1346,0);
                temoin=1;
            }else{
                //bloc orange bas
                for (int i = 0; i < 2; i++) setValeur((29+i)*16,48*16,1345,0);
                setValeur(28*16,47*16,1345,0);
                setValeur(31*16,47*16,1345,0);
                //bloc bleu haut
                for (int i = 0; i < 2; i++) setValeur((69+i)*16,48*16,1348,2);
                setValeur(68*16,47*16,1348,2);
                setValeur(71*16,47*16,1348,2);
                temoin=0;
            }
            break;
        case 22 :
            if (map[2][34]==1348) {
                //bloc orange haut
                for (int i = 0; i < 11; i++) setValeur((2+i)*16,40*16,1347,2);
                //bloc bleu bas
                for (int i = 0; i < 11; i++) setValeur((2+i)*16,34*16,1346,0);
                temoin=1;
            }else{
                //bloc orange bas
                for (int i = 0; i < 11; i++) setValeur((2+i)*16,40*16,1345,0);
                //bloc bleu haut
                for (int i = 0; i < 11; i++) setValeur((2+i)*16,34*16,1348,2);
                temoin=0;
            }
            break;
    }
    gpJeu->permuteBloc(temoin);
}

int Monde::etage() {
    int level = 0;
    switch (gpJeu->getZone()-12) {
        case 0 : if (region[0]<80*16) level=0; else level=-1; break;
        case 1 : case 7 : level=0; break;
        case 2 : if (region[0]<160*16) level=0; else level=1; break;
        case 3 : if (region[0]<100*16) level=0;
            if (region[0]>=100*16 && region[0]<200*16) level=1;
            if (region[0]>=200*16) level=2;
            break;
        case 4 : if (region[0]==0) level=0;
            if (region[0]>=20*16 && region[0]<60*16) level=-1;
            if (region[0]>=60*16) level=-2;
            break;
        case 5 : if (region[1]<30*16 && region[0]<60*16) level=1; else level=0; break;
        case 6 : if (region[0]<100*16) level=0; else level=1; break;
        case 8 : if (region[0]<100*16) level=0; else level=-2;
            if (region[0]==40*16 && region[1]>=75*16) level=1;
            if (region[0]>=120*16 && region[1]>=45*16
            && region[0]<160*16 && region[1]<75*16) level=-1;
            break;
        case 10 :
            if (region[0]==320*2 && region[1]>=240 && region[1]<=240*3) level=0;
            else level=-1;
            break;
        case 11 :
            if (region[0]==0 && region[1]==0) level=0;
            else level=-1;
            break;
    }
    return level;    
}

int Monde::motifValue(int x, int y) {
    return map[(int)(x/CASE)][(int)(y/CASE)];
}

int Monde::motifAirValue(int x, int y) {
    return air[(int)(x/CASE)][(int)(y/CASE)];
}

int Monde::murValue(int x, int y) {
    return mur[(int)(x/(CASE/2))][(int)(y/(CASE/2))];
}

int Monde::contenuValue(int x, int y) {
    return contenu[(int)(x/CASE)][(int)(y/CASE)];
}

int Monde::regionValue(int i) {
    return region[i];
}

void Monde::setValeur(int x, int y, int vsol, int vair, int vmur, int vcontenu) {
    setValeur(x, y, vsol, vmur);
    if (vair>-1) air[(int)(x/CASE)][(int)(y/CASE)] = vair;
    contenu[(int)(x/CASE)][(int)(y/CASE)] = vcontenu;
}

void Monde::setValeur(int x, int y, int vsol, int vmur) {
    map[(int)(x/CASE)][(int)(y/CASE)] = vsol;
    mur[((int)(x/CASE))*2][((int)(y/CASE))*2] = vmur;
    mur[((int)(x/CASE))*2+1][((int)(y/CASE))*2] = vmur;
    mur[((int)(x/CASE))*2][((int)(y/CASE))*2+1] = vmur;
    mur[((int)(x/CASE))*2+1][((int)(y/CASE))*2+1] = vmur;
}

void Monde::setMap(int x, int y, int val) {map[x][y]=val;}

void Monde::setMurs(int x, int y, int val) {mur[x][y]=val;}

void Monde::setContenu(int x, int y, int val) {contenu[x][y]=val;}

void Monde::drawSol(SDL_Surface* screen, int phg[2]) {
    int x=0,y=0,val;
    SDL_Rect src;    
    SDL_Rect dst;
    
    dst.x = 0;
    dst.y = 0;
    
    plafondmax=0;
    
    while(x<=320){
        while(y<=240){                                
            
            if(air[((phg[0]+x)-((phg[0]+x)%CASE))/CASE][((phg[1]+y)-((phg[1]+y)%CASE))/CASE ]>0) {
                plafond[plafondmax][0]=x;
                plafond[plafondmax][1]=y;
                plafondmax++;
            }    
            
            val=map[((phg[0]+x)-((phg[0]+x)%CASE))/CASE][((phg[1]+y)-((phg[1]+y)%CASE))/CASE ];
            
            if (val != 999) { // image vide
            
                if (val==1) {
                    switch (anim%4) {
                        case 0 : val=128; break;
                        case 2 : val=129; break;                    
                    }    
                }
                if ((val >= 425 && val <= 452) || (val >= 457 && val <= 466)
                    || val == 540 || val == 543) {
                    switch (anim%3) {
                        case 0 : val++; break;
                        case 1 : val+=2; break;
                    }    
                }
                if (val==1181) {
                    switch (anim%4) {
                        case 1 : case 3 : val++; break;
                        case 2 : val+=2; break;
                    }
                }
                if (val>=1305 && val<=1317) {
                    switch (anim%8) {
                        case 0 : case 1 : val+=13; break;
                        case 2 : case 3 : val+=26; break;
                        case 4 : case 5 : val+=13; break;
                    }
                }
            }

            SDL_Surface* img = image[(int)(val/1600)];
            val = val%1600;

            src.x=(val%40)*16; src.y=((int)(val/40))*16;
            if(x==0)src.x+=phg[0]%CASE;
            src.w=CASE-(src.x%CASE);
            if(y==0)src.y+=phg[1]%CASE;
            src.h=CASE-(src.y%CASE);
            dst.x = x; dst.y = y;
            
            if (val != 999) SDL_BlitSurface(img, &src, screen, &dst);
            
            y+=src.h;
        }    
        y=0;
        x+=src.w;
    }
    
    bool stop = gpJeu->getStop();
    bool transition = gpJeu->getTransition();
    
    if(SDL_GetTicks() > lastAnimTime + ANIM_REFRESH && (!stop || transition)) {
        lastAnimTime = SDL_GetTicks();
        anim++;            
        if (anim > 59) anim = 0;
    }
}


void Monde::drawAir(SDL_Surface* screen, int phg[2]) {
    int x=0,y=0,val=0,cpt=0;
    SDL_Rect src;    
    SDL_Rect dst;
    
    dst.x = 0;
    dst.y = 0;    
    while(cpt<plafondmax) {
        x=plafond[cpt][0];
        y=plafond[cpt][1];
        val=air[((phg[0]+x)-((phg[0]+x)%CASE))/CASE][((phg[1]+y)-((phg[1]+y)%CASE))/CASE];
        
        if (val > 0) {
        
            if (val==760) {
                switch (anim%3) {
                    case 0 : val+=3; break;
                    case 1 : val+=5; break;
                }
            }
            if (val==762) {
                switch (anim%3) {
                    case 0 : val+=2; break;
                    case 1 : val+=4; break;
                }
            }
            if (val==1193) {
                switch (anim%4) {
                    case 0 : case 2 : val++; break;
                    case 1 : val+=2; break;
                }
            }
        
        
            SDL_Surface* img = image[(int)(val/1600)];
            val = val%1600;

            src.x=(val%40)*16; src.y=((int)(val/40))*16;
            if(x==0)src.x+=phg[0]%CASE;
            src.w=CASE-(src.x%CASE);
            if(y==0)src.y+=phg[1]%CASE;
            src.h=CASE-(src.y%CASE);
            dst.x = x; dst.y = y;
            
            SDL_BlitSurface(img, &src, screen, &dst);
            
        }

        cpt++;
    }
    
    if (niveau > 0) {
        if(gpJeu->isDehors()) niveau=0;
        else {
            if (!gpJeu->getTransition() && !gpJeu->getStop()) niveau--;
            src.h = 16; src.w = 32;src.x = 0;src.y=32-(etage()*16);
            dst.x = 288; dst.y = 0;
            SDL_BlitSurface(imagelevel, &src, screen, &dst);
        }
    }
    
    if (gpJeu->getTransition() && animtransC>10) {
        dst.x = 0; dst.y = 0;
        SDL_BlitSurface(imagetransit, NULL, screen, &dst);
    }
}



void Monde::chargeMap(int zone) {
    int cpt1=0,cpt2=0,a,b;
    
    for (int i = 0; i < 4; i++) region[i]=0;
    for (int i = 0; i < 350; i++) 
        for (int j = 0; j < 350; j++) 
            setValeur(i*CASE, j*CASE, 0, 0, 0, 0);
            
    std::ostringstream oss;
    oss << zone;
    std::string result = "romfs:/map/carte" + oss.str() + ".map";
    std::ifstream file(result.c_str());
    
    file >> largeur;
    file >> hauteur;

    for (int i=0; i<largeur/16; i++){    
    for (int j=0; j<hauteur/16; j++){
        file >> map[i][j];
        file >> mur[i*2][j*2];
        file >> mur[i*2+1][j*2];
        file >> mur[i*2][j*2+1];
        file >> mur[i*2+1][j*2+1];
    }
    }
    
    file >> cpt1;
    for (int i=0; i<cpt1; i++) {
        file >> a;
        file >> b;
        file >> air[a][b];
    }
    
    file >> cpt2;
    for (int i=0; i<cpt2; i++) {
        file >> a;
        file >> b;
        file >> contenu[a][b];
    }
    corrigeMap(zone);
}

void Monde::transitFull(int nzone, int nx, int ny) {
    gpJeu->setStop(true);
    gpJeu->setTransition(true);
    if (gpJeu->getZone() != nzone) gpJeu->videListes();
    else niveau = 100;
    gpJeu->setNewZone(nzone);
    gpJeu->setNewX(nx);
    gpJeu->setNewY(ny);
    
    animtransC=10;
}

void Monde::transitSalle(Direction dir) {
    Joueur* gpJoueur = gpJeu->getJoueur();
    if (gpJoueur->getTypeAnim()==EMMENE) gpJoueur->setTypeAnim(MARCHE);
    
    gpJeu->setStop(true);
    gpJeu->setTransition(true);
    
    switch(dir){
        case N : animtransY=-248; gpJoueur->setY(gpJoueur->getY()-4); break;
        case S : animtransY=248; gpJoueur->setY(gpJoueur->getY()+20); break;
        case O : animtransX=-328; gpJoueur->setX(gpJoueur->getX()-4); break;
        case E : animtransX=328; gpJoueur->setX(gpJoueur->getX()+20); break;
    }
    
    //changement de sous zone par défaut
    switch (dir) {
        case N : region[0]=((int)(gpJoueur->getX()/320))*320; region[1]-=15*16; break;
        case S : region[0]=((int)(gpJoueur->getX()/320))*320; region[1]=region[3]; break;
        case O : region[0]-=20*16; region[1]=((int)(gpJoueur->getY()/240))*240; break;
        case E : region[0]=region[2]; region[1]=((int)(gpJoueur->getY()/240))*240; break;
    }
    region[3]=region[1]+15*16;
    region[2]=region[0]+20*16;
}

void Monde::transitZone(int nzone, int decx, int decy, Direction dir) {
    Joueur* gpJoueur = gpJeu->getJoueur();
    if (gpJoueur->getTypeAnim()==EMMENE) gpJoueur->setTypeAnim(MARCHE);
    int phg[2];
    phg[0] = gpJeu->getPhg(0);
    phg[1] = gpJeu->getPhg(1);
    
    gpJeu->setStop(true);
    gpJeu->setTransition(true);
    gpJeu->videListes();
    gpJeu->setNewZone(nzone);
    
    animtransX=0;
    animtransY=0;
    
    int nx = gpJoueur->getX()-320*decx;
    int ny = gpJoueur->getY()-240*decy;
    
    int savx=gpJoueur->getX()-phg[0];
    int savy=gpJoueur->getY()-phg[1];
    
    for (int i=0; i<42; i++)
        for (int j=0; j<32; j++){
            transMap[i][j]=0;
            transAir[i][j]=0;
        }
    
    //copie de la partie de la carte dans transMap et transAir
    int cor1=0,cor2=0;
    
    int ind1=21, ind2=16;
    if(phg[0]%16==0)ind1=20;
    if(phg[1]%16==0)ind2=15;
    switch(dir){
        case N:cor2=ind2;break;
        case O:cor1=ind1;break;
        default : break;
    }
    
    for (int i=0; i<ind1; i++){
        for (int j=0; j<ind2; j++){
            transMap[i+cor1][j+cor2]=map[((int)((phg[0])/16))+i][((int)((phg[1])/16))+j];
            transAir[i+cor1][j+cor2]=air[((int)((phg[0])/16))+i][((int)((phg[1])/16))+j];
        }
    }
    
    //changement de zone
    chargeMap(nzone);
    
    phg[0]-=decx*320;
    phg[1]-=decy*240;
    if (dir==N) phg[1]-=240;
    if (dir==S) phg[1]+=240;
    if (dir==O) phg[0]-=320;
    if (dir==E) phg[0]+=320;
    
    //copie de la partie de la nouvelle carte dans transMap et transAir
    cor1=0,cor2=0;
    switch(dir){
        case S:cor2=ind2;break;
        case E:cor1=ind1;break;
        default : break;
    }
    for (int i=0; i<ind1; i++){
        for (int j=0; j<ind2; j++){
            transMap[i+cor1][j+cor2]=map[((int)((phg[0])/16))+i][((int)((phg[1])/16))+j];
            transAir[i+cor1][j+cor2]=air[((int)((phg[0])/16))+i][((int)((phg[1])/16))+j];
        }
    }
    
    //copie de transMap vers map
    for (int i=0; i<42; i++){
        for (int j=0; j<32; j++){
            map[i][j]=transMap[i][j];
            air[i][j]=transAir[i][j];
        }
    }
    
    phg[0]=((phg[0])%16);
    phg[1]=((phg[1])%16);
    
    gpJoueur->setX(savx+phg[0]);
    gpJoueur->setY(savy+phg[1]);
    
    switch(dir){
        case N :
            ny-=(28+8);
            phg[1]+=16*ind2;
            gpJoueur->setX(gpJoueur->getX()%320);
            gpJoueur->setY(gpJoueur->getY()%240+240);
            animtransY=-248;
            break;
        case S :
            ny+=(28+4);
            gpJoueur->setX(gpJoueur->getX()%320);
            gpJoueur->setY(gpJoueur->getY()%240+16);
            animtransY=248;
            break;
        case O :
            nx-=28;
            phg[0]+=16*ind1;
            gpJoueur->setX(gpJoueur->getX()%320+320-4);
            gpJoueur->setY(gpJoueur->getY()%240);
            animtransX=-328;
            break;
        case E :
            nx+=28;
            gpJoueur->setX(gpJoueur->getX()%320+16+4);
            gpJoueur->setY(gpJoueur->getY()%240);
            animtransX=328;
            break;
    }
    
    gpJeu->setNewX(nx);
    gpJeu->setNewY(ny);
    
    //std::cout<< savx<< "  " <<nx <<std::endl;
    
    gpJeu->setPhg(phg[0],phg[1]);
}

void Monde::defilTransit() {
    int phg[2];
    phg[0] = gpJeu->getPhg(0);
    phg[1] = gpJeu->getPhg(1);
    Joueur* gpJoueur = gpJeu->getJoueur();
    int x = gpJoueur->getX();
    int y = gpJoueur->getY();
        
    if(animtransX>0) {
        phg[0]+=8;
        if(animtransX>=328-4) gpJoueur->setX(x+8);
        animtransX-=8;
        if(animtransX<=0){
            if (gpJeu->isDonjon() || gpJeu->isGrotte()) detRegion(gpJeu->getZone());
            gpJoueur->saveSemiEmpl();
            if (gpJeu->isDehors()) gpJeu->finZone();
            gpJeu->setStop(false);
            gpJeu->setTransition(false);
            return;
        }
    }
    if(animtransX<0) {
        phg[0]-=8;
        if(animtransX<=-328+20) gpJoueur->setX(x-8);
        animtransX+=8;
        if(animtransX>=0){
            if (gpJeu->isDonjon() || gpJeu->isGrotte()) detRegion(gpJeu->getZone());
            gpJoueur->saveSemiEmpl();
            if (gpJeu->isDehors()) gpJeu->finZone();
            gpJeu->setStop(false);
            gpJeu->setTransition(false);
            return;
        }
    }
    if(animtransY>0) {
        phg[1]+=8;
        if(animtransY>=248-8) gpJoueur->setY(y+8);
        animtransY-=8;
        if(animtransY<=0){
            if (gpJeu->isDonjon() || gpJeu->isGrotte()) detRegion(gpJeu->getZone());
            gpJoueur->saveSemiEmpl();
            if (gpJeu->isDehors()) gpJeu->finZone();
            gpJeu->setStop(false);
            gpJeu->setTransition(false);
            return;
        }
    }
    if(animtransY<0) {
        phg[1]-=8;
        if(animtransY<=-248+32-8+8) gpJoueur->setY(y-8);
        animtransY+=8;
        if(animtransY>=0){
            if (gpJeu->isDonjon() || gpJeu->isGrotte()) detRegion(gpJeu->getZone());
            gpJoueur->saveSemiEmpl();
            if (gpJeu->isDehors()) gpJeu->finZone();
            gpJeu->setStop(false);
            gpJeu->setTransition(false);
            return;
        }
    }
    if(animtransC>0) {
        animtransC+=10;
        SDL_FillRect(imagetransit, NULL, SDL_MapRGB(imagetransit->format, 0, 0, 0));
        int rayon;
        if (animtransC <=320) rayon = 320-animtransC;
        else rayon=animtransC-330;
        if (animtransC < 310 || animtransC >= 350) filledEllipseRGBA(imagetransit,
                        gpJeu->getJoueur()->getX()-phg[0]+8, 
                        gpJeu->getJoueur()->getY()-phg[1]+16,
                        rayon, rayon*2/3,
                        0, 0, 255, 255);
        SDL_SetColorKey(imagetransit,SDL_SRCCOLORKEY, SDL_MapRGB(imagetransit->format,0,0,255));
        if (animtransC==330) { 
            gpJeu->finZone();
            if (gpJoueur->getTypeAnim()!=TOUCHE && gpJoueur->getTypeAnim()!=MORT)
                gpJoueur->setTypeAnim(AUCUNE);
            return;
        }
        if (animtransC>=650) { 
            gpJeu->testAnim();
            if (!gpJeu->getText()) gpJeu->setStop(false);
            gpJeu->setTransition(false);
            animtransC=0;
            return;
        }
    }
    
    gpJeu->setPhg(phg[0],phg[1]);
}

void Monde::passage(int x, int y, int id) {
    if (gpJeu->isDehors()) {
        if (id < 2) {
            map[x][y]=534; mur[x*2+1][y*2+1]=0;
            map[x+1][y]=535; mur[(x+1)*2][y*2+1]=0;
            map[x][y+1]=536; mur[x*2+1][(y+1)*2]=0; mur[x*2+1][(y+1)*2+1]=0;
            map[x+1][y+1]=537; mur[(x+1)*2][(y+1)*2]=0; mur[(x+1)*2][(y+1)*2+1]=0;
            if (id==1) {
                map[x][y+1]=792;
                map[x+1][y+1]=793;
            }
        }
        if (id==2) {
            map[x][y]=971;
            setValeur((x+1)*16, y*16, 972, 999, LIBRE, 0);
            map[x+2][y]=973;
            map[x][y+1]=974;
            setValeur((x+1)*16, (y+1)*16, 975, 999, LIBRE, 0);
            map[x+2][y+1]=976;
            map[x][y+2]=977;
            setValeur((x+1)*16, (y+2)*16, 978, 999, LIBRE, 0);
            map[x+2][y+2]=979;
            mur[x*2][(y+2)*2]=1;
            mur[x*2+5][(y+2)*2]=1;
        }
        return;
    }
    switch (gpJeu->getZone()) {
        case 12 :
            if (id == 0) {
                map[x][y]=1150; map[x+1][y]=1151;
                air[x][y]=1152; air[x+1][y]=1153;
                map[x][y+1]=1068; map[x+1][y+1]=1069;        
                map[x][y-1]=1154; map[x+1][y-1]=1155;
                air[x][y-1]=1156; air[x+1][y-1]=1157;
                map[x][y-2]=1071; map[x+1][y-2]=1072;        
                for (int j = y*2-3; j < y*2+4; j++) mur[x*2+1][j]=0;
                for (int j = y*2-3; j < y*2+4; j++) mur[x*2+2][j]=0;
            }
            if (id == 1) {
                map[x][y]=1158; map[x][y+1]=1159;
                map[x][y+2]=1160; air[x][y+1]=1161; air[x][y]=1162;
                map[x+1][y]=1077; map[x+1][y+1]=1079; map[x+1][y+2]=1081;           
                map[x-1][y]=1163; map[x-1][y+1]=1164;
                map[x-1][y+2]=1165; air[x-1][y+1]=1166; air[x-1][y]=1167;
                map[x-2][y]=1083; map[x-2][y+1]=1085; map[x-2][y+2]=1087;
                for (int j = x*2-3; j < x*2+4; j++) mur[j][y*2+2]=0;
                for (int j = x*2-3; j < x*2+4; j++) mur[j][y*2+3]=0;
            }
            if (id == 3) {
                for (int jj=y*2+1;jj<y*2+3; jj++)
                    for (int ii=x*2-3; ii<x*2+3; ii++)
                        mur[ii][jj]=0;
                map[x][y]=1114+4;map[x+1][y]=1115+4;air[x][y]=1118+4;
                map[x][y+1]=1116+4;map[x+1][y+1]=1117+4;air[x][y+1]=1119+4;
                map[x-2][y]=1120+4;map[x-1][y]=1121+4;air[x-1][y]=1124+4;
                map[x-2][y+1]=1122+4;map[x-1][y+1]=1123+4;air[x-1][y+1]=1125+4;
            }
            if (id == 4) {
                for (int jj=y*2-3;jj<y*2+3; jj++)
                    for (int ii=x*2+1; ii<x*2+3; ii++)
                        mur[ii][jj]=0;
                map[x][y]=1102+4;map[x+1][y]=1103+4;air[x][y]=1106+4;
                map[x][y+1]=1104+4;map[x+1][y+1]=1105+4;air[x+1][y]=1107+4;
                map[x][y-2]=1108+4;map[x+1][y-2]=1109+4;air[x][y-1]=1112+4;
                map[x][y-1]=1110+4;map[x+1][y-1]=1111+4;air[x+1][y-1]=1113+4;
            }
            break;
        case 13 :
            if (id == 0) {
                map[x][y]=1283; map[x+1][y]=1284;
                air[x][y]=1285; air[x+1][y]=1286;
                map[x][y+1]=1241; map[x+1][y+1]=1242;
                map[x][y-1]=1287; map[x+1][y-1]=1288;
                air[x][y-1]=1289; air[x+1][y-1]=1290;
                map[x][y-2]=1244; map[x+1][y-2]=1245;        
                for (int j = y*2-3; j < y*2+4; j++) mur[x*2+1][j]=0;
                for (int j = y*2-3; j < y*2+4; j++) mur[x*2+2][j]=0;
            }
            if (id == 1) {
                map[x][y]=1291; map[x][y+1]=1292;
                map[x][y+2]=1293; air[x][y+1]=1294; air[x][y]=1295;
                map[x+1][y]=1250; map[x+1][y+1]=1252; map[x+1][y+2]=1254;           
                map[x-1][y]=1296; map[x-1][y+1]=1297;
                map[x-1][y+2]=1298; air[x-1][y+1]=1299; air[x-1][y]=1300;
                map[x-2][y]=1256; map[x-2][y+1]=1258; map[x-2][y+2]=1260;
                for (int j = x*2-3; j < x*2+4; j++) mur[j][y*2+2]=0;
                for (int j = x*2-3; j < x*2+4; j++) mur[j][y*2+3]=0;
            }
            break;
        case 14 :
            if (id == 0) {
                map[x][y]=1397; map[x+1][y]=1398;
                air[x][y]=1401; air[x+1][y]=1402;
                map[x][y+1]=1399; map[x+1][y+1]=1400;
                map[x][y-1]=1405; map[x+1][y-1]=1406;
                air[x][y-1]=1407; air[x+1][y-1]=1408;
                map[x][y-2]=1403; map[x+1][y-2]=1404;
                for (int j = y*2-3; j < y*2+4; j++) mur[x*2+1][j]=0;
                for (int j = y*2-3; j < y*2+4; j++) mur[x*2+2][j]=0;
            }
            if (id == 1) {
                map[x][y]=1409; map[x][y+1]=1411;
                map[x][y+2]=1413; air[x][y+1]=1415;
                map[x+1][y]=1410; map[x+1][y+1]=1412; map[x+1][y+2]=1414;
                map[x-1][y]=1417; map[x-1][y+1]=1419;
                map[x-1][y+2]=1421; air[x-1][y+1]=1422;
                map[x-2][y]=1416; map[x-2][y+1]=1418; map[x-2][y+2]=1420;
                for (int j = x*2-3; j < x*2+4; j++) mur[j][y*2+2]=0;
                for (int j = x*2-3; j < x*2+4; j++) mur[j][y*2+3]=0;
            }
            if (id == 3) {
                for (int jj=y*2+1;jj<y*2+3; jj++)
                for (int ii=x*2-3; ii<x*2+3; ii++)
                mur[ii][jj]=0;
                map[x][y]=1464;map[x+1][y]=1465;air[x][y]=1468;
                map[x][y+1]=1466;map[x+1][y+1]=1467;air[x][y+1]=1469;
                map[x-2][y]=1470;map[x-1][y]=1471;air[x-1][y]=1474;
                map[x-2][y+1]=1472;map[x-1][y+1]=1473;air[x-1][y+1]=1475;
            }
            break;
        case 15 :
            if (id == 0) {
                map[x][y]=1543; map[x+1][y]=1544;
                air[x][y]=1547; air[x+1][y]=1548;
                map[x][y+1]=1545; map[x+1][y+1]=1546;
                map[x][y-1]=1551; map[x+1][y-1]=1552;
                air[x][y-1]=1553; air[x+1][y-1]=1554;
                map[x][y-2]=1549; map[x+1][y-2]=1550;        
                for (int j = y*2-3; j < y*2+4; j++) mur[x*2+1][j]=0;
                for (int j = y*2-3; j < y*2+4; j++) mur[x*2+2][j]=0;
            }
            if (id == 1) {
                map[x][y]=1555; map[x][y+1]=1557;
                map[x][y+2]=1559; air[x][y+1]=1561;
                map[x+1][y]=1556; map[x+1][y+1]=1558; map[x+1][y+2]=1560;           
                map[x-1][y]=1563; map[x-1][y+1]=1565;
                map[x-1][y+2]=1567; air[x-1][y+1]=1568;
                map[x-2][y]=1562; map[x-2][y+1]=1564; map[x-2][y+2]=1566;
                for (int j = x*2-3; j < x*2+4; j++) mur[j][y*2+2]=0;
                for (int j = x*2-3; j < x*2+4; j++) mur[j][y*2+3]=0;
            }
            break;
        case 16 :
            if (id == 0) {
                map[x][y]=1710; map[x+1][y]=1711;
                air[x][y]=1712; air[x+1][y]=1713;
                map[x][y+1]=1665; map[x+1][y+1]=1666;
                map[x][y-1]=1734; map[x+1][y-1]=1735;
                air[x][y-1]=1736; air[x+1][y-1]=1737;
                map[x][y-2]=1668; map[x+1][y-2]=1669;
                for (int j = y*2-3; j < y*2+4; j++) mur[x*2+1][j]=0;
                for (int j = y*2-3; j < y*2+4; j++) mur[x*2+2][j]=0;
            }
            if (id == 1) {
                map[x][y]=1698; map[x][y+1]=1699;
                map[x][y+2]=1700; air[x][y+1]=1701;
                map[x+1][y]=1673; map[x+1][y+1]=1675; map[x+1][y+2]=1677;           
                map[x-1][y]=1702; map[x-1][y+1]=1703;
                map[x-1][y+2]=1704; air[x-1][y+1]=1705;
                map[x-2][y]=1679; map[x-2][y+1]=1681; map[x-2][y+2]=1683;
                for (int j = x*2-3; j < x*2+4; j++) mur[j][y*2+2]=0;
                for (int j = x*2-3; j < x*2+4; j++) mur[j][y*2+3]=0;
            }
            break;
        case 17 :
        case 23 :
            if (id == 0) {
                map[x][y]=1802; map[x+1][y]=1803;
                air[x][y]=1806; air[x+1][y]=1807;
                map[x][y+1]=1804; map[x+1][y+1]=1805;
                map[x][y-1]=1810; map[x+1][y-1]=1811;
                air[x][y-1]=1800; air[x+1][y-1]=1801;
                map[x][y-2]=1808; map[x+1][y-2]=1809;
                for (int j = y*2-3; j < y*2+4; j++) mur[x*2+1][j]=0;
                for (int j = y*2-3; j < y*2+4; j++) mur[x*2+2][j]=0;
            }
            if (id == 1) {
                map[x][y]=1812; map[x][y+1]=1814;
                map[x][y+2]=1816; air[x][y+1]=1818;
                map[x+1][y]=1813; map[x+1][y+1]=1815; map[x+1][y+2]=1817;           
                map[x-1][y]=1820; map[x-1][y+1]=1822;
                map[x-1][y+2]=1824; air[x-1][y+1]=1825;
                map[x-2][y]=1819; map[x-2][y+1]=1821; map[x-2][y+2]=1823;
                for (int j = x*2-3; j < x*2+4; j++) mur[j][y*2+2]=0;
                for (int j = x*2-3; j < x*2+4; j++) mur[j][y*2+3]=0;
            }
            break;
        case 18 :
            if (id == 0) {
                map[x][y]=1933; map[x+1][y]=1934;
                air[x][y]=1937; air[x+1][y]=1938;
                map[x][y+1]=1935; map[x+1][y+1]=1936;
                map[x][y-1]=1941; map[x+1][y-1]=1942;
                air[x][y-1]=1931; air[x+1][y-1]=1932;
                map[x][y-2]=1939; map[x+1][y-2]=1940;
                for (int j = y*2-3; j < y*2+4; j++) mur[x*2+1][j]=0;
                for (int j = y*2-3; j < y*2+4; j++) mur[x*2+2][j]=0;
            }
            if (id == 1) {
                map[x][y]=1943; map[x][y+1]=1945;
                map[x][y+2]=1947; air[x][y+1]=1949;
                map[x+1][y]=1944; map[x+1][y+1]=1946; map[x+1][y+2]=1948;
                map[x-1][y]=1951; map[x-1][y+1]=1953;
                map[x-1][y+2]=1955; air[x-1][y+1]=1956;
                map[x-2][y]=1950; map[x-2][y+1]=1952; map[x-2][y+2]=1954;
                for (int j = x*2-3; j < x*2+4; j++) mur[j][y*2+2]=0;
                for (int j = x*2-3; j < x*2+4; j++) mur[j][y*2+3]=0;
            }
            break;
        case 19 :
            if (id == 0) {
                map[x][y]=2065; map[x+1][y]=2066;
                air[x][y]=2069; air[x+1][y]=2070;
                map[x][y+1]=2067; map[x+1][y+1]=2068;
                map[x][y-1]=2073; map[x+1][y-1]=2074;
                air[x][y-1]=2075; air[x+1][y-1]=2076;
                map[x][y-2]=2071; map[x+1][y-2]=2072;
                for (int j = y*2-3; j < y*2+4; j++) mur[x*2+1][j]=0;
                for (int j = y*2-3; j < y*2+4; j++) mur[x*2+2][j]=0;
            }
            if (id == 1) {
                map[x][y]=2077; map[x][y+1]=2079;
                map[x][y+2]=2081; air[x][y+1]=2083;
                map[x+1][y]=2078; map[x+1][y+1]=2080; map[x+1][y+2]=2082;
                map[x-1][y]=2085; map[x-1][y+1]=2087;
                map[x-1][y+2]=2089; air[x-1][y+1]=2090;
                map[x-2][y]=2084; map[x-2][y+1]=2086; map[x-2][y+2]=2088;
                for (int j = x*2-3; j < x*2+4; j++) mur[j][y*2+2]=0;
                for (int j = x*2-3; j < x*2+4; j++) mur[j][y*2+3]=0;
            }
            break;
        case 20 :
            int add=0;
            if (x>99) add=124;
            if (id == 0) {
                map[x][y]=2197+add; map[x+1][y]=2198+add;
                air[x][y]=2201+add; air[x+1][y]=2202+add;
                map[x][y+1]=2199+add; map[x+1][y+1]=2200+add;
                map[x][y-1]=2205+add; map[x+1][y-1]=2206+add;
                air[x][y-1]=2207+add; air[x+1][y-1]=2208+add;
                map[x][y-2]=2203+add; map[x+1][y-2]=2204+add;
                for (int j = y*2-3; j < y*2+4; j++) mur[x*2+1][j]=0;
                for (int j = y*2-3; j < y*2+4; j++) mur[x*2+2][j]=0;
            }
            if (id == 1) {
                map[x][y]=2209+add; map[x][y+1]=2211+add;
                map[x][y+2]=2213+add; air[x][y+1]=2215+add;
                map[x+1][y]=2210+add; map[x+1][y+1]=2212+add; map[x+1][y+2]=2214+add;
                map[x-1][y]=2217+add; map[x-1][y+1]=2219+add;
                map[x-1][y+2]=2221+add; air[x-1][y+1]=2222+add;
                map[x-2][y]=2216+add; map[x-2][y+1]=2218+add; map[x-2][y+2]=2220+add;
                for (int j = x*2-3; j < x*2+4; j++) mur[j][y*2+2]=0;
                for (int j = x*2-3; j < x*2+4; j++) mur[j][y*2+3]=0;
            }
            if (id == 3) {
                for (int jj=y*2+1;jj<y*2+3; jj++)
                for (int ii=x*2-3; ii<x*2+3; ii++)
                mur[ii][jj]=0;
                map[x][y]=2395;map[x+1][y]=2396;air[x][y]=2399;
                map[x][y+1]=2397;map[x+1][y+1]=2398;air[x][y+1]=2400;
                map[x-2][y]=2401;map[x-1][y]=2402;air[x-1][y]=2405;
                map[x-2][y+1]=2403;map[x-1][y+1]=2404;air[x-1][y+1]=2406;
            }
            break;
    }
}

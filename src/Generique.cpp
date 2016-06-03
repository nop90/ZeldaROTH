/*

    Zelda Return of the Hylian

    Copyright (C) 2005-2008  Vincent Jouillat

    Please send bugreports with examples or suggestions to www.zeldaroth.fr

*/

#include "Generique.h"
#include "Jeu.h"
#include "Keyboard.h"
#include "os_3ds.h"

Generique::Generique(Jeu* jeu) : gpJeu(jeu), anim(0) {
    imageFin = NULL;
    imageArbre = NULL;
    image = SDL_CreateRGBSurface(SDL_HWSURFACE, 320, 240, 32, 0, 0, 0, 0);
    imageTitre = IMG_Load("romfs:/images/logos/fond.png");
    SDL_SetColorKey(imageTitre,SDL_SRCCOLORKEY,SDL_MapRGB(imageTitre->format,0,0,255));
    imageCurseur = IMG_Load("romfs:/images/logos/curseur.png");
    SDL_SetColorKey(imageCurseur,SDL_SRCCOLORKEY,SDL_MapRGB(imageCurseur->format,0,0,255));
    imageNiveau = IMG_Load("romfs:/images/logos/niveau.png");
    SDL_SetColorKey(imageNiveau,SDL_SRCCOLORKEY,SDL_MapRGB(imageNiveau->format,0,0,255));
    imageFee = IMG_Load("romfs:/images/logos/fee.png");
    imageCadre = IMG_Load("romfs:/images/logos/cadres.png");
    imageFond1 = NULL;
    imageFond2 = NULL;
    imageNuit = NULL;
    for (int i = 0; i < 5; i++) imageIntro[i]=NULL;
    imageBackground = SDL_CreateRGBSurface(0, 320, 240, 32, 0, 0, 0, 0);

    SDL_Rect src; src.w = 16; src.h = 16;
    SDL_Rect dst;
    src.x = 16; src.y = 16;
    for (int j = 0; j < 240; j+=16)
        for (int i = 0; i < 320; i+=16) {
            dst.x = i; dst.y = j; SDL_BlitSurface(imageCadre, &src, imageBackground, &dst);
        }

    imageFlags = IMG_Load("romfs:/images/logos/flags.png");
}

Generique::~Generique() {
    SDL_FreeSurface(imageFin);
    SDL_FreeSurface(imageArbre);
    SDL_FreeSurface(image);
    SDL_FreeSurface(imageTitre);
    SDL_FreeSurface(imageCurseur);
    SDL_FreeSurface(imageNiveau);
    SDL_FreeSurface(imageFee);
    SDL_FreeSurface(imageCadre);
    SDL_FreeSurface(imageFond1);
    SDL_FreeSurface(imageFond2);
    SDL_FreeSurface(imageNuit);
    for (int i = 0; i < 5; i++) SDL_FreeSurface(imageIntro[i]);
    SDL_FreeSurface(imageBackground);
    SDL_FreeSurface(imageFlags);
}

void Generique::drawFin(SDL_Surface* gpScreen) {
    if (!imageFin) imageFin = IMG_Load("romfs:/images/logos/fin.png");
    SDL_Rect dst; dst.x = 0; dst.y = 0;
    SDL_BlitSurface(imageFin, NULL, gpScreen, &dst);
    gpJeu->getTexte()->draw(gpScreen);
}

void Generique::initDisclamer() {
    SDL_Rect dst;
    SDL_Surface* attention = IMG_Load("romfs:/images/logos/attention.png");
    dst.x = 0; dst.y = 0; SDL_BlitSurface(attention, NULL, image, &dst);
    SDL_FreeSurface(attention);
}

void Generique::initLogo() {
    SDL_Rect dst;
    SDL_Surface* logo = IMG_Load("romfs:/images/logos/logo.png");
    dst.x = 0; dst.y = 0; SDL_BlitSurface(logo, NULL, image, &dst);
    SDL_FreeSurface(logo);
}

void Generique::initTitre() {
    SDL_Rect dst;
//    if (imageTitre) SDL_FreeSurface(imageTitre);
//    SDL_Surface* imageTitre = IMG_Load("romfs:/images/logos/fond.png");
//    dst.x = 0; dst.y = 0; SDL_BlitSurface(fond, NULL, image, &dst);
//    SDL_FreeSurface(fond);
    SDL_Surface* logo = IMG_Load("romfs:/images/logos/titre.png");
    SDL_SetColorKey(logo,SDL_SRCCOLORKEY,SDL_MapRGB(logo->format,0,0,255));
    //logo = SDL_DisplayFormat(logo); 
    dst.x = 60; dst.y = 56-32; SDL_BlitSurface(logo, NULL, imageTitre, &dst);
    SDL_FreeSurface(logo);
}

void Generique::initSelection() {
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
    
    for (int i = 0; i < 6; i++) {
        src.x = 16; src.y = 0; dst.x = 32+16*i; dst.y = 0; 
        SDL_BlitSurface(imageCadre, &src, image, &dst);
        src.x = 16; src.y = 64; dst.x = 32+16*i; dst.y = 16; 
        SDL_BlitSurface(imageCadre, &src, image, &dst);
        src.x = 16; src.y = 32; dst.x = 32+16*i; dst.y = 32; 
        SDL_BlitSurface(imageCadre, &src, image, &dst);
    }
    
    src.x = 32; src.y = 0; dst.x = 128; dst.y = 0; 
    SDL_BlitSurface(imageCadre, &src, image, &dst);
    src.x = 32; src.y = 16; dst.x = 128; dst.y = 16; 
    SDL_BlitSurface(imageCadre, &src, image, &dst);
    src.x = 32; src.y = 32; dst.x = 128; dst.y = 32; 
    SDL_BlitSurface(imageCadre, &src, image, &dst);
    
    cadre(16,48,288,32);
    cadre(16,96,288,32);
    cadre(16,144,288,32);
    cadre(16,192,136,32);
    cadre(168,192,136,32);
    
switch(getLanguage()) {

case 2: // French
   gpJeu->affiche(image, "SELECTION", 40, 16);
    gpJeu->affiche(image, "1.", 44, 56);
    gpJeu->affiche(image, "2.", 44, 104);
    gpJeu->affiche(image, "3.", 44, 152);
    gpJeu->affiche(image, "OPTIONS", 63, 200);
    gpJeu->affiche(image, "RECORDS", 215, 200);
break;

case 3: // German
    gpJeu->affiche(image, "AUSWAHL-SEITE", 40, 16);
    gpJeu->affiche(image, "1.", 44, 56);
    gpJeu->affiche(image, "2.", 44, 104);
    gpJeu->affiche(image, "3.", 44, 152);
    gpJeu->affiche(image, "OPTIONEN", 62, 200);
    gpJeu->affiche(image, "REKORDE", 215, 200);
break;

case 4: // Italian
    gpJeu->affiche(image, "SALVATAGGI", 40, 16);
    gpJeu->affiche(image, "1.", 44, 56);
    gpJeu->affiche(image, "2.", 44, 104);
    gpJeu->affiche(image, "3.", 44, 152);
    gpJeu->affiche(image, "OPZIONI", 63, 200);
    gpJeu->affiche(image, "RECORD", 215, 200);
break;

case 5: // Spanish
    gpJeu->affiche(image, "SELECCION", 40, 16);
    gpJeu->affiche(image, "1.", 44, 56);
    gpJeu->affiche(image, "2.", 44, 104);
    gpJeu->affiche(image, "3.", 44, 152);
    gpJeu->affiche(image, "OPCIONES", 63-6, 200);
    gpJeu->affiche(image, "RECORDS", 215, 200);
break;

default:
    gpJeu->affiche(image, "PLAYER SELECT", 40, 16);
    gpJeu->affiche(image, "1.", 44, 56);
    gpJeu->affiche(image, "2.", 44, 104);
    gpJeu->affiche(image, "3.", 44, 152);
    gpJeu->affiche(image, "OPTIONS", 63, 200);
    gpJeu->affiche(image, "RECORDS", 215, 200);
break;	
}
    
    //stats :
    SDL_Surface* imageStat = IMG_Load("romfs:/images/statut/statut.png");
    SDL_SetColorKey(imageStat,SDL_SRCCOLORKEY,SDL_MapRGB(imageStat->format,0,0,255));
    SDL_Surface* inventaire = IMG_Load("romfs:/images/statut/inventaire.png");
    SDL_Surface* objets = IMG_Load("romfs:/images/statut/objets.png");
    
    for (int i = 0; i < 3; i++) {
        Joueur* gpJoueur = new Joueur(gpJeu,i+1);
        if (gpJoueur->getLoader()) {
            //vie
            if(gpJoueur->getVie()>gpJoueur->getVieMax()) gpJoueur->setVie(gpJoueur->getVieMax());
            for(int j=0;j<gpJoueur->getVieMax()/2;j++) {
                src.h = 7; src.w = 7; src.y=9;
                dst.x = 64+((j%10)*8)+8;dst.y = 57+8*((int)(j/10))+i*48;
                if (gpJoueur->getVie()-(j*2)>1)src.x=141;
                if (gpJoueur->getVie()-(j*2)==1)src.x=149;
                if (gpJoueur->getVie()-(j*2)<1)src.x=157;
                SDL_BlitSurface(imageStat, &src, image, &dst);
            }
        
            //cristaux
            dst.x=176+8-16; dst.y=56+i*48;
            src.x=16; src.y=17*4; src.w=16; src.h=17;
            for (int j=0; j<7; j++) {
                if (gpJoueur->hasCristal(j)) src.x=16; else src.x=32;
                SDL_BlitSurface(inventaire, &src, image, &dst);
                dst.x+=16-3;
            }
            
            //triforce
            dst.x=272; dst.y=56+i*48;
            src.x=128; src.y=0; src.w=17; src.h=16;
            if (gpJoueur->getEnnemi(50)) SDL_BlitSurface(objets, &src, image, &dst);
            
            gpJeu->getKeyboard()->setSave(i,1);
        }else gpJeu->getKeyboard()->setSave(i,0);
		delete gpJoueur;
	}
    
    SDL_FreeSurface(objets);
    SDL_FreeSurface(imageStat);
    SDL_FreeSurface(inventaire);
}

void Generique::initOption() {
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
    
    for (int i = 0; i < 4; i++) {
        src.x = 16; src.y = 0; dst.x = 32+16*i; dst.y = 0; 
        SDL_BlitSurface(imageCadre, &src, image, &dst);
        src.x = 16; src.y = 64; dst.x = 32+16*i; dst.y = 16; 
        SDL_BlitSurface(imageCadre, &src, image, &dst);
        src.x = 16; src.y = 32; dst.x = 32+16*i; dst.y = 32; 
        SDL_BlitSurface(imageCadre, &src, image, &dst);
    }
    
    src.x = 32; src.y = 0; dst.x = 96; dst.y = 0; 
    SDL_BlitSurface(imageCadre, &src, image, &dst);
    src.x = 32; src.y = 16; dst.x = 96; dst.y = 16; 
    SDL_BlitSurface(imageCadre, &src, image, &dst);
    src.x = 32; src.y = 32; dst.x = 96; dst.y = 32; 
    SDL_BlitSurface(imageCadre, &src, image, &dst);
    
    cadre(16,64,288,32);
    cadre(16,107,288,32);
    cadre(16,150,288,32);
    cadre(16,192,136,32);

    src.x = 0; src.y = 96; dst.x = 128; dst.y = 72; 
    SDL_BlitSurface(imageCadre, &src, image, &dst);
    for (int i = 144; i < 264; i+=16) {
        src.x = 16; src.y = 96; dst.x = i; dst.y = 72; 
        SDL_BlitSurface(imageCadre, &src, image, &dst);
    }
    src.x = 32; src.y = 96; dst.x = 264; dst.y = 72; 
    SDL_BlitSurface(imageCadre, &src, image, &dst);
    
    src.x = 0; src.y = 96; dst.x = 128; dst.y = 115; 
    SDL_BlitSurface(imageCadre, &src, image, &dst);
    for (int i = 144; i < 264; i+=16) {
        src.x = 16; src.y = 96; dst.x = i; dst.y = 115; 
        SDL_BlitSurface(imageCadre, &src, image, &dst);
    }
    src.x = 32; src.y = 96; dst.x = 264; dst.y = 115; 
    SDL_BlitSurface(imageCadre, &src, image, &dst);
}

void Generique::initRecord() {
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
    
    for (int i = 0; i < 4; i++) {
        src.x = 16; src.y = 0; dst.x = 32+16*i; dst.y = 0; 
        SDL_BlitSurface(imageCadre, &src, image, &dst);
        src.x = 16; src.y = 64; dst.x = 32+16*i; dst.y = 16; 
        SDL_BlitSurface(imageCadre, &src, image, &dst);
        src.x = 16; src.y = 32; dst.x = 32+16*i; dst.y = 32; 
        SDL_BlitSurface(imageCadre, &src, image, &dst);
    }
    
    src.x = 32; src.y = 0; dst.x = 96; dst.y = 0; 
    SDL_BlitSurface(imageCadre, &src, image, &dst);
    src.x = 32; src.y = 16; dst.x = 96; dst.y = 16; 
    SDL_BlitSurface(imageCadre, &src, image, &dst);
    src.x = 32; src.y = 32; dst.x = 96; dst.y = 32; 
    SDL_BlitSurface(imageCadre, &src, image, &dst);
    
    cadre(128,8,176,32);
    cadre(16,48,240,32); cadre(272,48,32,32);
    cadre(16,96,240,32); cadre(272,96,32,32);
    cadre(16,144,240,32); cadre(272,144,32,32);
    cadre(16,192,136,32);
    cadre(168,192,136,32);
    
    ostringstream oss;
    int temps = gpJeu->getKeyboard()->getTemps();
    if (temps > 359999) temps = 359999;
    int h = temps/3600;
    int m = (temps - ((int)(temps/3600))*3600)/60;
    int s = temps%60;
    if (h < 10) oss<<"0"; oss << h << ":";
    if (m < 10) oss<<"0"; oss << m << ":";
    if (s < 10) oss<<"0"; oss << s;
 
switch(getLanguage()) {

case 2: // French
    gpJeu->affiche(image, "RECORDS", 40, 16);
    gpJeu->affiche(image, "MEILLEUR TEMPS : " + oss.str(), 140, 16);
    gpJeu->affiche(image, "RANG DES 100%", 44, 56);
    gpJeu->affiche(image, "RANG ULTIME", 44, 104);
    gpJeu->affiche(image, "RANG DE RAPIDITE", 44, 152);
    gpJeu->affiche(image, "RETOUR", 63, 200);
    gpJeu->affiche(image, "EFFACER", 215, 200);
break;

case 3: // German
    gpJeu->affiche(image, "REKORDE", 40, 16);
    gpJeu->affiche(image, "BEST-ZEIT: " + oss.str(), 140, 16);
    gpJeu->affiche(image, "TITEL: ERBSENZÄHLER", 44, 56);
    gpJeu->affiche(image, "TITEL: ULTIMATIV-HELD", 44, 104);
    gpJeu->affiche(image, "TITEL: SUPER-SPRINTER", 44, 152);
    gpJeu->affiche(image, "ZURÜCK", 63, 200);
    gpJeu->affiche(image, "LÖSCHEN", 215, 200);
break;

case 4: // Italian
    gpJeu->affiche(image, "RECORD", 40, 16);
    gpJeu->affiche(image, "MIGLIOR TEMPO: " + oss.str(), 140, 16);
    gpJeu->affiche(image, "RANGO 100%", 44, 56);
    gpJeu->affiche(image, "RANGO ESTREMO", 44, 104);
    gpJeu->affiche(image, "RANGO VELOCITA'", 44, 152);
    gpJeu->affiche(image, "INDIETRO", 63, 200);
    gpJeu->affiche(image, "CANCELLA", 215, 200);
break;

case 5: // Spanish
    gpJeu->affiche(image, "RECORDS", 40, 16);
    gpJeu->affiche(image, "MEJOR TIEMPO : " + oss.str(), 140, 16);
    gpJeu->affiche(image, "RANGO DE LOS 100%", 44, 56);
    gpJeu->affiche(image, "RANGO ULTIMO", 44, 104);
    gpJeu->affiche(image, "RANGO DE RAPIDEZ", 44, 152);
    gpJeu->affiche(image, "ANTERIOR", 63-6, 200);
    gpJeu->affiche(image, "BORRAR", 215, 200);
break;

default:
    gpJeu->affiche(image, "RECORDS", 40, 16);
    gpJeu->affiche(image, "BEST TIME: " + oss.str(), 140, 16);
    gpJeu->affiche(image, "100% RANK", 44, 56);
    gpJeu->affiche(image, "ULTIMATE RANK", 44, 104);
    gpJeu->affiche(image, "SPEED RANK", 44, 152);
    gpJeu->affiche(image, "RETURN", 63, 200);
    gpJeu->affiche(image, "ERASE", 215, 200);
break;	
}    
       
    SDL_Surface* objets = IMG_Load("romfs:/images/statut/objets.png");
    
    //triforce
    src.y=0; src.w=17; src.h=16;
    
    for (int i = 0; i < 3; i++) {
        dst.x=279; dst.y=56+i*48;
        gpJeu->getKeyboard()->getRang(i) ? src.x=128 : src.x=145; 
        SDL_BlitSurface(objets, &src, image, &dst);}
    
    SDL_FreeSurface(objets);
}

void Generique::initEffacer() {
    cadre(104,84,112,72);

switch(getLanguage()) {

case 2: // French
    gpJeu->affiche(image, "EFFACER ?", 133, 93);
    gpJeu->affiche(image, "OUI", 151, 113);
    gpJeu->affiche(image, "NON", 151, 133);
break;

case 3: // German
    gpJeu->affiche(image, "LÖSCHEN ?", 133, 93);
    gpJeu->affiche(image, "JA", 151, 113);
    gpJeu->affiche(image, "NEIN", 151, 133);
break;

case 4: // Italian
    gpJeu->affiche(image, "CANCELLARE ?", 133, 93);
    gpJeu->affiche(image, "SI", 151, 113);
    gpJeu->affiche(image, "NO", 151, 133);
break;

case 5: // Spanish
    gpJeu->affiche(image, "BORRAR ?", 133, 93);
    gpJeu->affiche(image, "SI", 151, 113);
    gpJeu->affiche(image, "NO", 151, 133);
break;

default:
    gpJeu->affiche(image, "ERASE ?", 133, 93);
    gpJeu->affiche(image, "YES", 151, 113);
    gpJeu->affiche(image, "NO", 151, 133);
break;	
}
}

void Generique::initCharger() {
    cadre(104,84,112,72);

switch(getLanguage()) {

case 2: // French
    gpJeu->affiche(image, "CHARGER", 139, 93);
    gpJeu->affiche(image, "EFFACER", 139, 113);
    gpJeu->affiche(image, "ANNULER", 139, 133);
break;

case 3: // German
    gpJeu->affiche(image, "LADEN", 139, 93);
    gpJeu->affiche(image, "LÖSCHEN", 139, 113);
    gpJeu->affiche(image, "ABBRUCH", 139, 133);
break;

case 4: // Italian
    gpJeu->affiche(image, "CARICA", 139, 93);
    gpJeu->affiche(image, "CANCELLA", 139, 113);
    gpJeu->affiche(image, "ANNULLA", 139, 133);
break;

case 5: // Spanish
    gpJeu->affiche(image, "CARGAR", 139, 93);
    gpJeu->affiche(image, "BORRAR", 139, 113);
    gpJeu->affiche(image, "ANULAR", 139, 133);
break;

default:
    gpJeu->affiche(image, "LOAD", 139, 93);
    gpJeu->affiche(image, "ERASE", 139, 113);
    gpJeu->affiche(image, "CANCEL", 139, 133);
break;	
}

}

void Generique::initIntro() {
    if (imageFond1==NULL) {
        imageFond1 = IMG_Load("romfs:/images/logos/fond1.png");
        SDL_SetColorKey(imageFond1,SDL_SRCCOLORKEY,SDL_MapRGB(imageFond1->format,0,0,255));}
    if (imageFond2==NULL) imageFond2 = IMG_Load("romfs:/images/logos/fond2.png");
    
    ostringstream oss;
    for (int i = 0; i < 5; i++) {
        if (imageIntro[i]==NULL) {
            oss.str(""); oss << (i+1);
            imageIntro[i] = IMG_Load(("romfs:/images/logos/intro" + oss.str() + ".png").c_str());
        }
    }
    
    lastAnimTime = SDL_GetTicks(); anim=0;
    gpJeu->ecrit(228, false, true, 32, 158, 256, 64);
}

void Generique::initNuit() {
    gpJeu->ecrit(233);
}

void Generique::initAide1() {
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
    
    for (int i = 0; i < 4; i++) {
        src.x = 16; src.y = 0; dst.x = 32+16*i; dst.y = 0; 
        SDL_BlitSurface(imageCadre, &src, image, &dst);
        src.x = 16; src.y = 64; dst.x = 32+16*i; dst.y = 16; 
        SDL_BlitSurface(imageCadre, &src, image, &dst);
        src.x = 16; src.y = 32; dst.x = 32+16*i; dst.y = 32; 
        SDL_BlitSurface(imageCadre, &src, image, &dst);
    }
    
    src.x = 32; src.y = 0; dst.x = 96; dst.y = 0; 
    SDL_BlitSurface(imageCadre, &src, image, &dst);
    src.x = 32; src.y = 16; dst.x = 96; dst.y = 16; 
    SDL_BlitSurface(imageCadre, &src, image, &dst);
    src.x = 32; src.y = 32; dst.x = 96; dst.y = 32; 
    SDL_BlitSurface(imageCadre, &src, image, &dst);
    
    cadre(16,64-8,288,128);
    
    cadre(16,192+8,288,32);


    int ligne = 64;
    Joueur* gpJoueur = gpJeu->getJoueur();
switch(getLanguage()) {

case 2: // French
    gpJeu->affiche(image, "AIDE 1/2", 40, 16);
    
    gpJeu->affiche(image, "Retour au jeu : Entrée - Suite : Droite", 24, 208);
    
    gpJeu->affiche(image, "Lire / Ouvrir / Parler : A", 24, ligne); ligne+=16;
    gpJeu->affiche(image, "Valider / Passer texte : A", 24, ligne); ligne+=16;
    gpJeu->affiche(image, "Déplacer Link : Flèches de direction", 24, ligne); ligne+=16;
    if (gpJoueur->hasObjet(O_BOTTES)) {
        gpJeu->affiche(image, "Courir : R maintenu", 24, ligne); 
        ligne+=16;}
    if (gpJoueur->getEpee()) {
        gpJeu->affiche(image, "Coup d'épée : B", 24, ligne); ligne+=16;
        gpJeu->affiche(image, "Attaque Tornade : B maintenu puis lâché", 24, ligne); 
        ligne+=16;}
    gpJeu->affiche(image, "Menu de sélection d'objet : L + START", 24, ligne); ligne+=16;
    if (ligne >= 176) return;
    gpJeu->affiche(image, "Utiliser l'objet sélectionné : X", 24, ligne); ligne+=16;
    if (ligne >= 176) return;
    if (gpJoueur->hasObjet(O_GANTS)) {
        gpJeu->affiche(image, "Porter sans équiper les gants : Y", 24, ligne); 
        ligne+=16;}
    if (ligne >= 176) return;
    if (gpJoueur->hasObjet(O_CARTE))
        gpJeu->affiche(image, "Afficher la carte : START (extérieur ou donjons)", 24, ligne);
    else gpJeu->affiche(image, "Afficher la carte : START (dans les donjons)", 24, ligne);
    ligne+=16;
    if (ligne >= 176) return;
    if (gpJoueur->hasObjet(O_ENCYCL)) {
        gpJeu->affiche(image, "Afficher les monstres vaincus : L et B", 24, ligne); 
        ligne+=16;}
    if (ligne >= 176) return;
    gpJeu->affiche(image, "Regarder autour : L et direction", 24, ligne); ligne+=16;
    if (ligne >= 176) return;
    gpJeu->affiche(image, "Agrandir / Rétrécir : L et A", 24, ligne); ligne+=16;
    if (ligne >= 176) return;
    gpJeu->affiche(image, "Sauvegarder / Quitter : SELECT", 24, ligne); ligne+=16;

break;

case 3: // German
    gpJeu->affiche(image, "HILFE 1/2", 40, 16);

    gpJeu->affiche(image, "Zurück zum Spiel: A - Nächste S.: Rechts", 24, 208);

    gpJeu->affiche(image, "Lesen / Öffnen / Sprechen: A", 24, ligne); ligne+=16;
    gpJeu->affiche(image, "Bestätigen / Weiter im Text: A", 24, ligne); ligne+=16;
    gpJeu->affiche(image, "Link bewegen: Cursortasten", 24, ligne); ligne+=16;
    if (gpJoueur->hasObjet(O_BOTTES)) {
        gpJeu->affiche(image, "Rennen : R und Cursortasten", 24, ligne);
        ligne+=16;}
    if (gpJoueur->getEpee()) {
        gpJeu->affiche(image, "Schwert benutzen: B", 24, ligne); ligne+=16;
        gpJeu->affiche(image, "Rundumschlag: B halten und loslassen", 24, ligne);
        ligne+=16;}
    gpJeu->affiche(image, "Gegenstand wählen: L und START", 24, ligne); ligne+=16;
    if (ligne >= 176) return;
    gpJeu->affiche(image, "Gegenstand benutzen: X", 24, ligne); ligne+=16;
    if (ligne >= 176) return;
    if (gpJoueur->hasObjet(O_GANTS)) {
        gpJeu->affiche(image, "Tragen, ohne ausgewählten Handschuh: Y", 24, ligne);
        ligne+=16;}
    if (ligne >= 176) return;
    if (gpJoueur->hasObjet(O_CARTE))
        gpJeu->affiche(image, "Plan ansehen: START (außen oder im Tempel)", 24, ligne);
    else gpJeu->affiche(image, "Plan ansehen: START (im Tempel)", 24, ligne);
    ligne+=16;
    if (ligne >= 176) return;
    if (gpJoueur->hasObjet(O_ENCYCL)) {
        gpJeu->affiche(image, "Bisher besiegte Monster: L und B", 24, ligne);
        ligne+=16;}
    if (ligne >= 176) return;
    gpJeu->affiche(image, "Sich umsehen: L und Cursortasten", 24, ligne); ligne+=16;
    if (ligne >= 176) return;
    gpJeu->affiche(image, "Vollbild / Fenster: L und A", 24, ligne); ligne+=16;
    if (ligne >= 176) return;
    gpJeu->affiche(image, "Sichern / Beenden: SELECT", 24, ligne); ligne+=16;
break;

case 4: // Italian
    gpJeu->affiche(image, "HELP 1/2", 40, 16);
    
    gpJeu->affiche(image, "Ritorna al gioco: A - Prossimo: Destra", 24, 208);
    
    gpJeu->affiche(image, "Leggi / Apri / Parla: A", 24, ligne); ligne+=16;
    gpJeu->affiche(image, "Conferma / Chiudi testo: A", 24, ligne); ligne+=16;
    gpJeu->affiche(image, "Muovere Link: pad direzionale", 24, ligne); ligne+=16;
    if (gpJoueur->hasObjet(O_BOTTES)) {
        gpJeu->affiche(image, "Run : R hold", 24, ligne); 
        ligne+=16;}
    if (gpJoueur->getEpee()) {
        gpJeu->affiche(image, "Usa la spada: B", 24, ligne); ligne+=16;
        gpJeu->affiche(image, "Attacco rotante: trattieni B poi lascia", 24, ligne); 
        ligne+=16;}
    gpJeu->affiche(image, "Selezione oggetto: L e START", 24, ligne); ligne+=16;
    if (ligne >= 176) return;
    gpJeu->affiche(image, "Usa oggetto selezionato: X", 24, ligne); ligne+=16;
    if (ligne >= 176) return;
    if (gpJoueur->hasObjet(O_GANTS)) {
        gpJeu->affiche(image, "raccogli oggetti (con guanti): Y", 24, ligne); 
        ligne+=16;}
    if (ligne >= 176) return;
    if (gpJoueur->hasObjet(O_CARTE))
        gpJeu->affiche(image, "Apri la mappa: START (fuori dai templi)", 24, ligne);
    else gpJeu->affiche(image, "Apri la mappa: START (nei templi)", 24, ligne);
    ligne+=16;
    if (ligne >= 176) return;
    if (gpJoueur->hasObjet(O_ENCYCL)) {
        gpJeu->affiche(image, "Gurda mostri uccsi: L e B", 24, ligne); 
        ligne+=16;}
    if (ligne >= 176) return;
    gpJeu->affiche(image, "Guarda intorno: L e direzione", 24, ligne); ligne+=16;
    if (ligne >= 176) return;
    gpJeu->affiche(image, "Allarga / Riduci: L e A", 24, ligne); ligne+=16;
    if (ligne >= 176) return;
    gpJeu->affiche(image, "Salva / Esci: SELECT", 24, ligne); ligne+=16;

break;

case 5: // Spanish
    gpJeu->affiche(image, "AYUDA 1/2", 40-3, 16);
    
    gpJeu->affiche(image, "Volver al juego : A  Siguiente : Derecha", 24, 208);
    
    gpJeu->affiche(image, "Leer / Abrir / Hablar : A", 24, ligne); ligne+=16;
    gpJeu->affiche(image, "Validar / Pasar texto : A", 24, ligne); ligne+=16;
    gpJeu->affiche(image, "Mover a Link : Flechas de dirección", 24, ligne); ligne+=16;
    if (gpJoueur->hasObjet(O_BOTTES)) {
        gpJeu->affiche(image, "Correr : R y dirección ", 24, ligne); 
        ligne+=16;}
    if (gpJoueur->getEpee()) {
        gpJeu->affiche(image, "Golpe de espada : B", 24, ligne); ligne+=16;
        gpJeu->affiche(image, "Giratorio : B mantenido luego soltado", 24, ligne); 
        ligne+=16;}
    gpJeu->affiche(image, "Menu de selección del objeto : L y START", 24, ligne); ligne+=16;
    if (ligne >= 176) return;
    gpJeu->affiche(image, "Utilizar el objeto seleccionnado : X", 24, ligne); ligne+=16;
    if (ligne >= 176) return;
    if (gpJoueur->hasObjet(O_GANTS)) {
        gpJeu->affiche(image, "Levantar sin equipar los guantes : Y", 24, ligne); 
        ligne+=16;}
    if (ligne >= 176) return;
    if (gpJoueur->hasObjet(O_CARTE))
        gpJeu->affiche(image, "Ver el mapa : START (exterior o templos)", 24, ligne);
    else gpJeu->affiche(image, "Ver el mapa : START (en los templos)", 24, ligne);
    ligne+=16;
    if (ligne >= 176) return;
    if (gpJoueur->hasObjet(O_ENCYCL)) {
        gpJeu->affiche(image, "Ver los monstros vencidos : L y B", 24, ligne); 
        ligne+=16;}
    if (ligne >= 176) return;
    gpJeu->affiche(image, "Mirar alrededor : L y dirección", 24, ligne); ligne+=16;
    if (ligne >= 176) return;
    gpJeu->affiche(image, "Agrandar / Encoger : L y A", 24, ligne); ligne+=16;
    if (ligne >= 176) return;
    gpJeu->affiche(image, "Grabar / Quitar : SELECT", 24, ligne); ligne+=16;
break;

default:
    gpJeu->affiche(image, "HELP 1/2", 40, 16);
    
    gpJeu->affiche(image, "Return to the game: A - Next: Right", 24, 208);
    
    gpJeu->affiche(image, "Read / Open / Speak: A", 24, ligne); ligne+=16;
    gpJeu->affiche(image, "Confirm / Pass text: A", 24, ligne); ligne+=16;
    gpJeu->affiche(image, "Move Link: Arrows", 24, ligne); ligne+=16;
    if (gpJoueur->hasObjet(O_BOTTES)) {
        gpJeu->affiche(image, "Run : R hold", 24, ligne); 
        ligne+=16;}
    if (gpJoueur->getEpee()) {
        gpJeu->affiche(image, "Use sword: B", 24, ligne); ligne+=16;
        gpJeu->affiche(image, "Spin attack: B hold then dropped", 24, ligne); 
        ligne+=16;}
    gpJeu->affiche(image, "Item selection: L and START", 24, ligne); ligne+=16;
    if (ligne >= 176) return;
    gpJeu->affiche(image, "Use selected object: X", 24, ligne); ligne+=16;
    if (ligne >= 176) return;
    if (gpJoueur->hasObjet(O_GANTS)) {
        gpJeu->affiche(image, "Pick up items (with the glove): Y", 24, ligne); 
        ligne+=16;}
    if (ligne >= 176) return;
    if (gpJoueur->hasObjet(O_CARTE))
        gpJeu->affiche(image, "Open the map: START (outside of dungeons)", 24, ligne);
    else gpJeu->affiche(image, "Open the map: START (in dungeons)", 24, ligne);
    ligne+=16;
    if (ligne >= 176) return;
    if (gpJoueur->hasObjet(O_ENCYCL)) {
        gpJeu->affiche(image, "View defeated monsters: L and B", 24, ligne); 
        ligne+=16;}
    if (ligne >= 176) return;
    gpJeu->affiche(image, "Look around: L and direction", 24, ligne); ligne+=16;
    if (ligne >= 176) return;
    gpJeu->affiche(image, "Enlarge / Shrink: L and A", 24, ligne); ligne+=16;
    if (ligne >= 176) return;
    gpJeu->affiche(image, "Save / Quit: SELECT", 24, ligne); ligne+=16;

break;	
}    


}

void Generique::initAide2() {
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
    
    for (int i = 0; i < 4; i++) {
        src.x = 16; src.y = 0; dst.x = 32+16*i; dst.y = 0; 
        SDL_BlitSurface(imageCadre, &src, image, &dst);
        src.x = 16; src.y = 64; dst.x = 32+16*i; dst.y = 16; 
        SDL_BlitSurface(imageCadre, &src, image, &dst);
        src.x = 16; src.y = 32; dst.x = 32+16*i; dst.y = 32; 
        SDL_BlitSurface(imageCadre, &src, image, &dst);
    }
    
    src.x = 32; src.y = 0; dst.x = 96; dst.y = 0; 
    SDL_BlitSurface(imageCadre, &src, image, &dst);
    src.x = 32; src.y = 16; dst.x = 96; dst.y = 16; 
    SDL_BlitSurface(imageCadre, &src, image, &dst);
    src.x = 32; src.y = 32; dst.x = 96; dst.y = 32; 
    SDL_BlitSurface(imageCadre, &src, image, &dst);
    
    cadre(16,64-8,288,128);
    
    cadre(16,192+8,288,32);
    
    Joueur* gpJoueur = gpJeu->getJoueur();
    int ligne = 64-112;

switch(getLanguage()) {

case 2: // French
    gpJeu->affiche(image, "AIDE 2/2", 40, 16);
    
    gpJeu->affiche(image, "Retour au jeu : A - Précédent : Gauche", 24, 208);
    
    ligne+=64;
    if (gpJoueur->hasObjet(O_BOTTES)) ligne+=16;
    if (gpJoueur->getEpee()) ligne+=32;
    if (ligne >= 64) 
    gpJeu->affiche(image, "Utiliser l'objet sélectionné : X", 24, ligne); ligne+=16;
    if (ligne >= 64) 
    if (gpJoueur->hasObjet(O_GANTS)) {
        gpJeu->affiche(image, "Porter sans équiper les gants : Y", 24, ligne); 
        ligne+=16;}
    if (ligne >= 64) {
    if (gpJoueur->hasObjet(O_CARTE))
        gpJeu->affiche(image, "Afficher la carte : START (extérieur ou donjons)", 24, ligne);
    else gpJeu->affiche(image, "Afficher la carte : START (dans les donjons)", 24, ligne);}
    ligne+=16;
    if (ligne >= 64) 
    if (gpJoueur->hasObjet(O_ENCYCL)) {
        gpJeu->affiche(image, "Afficher les monstres vaincus : L et B", 24, ligne); 
        ligne+=16;}
    if (ligne >= 64) 
    gpJeu->affiche(image, "Regarder autour : L et direction", 24, ligne); ligne+=16;
    if (ligne >= 64) 
    gpJeu->affiche(image, "Agrandir / Rétrécir : L et A", 24, ligne); ligne+=16;
    if (ligne >= 64) 
    gpJeu->affiche(image, "Sauvegarder / Quitter : SELECT", 24, ligne); ligne+=16;
break;

case 3: // German
    gpJeu->affiche(image, "HILFE 2/2", 40, 16);

    gpJeu->affiche(image, "Zurück zum Spiel: A - Vorige S.: Links", 24, 208);

    ligne+=64;
    if (gpJoueur->hasObjet(O_BOTTES)) ligne+=16;
    if (gpJoueur->getEpee()) ligne+=32;
    if (ligne >= 64)
    gpJeu->affiche(image, "Gegenstand benutzen: X", 24, ligne); ligne+=16;
    if (ligne >= 64)
    if (gpJoueur->hasObjet(O_GANTS)) {
        gpJeu->affiche(image, "Tragen, ohne ausgewählten Handschuh: Y", 24, ligne);
        ligne+=16;}
    if (ligne >= 64) {
    if (gpJoueur->hasObjet(O_CARTE))
        gpJeu->affiche(image, "Plan ansehen: START (außen oder im Tempel)", 24, ligne);
    else gpJeu->affiche(image, "Plan ansehen:START (im Tempel)", 24, ligne);}
    ligne+=16;
    if (ligne >= 64)
    if (gpJoueur->hasObjet(O_ENCYCL)) {
        gpJeu->affiche(image, "Bisher besiegte Monster: L und B", 24, ligne);
        ligne+=16;}
    if (ligne >= 64)
    gpJeu->affiche(image, "Sich umsehen: L und Cursortasten", 24, ligne); ligne+=16;
    if (ligne >= 64)
    gpJeu->affiche(image, "Vollbild / Fenster: L und A", 24, ligne); ligne+=16;
    if (ligne >= 64)
    gpJeu->affiche(image, "Sichern / Beenden: SELECT", 24, ligne); ligne+=16;
break;

case 4: // Italian
    gpJeu->affiche(image, "HELP 2/2", 40, 16);
    
    gpJeu->affiche(image, "Ritorna al gioco: A - Precedente: Sinistra", 24, 208);
    
    ligne+=64;
    if (gpJoueur->hasObjet(O_BOTTES)) ligne+=16;
    if (gpJoueur->getEpee()) ligne+=32;
    if (ligne >= 64) 
    gpJeu->affiche(image, "Usa oggetto selezionato: X", 24, ligne); ligne+=16;
    if (ligne >= 64) 
    if (gpJoueur->hasObjet(O_GANTS)) {
        gpJeu->affiche(image, "Raccogli oggetti (con guanti): Y", 24, ligne); 
        ligne+=16;}
    if (ligne >= 64) {
    if (gpJoueur->hasObjet(O_CARTE))
        gpJeu->affiche(image, "Apri la mappa: START (fuori dai dungeon)", 24, ligne);
    else gpJeu->affiche(image, "Apri la mappa: START (nei dungeon)", 24, ligne);}
    ligne+=16;
    if (ligne >= 64) 
    if (gpJoueur->hasObjet(O_ENCYCL)) {
        gpJeu->affiche(image, "Gurda mostri uccsi: L e B", 24, ligne); 
        ligne+=16;}
    if (ligne >= 64) 
    gpJeu->affiche(image, "Guarda intorno: L e direzione", 24, ligne); ligne+=16;
    if (ligne >= 64) 
    gpJeu->affiche(image, "Allarga / Riduci: L e A", 24, ligne); ligne+=16;
    if (ligne >= 64) 
    gpJeu->affiche(image, "Salva / Esci: SELECT", 24, ligne); ligne+=16;

break;

case 5: // Spanish
    gpJeu->affiche(image, "AYUDA 2/2", 40-3, 16);
    
    gpJeu->affiche(image, "Volver al juego : A  Anterior : Izquierda", 24, 208);
    
    ligne+=64;
    if (gpJoueur->hasObjet(O_BOTTES)) ligne+=16;
    if (gpJoueur->getEpee()) ligne+=32;
    if (ligne >= 64) 
    gpJeu->affiche(image, "Utilizar el objeto seleccionnado : X", 24, ligne); ligne+=16;
    if (ligne >= 64) 
    if (gpJoueur->hasObjet(O_GANTS)) {
        gpJeu->affiche(image, "Levantar sin equipar los guantes : Y", 24, ligne); 
        ligne+=16;}
    if (ligne >= 64) {
    if (gpJoueur->hasObjet(O_CARTE))
        gpJeu->affiche(image, "Ver el mapa : START (exterior o templos)", 24, ligne);
    else gpJeu->affiche(image, "Ver el mapa : START (en los templos)", 24, ligne);}
    ligne+=16;
    if (ligne >= 64) 
    if (gpJoueur->hasObjet(O_ENCYCL)) {
        gpJeu->affiche(image, "Ver los monstruos vencidos : L Y B", 24, ligne); 
        ligne+=16;}
    if (ligne >= 64) 
    gpJeu->affiche(image, "Mirar alrededor : L y dirección", 24, ligne); ligne+=16;
    if (ligne >= 64) 
    gpJeu->affiche(image, "Agrandar / Encoger : L y A", 24, ligne); ligne+=16;
    if (ligne >= 64) 
    gpJeu->affiche(image, "Grabar / Quitar : SELECT", 24, ligne); ligne+=16;
break;

default:
    gpJeu->affiche(image, "HELP 2/2", 40, 16);
    
    gpJeu->affiche(image, "Return to the game: A - Previous: Left", 24, 208);
    
    ligne+=64;
    if (gpJoueur->hasObjet(O_BOTTES)) ligne+=16;
    if (gpJoueur->getEpee()) ligne+=32;
    if (ligne >= 64) 
    gpJeu->affiche(image, "Use selected object: X", 24, ligne); ligne+=16;
    if (ligne >= 64) 
    if (gpJoueur->hasObjet(O_GANTS)) {
        gpJeu->affiche(image, "Pick up items (with the glove): Y", 24, ligne); 
        ligne+=16;}
    if (ligne >= 64) {
    if (gpJoueur->hasObjet(O_CARTE))
        gpJeu->affiche(image, "Open the map: START (outside of dungeons)", 24, ligne);
    else gpJeu->affiche(image, "Open the map: START (in dungeons)", 24, ligne);}
    ligne+=16;
    if (ligne >= 64) 
    if (gpJoueur->hasObjet(O_ENCYCL)) {
        gpJeu->affiche(image, "View defeated monsters: L and B", 24, ligne); 
        ligne+=16;}
    if (ligne >= 64) 
    gpJeu->affiche(image, "Look around: R and direction", 24, ligne); ligne+=16;
    if (ligne >= 64) 
    gpJeu->affiche(image, "Enlarge / Shrink: L and A", 24, ligne); ligne+=16;
    if (ligne >= 64) 
    gpJeu->affiche(image, "Save / Quit: SELECT", 24, ligne); ligne+=16;
break;	
}

}

void Generique::initRang(int i) {
    
switch(getLanguage()) {

case 2: // French
    switch (i) {
        case 0 :
            cadre(88,96,144,48);
            if (gpJeu->getKeyboard()->getRang(i)) 
                gpJeu->affiche(image, "RANG : Héros d'Hyrule", 96, 104);
            else gpJeu->affiche(image, "RANG : INCONNU", 96, 104);
            gpJeu->affiche(image, "Finir le jeu à 100%", 96, 120);
            break;
        case 1 :
            cadre(64,64,192,112);
            int l; l=72;
            if (gpJeu->getKeyboard()->getRang(i)) 
                gpJeu->affiche(image, "RANG : Frimeur", 72, l);
            else gpJeu->affiche(image, "RANG : INCONNU", 72, l);
            l+=16;
            gpJeu->affiche(image, "Finir le jeu sans mourir,", 72, l);l+=16;
            gpJeu->affiche(image, "avec 10 coeurs au maximum,", 72, l);l+=16;
            gpJeu->affiche(image, "sans parler aux Grandes Fées,", 72, l);l+=16;
            gpJeu->affiche(image, "sans acheter de flacons", 72, l);l+=16;
            gpJeu->affiche(image, "et sans prendre le bouclier.", 72, l);l+=16;
            break;
        case 2 :
            cadre(71,88,178,64);
            if (gpJeu->getKeyboard()->getRang(i)) 
                gpJeu->affiche(image, "RANG : Marathonien d'Hyrule", 78, 96);
            else gpJeu->affiche(image, "RANG : INCONNU", 78, 96);
            gpJeu->affiche(image, "Finir le jeu en moins de", 78, 112);
            gpJeu->affiche(image, "deux heures", 78, 128);
            break;
    }
break;

case 3: // German
    switch (i) {
        case 0 :
            cadre(88-2-16-8,96,144+4+32+16,48);
            if (gpJeu->getKeyboard()->getRang(i))
                gpJeu->affiche(image, "TITEL: Held von Hyrule", 96-2-16-8, 104);
            else gpJeu->affiche(image, "TITEL: -nicht geschafft-", 96-2-16-8, 104);
            gpJeu->affiche(image, "Alle Aufgaben 100%-ig erfüllt.", 96-2-16-8, 120);
            break;
        case 1 :
            cadre(64-8,64-8,192+16,112);
            int l; l=72 - 8;
            if (gpJeu->getKeyboard()->getRang(i))
                gpJeu->affiche(image, "TITEL: Graf Protz", 72-8, l);
            else gpJeu->affiche(image, "TITEL: -nicht geschafft-", 72-8, l);
            l+=16;
            gpJeu->affiche(image, "Hyrule gerettet, ohne jemals zu", 72-8, l);l+=16;
            gpJeu->affiche(image, "sterben, kein Schild genommen,", 72-8, l);l+=16;
            gpJeu->affiche(image, "höchstens 10 Leben besitzend,", 72-8, l);l+=16;
            gpJeu->affiche(image, "mit keiner Fee gesprochen", 72-8, l);l+=16;
            gpJeu->affiche(image, "und keine Flaschen gekauft", 72-8, l);l+=16;
            break;
        case 2 :
            cadre(71-10,88,178+20,64);
            if (gpJeu->getKeyboard()->getRang(i))
                gpJeu->affiche(image, "TITEL: Turbozocker", 78-10, 96);
            else gpJeu->affiche(image, "TITEL: -nicht geschafft-", 78-10, 96);
            gpJeu->affiche(image, "Hyrule gerettet in weniger als", 78-10, 112);
            gpJeu->affiche(image, "zwei Stunden!!!", 78-10, 128);
            break;
    }
break;

case 4: // Italian
    switch (i) {
        case 0 :
            cadre(88-2-16,96,144+4+32,48);
            if (gpJeu->getKeyboard()->getRang(i)) 
                gpJeu->affiche(image, "GRADO: Eroe di Hyrule", 96-2-16, 104);
            else gpJeu->affiche(image, "GRADO: SCONOSCIUTO", 96-2-16, 104);
            gpJeu->affiche(image, "100% del gioco completato", 96-2-16, 120);
            break;
        case 1 :
            cadre(64-8-16,64,192+16+32,112);
            int l; l=72;
            if (gpJeu->getKeyboard()->getRang(i)) 
                gpJeu->affiche(image, "GRADO: Campione", 72-8-16, l);
            else gpJeu->affiche(image, "GRADO: SCONOSCIUTO", 72-8-16, l);
            l+=16;
            gpJeu->affiche(image, "Gioco completato senza morire,", 72-8-16, l);l+=16;
            gpJeu->affiche(image, "con 10 cuori al completo,", 72-8-16, l);l+=16;
            gpJeu->affiche(image, "senza parlare con le Grandi Fate,", 72-8-16, l);l+=16;
            gpJeu->affiche(image, "senza comprare bottiglie", 72-8-16, l);l+=16;
            gpJeu->affiche(image, "e senza uno schudo.", 72-8-16, l);l+=16;
            break;
        case 2 :
            cadre(71-10,88,178+20,64);
            if (gpJeu->getKeyboard()->getRang(i)) 
                gpJeu->affiche(image, "GRADO: Maratoneta di Hyrule", 78-10, 96);
            else gpJeu->affiche(image, "GRADO: SCONOSCIUTO", 78-10, 96);
            gpJeu->affiche(image, "Gioco completato in meno di", 78-10, 112);
            gpJeu->affiche(image, "due ore", 78-10, 128);
            break;
    }
break;	

case 5: // Spanish
    switch (i) {
        case 0 :
            cadre(88-8,96,144+16,48);
            if (gpJeu->getKeyboard()->getRang(i)) 
                gpJeu->affiche(image, "RANGO : Héroe de Hyrule", 96-8, 104);
            else gpJeu->affiche(image, "RANGO : DESCONOCIDO", 96-8, 104);
            gpJeu->affiche(image, "Acabar el juego a 100%", 96-8, 120);
            break;
        case 1 :
            cadre(64-8,64,192+16,112);
            int l; l=72;
            if (gpJeu->getKeyboard()->getRang(i)) 
                gpJeu->affiche(image, "RANGO : Chulo", 72-8, l);
            else gpJeu->affiche(image, "RANGO : DESCONOCIDO", 72-8, l);
            l+=16;
            gpJeu->affiche(image, "Acabar el juego sin morir,", 72-8, l);l+=16;
            gpJeu->affiche(image, "con 10 corazones como máximo,", 72-8, l);l+=16;
            gpJeu->affiche(image, "sin hablar a las Grandes Hadas,", 72-8, l);l+=16;
            gpJeu->affiche(image, "sin comprar botellas", 72-8, l);l+=16;
            gpJeu->affiche(image, "y sin coger el escudo.", 72-8, l);l+=16;
            break;
        case 2 :
            cadre(71,88,178,64);
            if (gpJeu->getKeyboard()->getRang(i)) 
                gpJeu->affiche(image, "RANGO : Corredor de Hyrule", 78, 96);
            else gpJeu->affiche(image, "RANGO : DESCONOCIDO", 78, 96);
            gpJeu->affiche(image, "Acabar el juego en menos de", 78, 112);
            gpJeu->affiche(image, "dos horas", 78, 128);
    }
break;

default:
    switch (i) {
        case 0 :
            cadre(88-2-16,96,144+4+32,48);
            if (gpJeu->getKeyboard()->getRang(i)) 
                gpJeu->affiche(image, "RANK: Hero of Hyrule", 96-2-16, 104);
            else gpJeu->affiche(image, "RANK: UNKNOWN", 96-2-16, 104);
            gpJeu->affiche(image, "100% completion of the game", 96-2-16, 120);
            break;
        case 1 :
            cadre(64-8-16,64,192+16+32,112);
            int l; l=72;
            if (gpJeu->getKeyboard()->getRang(i)) 
                gpJeu->affiche(image, "RANK: Swanky", 72-8-16, l);
            else gpJeu->affiche(image, "RANK: UNKNOW", 72-8-16, l);
            l+=16;
            gpJeu->affiche(image, "Completed the game without dying,", 72-8-16, l);l+=16;
            gpJeu->affiche(image, "with 10 hearts at the very most,", 72-8-16, l);l+=16;
            gpJeu->affiche(image, "without talking to the Great Fairies,", 72-8-16, l);l+=16;
            gpJeu->affiche(image, "without buying bottles", 72-8-16, l);l+=16;
            gpJeu->affiche(image, "and without a shield.", 72-8-16, l);l+=16;
            break;
        case 2 :
            cadre(71-10,88,178+20,64);
            if (gpJeu->getKeyboard()->getRang(i)) 
                gpJeu->affiche(image, "RANK: Hyrule Marathon Runner", 78-10, 96);
            else gpJeu->affiche(image, "RANK: UNKNOW", 78-10, 96);
            gpJeu->affiche(image, "Complete the game in less than", 78-10, 112);
            gpJeu->affiche(image, "two hours", 78-10, 128);
            break;
    }
break;	
}

}

void Generique::cadre(int x, int y, int w, int h) {
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
}

void Generique::initScore() {
    SDL_FreeSurface(imageArbre);
    imageArbre = IMG_Load("romfs:/images/logos/arbre.png");
    image = SDL_CreateRGBSurface(SDL_HWSURFACE, 320, 240, 32, 0, 0, 0, 0);
    
    SDL_Rect dst; 
    
    for (int i = 0; i < 320; i+=48) {
        dst.x = i;
        for (int j = 0; j < 240; j+=96) {
            dst.y = j;
            SDL_BlitSurface(imageArbre, NULL, image, &dst);
        }
    }
    
    Joueur* gpJoueur = gpJeu->getJoueur();
    
    ostringstream oss;
    int pctg = 0;

switch(getLanguage()) {

case 2: // French
    gpJeu->affiche(image, "SCORES :", 136, 10);
break;

case 3: // German
    gpJeu->affiche(image, "ERGEBNISSE:", 136, 10);
break;

case 4: // Italian
    gpJeu->affiche(image, "RISULTATI :", 136, 10);
break;

case 5: // Spanish
    gpJeu->affiche(image, "RESULTADOS :", 136, 10);
break;

default:
    gpJeu->affiche(image, "RESULTS:", 136, 10);
break;	
}
    
    int temps = gpJoueur->getTemps(0)+gpJoueur->getTemps(1)*60+gpJoueur->getTemps(2)*3600;
    if (temps < gpJeu->getKeyboard()->getTemps() || gpJeu->getKeyboard()->getTemps()==0) {
        gpJeu->getKeyboard()->setTemps(temps); gpJeu->getKeyboard()->saveP();}
        
    oss.str(""); 
    if (gpJoueur->getTemps(2)<10)oss<<"0";oss << gpJoueur->getTemps(2) << ":";
    if (gpJoueur->getTemps(1)<10)oss<<"0";oss << gpJoueur->getTemps(1) << ":";
    if (gpJoueur->getTemps(0)<10)oss<<"0";oss << gpJoueur->getTemps(0);

switch(getLanguage()) {

case 2: // French
    gpJeu->affiche(image, ("Temps de jeu : " + oss.str()).c_str(), 10, 30);
break;

case 3: // German
    gpJeu->affiche(image, ("Spielzeit: " + oss.str()).c_str(), 10, 30);
break;

case 4: // Italian
    gpJeu->affiche(image, ("Tempo di gioco : " + oss.str()).c_str(), 10, 30);
break;

case 5: // Spanish
    gpJeu->affiche(image, ("Tiempo jugando : " + oss.str()).c_str(), 10, 30);
break;

default:
    gpJeu->affiche(image, ("Game time: " + oss.str()).c_str(), 10, 30);
break;	
}
    
    int tmp=gpJoueur->getMort();
    oss.str(""); oss << tmp;

switch(getLanguage()) {

case 2: // French
    gpJeu->affiche(image, ("Mort " + oss.str() + " fois").c_str(), 10, 50);
break;

case 3: // German
    if (tmp) gpJeu->affiche(image, ("Gestorben: " + oss.str() + " mal").c_str(), 10, 50);
    else gpJeu->affiche(image, "Gestorben: kein mal", 10, 50);
break;

case 4: // Italian
    if (tmp==1) gpJeu->affiche(image, ("Morto " + oss.str() + " volte").c_str(), 10, 50);
    else gpJeu->affiche(image, ("Morto " + oss.str() + " volte").c_str(), 10, 50);
break;

case 5: // Spanish
    gpJeu->affiche(image, ("Muerto " + oss.str() + " veces").c_str(), 10, 50);
break;

default:
    if (tmp==1) gpJeu->affiche(image, ("Dead " + oss.str() + " time").c_str(), 10, 50);
    else gpJeu->affiche(image, ("Dead " + oss.str() + " times").c_str(), 10, 50);
break;	
}
    
    tmp=gpJoueur->nbQuarts();
    oss.str(""); oss << tmp; pctg+=tmp;

switch(getLanguage()) {

case 2: // French
    gpJeu->affiche(image, ("Quarts de coeurs : " + oss.str() + " / 36").c_str(), 10, 70);
break;

case 3: // German
    gpJeu->affiche(image, ("Herzfragmente: " + oss.str() + " / 36").c_str(), 10, 70);
break;

case 4: // Italian
    gpJeu->affiche(image, ("Parti di cuore : " + oss.str() + " / 36").c_str(), 10, 70);
break;

case 5: // Spanish
    gpJeu->affiche(image, ("Cuarto de corazón : " + oss.str() + " / 36").c_str(), 10, 70);
break;

default:
    gpJeu->affiche(image, ("Pieces of heart: " + oss.str() + " / 36").c_str(), 10, 70);
break;	
}

    tmp=0;
    for (int i = 0; i < 10; i++) if (i != O_SAC_BOMBES) if (gpJoueur->hasObjet(i)) tmp++;
    if (gpJoueur->hasObjet(O_ARC) == 5) tmp++;
    if (gpJoueur->hasObjet(O_GANTS) == 2) tmp++;
    if (gpJoueur->getBouclier()) tmp++;
    for (int i = 0; i < 3; i++) if (gpJoueur->hasBouteille(i)) tmp++;
    if (gpJoueur->getEpee()) tmp++;
    if (gpJoueur->getEpee()>1) tmp++;
    if (gpJoueur->getEpee()>3) tmp++;
    if (gpJoueur->getMagieMax() > 32) tmp++;
    for (int i = 0; i < 3; i++) if (gpJoueur->hasMedaillon(i)) tmp++;
    for (int i = 0; i < 7; i++) if (gpJoueur->hasCristal(i)) tmp++;
    for (int j = 0; j < 9; j++) for (int i = 0; i < 3; i++) if (gpJoueur->getCle(j,i)) tmp++;
    oss.str(""); oss << tmp; pctg+=tmp;


    int ligne = 170;
    string result;
switch(getLanguage()) {

case 2: // French
   gpJeu->affiche(image, ("Objets : " + oss.str() + " / 56").c_str(), 10, 90);
    
    tmp=gpJoueur->nbEnnemis();
    oss.str(""); oss << tmp; pctg+=tmp;
    gpJeu->affiche(image, ("Types d'ennemis vaincus : " + oss.str() + " / 46").c_str(), 10, 110);
    
    pctg=(int)((pctg*100)/138);
    oss.str(""); oss << pctg;
    gpJeu->affiche(image, ("Tu as fini ce jeu à " + oss.str() + "%, félicitation !").c_str(), 
        10, 130);
    
    gpJeu->affiche(image, "RANG : ", 10, ligne);
    
    if (gpJoueur->getMort()==0 && gpJoueur->hasBouteille(0)==0
        && gpJoueur->hasBouteille(1)==0 && gpJoueur->hasBouteille(2)==0
        && gpJoueur->hasMedaillon(0)==0 && gpJoueur->hasMedaillon(1)==0
        && gpJoueur->hasMedaillon(2)==0 && gpJoueur->getVieMax()==20
        && gpJoueur->getMagieMax()==32 && gpJoueur->getBouclier()==0) {
        result="Frimeur"; gpJeu->affiche(image, " - "+result, 52, ligne); ligne+=20;
        gpJeu->getKeyboard()->setRang(1); gpJeu->getKeyboard()->saveP();}
    else {
        if (pctg==100) {result="Héros d'Hyrule"; 
            gpJeu->getKeyboard()->setRang(0); gpJeu->getKeyboard()->saveP();}
        if (pctg<100) result="Perfectionniste";
        if (pctg<95) result="Fan de Zelda";
        if (pctg<90) result="Pro du clavier";
        if (pctg<85) result="Confirmé";
        if (pctg<80) result="Débutant";
        gpJeu->affiche(image, " - "+result, 52, ligne);
        ligne+=20;
    }
    
    if (gpJoueur->getMort()>=50) {
        result="Mort-vivant"; gpJeu->affiche(image, " - "+result, 52, ligne); ligne+=20;}
    else if (gpJoueur->getMort()>=20) {
        result="Kamikaze"; gpJeu->affiche(image, " - "+result, 52, ligne); ligne+=20;}
        
    if (temps<=7200) {
        result="Marathonien d'Hyrule"; 
        gpJeu->getKeyboard()->setRang(2); gpJeu->getKeyboard()->saveP();
        gpJeu->affiche(image, " - "+result, 52, ligne); ligne+=20;}
break;

case 3: // German
    gpJeu->affiche(image, ("Besiegte Arten von Monstern: " + oss.str() + " / 46").c_str(), 10, 110);

    pctg=(int)((pctg*100)/138);
    oss.str(""); oss << pctg;
    gpJeu->affiche(image, ("Du hast die Aufgaben zu " + oss.str() + "% erfüllt, gratuliere!").c_str(), 10, 130);

    gpJeu->affiche(image, "TITEL: ", 10, ligne);

    if (gpJoueur->getMort()==0 && gpJoueur->hasBouteille(0)==0
        && gpJoueur->hasBouteille(1)==0 && gpJoueur->hasBouteille(2)==0
        && gpJoueur->hasMedaillon(0)==0 && gpJoueur->hasMedaillon(1)==0
        && gpJoueur->hasMedaillon(2)==0 && gpJoueur->getVieMax()==20
        && gpJoueur->getMagieMax()==32 && gpJoueur->getBouclier()==0) {
        result="ULTIMATIV-HELD"; gpJeu->affiche(image, " - "+result, 52, ligne); ligne+=20;
        gpJeu->getKeyboard()->setRang(1); gpJeu->getKeyboard()->saveP();}
    else {
        if (pctg==100) {result="ERBSENZÄHLER";
            gpJeu->getKeyboard()->setRang(0); gpJeu->getKeyboard()->saveP();}
        if (pctg<100) result="Zelda-Meister";
        if (pctg<95) result="Zelda-Experte";
        if (pctg<90) result="Zelda-Profi";
        if (pctg<85) result="Zelda-Kenner";
        if (pctg<80) result="Zelda-Neuling";
        gpJeu->affiche(image, " - "+result, 52, ligne);
        ligne+=20;
    }

    if (gpJoueur->getMort()>=50) {
        result="Todessüchtiger"; gpJeu->affiche(image, " - "+result, 52, ligne); ligne+=20;}
    else if (gpJoueur->getMort()>=20) {
        result="Todesmutiger"; gpJeu->affiche(image, " - "+result, 52, ligne); ligne+=20;}

    if (temps<=7200) {
        result="SUPER-SPRINTER";
        gpJeu->getKeyboard()->setRang(2); gpJeu->getKeyboard()->saveP();
        gpJeu->affiche(image, " - "+result, 52, ligne); ligne+=20;}
break;

case 4: // Italian
    gpJeu->affiche(image, ("Oggetti: " + oss.str() + " / 56").c_str(), 10, 90);
    
    tmp=gpJoueur->nbEnnemis();
    oss.str(""); oss << tmp; pctg+=tmp;
    gpJeu->affiche(image, ("Tipi di nemici uccisi: " + oss.str() + " / 46").c_str(), 10, 110);
    
    pctg=(int)((pctg*100)/138);
    oss.str(""); oss << pctg;
    gpJeu->affiche(image, ("Hai raggiunto " + oss.str() + "% di completamento, congratulazioni!").c_str(), 
        10, 130);
    
    gpJeu->affiche(image, "GRADO: ", 10, ligne);
    
    if (gpJoueur->getMort()==0 && gpJoueur->hasBouteille(0)==0
        && gpJoueur->hasBouteille(1)==0 && gpJoueur->hasBouteille(2)==0
        && gpJoueur->hasMedaillon(0)==0 && gpJoueur->hasMedaillon(1)==0
        && gpJoueur->hasMedaillon(2)==0 && gpJoueur->getVieMax()==20
        && gpJoueur->getMagieMax()==32 && gpJoueur->getBouclier()==0) {
        result="Swanky"; gpJeu->affiche(image, " - "+result, 52, ligne); ligne+=20;
        gpJeu->getKeyboard()->setRang(1); gpJeu->getKeyboard()->saveP();}
    else {
        if (pctg==100) {result="Eroe di Hyrule"; 
            gpJeu->getKeyboard()->setRang(0); gpJeu->getKeyboard()->saveP();}
        if (pctg<100) result="Perfezionista";
        if (pctg<95) result="Zelda Fan";
        if (pctg<90) result="Giocatore professionista";
        if (pctg<85) result="Esperto";
        if (pctg<80) result="Novizio";
        gpJeu->affiche(image, " - "+result, 52, ligne);
        ligne+=20;
    }
    
    if (gpJoueur->getMort()>=50) {
        result="Morto vivente"; gpJeu->affiche(image, " - "+result, 52, ligne); ligne+=20;}
    else if (gpJoueur->getMort()>=20) {
        result="Kamikaze"; gpJeu->affiche(image, " - "+result, 52, ligne); ligne+=20;}
        
    if (temps<=7200) {
        result="Maratoneta di Hyrule"; 
        gpJeu->getKeyboard()->setRang(2); gpJeu->getKeyboard()->saveP();
        gpJeu->affiche(image, " - "+result, 52, ligne); ligne+=20;}
break;

case 5: // Spanish
    gpJeu->affiche(image, ("Objetos : " + oss.str() + " / 56").c_str(), 10, 90);
    
    tmp=gpJoueur->nbEnnemis();
    oss.str(""); oss << tmp; pctg+=tmp;
    gpJeu->affiche(image, ("Tipos de enemigos vencidos : " + oss.str() + " / 46").c_str(), 10, 110);
    
    pctg=(int)((pctg*100)/138);
    oss.str(""); oss << pctg;
    gpJeu->affiche(image, ("Has acabado este juego al " + oss.str() + "%, felicitaciones !").c_str(), 
        10, 130);
    
    gpJeu->affiche(image, "RANGO : ", 10, ligne);
    
    if (gpJoueur->getMort()==0 && gpJoueur->hasBouteille(0)==0
        && gpJoueur->hasBouteille(1)==0 && gpJoueur->hasBouteille(2)==0
        && gpJoueur->hasMedaillon(0)==0 && gpJoueur->hasMedaillon(1)==0
        && gpJoueur->hasMedaillon(2)==0 && gpJoueur->getVieMax()==20
        && gpJoueur->getMagieMax()==32 && gpJoueur->getBouclier()==0) {
        result="Chulo"; gpJeu->affiche(image, " - "+result, 52, ligne); ligne+=20;
        gpJeu->getKeyboard()->setRang(1); gpJeu->getKeyboard()->saveP();}
    else {
        if (pctg==100) {result="Héroe de Hyrule"; 
            gpJeu->getKeyboard()->setRang(0); gpJeu->getKeyboard()->saveP();}
        if (pctg<100) result="Perfeccionista";
        if (pctg<95) result="Fan de Zelda";
        if (pctg<90) result="Genio del teclado";
        if (pctg<85) result="Veterano";
        if (pctg<80) result="Principiante";
        gpJeu->affiche(image, " - "+result, 52, ligne);
        ligne+=20;
    }
    
    if (gpJoueur->getMort()>=50) {
        result="Muerto en vida"; gpJeu->affiche(image, " - "+result, 52, ligne); ligne+=20;}
    else if (gpJoueur->getMort()>=20) {
        result="Kamikaze"; gpJeu->affiche(image, " - "+result, 52, ligne); ligne+=20;}
        
    if (temps<=7200) {
        result="Corredor de Hyrule"; 
        gpJeu->getKeyboard()->setRang(2); gpJeu->getKeyboard()->saveP();
        gpJeu->affiche(image, " - "+result, 52, ligne); ligne+=20;}
break;

default:
    gpJeu->affiche(image, ("Objects: " + oss.str() + " / 56").c_str(), 10, 90);
    
    tmp=gpJoueur->nbEnnemis();
    oss.str(""); oss << tmp; pctg+=tmp;
    gpJeu->affiche(image, ("Kinds of defeated enemies: " + oss.str() + " / 46").c_str(), 10, 110);
    
    pctg=(int)((pctg*100)/138);
    oss.str(""); oss << pctg;
    gpJeu->affiche(image, ("You've gotten " + oss.str() + "% completion, congratulations!").c_str(), 
        10, 130);
    
    gpJeu->affiche(image, "RANK: ", 10, ligne);
    
    if (gpJoueur->getMort()==0 && gpJoueur->hasBouteille(0)==0
        && gpJoueur->hasBouteille(1)==0 && gpJoueur->hasBouteille(2)==0
        && gpJoueur->hasMedaillon(0)==0 && gpJoueur->hasMedaillon(1)==0
        && gpJoueur->hasMedaillon(2)==0 && gpJoueur->getVieMax()==20
        && gpJoueur->getMagieMax()==32 && gpJoueur->getBouclier()==0) {
        result="Swanky"; gpJeu->affiche(image, " - "+result, 52, ligne); ligne+=20;
        gpJeu->getKeyboard()->setRang(1); gpJeu->getKeyboard()->saveP();}
    else {
        if (pctg==100) {result="Hero of Hyrule"; 
            gpJeu->getKeyboard()->setRang(0); gpJeu->getKeyboard()->saveP();}
        if (pctg<100) result="Perfectionist";
        if (pctg<95) result="Zelda Fan";
        if (pctg<90) result="Keyboard professional";
        if (pctg<85) result="Experienced";
        if (pctg<80) result="Novice";
        gpJeu->affiche(image, " - "+result, 52, ligne);
        ligne+=20;
    }
    
    if (gpJoueur->getMort()>=50) {
        result="Living dead"; gpJeu->affiche(image, " - "+result, 52, ligne); ligne+=20;}
    else if (gpJoueur->getMort()>=20) {
        result="Kamikaze"; gpJeu->affiche(image, " - "+result, 52, ligne); ligne+=20;}
        
    if (temps<=7200) {
        result="Hyrule Marathon Runner"; 
        gpJeu->getKeyboard()->setRang(2); gpJeu->getKeyboard()->saveP();
        gpJeu->affiche(image, " - "+result, 52, ligne); ligne+=20;}

break;	
}
}

void Generique::draw(SDL_Surface* gpScreen) {
    SDL_Rect dst; dst.x = 0; dst.y = 0;
    SDL_BlitSurface(image, NULL, gpScreen, &dst);
}


void Generique::drawTitre(SDL_Surface* gpScreen) {
    SDL_Rect dst; dst.x = 0; dst.y = 0;
    SDL_BlitSurface(imageTitre, NULL, gpScreen, &dst);
}

void Generique::drawSelection(SDL_Surface* gpScreen, int ligne, int colonne) {
    draw(gpScreen);
    SDL_Rect src;
    SDL_Rect dst;
    
    src.h = 21; src.w = 16;src.x = 0;src.y=0;
    dst.x = 26+152*colonne; dst.y = 53+48*ligne;
    
    SDL_BlitSurface(imageCurseur, &src, gpScreen, &dst);
}

void Generique::drawOption(SDL_Surface* gpScreen, int ligne, int opt1, int opt2) {
    draw(gpScreen);
    SDL_Rect src;
    SDL_Rect dst;
	
switch(getLanguage()) {

case 2: // French
    gpJeu->affiche(gpScreen, "OPTIONS", 40, 16);
    gpJeu->affiche(gpScreen, "MUSIQUE", 60, 72);
    gpJeu->affiche(gpScreen, "SON", 60, 115);
    gpJeu->affiche(gpScreen, "LANGUE", 60, 158);
    gpJeu->affiche(gpScreen, "Francaise", 170, 158);
    gpJeu->affiche(gpScreen, "RETOUR", 63, 200);
break;

case 3: // German
    gpJeu->affiche(gpScreen, "OPTIONEN", 42, 16);
    gpJeu->affiche(gpScreen, "MUSIK", 60, 72);
    gpJeu->affiche(gpScreen, "TON", 60, 115);
    gpJeu->affiche(gpScreen, "SPRACHE", 60, 158);
    gpJeu->affiche(gpScreen, "Deutsche", 170, 158);
    gpJeu->affiche(gpScreen, "ZURÜCK", 60, 200);
break;

case 4: // Italian
    gpJeu->affiche(gpScreen, "OPZIONI", 40, 16);
    gpJeu->affiche(gpScreen, "MUSICA", 60, 72);
    gpJeu->affiche(gpScreen, "SUONI", 60, 115);
    gpJeu->affiche(gpScreen, "LINGUA", 60, 158);
    gpJeu->affiche(gpScreen, "Italiano", 170, 158);
    gpJeu->affiche(gpScreen, "INDIETRO", 63-6, 200);
break;

case 5: // Spanish
   gpJeu->affiche(gpScreen, "OPCIONES", 40-1, 16);
    gpJeu->affiche(gpScreen, "MUSICA", 60, 72);
    gpJeu->affiche(gpScreen, "SONIDOS", 60, 115);
    gpJeu->affiche(gpScreen, "IDIOMA", 60, 158);
    gpJeu->affiche(gpScreen, "Espanol", 170, 158);
    gpJeu->affiche(gpScreen, "ANTERIOR", 63-6, 200);
break;

default:
    gpJeu->affiche(gpScreen, "OPTIONS", 40, 16);
    gpJeu->affiche(gpScreen, "MUSIC", 60, 72);
    gpJeu->affiche(gpScreen, "SOUNDS", 60, 115);
    gpJeu->affiche(gpScreen, "LANGUAGE", 60, 158);
    gpJeu->affiche(gpScreen, "English", 170, 158);
    gpJeu->affiche(gpScreen, "RETURN", 63, 200);
break;	
}

	int languageID = getLanguage();
	if (languageID<1 || languageID>5) languageID = 1;
    
    src.h = 21; src.w = 16;src.x = 0;src.y=0;
    dst.x = 26; dst.y = 69+43*ligne;
    
    SDL_BlitSurface(imageCurseur, &src, gpScreen, &dst);
    
    src.h = 16; src.w = 8;src.x = 0;src.y=0;
    dst.x = 136+8*opt1; dst.y = 56+16;
    SDL_BlitSurface(imageNiveau, &src, gpScreen, &dst);
                
    src.h = 16; src.w = 8;src.x = 0;src.y=0;
    dst.x = 136+8*opt2; dst.y = 56+16+43;
    SDL_BlitSurface(imageNiveau, &src, gpScreen, &dst);
	drawFlag(gpScreen, languageID);
	
}

void Generique::drawRecord(SDL_Surface* gpScreen, int ligne, int colonne) {
    draw(gpScreen);
    SDL_Rect src;
    SDL_Rect dst;
    
    src.h = 21; src.w = 16;src.x = 0;src.y=0;
    dst.x = 26+152*colonne; dst.y = 53+48*ligne;
    
    SDL_BlitSurface(imageCurseur, &src, gpScreen, &dst);
}

void Generique::drawEffacer(SDL_Surface* gpScreen, int ligne) {
    drawRecord(gpScreen, 3, 1);
    SDL_Rect src;
    SDL_Rect dst;
    
    src.h = 16; src.w = 16;src.x = 0;src.y=0;
    dst.x = 112; dst.y = 113+20*ligne;
    
    SDL_BlitSurface(imageFee, &src, gpScreen, &dst);
}

void Generique::drawCharger(SDL_Surface* gpScreen, int ligne, int ligne2) {
    drawSelection(gpScreen, ligne, 0);
    SDL_Rect src;
    SDL_Rect dst;
    
    src.h = 16; src.w = 16;src.x = 0;src.y=0;
    dst.x = 112; dst.y = 93+20*ligne2;
    
    SDL_BlitSurface(imageFee, &src, gpScreen, &dst);
}

void Generique::drawEffacerSave(SDL_Surface* gpScreen, int ligne, int ligne2) {
    drawSelection(gpScreen, ligne, 0);
    SDL_Rect src;
    SDL_Rect dst;
    
    src.h = 16; src.w = 16;src.x = 0;src.y=0;
    dst.x = 112; dst.y = 113+20*ligne2;
    
    SDL_BlitSurface(imageFee, &src, gpScreen, &dst);
}

void Generique::drawIntro(SDL_Surface* gpScreen, int etape) {
    SDL_Rect src;
    SDL_Rect dst;
    
    if(SDL_GetTicks() > lastAnimTime + 40) {
        lastAnimTime = SDL_GetTicks();
        anim++;            
        if (anim > 31) anim = 0;
    }
                
    src.h = 16; src.w = 16;
    for (int j=0; j<240; j+=16) 
        for (int i=0; i<320; i+=16) {
            src.x = 16-anim%16;src.y=anim%16; dst.x = i; dst.y = j;
            if (i>16 && i<288 && j>144 && j<224) src.x+=32;
            SDL_BlitSurface(imageFond2, &src, gpScreen, &dst);
        }
                
    src.h = 32; src.w = 32;
    for (int j=0; j<240; j+=32) 
        for (int i=0; i<320; i+=32) {
            src.x = anim;src.y=anim; dst.x = i; dst.y = j;
            if (i>16 && i<288 && j>144 && j<224) src.x+=64;
            SDL_BlitSurface(imageFond1, &src, gpScreen, &dst);
        }
                
    src.h = 111; src.w = 320;
    src.x = 0; src.y=0; dst.x = 0; dst.y = 17;
    SDL_BlitSurface(imageIntro[etape], &src, gpScreen, &dst);
                
    gpJeu->getTexte()->draw(gpScreen);
}

void Generique::drawDebut(SDL_Surface* gpScreen) {
    if (!imageNuit) imageNuit = IMG_Load("romfs:/images/logos/nuit.png");
    SDL_Rect dst; dst.x = 0; dst.y = 0;
    SDL_BlitSurface(imageNuit, NULL, gpScreen, &dst);
    gpJeu->getTexte()->draw(gpScreen);
}

void Generique::drawBackground(SDL_Surface* gpScreen) {
    SDL_BlitSurface(imageBackground, NULL, gpScreen, NULL);
}

void Generique::drawFlag(SDL_Surface* gpScreen, int flagID){
    SDL_Rect src; src.x = (flagID-1)*26; src.y = 0;src.h = 16; src.w = 26;
    SDL_Rect dst; dst.x = 136; dst.y = 158;
    SDL_BlitSurface(imageFlags, &src, gpScreen, &dst);
}



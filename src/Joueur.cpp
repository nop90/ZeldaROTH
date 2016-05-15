/*

    Zelda Return of the Hylian

    Copyright (C) 2005-2008  Vincent Jouillat

    Please send bugreports with examples or suggestions to www.zeldaroth.fr

*/

//#include <SDL/SDL.h>
//#include <SDL/SDL_image.h>
#include "3ds/os_3ds.h"

#include "Menu.h"
#include "Joueur.h"
#include "Monde.h"
#include "Texte.h"
#include "Projectile.h"
#include "Jeu.h"

Joueur::Joueur(Jeu* jeu, int nmsv) : Personnage(), numSave(nmsv), temps(0), xdep2(0), 
ydep2(0), dirdep2(S), magie(0), 
magiemax(32), typeanim(AUCUNE), minanim(0), fleche(0), bombe(0), rubis(0), objet(0), 
boucl(0), bouclier(0), epee(0), gpJeu(jeu), charge(0), flechemax(30), bombemax(10),
rubismax(999), lapin(false), onilink(false), oni(false), onijauge(0), onimax(5), plouf(0), 
mort(0), porte(0), boostVie(0), boostMagie(0), boostRubis(0), pousse(0), trouve(0), 
avancement(0), invisible(false), dirglisse(0), dirglace(0), glace(0), glisse(0), vitesse(0),
oldxg(0), oldyg(0), immo(false), loader(false) {
    image = IMG_Load("romfs:/images/link/link1.png");
    SDL_SetColorKey(image,SDL_SRCCOLORKEY,SDL_MapRGB(image->format,0,0,255));
    imageSpin = IMG_Load("romfs:/images/link/spin.png");
    SDL_SetColorKey(imageSpin,SDL_SRCCOLORKEY,SDL_MapRGB(imageSpin->format,0,0,255));
    imageObjets = IMG_Load("romfs:/images/link/objets.png");
    SDL_SetColorKey(imageObjets,SDL_SRCCOLORKEY,SDL_MapRGB(imageObjets->format,0,0,255));
    x=86;y=24;w=16;h=24;viemax=6;vie=viemax;magie=magiemax;
    for (int i = 0; i < 44; i++) coeur[i]=0;
    for (int i = 0; i < 7; i++) cristaux[i]=0;
    for (int i = 0; i < 3; i++) medaillon[i]=0;
    for (int i = 0; i < 13; i++) objets[i]=0;
    for (int i = 0; i < 3; i++) bouteille[i]=0;
    for (int i = 0; i < 53; i++) ennemis[i]=0;
    for (int i = 0; i < 8; i++) {oldx[i]=x; oldy[i]=y;}
    for (int i = 0; i < 9; i++) for (int j = 0; j < 4; j++) cle[i][j]=0;
    for (int i = 0; i < 10; i++) for (int j = 0; j < 15; j++) coffre[i][j]=0;
    for (int i = 0; i < 9; i++) for (int j = 0; j < 16; j++) for (int k = 0; k < 10; k++)
        explore[i][j][k]=false;
    
    gpZoneBase->getZone()->x=0;
    gpZoneBase->getZone()->y=8;
    gpZoneBase->getZone()->w=w;
    gpZoneBase->getZone()->h=16;
    imageEpee = NULL;
    
    load();
    
    saveEmpl();
    saveSemiEmpl();
    
    calculDef();
    
    TimeB = SDL_GetTicks();
    
    // pour ROTH
    objets[O_SAC_BOMBES]=1;
    objets[O_BOTTES]=1;
    objets[O_CARTE]=1;
    
    //à virer
    /*bombe=10;
    setEpee(1);
    setBouclier(1);
    cristaux[0]=1;
    cristaux[2]=1;
    viemax=28; vie=viemax;
    objets[O_GANTS]=1;
    coeur[0]=1;
    coeur[9]=1;
    objets[O_ARC]=2;
    objets[O_ENCYCL]=1;
    avancement=0;
    //objets[O_LIVRE]=0;
    //medaillon[0]=1;
    //medaillon[1]=1;
    //medaillon[2]=1;
    calculDef();
    fleche=30;
    //y+=16;
    rubis = 598;
    //x+=320*6; y+=64;
    coeur[16]=1;
    coeur[17]=1;
    magiemax=64;
    magie=64;
    //onilink=true;
    //onijauge=3;
    //y+=64;
    saveEmpl();
    saveSemiEmpl();*/
}

Joueur::~Joueur() {
    delete boucl;
    SDL_FreeSurface(imageEpee);
    SDL_FreeSurface(imageSpin);
    SDL_FreeSurface(imageObjets);
}

//sauvegarde
void Joueur::save() {
    int zone = gpJeu->getZone();
    int xd = xdep;
    int yd = ydep;
    int dird = dirdep;
    if (!gpJeu->isDonjon()) {zone = 21; xd = 86; yd = 24; dird = S;}
    int tps = temps + ((SDL_GetTicks()-TimeB)/1000);
    if (tps > 359999) tps = 359999;
    ostringstream im;
    im << numSave;

    ofstream f(("sdmc:/3ds/ZeldaROTH/roth" + im.str() + ".dat").c_str(),ios::out | ios::binary);
    f.write((char *)&tps,sizeof(int));
    f.write((char *)&zone,sizeof(int));
    f.write((char *)&xd,sizeof(int));
    f.write((char *)&yd,sizeof(int));
    f.write((char *)&dird,sizeof(int));
    f.write((char *)&vie,sizeof(int));
    f.write((char *)&viemax,sizeof(int));
    f.write((char *)&magie,sizeof(int));
    f.write((char *)&magiemax,sizeof(int));
    f.write((char *)&fleche,sizeof(int));
    f.write((char *)&bombe,sizeof(int));
    f.write((char *)&rubis,sizeof(int));
    f.write((char *)&bouclier,sizeof(int));
    f.write((char *)&epee,sizeof(int));
    f.write((char *)&mort,sizeof(int));
    f.write((char *)&boostVie,sizeof(int));
    f.write((char *)&boostMagie,sizeof(int));
    f.write((char *)&boostRubis,sizeof(int));
    f.write((char *)&avancement,sizeof(int));
    for (int i = 0; i < 44; i++) f.write((char *)&coeur[i],sizeof(int));
    for (int i = 0; i < 7; i++) f.write((char *)&cristaux[i],sizeof(int));
    for (int i = 0; i < 3; i++) f.write((char *)&medaillon[i],sizeof(int));
    for (int i = 0; i < 13; i++) f.write((char *)&objets[i],sizeof(int));
    for (int i = 0; i < 3; i++) f.write((char *)&bouteille[i],sizeof(int));
    for (int i = 0; i < 53; i++) f.write((char *)&ennemis[i],sizeof(int));
    for (int j = 0; j < 4; j++) 
        for (int i = 0; i < 9; i++) 
            f.write((char *)&cle[i][j],sizeof(int));
    for (int j = 0; j < 15; j++) 
        for (int i = 0; i < 10; i++) 
            f.write((char *)&coffre[i][j],sizeof(int));
    for (int k = 0; k < 10; k++) 
        for (int j = 0; j < 16; j++) 
            for (int i = 0; i < 9; i++) 
                f.write((char *)&explore[i][j][k],sizeof(int));
    f.close();
}

void Joueur::load() {
    int zone;
    ostringstream im;
    im << numSave;
    ifstream f(("sdmc:/3ds/ZeldaROTH/roth" + im.str() + ".dat").c_str(),ios::in | ios::binary);
    if(!f.is_open()) return;
    f.read((char *)&temps,sizeof(int));
    f.read((char *)&zone,sizeof(int)); gpJeu->setZone(zone);
    f.read((char *)&x,sizeof(int));
    f.read((char *)&y,sizeof(int));
    f.read((char *)&direction,sizeof(int));
    f.read((char *)&vie,sizeof(int));
    f.read((char *)&viemax,sizeof(int));
    f.read((char *)&magie,sizeof(int));
    f.read((char *)&magiemax,sizeof(int));
    f.read((char *)&fleche,sizeof(int));
    f.read((char *)&bombe,sizeof(int));
    f.read((char *)&rubis,sizeof(int));
    f.read((char *)&bouclier,sizeof(int)); if (bouclier) setBouclier(bouclier);
    f.read((char *)&epee,sizeof(int)); if (epee) setEpee(epee);
    f.read((char *)&mort,sizeof(int));
    f.read((char *)&boostVie,sizeof(int));
    f.read((char *)&boostMagie,sizeof(int));
    f.read((char *)&boostRubis,sizeof(int));
    f.read((char *)&avancement,sizeof(int));
    for (int i = 0; i < 44; i++) f.read((char *)&coeur[i],sizeof(int));
    for (int i = 0; i < 7; i++) f.read((char *)&cristaux[i],sizeof(int));
    for (int i = 0; i < 3; i++) f.read((char *)&medaillon[i],sizeof(int));
    for (int i = 0; i < 13; i++) f.read((char *)&objets[i],sizeof(int));
    for (int i = 0; i < 3; i++) f.read((char *)&bouteille[i],sizeof(int));
    for (int i = 0; i < 53; i++) f.read((char *)&ennemis[i],sizeof(int));
    for (int j = 0; j < 4; j++) 
        for (int i = 0; i < 9; i++) 
            f.read((char *)&cle[i][j],sizeof(int));
    for (int j = 0; j < 15; j++) 
        for (int i = 0; i < 10; i++) 
            f.read((char *)&coffre[i][j],sizeof(int));
    for (int k = 0; k < 10; k++) 
        for (int j = 0; j < 16; j++) 
            for (int i = 0; i < 9; i++) 
                f.read((char *)&explore[i][j][k],sizeof(int));
    f.close();
    loader = true;
}

bool Joueur::getLoader() {return loader;}

int Joueur::getTemps(int i) {
    
    int tps = temps + ((SDL_GetTicks()-TimeB)/1000); //temps en secondes
    if (tps > 359999) tps = 359999;
    
    switch (i) {
        case 0 : return tps%60; break; //secondes
        case 1 : return (tps - ((int)(tps/3600))*3600)/60 ; break; //minutes
        case 2 : return (tps/3600); break; //heures
        default : return 0; break;
    }
}

bool Joueur::getExplore(int z, int i, int j) {return explore[z][i][j];}
void Joueur::setExplore(int z, int i, int j) {explore[z][i][j]=true;}
void Joueur::visible() {invisible=false;}

void Joueur::draw(SDL_Surface* gpScreen) {
    
    glisser();
    
    if (invisible) return;
    
    // marche ou immobile
    if (typeanim == POUSSE) {
        drawPousse(gpScreen);
    }
    
    if (trouve && !charge && !porte && !lapin && (typeanim == AUCUNE || typeanim == MARCHE)) 
        drawTrouve(gpScreen);
    else if (charge && !lapin && (typeanim == AUCUNE || typeanim == MARCHE)) drawCharge(gpScreen);
    else if (typeanim == AUCUNE || typeanim == MARCHE) drawImmoMarche(gpScreen);
    
    // chute
    if (typeanim == CHUTE) drawChute(gpScreen);
    
    // coup d'épée
    if (typeanim == EPEE) drawEpee(gpScreen);
    
    // attaque spin
    if (typeanim == SPIN) drawSpin(gpScreen);
    
    // arc
    if (typeanim == ARC) drawArc(gpScreen);
    
    // grappin
    if (typeanim == GRAPPIN || typeanim == LANTERNE) drawGrappin(gpScreen);
    
    // baguette de feu ou de glace
    if (typeanim == BFEU || typeanim == BGLACE || typeanim == MARTEAU) drawBaguette(gpScreen);
    
    // plouf
    //if (plouf > 0 && plouf <=4) drawPlouf(gpScreen);
    
    // fotte
    if (typeanim == FLOTTE || typeanim == NAGE) drawFlotte(gpScreen);
    
    // soulève
    if (typeanim == LOURD || typeanim == SOULEVE || typeanim == PORTE 
    || typeanim == EMMENE || typeanim == LANCE) drawSouleve(gpScreen);
    
    // boire
    if (typeanim == BOIRE) drawBoire(gpScreen);
    
    // touche
    if (typeanim == TOUCHE) drawTouche(gpScreen);
    
    // mort
    if (typeanim == MORT && !gpJeu->isWater(x, y+8, 16, 16)
     && !gpJeu->isVide(x, y+8, 16, 16)) drawMort(gpScreen);
    
    bool stop = gpJeu->getStop();
    //bool transition = gpJeu->getTransition();
    
    if((SDL_GetTicks() > lastAnimTime + (ANIM_REFRESH/3) || typeanim == SPIN)
     && !stop) {
        lastAnimTime = SDL_GetTicks();
        anim++;            
        if (anim > animmax) anim = minanim;
        if (typeanim == ARC && anim == minanim && fleche > 0) tireFleche();
        if ((typeanim == BFEU || typeanim == BGLACE) && anim == minanim) tireMagie();
        if (typeanim == MARTEAU && anim == 4) gpJeu->getAudio()->playSound(30);
        if (typeanim == NOYADE && anim == minanim) {
            setVie(vie-1);
            if (vie > 0) { 
                x=oldx[7]; y=oldy[7]; plouf=0; invincible=1;
                if (!gpJeu->isNotFull(x, y+8, 16, 16)) {x=xdep2; y=ydep2; direction=dirdep2;}
            }
            else setTypeAnim(MORT);
        }
        if (typeanim == CHUTE && anim == minanim) {
            if ((gpJeu->getZone()==15 || (gpJeu->getZone()==18 && x>1600)) && vie > 0) {
                invisible = true;
                gpJeu->getMonde()->transitFull(gpJeu->getZone(), x-(16*100), y);
            }else {
                int retire=2;
                if (defense) retire/=defense; else retire*=2;
                if (retire<1) retire=1;
                setVie(vie-retire);
                if (vie > 0) {invincible=1; x=xdep2; y=ydep2; direction=dirdep2;}
                else setTypeAnim(MORT);
            }
        }
        if (typeanim == SOULEVE && anim == minanim) setTypeAnim(PORTE);
        if (anim < 0) {
            setTypeAnim(AUCUNE);
            if (gpJeu->isWater(x, y+8, 16, 16)) {
                plouf=0; bigPlouf();}
            if (vie<=0) setTypeAnim(MORT);
        }
        if (typeanim == EPEE && anim == 1) gpJeu->getAudio()->playSound(22);
        if (charge && charge < 21  && typeanim != EPEE) charge++;
        if (charge == 20) gpJeu->getAudio()->playSound(23);
        if (typeanim == SPIN && anim == 10) gpJeu->getAudio()->playSound(24);
        if (typeanim == SOULEVE && anim == 4) porte = gpJeu->enleve();
        if (typeanim == BOIRE && anim == 6) boit();
        if (pousse) pousse++;
        if (pousse == 10) {setTypeAnim(POUSSE);}
        if (typeanim == POUSSE && anim == 14) {
            int a;
            int b;
            switch(direction) {
                case N : a=x+8; b=y; break;
                case S : a=x+8; b=y+32; break;
                case O : a=x-8; b=y+16; break;
                case E : a=x+24; b=y+16; break;
            }
            gpJeu->pousse(a,b,direction);
            pousse=0;
            setTypeAnim(AUCUNE);
        }
        
        //if (plouf > 0 && plouf <5) plouf++;
        if (invincible > 0) invincible++;
        if (invincible >= 40) invincible=0;
        
        if (boostVie > 0 && vie > 0) {
            boostVie--;
            if ((vie%2) && vie < viemax) gpJeu->getAudio()->playSound(15);
            setVie(vie+1);
            
        }
        if (boostMagie > 0 && vie > 0) {
            setMagie(magie+1);
            boostMagie--;
        }
        
        if (boostRubis > 0) {
            setRubis(rubis+1);
            boostRubis--;
        }
        
        if (boostRubis < 0) {
            setRubis(rubis-1);
            boostRubis++;
        }
    }
    
    if(typeanim==AUCUNE || typeanim==MARCHE) {oldxg = x; oldyg = y;}
}

void Joueur::revie() {
    if (avancement==5 || avancement==6) avancement=4; 
    gpJeu->getAudio()->stopSpecial();
    x=xdep; y=ydep; direction=dirdep; mort++; vie=viemax; invincible=1; plouf=0;
    boostVie=0; boostMagie=0; lapin=false;
    gpJeu->getStatut()->setJauge(false);
    setTypeAnim(AUCUNE);
    gpJeu->videListes();
    gpJeu->getMonde()->changeZone(gpJeu->getZone());
    gpJeu->testFin();
}

void Joueur::saveSemiEmpl() {
    xdep2 = x;
    ydep2 = y;
    dirdep2 = direction;
}

void Joueur::restoreSemiEmpl() {
    x = xdep2;
    y = ydep2;
    direction = dirdep2;
}

void Joueur::savePrec() {
    if (x==oldx[0] && y==oldy[0]) return;
    for (int i=0; i<7; i++) {
        oldx[7-i]=oldx[6-i];
        oldy[7-i]=oldy[6-i];
    }
    oldx[0] = x; oldy[0] = y;
}

void Joueur::setVie(int i) {
    // rajouter augmentation avec anim 
    vie = i;
    if (vie < 0) vie = 0;
    if (vie > viemax) vie = viemax;
    if (vie < viemax/3 && vie > 0) gpJeu->getAudio()->playSound(32);
    if (vie==0) {charge=0; setTypeAnim(MORT); gpJeu->getAudio()->playSpecial(2);}
}

void Joueur::setVieMax(int i) {
    viemax = i; //setVie(viemax);
    if (viemax < 6) viemax = 6;
    if (viemax > 40) viemax = 40;
}

int Joueur::getCoeur(int i) {return coeur[i];}
void Joueur::setCoeur(int i) {
    coeur[i]=1; 
    if (nbQuarts()%4==0 || i < 8) {
        setVieMax(viemax+2);
    }
    setBoostVie(viemax-vie);
}

void Joueur::setLapin() {
    if (!lapin) {
        setTypeAnim(AUCUNE);
        gpJeu->ajouteProjectile(16, direction, x-6, y-1, 0);
        invincible=0;
    }
    lapin=true;
}

bool Joueur::isLapin() {return lapin;}
void Joueur::setCristal(int i) {cristaux[i]=1; setCoeur(i);}
int Joueur::getBoostVie() { return boostVie;}
void Joueur::setBoostVie(int i) {boostVie+=i;}
int Joueur::getBoostMagie() { return boostMagie;}
void Joueur::setBoostMagie(int i) {boostMagie+=i;}
int Joueur::getBoostRubis() { return boostRubis;}
void Joueur::setBoostRubis(int i) {boostRubis+=i;}
int Joueur::getPousse() {return pousse;}
void Joueur::setPousse(int i) {pousse=i;}
void Joueur::setTrouve(int i) {trouve=i;}
int Joueur::getAvancement() {return avancement;}
void Joueur::setAvancement(int i) {avancement = i;}

void Joueur::setBouteille(int i, int v) {bouteille[i]=v;}

bool Joueur::verifZone() {
    SDL_Rect* z = gpZoneBase->getZone();
    return gpJeu->isNotFull(x+z->x, y+z->y, z->w, z->h);
}

void Joueur::moveX(int i, int nbdir) {
    if (x+i <= gpJeu->getMonde()->regionValue(0) || x+i+16 >= gpJeu->getMonde()->regionValue(2)) return;
    SDL_Rect* z = gpZoneBase->getZone();
    if (gpJeu->isPnj(x + z->x + i, y + z->y, z->w, z->h)) return;
    if (gpJeu->verifZoneJoueur(x+z->x+i, y+z->y, z->w, z->h, nbdir)) {moveX(i); pousse=0;}
    else if (!gpJeu->isGlace(x,y+8,16,16) && !gpJeu->isWater(x,y+8,16,16) && !pousse 
        && !charge && !porte && !lapin && typeanim!=TOUCHE) pousse=1;
}

void Joueur::moveY(int i, int nbdir) {
    if (y+i <= gpJeu->getMonde()->regionValue(1) || y+i+16 >= gpJeu->getMonde()->regionValue(3)) return;
    SDL_Rect* z = gpZoneBase->getZone();
    if (gpJeu->isPnj(x + z->x, y + z->y + i, z->w, z->h)) return;
    if (gpJeu->verifZoneJoueur(x+z->x, y+z->y+i, z->w, z->h, nbdir)) {moveY(i); pousse=0;}
    else if (!gpJeu->isGlace(x,y+8,16,16) && !gpJeu->isWater(x,y+8,16,16) && !pousse 
        && !charge && !porte && !lapin && typeanim!=TOUCHE) pousse=1;
}

void Joueur::moveX(int i) { x += i; vitesse = abs(i);}

void Joueur::moveY(int i) { y += i; vitesse = abs(i);}

bool Joueur::isDauphin() {
    return (hasObjet(O_PALMES) && !lapin);
}

void Joueur::setTypeAnim(int t) {
    if (t == BOIRE) {
        if ((bouteille[objet-9]==2 && vie == viemax)
        || (bouteille[objet-9]==3 && magie == magiemax)
        || (bouteille[objet-9]==4 && vie == viemax && magie == magiemax)) {
            gpJeu->ecrit(160);
            return;
        }
    }
    
    typeanim = t;
    switch (t) {
        case AUCUNE : minanim = 0; animmax = 0; pousse=0; break;
        case MARCHE : if (!charge) {minanim = 1; animmax = 16;} 
            else {minanim = 1; animmax = 8;} porte=0; break;
        case EPEE : if (!epee) return; minanim = -1; animmax = 4; pousse=0; break;
        case SPIN : if (!epee) return; minanim = -1; animmax = 30; pousse=0; break;
        case ARC : minanim = -1; animmax = 4; pousse=0; break;
        case GRAPPIN : minanim = 0; animmax = 0; tireGrappin(); pousse=0; break;
        case BOMBE : minanim = 0; animmax = 0; poseBombe(); pousse=0; break;
        case BFEU : case BGLACE : minanim = -1; animmax = 4; pousse=0; break;
        case LANTERNE : minanim = 0; animmax = 0; tireLanterne(); pousse=0; break;
        case MARTEAU : minanim = -1; animmax = 4; pousse=0; break;
        case NOYADE : minanim = -1; animmax = 16; charge=0; lance(0); pousse=0; invincible=1; break;
        case FLOTTE : minanim = 0; animmax = 5; charge=0; lance(0); pousse=0; break;
        case NAGE : minanim = 0; animmax = 11; pousse=0; break; 
        case LOURD : minanim = -1; animmax = 8; pousse=0; charge=0; break;
        case SOULEVE : minanim = -1; animmax = 8; pousse=0; charge=0; break;
        case PORTE : minanim = 0; animmax = 0; pousse=0; break;
        case EMMENE : minanim = 0; animmax = 11; pousse=0; break;
        case LANCE : minanim = -1; animmax = 2; lance(8); gpJeu->getAudio()->playSound(35); pousse=0; break;
        case BOIRE : minanim = -1; animmax = 8; pousse=0; break;
        case POUSSE : minanim = 0; animmax = 15; charge=0; pousse=0; break;
        case CHUTE : minanim = -1; animmax = 17; charge=0; invincible=1; lance(0); pousse=0; break;
        case TOUCHE : minanim = -1; animmax = 9; charge=0; invincible=1; //plouf=0;
            if (lapin) gpJeu->ajouteProjectile(16, direction, x-6, y-1, 0);
            lance(0); pousse=0; 
            lapin=false; break;
        case MORT : minanim = 0; animmax = 0; charge=0; pousse=0; break;
    }
    anim = minanim;
    if (anim < 0) anim = 0;
}

int Joueur::getTypeAnim() {
    return typeanim;
}

void Joueur::setBouclier(int b) {
    bouclier = b;
    delete boucl;
    std::ostringstream im;
    im << bouclier;
    if (bouclier) 
        boucl = new Bouclier(IMG_Load(("romfs:/images/link/bouclier" + im.str() + ".png").c_str()), bouclier);
    calculDef();
}

void Joueur::setEpee(int e) {
    epee = e;
    SDL_FreeSurface(imageEpee);
    std::ostringstream im;
    im << epee;
    imageEpee = IMG_Load(("romfs:/images/link/epee" + im.str() + ".png").c_str());
    SDL_SetColorKey(imageEpee,SDL_SRCCOLORKEY,SDL_MapRGB(imageEpee->format,0,0,255));
}

void Joueur::calculDef() {
    defense = bouclier + medaillon[0] + medaillon[1] + 2*medaillon[2];
    if (oni) defense--;
}

int Joueur::nbQuarts() {
    int total = 0;
    for (int i = 8; i < 44; i++) total += coeur[i];
    return total;
}

int Joueur::nbCristaux() {
    int total = 0;
    for (int i = 0; i < 7; i++) total += cristaux[i];
    return total;
}

int Joueur::nbEnnemis() {
    int total = 0;
    for (int i = 1; i < 53; i++) total += ennemis[i];
    return total;
}

int Joueur::getMort() {return mort;}

void Joueur::setEnnemi(int i) {
    ennemis[i] = 1;
}

int Joueur::getEnnemi(int i) {
    return ennemis[i];
}

bool Joueur::hasCristal(int i) {
    return (cristaux[i]==1);
}

bool Joueur::hasMedaillon(int i) {
    return (medaillon[i]==1);
}

void Joueur::trouveMedaillon(int i) {
    medaillon[i]=1;
    calculDef();
}

int Joueur::hasObjet(int i) {
    return objets[i];
}

int Joueur::hasBouteille(int i) {
    return bouteille[i];
}

void Joueur::chargeSpin() { if (epee) charge = 1;}

void Joueur::dechargeSpin() { charge=0;}

int Joueur::getCharge() { return charge;}

int Joueur::getObjet() { return objet;}
int Joueur::getRubis() { return rubis;}
int Joueur::getFleche() { return fleche;}
int Joueur::getBombe() { return bombe;}
int Joueur::getRubisMax() { return rubismax;}
int Joueur::getFlecheMax() { return flechemax;}
int Joueur::getBombeMax() { return bombemax;}
int Joueur::getMagie() { return magie;}
int Joueur::getMagieMax() { return magiemax;}
int Joueur::getForce() { if (!oni) return epee; else return epee+1;} //peut être différent si en Oni Link
int Joueur::getEpee() { return epee;}
int Joueur::getDefense() { return defense;}
int Joueur::getBouclier() { return bouclier;}
int Joueur::getCle() { return cle[gpJeu->getZone()-12][3];}

bool Joueur::getOnilink() {return onilink;}
bool Joueur::getOni() {return oni;}
int Joueur::getOnijauge() {return onijauge;}
int Joueur::getOnimax() {return onimax;}

void Joueur::setMagie(int m) {
    magie=m; 
    if (magie > magiemax) magie = magiemax; 
    if (magie<0) magie=0;
}
void Joueur::setMagieMax(int m) {
    magiemax = m;
    if (magiemax > 64) magiemax = 64;
    if (magiemax < 32) magiemax = 32;
    setMagie(magiemax);
}

void Joueur::setOnilink(bool b) {if (!b) setOni(b); oni = b;}
void Joueur::setOni(bool b) {
    oni = b;
    calculDef();
}
void Joueur::setOnijauge(int i) {
    onijauge=i;
    if (onijauge<0) onijauge=0;
    if (onijauge==0 && oni) setOni(false);
    if (onijauge>onimax) onijauge=onimax;
    if (onijauge==onimax && !oni) setOni(true);
}
void Joueur::setOnimax(int i) {onimax=i;}

void Joueur::trouveObjet(int obj, int val) {objets[obj]=val;}

void Joueur::setObjet(int i) { objet=i;}
void Joueur::setRubis(int i) { 
    rubis=i;
    if (rubis < 0) rubis = 0;
    if (rubis > rubismax) rubis = rubismax;
}
void Joueur::setFleche(int i) { 
    fleche=i;
    if (fleche < 0) fleche = 0;
    if (fleche > flechemax) fleche = flechemax;
}
void Joueur::setBombe(int i) { 
    bombe=i;
    if (bombe < 0) bombe = 0;
    if (bombe > bombemax) bombe = bombemax;
}
void Joueur::setRubisMax(int i) { 
    rubismax=i;
    if (rubismax < 0) rubismax = 0;
    if (rubismax > 999) rubismax = 999;
}
void Joueur::setFlecheMax(int i) { 
    flechemax=i;
    if (flechemax < 0) flechemax = 0;
    if (flechemax > 999) flechemax = 999;
}
void Joueur::setBombeMax(int i) { 
    bombemax=i;
    if (bombemax < 0) bombemax = 0;
    if (bombemax > 999) bombemax = 999;
}

void Joueur::bigPlouf() {
    if (typeanim!=FLOTTE && typeanim!=NAGE && typeanim!=NOYADE && typeanim!=TOUCHE) {
        if (plouf == 0) gpJeu->ajouteProjectile(8, direction, x, y, 0);
        plouf=1;
        if (vie<=0) return;
        if (isDauphin()) {setTypeAnim(FLOTTE); objet=O_PALMES;}
        else setTypeAnim(NOYADE);
    }
}

void Joueur::antiPlouf() {
    plouf=0;
}

void Joueur::tireFleche() {
    if (fleche <=0) return;
    gpJeu->getAudio()->playSound(25);
    
    int i=x;
    int j=y;
    int f;
    
    switch (direction) {
        case N : i+=0; j+=18; break;
        case S : i+=2; j+=7; break;
        case O : i-=8-4; j+=7; break;
        case E : i+=5-1; j+=7; break;
    }
    
    if (objets[O_ARC] == 2) f=1; else f=2;
    
    gpJeu->ajouteProjectile(f, direction, i, j, 8);
    setFleche(fleche-1);
}

void Joueur::tireGrappin() {
    gpJeu->getAudio()->playSound(26);
    
    int i=x+2;
    int j=y+10;
    
    if (direction==N) j+=4;
    
    gpJeu->ajouteProjectile(3, direction, i, j, 8);
}

void Joueur::poseBombe() {
    gpJeu->getAudio()->playSound(27);
    gpJeu->ajouteProjectile(4, direction, x, y+8, 0);
    setBombe(bombe-1);
    setTypeAnim(AUCUNE);
}

void Joueur::tireMagie() {
    if (magie <4) return;
    gpJeu->getAudio()->playSound(28);
    int m;
    if (typeanim == BFEU) m=5; else m=6;
    gpJeu->ajouteProjectile(m, direction, x, y+8, 8);
    setMagie(magie-4);
}

void Joueur::tireLanterne() {
    if (magie <2) return;
    gpJeu->getAudio()->playSound(29);
    int i=x;
    int j=y+5;
    switch(direction) {
        case N  : j-=16; break;
        case S  : j+=16; break;
        case O  : i-=16; break;
        case E  : i+=16; break;
    }
    gpJeu->ajouteProjectile(7, direction, i, j, 0);
    setMagie(magie-2);
}

void Joueur::lance(int v) {
    if (!porte) return;
    gpJeu->ajouteProjectile(porte+20, direction, x, y-8+8, v);
    porte=0;
}

void Joueur::boit() {
    if (bouteille[objet-9] < 2) return;
    if (bouteille[objet-9]==2 || bouteille[objet-9]==4) boostVie = viemax - vie;
    if (bouteille[objet-9]==3 || bouteille[objet-9]==4) {
        boostMagie = magiemax - magie;
        if (boostMagie > 0) gpJeu->getAudio()->playSound(37);
    }
    bouteille[objet-9]=1;
}

bool Joueur::getCoffre(int i, int j) {return (coffre[i][j]==1);}

void Joueur::setCoffre(int i, int j) {coffre[i][j]=1;}

int Joueur::getCle(int i, int j) {return cle[i][j];}

void Joueur::setCle(int i, int j, int val) {cle[i][j]=val;}

void Joueur::setImmo(bool b) {immo=b;}

bool Joueur::getImmo() {return immo;}

void Joueur::glisser() {
    if(!gpJeu->getStop()){
    if((typeanim==AUCUNE || typeanim==MARCHE) && gpJeu->isGlace(x,y,16,16)) {
        
        int xgl, ygl;
        xgl = x - oldxg; ygl = y - oldyg;
        
        int olddirglisse = dirglisse;
        
        if(xgl<0 && ygl<0) dirglisse =1;
        if(xgl==0 && ygl<0) dirglisse =2;
        if(xgl>0 && ygl<0) dirglisse =3;
        if(xgl<0 && ygl==0) dirglisse =4;
        if(xgl==0 && ygl==0) dirglisse =0;
        if(xgl>0 && ygl==0) dirglisse =5;
        if(xgl<0 && ygl>0) dirglisse =6;
        if(xgl==0 && ygl>0) dirglisse =7;
        if(xgl>0 && ygl>0) dirglisse =8;
        
        if (olddirglisse == dirglisse && glisse == 0) {
            glace+=4;
            if (glace>64) glace=64;
        }else {
            if (glisse == 0) {
                dirglace=olddirglisse;
                glisse = glace;
            }
            if (glisse>=4) {
                glisse-=4;
                if ((dirglace == 1 || dirglace == 4 || dirglace == 6) && (dirglisse != 1 || dirglisse != 4 || dirglisse != 6)) moveX(-vitesse,2);
                if ((dirglace == 3 || dirglace == 5 || dirglace == 8) && (dirglisse != 3 || dirglisse != 5 || dirglisse != 8)) moveX(vitesse,2);
                if ((dirglace == 1 || dirglace == 2 || dirglace == 3) && (dirglisse != 1 || dirglisse != 2 || dirglisse != 3)) moveY(-vitesse,2);
                if ((dirglace == 6 || dirglace == 7 || dirglace == 8) && (dirglisse != 6 || dirglisse != 7 || dirglisse != 8)) moveY(vitesse,2);
                gpJeu->testFin();
            }
        }
        
    }else {glisse=0; glace=0;}
    }
}

void Joueur::drawImmoMarche(SDL_Surface* gpScreen) {
    int anim2 = (int)((anim+1)/2);
    if (lapin && anim2>0) anim2 = (((anim2-1)/2)%2)+1;
    
    int phg[2];
    phg[0] = gpJeu->getPhg(0);
    phg[1] = gpJeu->getPhg(1);
    
    SDL_Rect src;
    SDL_Rect dst;
    SDL_Rect* bcl;
    int xbcl=0, ybcl=0;
    
    src.w = w; src.h = h;
    dst.x = x - phg[0]; dst.y = y - phg[1];
    
    if (!lapin) {
        
        if (bouclier && direction != S) {
            bcl = new SDL_Rect();
            switch (direction) {
                case N :
                    bcl->x = 9 + x - phg[0]; bcl->y = 9 + y - phg[1]; 
                    bcl->w = 8; bcl->h = 10; xbcl = 0; ybcl = 0;
                    if (anim2 == 1 || anim2 == 5) bcl->y--;
                    if (anim2 == 2 || anim2 == 7) bcl->y-=2;
                    break;
                case O :
                    bcl->x = -3 + x - phg[0]; bcl->y = 8 + y - phg[1]; 
                    bcl->w = 4; bcl->h = 10; xbcl = 16; ybcl = 0;
                    if (anim2 == 1 || anim2 == 3 || anim2 == 5 || anim2 == 7) bcl->y--;
                    if (anim2 == 2 || anim2 == 6) bcl->y-=2;
                    break;
                case E :
                    bcl->x = 15 + x - phg[0]; bcl->y = 8 + y - phg[1]; 
                    bcl->w = 4; bcl->h = 10; xbcl = 20; ybcl = 0;
                    if (anim2 == 1 || anim2 == 3 || anim2 == 5 || anim2 == 7) bcl->y--;
                    if (anim2 == 2 || anim2 == 6) bcl->y-=2;
                    break;
                default : break;
            }
            boucl->draw(gpScreen, xbcl, ybcl, new ZoneRect(bcl), direction);
        }
    }
    
    src.x = w*direction; src.y = h*anim2;
    if (lapin) {src.x+=352; src.y+=120;}
    SDL_BlitSurface(image, &src, gpScreen, &dst);
    if (bouclier && direction == S && !lapin) {
        bcl = new SDL_Rect();
        bcl->x = x - phg[0]; bcl->y = 14 + y - phg[1]; bcl->w = 8; bcl->h = 10;
        xbcl = 8; ybcl = 0;
        if (anim2 == 2 || anim2 == 6) bcl->y--;
        if (anim2 == 3 || anim2 == 7) bcl->y-=3;
        if (anim2 == 4 || anim2 == 8) bcl->y-=2;
        boucl->draw(gpScreen, xbcl, ybcl, new ZoneRect(bcl), direction);
    }
}

void Joueur::drawCharge(SDL_Surface* gpScreen) {
    int anim2 = (int)((anim+1)/2);
    int phg[2];
    phg[0] = gpJeu->getPhg(0);
    phg[1] = gpJeu->getPhg(1);
    
    SDL_Rect src;
    SDL_Rect dst;
    SDL_Rect* bcl;
    int xbcl, ybcl;
    
    src.w = w; src.h = h;
    dst.x = x - phg[0]; dst.y = y - phg[1];
    
    // Link
    src.x = 128+w*direction; src.y = h*anim2;
    SDL_BlitSurface(image, &src, gpScreen, &dst);
    
    // Bouclier
    if (bouclier) {
        switch (direction) {
            case N :
                bcl = new SDL_Rect();
                bcl->x = 15 + dst.x; bcl->y = 9 + dst.y; 
                bcl->w = 4; bcl->h = 10; xbcl = 20; ybcl = 0;
                if (anim) bcl->y++;
                boucl->draw(gpScreen, xbcl, ybcl, new ZoneRect(bcl), E);
                break;
            case S :
                bcl = new SDL_Rect();
                bcl->x = -3 + dst.x; bcl->y = 10 + dst.y; 
                bcl->w = 4; bcl->h = 10; xbcl = 16; ybcl = 0;
                if (anim) bcl->y++;
                boucl->draw(gpScreen, xbcl, ybcl, new ZoneRect(bcl), O);
                break;
            default : break;
        }
    }
    
    // Epee
    switch (epee) {
        case 1 : case 3 : src.w = 14; src.h = 14; break;
        case 5 : src.w = 20; src.h = 20; break;
    }
    src.x = src.w*direction; src.y = src.h*5;
    
    switch (direction) {
        case N : dst.x+=4; src.w = 8; src.h = 12;
            if (!anim) dst.y-=2; else dst.y--; 
            if (epee>3) { dst.x-=2; dst.y-=2; src.h = 15;} break;
        case S : dst.x+=8; src.w = 8; src.h = 12;
            if (!anim) dst.y+=19; else dst.y+=20;
            if (epee>3) {dst.x--; src.h = 14;} break;
        case O : dst.x-=8; src.w = 12; src.h = 8;
            if (!anim) dst.y+=14; else dst.y+=15;
            if (epee>3) {dst.x-=3; src.w = 15;} break;
        case E : dst.x+=12; src.w = 12; src.h = 8;
            if (!anim) dst.y+=14; else dst.y+=15;
            if (epee>3) src.w = 15; break;
    }
    SDL_BlitSurface(imageEpee, &src, gpScreen, &dst);
    if (gpJeu->testEpee(
        dst.x+phg[0], dst.y+phg[1], src.w, src.h, COUP_EPEE, epee, direction)) 
        dechargeSpin();
    
    // étoile
    if (charge < 20) {
        src.x = 18; src.y = 14; src.w = 7; src.h = 7;
        switch (direction) {
            case N : dst.x+=(charge/3)-6+((charge+2)%3); dst.y-=(charge/2)-5; break;
            case S : dst.x-=(charge/3)-2+((charge+2)%3); dst.y+=(charge/2); break;
            case O : dst.y+=((charge+2)%3)-2; dst.x-=(charge/2)-5; break;
            case E : dst.y+=((charge+2)%3)-2; dst.x+=(charge/2); break;
        }
        SDL_BlitSurface(imageSpin, &src, gpScreen, &dst);
        src.x = 5; src.y = 16; src.w = 5; src.h = 5;
        switch (direction) {
            case N : dst.x-=(charge%3)-1; dst.y+=4; break;
            case S : dst.x+=(charge%3)-1; dst.y-=2; break;
            case O : dst.y+=(charge%3)-1; dst.x+=4; break;
            case E : dst.y+=(charge%3)-1; dst.x-=2; break;
        }
        SDL_BlitSurface(imageSpin, &src, gpScreen, &dst);
    }
    if (charge == 20) {
        src.x = 0; src.y = 8; src.w = 8; src.h = 8;
        switch (direction) {
            case N : dst.x-=2; dst.y-=5; if (epee>3) dst.x+=4; break;
            case S : dst.x-=4; dst.y+=9; break;
            case O : dst.x-=7; dst.y-=2; break;
            case E : dst.x+=12; dst.y-=2; break;
        }
        SDL_BlitSurface(imageSpin, &src, gpScreen, &dst);
    }
}

void Joueur::drawEpee(SDL_Surface* gpScreen) {
    //int anim2 = (int)((anim+1)/2);
    int phg[2];
    phg[0] = gpJeu->getPhg(0);
    phg[1] = gpJeu->getPhg(1);
    
    SDL_Rect src;
    SDL_Rect dst;
    SDL_Rect* bcl;
    int xbcl, ybcl;
    
    src.w = w; src.h = h;
    dst.x = x - phg[0]; dst.y = y - phg[1];
    
    // Link
    src.x = 64+w*direction; src.y = h*anim;
    if (direction == O) dst.x-=2;
    if (anim==1 && direction == O) dst.x-=2;
    if (anim==2 && direction == O) dst.x-=3;
    if (anim==3 && direction == O) dst.x-=2;
    if (anim==4 && direction == O) dst.x--;
    if (direction == E) dst.x+=2;
    if (anim==1 && direction == E) dst.x+=2;
    if (anim==2 && direction == E) dst.x+=3;
    if (anim==3 && direction == E) dst.x+=2;
    if (anim==4 && direction == E) dst.x++;
    SDL_BlitSurface(image, &src, gpScreen, &dst);
    // Bouclier
    if (bouclier) {
        switch (direction) {
            case N :
                bcl = new SDL_Rect();
                bcl->x = 16 + dst.x; bcl->y = 12 + dst.y; 
                bcl->w = 4; bcl->h = 10; xbcl = 20; ybcl = 0;
                if (anim == 1 || anim == 4) bcl->y-=2;
                if (anim == 2) bcl->y-=4;
                if (anim == 3) bcl->y-=3;
                boucl->draw(gpScreen, xbcl, ybcl, new ZoneRect(bcl), E);
                break;
            case S :
                bcl = new SDL_Rect();
                bcl->x = -3 + dst.x; bcl->y = 4 + dst.y; 
                bcl->w = 4; bcl->h = 10; xbcl = 16; ybcl = 0;
                if (anim == 1) bcl->y+=3;
                if (anim > 1) {bcl->y+=5; bcl->x--;}
                boucl->draw(gpScreen, xbcl, ybcl, new ZoneRect(bcl), O);
                break;
            default : break;
        }
    }
    // Epee
    switch (epee) {
        case 1 : case 3 : src.w = 14; src.h = 14; break;
        case 5 : src.w = 20; src.h = 20; break;
    }
    src.x = src.w*direction; src.y = src.h*anim;
    switch (direction) {
        case N :
            switch(anim) {
                case 0 : dst.x+=11; dst.y+=7; if (epee > 3) { dst.x+=2; dst.y--;} break;
                case 1 : dst.x+=8; dst.y-=6; if (epee > 3) { dst.x-=5; dst.y-=7;} break;
                case 2 : dst.y-=8; if (epee > 3) dst.y-=7; break;
                case 3 : dst.x-=4; dst.y-=6; if (epee > 3) dst.x-=10;break;
                case 4 : dst.x-=12; dst.y+=1; if (epee > 3) { dst.x-=2; dst.y++;} break;
            }
            break;
        case S :
            switch(anim) {
                case 0 : dst.x-=5; dst.y+=12; if (epee > 3) { dst.x-=2; dst.y+=2;} break;
                case 1 : dst.x-=6; dst.y+=12; if (epee > 3) { dst.x++; dst.y+=7;} break;
                case 2 : dst.x+=9; dst.y+=21; if (epee > 3) dst.y--; break;
                case 3 : dst.x+=10; dst.y+=17; if (epee > 3) dst.x+=4; break;
                case 4 : dst.x+=16; dst.y+=15; break;
            }
            break;
        case O :
            switch(anim) {
                case 0 : dst.x-=2; dst.y+=1; if (epee > 3) { dst.x--; dst.y-=2;} break;
                case 1 : dst.x-=10; if (epee > 3) { dst.x-=4; dst.y++;}break;
                case 2 : dst.x-=13; dst.y+=13; if (epee > 3) { dst.x-=7; dst.y--;} break;
                case 3 : dst.x-=12; dst.y+=16; if (epee > 3) dst.x-=2; break;
                case 4 : dst.x-=7; dst.y+=21; break;
            }
            break;
        case E :
            switch(anim) {
                case 0 : dst.x+=15; dst.y+=1; if (epee > 3) dst.y-=2; break;
                case 1 : dst.x+=15; if (epee > 3) dst.y++; break;
                case 2 : dst.x+=16; dst.y+=13; if (epee > 3) dst.y--; break;
                case 3 : dst.x+=16; dst.y+=16; break;
                case 4 : dst.x+=9; dst.y+=21; break;
            }
            break;
    }
    SDL_BlitSurface(imageEpee, &src, gpScreen, &dst);
    
    collisionEpee();
}

void Joueur::drawSpin(SDL_Surface* gpScreen) {
    int phg[2];
    phg[0] = gpJeu->getPhg(0);
    phg[1] = gpJeu->getPhg(1);
    
    SDL_Rect src;
    SDL_Rect dst;
    SDL_Rect src2;
    SDL_Rect dst2;
    
    src.w = w; src.h = h;
    dst.x = x - phg[0]; dst.y = y - phg[1];
    
    src2.w = 12; src2.h = 12; //épée
    if (epee > 3) { src2.w+=2; src2.h+=2;}
    
    int anim2;
    Direction dir=N;
    // Link
    switch (direction) {
        case N :
            //Link
            if (anim==0 || anim==5 || (anim>=21 && anim<=26)) { src.x = 64; src.y = 120;}
            if (anim>=1 && anim<=4) { src.x = 64; src.y = 144;}
            if (anim>=6 && anim<=8) { src.x = 64; src.y = 168;}
            if (anim>=9 && anim<=12) { src.x = 80; src.y = 120;}
            if (anim>=13 && anim<=16) { src.x = 80; src.y = 144;}
            if (anim>=17 && anim<=20) { src.x = 80; src.y = 168;}
            
            if (anim > 26) {anim2=anim; anim=0; drawImmoMarche(gpScreen); anim=anim2;}
            else {
            
                //Epée
                if (anim==0 || anim==5 || (anim>=21 && anim<=26)) { 
                    dst2.x=dst.x-4; dst2.y=dst.y+17;src2.x = 68; src2.y = 12;
                    if (epee > 3) {dst2.x--; src2.x = 94; src2.y = 14;}
                }
                if (anim>=1 && anim<=4) { 
                    dst2.x=dst.x+3; dst2.y=dst.y+20;src2.x = 56; src2.y = 12;
                    if (epee > 3) {src2.x = 80; src2.y = 14;}
                }
                if (anim==6) { 
                    dst2.x=dst.x-12; dst2.y=dst.y+15;src2.x = 56; src2.y = 24;
                    if (epee > 3) {dst2.x-=2; src2.x = 80; src2.y = 28;}
                }
                if (anim==7 || anim==8) { 
                    dst2.x=dst.x-12; dst2.y=dst.y+8;src2.x = 56; src2.y = 36;
                    if (epee > 3) {dst2.x-=2; dst2.y--; src2.x = 80; src2.y = 42;}
                }
                if (anim==9 || anim==10) { 
                    dst2.x=dst.x+1; dst2.y=dst.y-4;src2.x = 68; src2.y = 0;
                    if (epee > 3) {dst2.x--; dst2.y-=2; src2.x = 94; src2.y = 0;}
                }
                if (anim==11 || anim==12) { 
                    dst2.x=dst.x+10; dst2.y=dst.y-4;src2.x = 56; src2.y = 0;
                    if (epee > 3) {dst2.y-=2; src2.x = 80; src2.y = 0;}
                }
                if (anim==13 || anim==14) { 
                    dst2.x=dst.x+16; dst2.y=dst.y+8;src2.x = 68; src2.y = 36;
                    if (epee > 3) {dst2.y--; src2.x = 94; src2.y = 42;}
                }
                if (anim==15 || anim==16) { 
                    dst2.x=dst.x+16; dst2.y=dst.y+16;src2.x = 68; src2.y = 24;
                    if (epee > 3) {src2.x = 94; src2.y = 28;}
                }
                if (anim==17 || anim==18) { 
                    dst2.x=dst.x+7; dst2.y=dst.y+21;src2.x = 56; src2.y = 12;
                    if (epee > 3) {src2.x = 80; src2.y = 14;}
                }
                if (anim==19 || anim==20) { 
                    dst2.x=dst.x-1; dst2.y=dst.y+21;src2.x = 68; src2.y = 12;
                    if (epee > 3) {dst2.x--; src2.x = 94; src2.y = 14;}
                }
            
                if (anim>=9 && anim <=12) {
                    SDL_BlitSurface(imageEpee, &src2, gpScreen, &dst2);
                    SDL_BlitSurface(image, &src, gpScreen, &dst);
                }else {
                    SDL_BlitSurface(image, &src, gpScreen, &dst);
                    SDL_BlitSurface(imageEpee, &src2, gpScreen, &dst2);
                }
            }
            break;
        case S :
            if (anim==0 || anim==5 || (anim>=21 && anim<=26)) { src.x = 96; src.y = 120;}
            if (anim>=1 && anim<=4) { src.x = 96; src.y = 144;}
            if (anim>=6 && anim<=8) { src.x = 80; src.y = 144;}
            if (anim>=9 && anim<=12) { src.x = 80; src.y = 168;}
            if (anim>=13 && anim<=16) { src.x = 64; src.y = 168;}
            if (anim>=17 && anim<=20) { src.x = 80; src.y = 120;}
            if (anim > 26) {anim2=anim; anim=0; drawImmoMarche(gpScreen); anim=anim2;}
            else {
            
                //Epée
                if (anim==0 || anim==5 || (anim>=21 && anim<=26)) { 
                    dst2.x=dst.x+13; dst2.y=dst.y-2;src2.x = 56; src2.y = 0;
                    if (epee > 3) {dst2.y-=2; src2.x = 80; src2.y = 0;}
                }
                if (anim>=1 && anim<=4) { 
                    dst2.x=dst.x+7; dst2.y=dst.y-6;src2.x = 68; src2.y = 0;
                    if (epee > 3) {dst2.x--; dst2.y-=2; src2.x = 94; src2.y = 0;}
                }
                if (anim==6) { 
                    dst2.x=dst.x+16; dst2.y=dst.y+8;src2.x = 68; src2.y = 36;
                    if (epee > 3) {dst2.y--; src2.x = 94; src2.y = 42;}
                }
                if (anim==7 || anim==8) { 
                    dst2.x=dst.x+16; dst2.y=dst.y+16;src2.x = 68; src2.y = 24;
                    if (epee > 3) {src2.x = 94; src2.y = 28;}
                }
                if (anim==9 || anim==10) { 
                    dst2.x=dst.x+7; dst2.y=dst.y+21;src2.x = 56; src2.y = 12;
                    if (epee > 3) {src2.x = 80; src2.y = 14;}
                }
                if (anim==11 || anim==12) { 
                    dst2.x=dst.x-1; dst2.y=dst.y+21;src2.x = 68; src2.y = 12;
                    if (epee > 3) {dst2.x--; src2.x = 94; src2.y = 14;}
                }
                if (anim==13 || anim==14) { 
                    dst2.x=dst.x-12; dst2.y=dst.y+15;src2.x = 56; src2.y = 24;
                    if (epee > 3) {dst2.x-=2; src2.x = 80; src2.y = 28;}
                }
                if (anim==15 || anim==16) { 
                    dst2.x=dst.x-12; dst2.y=dst.y+8;src2.x = 56; src2.y = 36;
                    if (epee > 3) {dst2.x-=2; dst2.y--; src2.x = 80; src2.y = 42;}
                }
                if (anim==17 || anim==18) { 
                    dst2.x=dst.x+1; dst2.y=dst.y-4;src2.x = 68; src2.y = 0;
                    if (epee > 3) {dst2.x--; dst2.y-=2; src2.x = 94; src2.y = 0;}
                }
                if (anim==19 || anim==20) { 
                    dst2.x=dst.x+10; dst2.y=dst.y-4;src2.x = 56; src2.y = 0;
                    if (epee > 3) {dst2.y-=2; src2.x = 80; src2.y = 0;}
                }
                
                if (anim<=5 || anim >=17) {
                    SDL_BlitSurface(imageEpee, &src2, gpScreen, &dst2);
                    SDL_BlitSurface(image, &src, gpScreen, &dst);
                }else {
                    SDL_BlitSurface(image, &src, gpScreen, &dst);
                    SDL_BlitSurface(imageEpee, &src2, gpScreen, &dst2);
                }
            }
            break;
        case O :
            if (anim==0 || anim==5 || (anim>=21 && anim<=26)) { src.x = 96; src.y = 168;}
            if (anim>=1 && anim<=4) { src.x = 112; src.y = 120;}
            if (anim>=6 && anim<=8) { src.x = 80; src.y = 168;}
            if (anim>=9 && anim<=12) { src.x = 64; src.y = 168;}
            if (anim>=13 && anim<=16) { src.x = 80; src.y = 120;}
            if (anim>=17 && anim<=20) { src.x = 80; src.y = 144;}
            if (anim > 26) {anim2=anim; anim=0; drawImmoMarche(gpScreen); anim=anim2;}
            else {
            
                //Epée
                if (anim==0 || anim==5 || (anim>=21 && anim<=26)) { 
                    dst2.x=dst.x+11; dst2.y=dst.y+16;src2.x = 68; src2.y = 24;
                    if (epee > 3) {src2.x = 94; src2.y = 28;}
                }
                if (anim>=1 && anim<=4) { 
                    dst2.x=dst.x+13; dst2.y=dst.y+9;src2.x = 68; src2.y = 36;
                    if (epee > 3) {src2.x = 94; src2.y = 42;}
                }
                if (anim==6) { 
                    dst2.x=dst.x+7; dst2.y=dst.y+21;src2.x = 56; src2.y = 12;
                    if (epee > 3) {src2.x = 80; src2.y = 14;}
                }
                if (anim==7 || anim==8) { 
                    dst2.x=dst.x-1; dst2.y=dst.y+21;src2.x = 68; src2.y = 12;
                    if (epee > 3) {dst2.x--; src2.x = 94; src2.y = 14;}
                }
                if (anim==9 || anim==10) { 
                    dst2.x=dst.x-12; dst2.y=dst.y+15;src2.x = 56; src2.y = 24;
                    if (epee > 3) {dst2.x-=2; src2.x = 80; src2.y = 28;}
                }
                if (anim==11 || anim==12) { 
                    dst2.x=dst.x-12; dst2.y=dst.y+8;src2.x = 56; src2.y = 36;
                    if (epee > 3) {dst2.x-=2; dst2.y--; src2.x = 80; src2.y = 42;}
                }
                if (anim==13 || anim==14) { 
                    dst2.x=dst.x+1; dst2.y=dst.y-4;src2.x = 68; src2.y = 0;
                    if (epee > 3) {dst2.x--; dst2.y-=6; src2.x = 94; src2.y = 0;}
                }
                if (anim==15 || anim==16) { 
                    dst2.x=dst.x+10; dst2.y=dst.y-4;src2.x = 56; src2.y = 0;
                    if (epee > 3) {dst2.y-=2; src2.x = 80; src2.y = 0;}
                }
                if (anim==17 || anim==18) { 
                    dst2.x=dst.x+16; dst2.y=dst.y+8;src2.x = 68; src2.y = 36;
                    if (epee > 3) {dst2.y--; src2.x = 94; src2.y = 42;}
                }
                if (anim==19 || anim==20) { 
                    dst2.x=dst.x+16; dst2.y=dst.y+16;src2.x = 68; src2.y = 24;
                    if (epee > 3) {src2.x = 94; src2.y = 28;}
                }
                
            
                if (anim>=13 && anim <=16) {
                    SDL_BlitSurface(imageEpee, &src2, gpScreen, &dst2);
                    SDL_BlitSurface(image, &src, gpScreen, &dst);
                }else {
                    SDL_BlitSurface(image, &src, gpScreen, &dst);
                    SDL_BlitSurface(imageEpee, &src2, gpScreen, &dst2);
                }
            }
            break;
        case E :
            if (anim==0 || anim==5 || (anim>=21 && anim<=26)) { src.x = 112; src.y = 144;}
            if (anim>=1 && anim<=4) { src.x = 112; src.y = 168;}
            if (anim>=6 && anim<=8) { src.x = 80; src.y = 120;}
            if (anim>=9 && anim<=12) { src.x = 80; src.y = 144;}
            if (anim>=13 && anim<=16) { src.x = 80; src.y = 168;}
            if (anim>=17 && anim<=20) { src.x = 64; src.y = 168;}
            if (anim > 26) {anim2=anim; anim=0; drawImmoMarche(gpScreen); anim=anim2;}
            else {
            
                //Epée
                if (anim==0 || anim==5 || (anim>=21 && anim<=26)) { 
                    dst2.x=dst.x-8; dst2.y=dst.y+6;src2.x = 56; src2.y = 36;
                    if (epee > 3) {dst2.x-=2; dst2.y--; src2.x = 80; src2.y = 42;}
                }
                if (anim>=1 && anim<=4) { 
                    dst2.x=dst.x-8; dst2.y=dst.y+12;src2.x = 56; src2.y = 24;
                    if (epee > 3) {dst2.x-=2; src2.x = 80; src2.y = 28;}
                }
                if (anim==6) { 
                    dst2.x=dst.x+1; dst2.y=dst.y-4;src2.x = 68; src2.y = 0;
                    if (epee > 3) {dst2.x--; dst2.y-=2; src2.x = 94; src2.y = 0;}
                }
                if (anim==7 || anim==8) { 
                    dst2.x=dst.x+10; dst2.y=dst.y-4;src2.x = 56; src2.y = 0;
                    if (epee > 3) {dst2.y-=2; src2.x = 80; src2.y = 0;}
                }
                if (anim==9 || anim==10) { 
                    dst2.x=dst.x+16; dst2.y=dst.y+8;src2.x = 68; src2.y = 36;
                    if (epee > 3) {dst2.y--; src2.x = 94; src2.y = 42;}
                }
                if (anim==11 || anim==12) { 
                    dst2.x=dst.x+16; dst2.y=dst.y+16;src2.x = 68; src2.y = 24;
                    if (epee > 3) {src2.x = 94; src2.y = 28;}
                }
                if (anim==13 || anim==14) { 
                    dst2.x=dst.x+7; dst2.y=dst.y+21;src2.x = 56; src2.y = 12;
                    if (epee > 3) {src2.x = 80; src2.y = 14;}
                }
                if (anim==15 || anim==16) { 
                    dst2.x=dst.x-1; dst2.y=dst.y+21;src2.x = 68; src2.y = 12;
                    if (epee > 3) {dst2.x--; src2.x = 94; src2.y = 14;}
                }
                if (anim==17 || anim==18) { 
                    dst2.x=dst.x-12; dst2.y=dst.y+15;src2.x = 56; src2.y = 24;
                    if (epee > 3) {dst2.x-=2; src2.x = 80; src2.y = 28;}
                }
                if (anim==19 || anim==20) { 
                    dst2.x=dst.x-12; dst2.y=dst.y+8;src2.x = 56; src2.y = 36;
                    if (epee > 3) {dst2.x-=2; dst2.y--; src2.x = 80; src2.y = 42;}
                }
                
                if (anim>=6 && anim <=8) {
                    SDL_BlitSurface(imageEpee, &src2, gpScreen, &dst2);
                    SDL_BlitSurface(image, &src, gpScreen, &dst);
                }else {
                    SDL_BlitSurface(image, &src, gpScreen, &dst);
                    SDL_BlitSurface(imageEpee, &src2, gpScreen, &dst2);
                }
            }
            break;
    }
    
    // lumière
    switch (anim) {
        case 0 :
            src.x = 5; src.y = 16; src.w = 5; src.h = 5;
            dir=direction;
            if (direction==N) { dst.x += 7; dst.y -= 8;}
            if (direction==S) { dst.x += 6; dst.y += 30;}
            if (direction==O) { dst.x -= 12; dst.y += 17;}
            if (direction==E) { dst.x += 23; dst.y += 15;}
            SDL_BlitSurface(imageSpin, &src, gpScreen, &dst);
            gpJeu->testEpee(dst.x+phg[0],dst.y+phg[1],src.w,src.h,EPEE_SPIN,epee*2,dir);
            break;
        case 1 :
            src.x = 33; src.y = 15; src.w = 1; src.h = 1;
            dir=direction;
            if (direction==N) { dst.x += 9; dst.y -= 6; dir=N;}
            if (direction==S) { dst.x += 8; dst.y += 32; dir=S;}
            if (direction==O) { dst.x -= 10; dst.y += 19; dir=O;}
            if (direction==E) { dst.x += 25; dst.y += 17; dir=E;}
            SDL_BlitSurface(imageSpin, &src, gpScreen, &dst);
            gpJeu->testEpee(dst.x+phg[0],dst.y+phg[1],src.w,src.h,EPEE_SPIN,epee*2,dir);
            
            src.x = 0; src.y = 0; src.w = 8; src.h = 8;
            if (direction==N) { dst.x -= 3; dst.y += 35; dir=S;}
            if (direction==S) { dst.x -= 5; dst.y -= 44; dir=N;}
            if (direction==O) { dst.x += 32; dst.y -= 13; dir=E;}
            if (direction==E) { dst.x -= 38; dst.y -= 1; dir=O;}
            SDL_BlitSurface(imageSpin, &src, gpScreen, &dst);
            gpJeu->testEpee(dst.x+phg[0],dst.y+phg[1],src.w,src.h,EPEE_SPIN,epee*2,dir);
            break;
        case 2 :
            src.x = 33; src.y = 15; src.w = 1; src.h = 1;
            dir=direction;
            if (direction==N) { dst.x += 9; dst.y -= 6;}
            if (direction==S) { dst.x += 8; dst.y += 32;}
            if (direction==O) { dst.x -= 10; dst.y += 19;}
            if (direction==E) { dst.x += 25; dst.y += 17;}
            SDL_BlitSurface(imageSpin, &src, gpScreen, &dst);
            gpJeu->testEpee(dst.x+phg[0],dst.y+phg[1],src.w,src.h,EPEE_SPIN,epee*2,dir);
            
            src.x = 8; src.y = 0; src.w = 14; src.h = 14;
            if (direction==N) { dst.x -= 6; dst.y += 32; dir=S;}
            if (direction==S) { dst.x -= 8; dst.y -= 47; dir=N;}
            if (direction==O) { dst.x += 29; dst.y -= 16; dir=E;}
            if (direction==E) { dst.x -= 41; dst.y -= 4; dir=O;}
            SDL_BlitSurface(imageSpin, &src, gpScreen, &dst);
            gpJeu->testEpee(dst.x+phg[0],dst.y+phg[1],src.w,src.h,EPEE_SPIN,epee*2,dir);
            break;
        case 3 :
            src.x = 8; src.y = 0; src.w = 14; src.h = 14;
            if (direction==N) { dst.x += 3; dst.y += 26; dir=S;}
            if (direction==S) { dst.y -= 15; dir=N;}
            if (direction==O) { dst.x += 19; dst.y += 3; dir=E;}
            if (direction==E) { dst.x -= 15; dst.y += 13; dir=O;}
            SDL_BlitSurface(imageSpin, &src, gpScreen, &dst);
            gpJeu->testEpee(dst.x+phg[0],dst.y+phg[1],src.w,src.h,EPEE_SPIN,epee*2,dir);
            break;
        case 4 :
        case 5 :
            src.x = 22; src.y = 0; src.w = 14; src.h = 14;
            if (direction==N) { dst.x += 3; dst.y += 26; dir=S;}
            if (direction==S) { dst.y -= 15; dir=N;}
            if (direction==O) { dst.x += 19; dst.y += 3; dir=E;}
            if (direction==E) { dst.x -= 15; dst.y += 13; dir=O;}
            SDL_BlitSurface(imageSpin, &src, gpScreen, &dst);
            gpJeu->testEpee(dst.x+phg[0],dst.y+phg[1],src.w,src.h,EPEE_SPIN,epee*2,dir);
            break;
        case 6 :
            src.x = 36; src.y = 0; src.w = 16; src.h = 16;
            if (direction==N) { dst.x += 2; dst.y += 25; dir=S;}
            if (direction==S) { dst.x -= 1; dst.y -= 15; dir=N;}
            if (direction==O) { dst.x += 19; dst.y += 2; dir=E;}
            if (direction==E) { dst.x -= 17; dst.y += 12; dir=O;}
            SDL_BlitSurface(imageSpin, &src, gpScreen, &dst);
            gpJeu->testEpee(dst.x+phg[0],dst.y+phg[1],src.w,src.h,EPEE_SPIN,epee*2,dir);
            break;
        case 7 :
            src.x = 0; src.y = 8; src.w = 8; src.h = 8;
            if (direction==N) { dst.x -= 5; dst.y += 27; dir=S;}
            if (direction==S) { dst.x += 17; dst.y -= 6; dir=N;}
            if (direction==O) { dst.x += 20; dst.y += 19; dir=E;}
            if (direction==E) { dst.x -= 13; dst.y -= 1; dir=O;}
            SDL_BlitSurface(imageSpin, &src, gpScreen, &dst);
            gpJeu->testEpee(dst.x+phg[0],dst.y+phg[1],src.w,src.h,EPEE_SPIN,epee*2,dir);
            break;
        case 8 :
            src.x = 0; src.y = 8; src.w = 8; src.h = 8;
            if (direction==N) { dst.x -= 11; dst.y += 22; dir=S;}
            if (direction==S) { dst.x += 22; dir=N;}
            if (direction==O) { dst.x += 14; dst.y += 25; dir=E;}
            if (direction==E) { dst.x -= 8; dst.y -= 7; dir=O;}
            SDL_BlitSurface(imageSpin, &src, gpScreen, &dst);
            gpJeu->testEpee(dst.x+phg[0],dst.y+phg[1],src.w,src.h,EPEE_SPIN,epee*2,dir);
            
            src.x = 25; src.y = 14; src.w = 7; src.h = 7; 
            if (direction==N) { dst.x += 7; dst.y += 6;}
            if (direction==S) { dst.x -= 7; dst.y -= 7;}
            if (direction==O) { dst.x += 7; dst.y -= 7;}
            if (direction==E) { dst.x -= 5; dst.y += 8;}
            SDL_BlitSurface(imageSpin, &src, gpScreen, &dst);
            gpJeu->testEpee(dst.x+phg[0],dst.y+phg[1],src.w,src.h,EPEE_SPIN,epee*2,dir);
            break;
        case 9 :
            src.x = 10; src.y = 14; src.w = 8; src.h = 8;
            if (direction==N) { dst.x -= 14; dst.y += 15; dir=O;}
            if (direction==S) { dst.x += 23; dst.y += 7; dir=E;}
            if (direction==O) { dst.x += 8; dst.y += 28; dir=S;}
            if (direction==E) { dst.x -= 2; dst.y -= 10; dir=N;}
            SDL_BlitSurface(imageSpin, &src, gpScreen, &dst);
            gpJeu->testEpee(dst.x+phg[0],dst.y+phg[1],src.w,src.h,EPEE_SPIN,epee*2,dir);
            
            src.x = 18; src.y = 14; src.w = 7; src.h = 7;
            if (direction==N) { dst.x += 5; dst.y += 9; dir=S;}
            if (direction==S) { dst.x -= 3; dst.y -= 8; dir=N;}
            if (direction==O) { dst.x += 9; dst.y -= 4; dir=E;}
            if (direction==E) { dst.x -= 8; dst.y += 6; dir=O;}
            SDL_BlitSurface(imageSpin, &src, gpScreen, &dst);
            gpJeu->testEpee(dst.x+phg[0],dst.y+phg[1],src.w,src.h,EPEE_SPIN,epee*2,dir);
            
            src.x = 5; src.y = 16; src.w = 5; src.h = 5;
            if (direction==N) { dst.x += 9; dst.y += 6;}
            if (direction==S) { dst.x -= 7; dst.y -= 6;}
            if (direction==O) { dst.x += 6; dst.y -= 6;}
            if (direction==E) { dst.x -= 4; dst.y += 9;}
            SDL_BlitSurface(imageSpin, &src, gpScreen, &dst);
            gpJeu->testEpee(dst.x+phg[0],dst.y+phg[1],src.w,src.h,EPEE_SPIN,epee*2,dir);
            break;
        case 10 :
            src.x = 0; src.y = 8; src.w = 8; src.h = 8;
            if (direction==N) { dst.x -= 15; dst.y += 7; dir=O;}
            if (direction==S) { dst.x += 22; dst.y += 15; dir=E;}
            if (direction==O) { dst.y += 28; dir=S;}
            if (direction==E) { dst.x += 6; dst.y -= 11; dir=N;}
            SDL_BlitSurface(imageSpin, &src, gpScreen, &dst);
            gpJeu->testEpee(dst.x+phg[0],dst.y+phg[1],src.w,src.h,EPEE_SPIN,epee*2,dir);
                
            src.x = 25; src.y = 14; src.w = 7; src.h = 7;
            if (direction==N) { dst.x += 11; dst.y += 11;}
            if (direction==S) { dst.x += 1; dst.y -= 9;}
            if (direction==O) dst.x += 10;
            if (direction==E) { dst.x -= 10; dst.y += 3;}
            SDL_BlitSurface(imageSpin, &src, gpScreen, &dst);
            gpJeu->testEpee(dst.x+phg[0],dst.y+phg[1],src.w,src.h,EPEE_SPIN,epee*2,dir);
            
            src.x = 0; src.y = 16; src.w = 5; src.h = 5;
            if (direction==N) { dst.x += 6; dst.y += 8; dir=S;}
            if (direction==S) { dst.x -= 4; dst.y -= 7; dir=N;}
            if (direction==O) { dst.x += 9; dst.y -= 4; dir=E;}
            if (direction==E) { dst.x -= 6; dst.y += 6; dir=O;}
            SDL_BlitSurface(imageSpin, &src, gpScreen, &dst);
            gpJeu->testEpee(dst.x+phg[0],dst.y+phg[1],src.w,src.h,EPEE_SPIN,epee*2,dir);
            
            src.x = 33; src.y = 15; src.w = 1; src.h = 1;
            if (direction==N) { dst.x += 11; dst.y += 6;}
            if (direction==S) { dst.x -= 5; dst.y -= 4;}
            if (direction==O) { dst.x += 7; dst.y -= 7;}
            if (direction==E) { dst.x -= 3; dst.y += 10;}
            SDL_BlitSurface(imageSpin, &src, gpScreen, &dst);
            gpJeu->testEpee(dst.x+phg[0],dst.y+phg[1],src.w,src.h,EPEE_SPIN,epee*2,dir);
            break;
        case 11 :
            src.x = 0; src.y = 8; src.w = 8; src.h = 8;
            if (direction==N) { dst.x -= 13; dir=O;}
            if (direction==S) { dst.x += 18; dst.y += 22; dir=E;}
            if (direction==O) { dst.x -= 7; dst.y += 25; dir=S;}
            if (direction==E) { dst.x += 13; dst.y -= 9; dir=N;}
            SDL_BlitSurface(imageSpin, &src, gpScreen, &dst);
            gpJeu->testEpee(dst.x+phg[0],dst.y+phg[1],src.w,src.h,EPEE_SPIN,epee*2,dir);
            
            src.x = 25; src.y = 14; src.w = 7; src.h = 7;
            if (direction==N) { dst.x -= 2; dst.y += 10;}
            if (direction==S) { dst.x += 5; dst.y -= 8;}
            if (direction==O) { dst.x += 9; dst.y += 4;}
            if (direction==E) { dst.x -= 9; dst.y -= 1;}
            SDL_BlitSurface(imageSpin, &src, gpScreen, &dst);
            gpJeu->testEpee(dst.x+phg[0],dst.y+phg[1],src.w,src.h,EPEE_SPIN,epee*2,dir);
            
            src.x = 0; src.y = 16; src.w = 5; src.h = 5;
            if (direction==N) { dst.x += 3; dst.y += 10;}
            if (direction==S) { dst.y -= 9;}
            if (direction==O) { dst.x += 11; dst.y -= 1;}
            if (direction==E) { dst.x -= 8; dst.y += 3;}
            SDL_BlitSurface(imageSpin, &src, gpScreen, &dst);
            gpJeu->testEpee(dst.x+phg[0],dst.y+phg[1],src.w,src.h,EPEE_SPIN,epee*2,dir);
            
            src.x = 33; src.y = 15; src.w = 1; src.h = 1;
            if (direction==N) { dst.x += 9; dst.y += 9; dir=S;}
            if (direction==S) { dst.x -= 3; dst.y -= 7; dir=N;}
            if (direction==O) { dst.x += 9; dst.y -= 4; dir=E;}
            if (direction==E) { dst.x -= 6; dst.y += 8; dir=O;}
            SDL_BlitSurface(imageSpin, &src, gpScreen, &dst);
            gpJeu->testEpee(dst.x+phg[0],dst.y+phg[1],src.w,src.h,EPEE_SPIN,epee*2,dir);
            break;
        case 12 :
            src.x = 10; src.y = 14; src.w = 8; src.h = 8;
            if (direction==N) { dst.x -= 8; dst.y -= 6; dir=O;}
            if (direction==S) { dst.x += 12; dst.y += 27; dir=E;}
            if (direction==O) { dst.x -= 13; dst.y += 19; dir=S;}
            if (direction==E) { dst.x += 19; dst.y -= 4; dir=N;}
            SDL_BlitSurface(imageSpin, &src, gpScreen, &dst);
            gpJeu->testEpee(dst.x+phg[0],dst.y+phg[1],src.w,src.h,EPEE_SPIN,epee*2,dir);
                
            src.x = 18; src.y = 14; src.w = 7; src.h = 7;
            if (direction==N) { dst.x -= 5; dst.y += 8;}
            if (direction==S) { dst.x += 8; dst.y -= 6;}
            if (direction==O) { dst.x += 8; dst.y += 8;}
            if (direction==E) { dst.x -= 7; dst.y += 4;}
            SDL_BlitSurface(imageSpin, &src, gpScreen, &dst);
            gpJeu->testEpee(dst.x+phg[0],dst.y+phg[1],src.w,src.h,EPEE_SPIN,epee*2,dir);
                
            src.x = 5; src.y = 16; src.w = 5; src.h = 5;
            if (direction==N) { dst.x -= 1; dst.y += 11;}
            if (direction==S) { dst.x += 4; dst.y -= 8;}
            if (direction==O) { dst.x += 10; dst.y += 3;}
            if (direction==E) { dst.x -= 9; dst.y -= 1;}
            SDL_BlitSurface(imageSpin, &src, gpScreen, &dst);
            gpJeu->testEpee(dst.x+phg[0],dst.y+phg[1],src.w,src.h,EPEE_SPIN,epee*2,dir);
                
            src.x = 33; src.y = 15; src.w = 1; src.h = 1;
            if (direction==N) { dst.x += 5; dst.y += 10;}
            if (direction==S) { dst.y -= 9;}
            if (direction==O) { dst.x += 11; dst.y -= 1;}
            if (direction==E) { dst.x -= 7; dst.y += 4;}
            SDL_BlitSurface(imageSpin, &src, gpScreen, &dst);
            gpJeu->testEpee(dst.x+phg[0],dst.y+phg[1],src.w,src.h,EPEE_SPIN,epee*2,dir);
            break;
        case 13 :
            src.x = 0; src.y = 8; src.w = 8; src.h = 8;
            dir=direction;
            if (direction==N) { dst.x -= 2; dst.y -= 10;}
            if (direction==S) { dst.x += 6; dst.y += 29;}
            if (direction==O) { dst.x -= 15; dst.y += 12;}
            if (direction==E) { dst.x += 22; dst.y += 3;}
            SDL_BlitSurface(imageSpin, &src, gpScreen, &dst);
            gpJeu->testEpee(dst.x+phg[0],dst.y+phg[1],src.w,src.h,EPEE_SPIN,epee*2,dir);
                
            src.x = 25; src.y = 14; src.w = 7; src.h = 7;
            if (direction==N) { dst.x -= 8; dst.y += 6; dir=O;}
            if (direction==S) { dst.x += 9; dst.y -= 2; dir=E;}
            if (direction==O) { dst.x += 5; dst.y += 10; dir=S;}
            if (direction==E) { dst.x -= 5; dst.y -= 7; dir=N;}
            SDL_BlitSurface(imageSpin, &src, gpScreen, &dst);
            gpJeu->testEpee(dst.x+phg[0],dst.y+phg[1],src.w,src.h,EPEE_SPIN,epee*2,dir);
                
            src.x = 0; src.y = 16; src.w = 5; src.h = 5;
            if (direction==N) { dst.x -= 4; dst.y += 9;}
            if (direction==S) { dst.x += 8; dst.y -= 7;}
            if (direction==O) { dst.x += 8; dst.y += 6;}
            if (direction==E) { dst.x -= 7; dst.y -= 4;}
            SDL_BlitSurface(imageSpin, &src, gpScreen, &dst);
            gpJeu->testEpee(dst.x+phg[0],dst.y+phg[1],src.w,src.h,EPEE_SPIN,epee*2,dir);
                
            src.x = 33; src.y = 15; src.w = 1; src.h = 1;
            if (direction==N) { dst.x += 1; dst.y += 11;}
            if (direction==S) { dst.x += 4; dst.y -= 8;}
            if (direction==O) { dst.x += 12; dst.y += 3;}
            if (direction==E) dst.x -= 8;
            SDL_BlitSurface(imageSpin, &src, gpScreen, &dst);
            gpJeu->testEpee(dst.x+phg[0],dst.y+phg[1],src.w,src.h,EPEE_SPIN,epee*2,dir);
            break;
        case 14 :
            src.x = 0; src.y = 8; src.w = 8; src.h = 8;
            dir=direction;
            if (direction==N) { dst.x += 6; dst.y -= 11;}
            if (direction==S) { dst.x -= 2; dst.y += 28;}
            if (direction==O) { dst.x -= 15; dst.y += 4;}
            if (direction==E) { dst.x += 23; dst.y += 11;}
            SDL_BlitSurface(imageSpin, &src, gpScreen, &dst);
            gpJeu->testEpee(dst.x+phg[0],dst.y+phg[1],src.w,src.h,EPEE_SPIN,epee*2,dir);
                
            src.x = 25; src.y = 14; src.w = 7; src.h = 7;
            if (direction==N) { dst.x -= 10; dst.y += 3;}
            if (direction==S) { dst.x += 10; dst.y += 2;}
            if (direction==O) { dst.x += 1; dst.y += 11;}
            if (direction==E) { dst.x -= 2; dst.y -= 9;}
            SDL_BlitSurface(imageSpin, &src, gpScreen, &dst);
            gpJeu->testEpee(dst.x+phg[0],dst.y+phg[1],src.w,src.h,EPEE_SPIN,epee*2,dir);
                
            src.x = 0; src.y = 16; src.w = 5; src.h = 5;
            if (direction==N) { dst.x -= 6; dst.y += 6; dir=O;}
            if (direction==S) { dst.x += 10; dst.y -= 4; dir=E;}
            if (direction==O) { dst.x += 6; dst.y += 9; dir=S;}
            if (direction==E) { dst.x -= 4; dst.y += 6; dir=N;}
            SDL_BlitSurface(imageSpin, &src, gpScreen, &dst);
            gpJeu->testEpee(dst.x+phg[0],dst.y+phg[1],src.w,src.h,EPEE_SPIN,epee*2,dir);
                
            src.x = 33; src.y = 15; src.w = 1; src.h = 1;
            if (direction==N) { dst.x -= 3; dst.y += 10;}
            if (direction==S) { dst.x += 7; dst.y -= 6;}
            if (direction==O) { dst.x += 10; dst.y += 6;}
            if (direction==E) { dst.x -= 7; dst.y -= 3;}
            SDL_BlitSurface(imageSpin, &src, gpScreen, &dst);
            gpJeu->testEpee(dst.x+phg[0],dst.y+phg[1],src.w,src.h,EPEE_SPIN,epee*2,dir);
            break;
        case 15 :
            dir=direction;
            src.x = 10; src.y = 14; src.w = 8; src.h = 8;
            if (direction==N) { dst.x += 13; dst.y -= 9;}
            if (direction==S) { dst.x -= 9; dst.y += 24;}
            if (direction==O) { dst.x -= 12; dst.y -= 3;}
            if (direction==E) { dst.x += 21; dst.y += 18;}
            SDL_BlitSurface(imageSpin, &src, gpScreen, &dst);
            gpJeu->testEpee(dst.x+phg[0],dst.y+phg[1],src.w,src.h,EPEE_SPIN,epee*2,dir);
            
            src.x = 18; src.y = 14; src.w = 7; src.h = 7;
            if (direction==N) { dst.x -= 9; dst.y -= 1;}
            if (direction==S) { dst.x += 9; dst.y += 6;}
            if (direction==O) { dst.x -= 3; dst.y += 10;}
            if (direction==E) { dst.x += 2; dst.y -= 8;}
            SDL_BlitSurface(imageSpin, &src, gpScreen, &dst);
            gpJeu->testEpee(dst.x+phg[0],dst.y+phg[1],src.w,src.h,EPEE_SPIN,epee*2,dir);
            
            src.x = 5; src.y = 16; src.w = 5; src.h = 5;
            if (direction==N) { dst.x -= 8; dst.y += 3;}
            if (direction==S) dst.x += 11;
            if (direction==O) { dst.x += 3; dst.y += 11;}
            if (direction==E) { dst.x -= 1; dst.y -= 8;}
            SDL_BlitSurface(imageSpin, &src, gpScreen, &dst);
            gpJeu->testEpee(dst.x+phg[0],dst.y+phg[1],src.w,src.h,EPEE_SPIN,epee*2,dir);
            
            src.x = 33; src.y = 15; src.w = 1; src.h = 1;
            if (direction==N) { dst.x -= 6; dst.y += 8; dir=O;}
            if (direction==S) { dst.x += 10; dst.y -= 4; dir=E;}
            if (direction==O) { dst.x += 7; dst.y += 8; dir=S;}
            if (direction==E) { dst.x -= 5; dst.y -= 6; dir=N;}
            SDL_BlitSurface(imageSpin, &src, gpScreen, &dst);
            gpJeu->testEpee(dst.x+phg[0],dst.y+phg[1],src.w,src.h,EPEE_SPIN,epee*2,dir);
            
            src.x = 18; src.y = 14; src.w = 7; src.h = 7;
            if (direction==N) { dst.x += 18; dst.y += 29;}
            if (direction==S) { dst.x -= 17; dst.y -= 36;}
            if (direction==O) { dst.x += 30; dst.y -= 19;}
            if (direction==E) { dst.x -= 34; dst.y += 19;}
            SDL_BlitSurface(imageSpin, &src, gpScreen, &dst);
            break;
        case 16 :
            src.x = 0; src.y = 8; src.w = 8; src.h = 8;
            dir=direction;
            if (direction==N) { dst.x += 19; dst.y -= 4;}
            if (direction==S) { dst.x -= 14; dst.y += 18;}
            if (direction==O) { dst.x -= 6; dst.y -= 9;}
            if (direction==E) { dst.x += 16; dst.y += 24;}
            SDL_BlitSurface(imageSpin, &src, gpScreen, &dst);
            gpJeu->testEpee(dst.x+phg[0],dst.y+phg[1],src.w,src.h,EPEE_SPIN,epee*2,dir);
            
            src.x = 25; src.y = 14; src.w = 7; src.h = 7;
            if (direction==N) { dst.x -= 7; dst.y -= 4;}
            if (direction==S) { dst.x += 7; dst.y += 9;}
            if (direction==O) { dst.x -= 7; dst.y += 9;}
            if (direction==E) { dst.x += 5; dst.y -= 6;}
            SDL_BlitSurface(imageSpin, &src, gpScreen, &dst);
            gpJeu->testEpee(dst.x+phg[0],dst.y+phg[1],src.w,src.h,EPEE_SPIN,epee*2,dir);
            
            src.x = 0; src.y = 16; src.w = 5; src.h = 5;
            if (direction==N) { dst.x -= 9; dst.y -= 1;}
            if (direction==S) { dst.x += 10; dst.y += 4;}
            if (direction==O) { dst.x -= 1; dst.y += 10;}
            if (direction==E) { dst.x += 3; dst.y -= 9;}
            SDL_BlitSurface(imageSpin, &src, gpScreen, &dst);
            gpJeu->testEpee(dst.x+phg[0],dst.y+phg[1],src.w,src.h,EPEE_SPIN,epee*2,dir);
            
            src.x = 33; src.y = 15; src.w = 1; src.h = 1;
            if (direction==N) { dst.x -= 7; dst.y += 4;}
            if (direction==S) { dst.x += 12; dst.y -= 1;}
            if (direction==O) { dst.x += 4; dst.y += 10;}
            if (direction==E) { dst.x -= 1; dst.y -= 8;}
            SDL_BlitSurface(imageSpin, &src, gpScreen, &dst);
            gpJeu->testEpee(dst.x+phg[0],dst.y+phg[1],src.w,src.h,EPEE_SPIN,epee*2,dir);
            
            src.x = 5; src.y = 16; src.w = 5; src.h = 5;
            if (direction==N) { dst.x += 13; dst.y += 36;}
            if (direction==S) { dst.x -= 10; dst.y -= 39;}
            if (direction==O) { dst.x += 36; dst.y -= 12;}
            if (direction==E) { dst.x -= 39; dst.y += 14;}
            SDL_BlitSurface(imageSpin, &src, gpScreen, &dst);
            break;
        case 17 :
            src.x = 0; src.y = 8; src.w = 8; src.h = 8;
            dir=direction;
            if (direction==N) { dst.x += 22; dst.y += 3;}
            if (direction==S) { dst.x -= 15; dst.y += 11;}
            if (direction==O) dst.y -= 11;
            if (direction==E) { dst.x += 10; dst.y += 28;}
            SDL_BlitSurface(imageSpin, &src, gpScreen, &dst);
            gpJeu->testEpee(dst.x+phg[0],dst.y+phg[1],src.w,src.h,EPEE_SPIN,epee*2,dir);
            
            src.x = 25; src.y = 14; src.w = 7; src.h = 7;
            if (direction==N) { dst.x -= 5; dst.y -= 7;}
            if (direction==S) { dst.x += 3; dst.y += 10;}
            if (direction==O) { dst.x -= 9; dst.y += 6;}
            if (direction==E) { dst.x += 8; dst.y -= 4;}
            SDL_BlitSurface(imageSpin, &src, gpScreen, &dst);
            gpJeu->testEpee(dst.x+phg[0],dst.y+phg[1],src.w,src.h,EPEE_SPIN,epee*2,dir);
            
            src.x = 0; src.y = 16; src.w = 5; src.h = 5;
            if (direction==N) { dst.x -= 7; dst.y -= 4;}
            if (direction==S) { dst.x += 9; dst.y += 8;}
            if (direction==O) { dst.x -= 4; dst.y += 8;}
            if (direction==E) { dst.x += 6; dst.y -= 7;}
            SDL_BlitSurface(imageSpin, &src, gpScreen, &dst);
            gpJeu->testEpee(dst.x+phg[0],dst.y+phg[1],src.w,src.h,EPEE_SPIN,epee*2,dir);
            
            src.x = 33; src.y = 15; src.w = 1; src.h = 1;
            if (direction==N) { dst.x -= 8; dir=E;}
            if (direction==S) { dst.x += 11; dst.y += 3; dir=O;}
            if (direction==O) { dst.y += 11; dir=N;}
            if (direction==E) { dst.x += 3; dst.y -= 9; dir=S;}
            SDL_BlitSurface(imageSpin, &src, gpScreen, &dst);
            gpJeu->testEpee(dst.x+phg[0],dst.y+phg[1],src.w,src.h,EPEE_SPIN,epee*2,dir);
            
            if (direction==N) { dst.x += 8; dst.y += 40;}
            if (direction==S) dst.y -= 40;
            if (direction==O) { dst.x += 40; dst.y -= 4;}
            if (direction==E) { dst.x -= 40; dst.y += 8;}
            SDL_BlitSurface(imageSpin, &src, gpScreen, &dst);
            break;
        case 18 :
            src.x = 10; src.y = 14; src.w = 8; src.h = 8;
            dir=direction;
            if (direction==N) { dst.x += 23; dst.y += 11;}
            if (direction==S) { dst.x -= 14; dst.y += 3;}
            if (direction==O) { dst.x += 8; dst.y -= 11;}
            if (direction==E) { dst.x += 2; dst.y += 29;}
            SDL_BlitSurface(imageSpin, &src, gpScreen, &dst);
            gpJeu->testEpee(dst.x+phg[0],dst.y+phg[1],src.w,src.h,EPEE_SPIN,epee*2,dir);
            
            src.x = 18; src.y = 14; src.w = 7; src.h = 7;
            if (direction==N) { dst.x -= 2; dst.y -= 9;}
            if (direction==S) { dst.x -= 1; dst.y += 11;}
            if (direction==O) { dst.x -= 10; dst.y += 2;}
            if (direction==E) { dst.x += 10; dst.y -= 1;}
            SDL_BlitSurface(imageSpin, &src, gpScreen, &dst);
            gpJeu->testEpee(dst.x+phg[0],dst.y+phg[1],src.w,src.h,EPEE_SPIN,epee*2,dir);
            
            src.x = 5; src.y = 16; src.w = 5; src.h = 5;
            if (direction==N) { dst.x -= 4; dst.y -= 6; dir=E;}
            if (direction==S) { dst.x += 6; dst.y += 10; dir=O;}
            if (direction==O) { dst.x -= 7; dst.y += 6; dir=N;}
            if (direction==E) { dst.x += 8; dst.y -= 4; dir=S;}
            SDL_BlitSurface(imageSpin, &src, gpScreen, &dst);
            gpJeu->testEpee(dst.x+phg[0],dst.y+phg[1],src.w,src.h,EPEE_SPIN,epee*2,dir);
            
            src.x = 33; src.y = 15; src.w = 1; src.h = 1;
            if (direction==N) { dst.x -= 7; dst.y -= 4;}
            if (direction==S) { dst.x += 9; dst.y += 6;}
            if (direction==O) { dst.x -= 3; dst.y += 9;}
            if (direction==E) { dst.x += 7; dst.y -= 8;}
            SDL_BlitSurface(imageSpin, &src, gpScreen, &dst);
            gpJeu->testEpee(dst.x+phg[0],dst.y+phg[1],src.w,src.h,EPEE_SPIN,epee*2,dir);
            break;
        case 19 :
            src.x = 0; src.y = 8; src.w = 8; src.h = 8;
            dir=direction;
            if (direction==N) { dst.x += 21; dst.y += 18;}
            if (direction==S) { dst.x -= 10; dst.y -= 4;}
            if (direction==O) { dst.x += 15; dst.y -= 8;}
            if (direction==E) { dst.x -= 5; dst.y += 27;}
            SDL_BlitSurface(imageSpin, &src, gpScreen, &dst);
            gpJeu->testEpee(dst.x+phg[0],dst.y+phg[1],src.w,src.h,EPEE_SPIN,epee*2,dir);
            
            src.x = 25; src.y = 14; src.w = 7; src.h = 7;
            if (direction==N) { dst.x += 2; dst.y -= 8; dir=E;}
            if (direction==S) { dst.x -= 5; dst.y += 10; dir=O;}
            if (direction==O) { dst.x -= 9; dst.y -= 2; dir=N;}
            if (direction==E) { dst.x += 9; dst.y += 3; dir=S;}
            SDL_BlitSurface(imageSpin, &src, gpScreen, &dst);
            gpJeu->testEpee(dst.x+phg[0],dst.y+phg[1],src.w,src.h,EPEE_SPIN,epee*2,dir);
            
            src.x = 0; src.y = 16; src.w = 5; src.h = 5;
            if (direction==N) { dst.x -= 1; dst.y -= 8;}
            if (direction==S) { dst.x += 2; dst.y += 11;}
            if (direction==O) { dst.x -= 9; dst.y += 3;}
            if (direction==E) { dst.x += 10; dst.y -= 1;}
            SDL_BlitSurface(imageSpin, &src, gpScreen, &dst);
            gpJeu->testEpee(dst.x+phg[0],dst.y+phg[1],src.w,src.h,EPEE_SPIN,epee*2,dir);
            
            src.x = 33; src.y = 15; src.w = 1; src.h = 1;
            if (direction==N) { dst.x -= 5; dst.y -= 7;}
            if (direction==S) { dst.x += 7; dst.y += 9;}
            if (direction==O) { dst.x -= 5; dst.y += 6;}
            if (direction==E) { dst.x += 10; dst.y -= 6;}
            SDL_BlitSurface(imageSpin, &src, gpScreen, &dst);
            gpJeu->testEpee(dst.x+phg[0],dst.y+phg[1],src.w,src.h,EPEE_SPIN,epee*2,dir);
            
            src.x = 18; src.y = 14; src.w = 7; src.h = 7;
            if (direction==N) { dst.x -= 27; dst.y += 30;}
            if (direction==S) { dst.x += 26; dst.y -= 29;}
            if (direction==O) { dst.x += 26; dst.y += 25;}
            if (direction==E) { dst.x -= 35; dst.y -= 26;}
            SDL_BlitSurface(imageSpin, &src, gpScreen, &dst);
            break;
        case 20 :
            src.x = 0; src.y = 8; src.w = 8; src.h = 8;
            if (direction==N) { dst.x += 16; dst.y += 24; dir=E;}
            if (direction==S) { dst.x -= 4; dst.y -= 9; dir=O;}
            if (direction==O) { dst.x += 21; dst.y -= 2; dir=N;}
            if (direction==E) { dst.x -= 11; dst.y += 22; dir=S;}
            SDL_BlitSurface(imageSpin, &src, gpScreen, &dst);
            gpJeu->testEpee(dst.x+phg[0],dst.y+phg[1],src.w,src.h,EPEE_SPIN,epee*2,dir);
            
            src.x = 25; src.y = 14; src.w = 7; src.h = 7;
            if (direction==N) { dst.x += 5; dst.y -= 6;}
            if (direction==S) { dst.x -= 8; dst.y += 8;}
            if (direction==O) { dst.x -= 8; dst.y -= 6;}
            if (direction==E) { dst.x += 7; dst.y += 6;}
            SDL_BlitSurface(imageSpin, &src, gpScreen, &dst);
            gpJeu->testEpee(dst.x+phg[0],dst.y+phg[1],src.w,src.h,EPEE_SPIN,epee*2,dir);
            
            src.x = 0; src.y = 16; src.w = 5; src.h = 5;
            if (direction==N) { dst.x += 3; dst.y -= 9;}
            if (direction==S) { dst.x -= 2; dst.y += 10;}
            if (direction==O) { dst.x -= 8; dst.y -= 1;}
            if (direction==E) { dst.x += 11; dst.y += 3;}
            SDL_BlitSurface(imageSpin, &src, gpScreen, &dst);
            gpJeu->testEpee(dst.x+phg[0],dst.y+phg[1],src.w,src.h,EPEE_SPIN,epee*2,dir);
            
            src.x = 33; src.y = 15; src.w = 1; src.h = 1;
            if (direction==N) { dst.x -= 1; dst.y -= 8;}
            if (direction==S) { dst.x += 4; dst.y += 11;}
            if (direction==O) { dst.x -= 7; dst.y += 3;}
            if (direction==E) { dst.x += 11; dst.y -= 2;}
            SDL_BlitSurface(imageSpin, &src, gpScreen, &dst);
            gpJeu->testEpee(dst.x+phg[0],dst.y+phg[1],src.w,src.h,EPEE_SPIN,epee*2,dir);
            
            src.x = 5; src.y = 16; src.w = 5; src.h = 5;
            if (direction==N) { dst.x -= 32; dst.y += 25;}
            if (direction==S) { dst.x += 31; dst.y -= 22;}
            if (direction==O) { dst.x += 21; dst.y += 31;}
            if (direction==E) { dst.x -= 28; dst.y -= 31;}
            SDL_BlitSurface(imageSpin, &src, gpScreen, &dst);
            break;
        case 21 :
            src.x = 10; src.y = 14; src.w = 8; src.h = 8;
            if (direction==N) { dst.x += 10; dst.y += 28; dir=S;}
            if (direction==S) { dst.x += 2; dst.y -= 12; dir=N;}
            if (direction==O) { dst.x += 23; dst.y += 4; dir=E;}
            if (direction==E) { dst.x -= 14; dst.y += 14; dir=O;}
            SDL_BlitSurface(imageSpin, &src, gpScreen, &dst);
            gpJeu->testEpee(dst.x+phg[0],dst.y+phg[1],src.w,src.h,EPEE_SPIN,epee*2,dir);
            
            src.x = 18; src.y = 14; src.w = 7; src.h = 7;
            if (direction==N) { dst.x += 8; dst.y -= 4; dir=E;}
            if (direction==S) { dst.x -= 9; dst.y += 4; dir=O;}
            if (direction==O) { dst.x -= 5; dst.y -= 8; dir=N;}
            if (direction==E) { dst.x += 5; dst.y += 9; dir=S;}
            SDL_BlitSurface(imageSpin, &src, gpScreen, &dst);
            gpJeu->testEpee(dst.x+phg[0],dst.y+phg[1],src.w,src.h,EPEE_SPIN,epee*2,dir);
            
            src.x = 5; src.y = 16; src.w = 5; src.h = 5;
            if (direction==N) { dst.x += 6; dst.y -= 7;}
            if (direction==S) { dst.x -= 6; dst.y += 9;}
            if (direction==O) { dst.x -= 6; dst.y -= 4;}
            if (direction==E) { dst.x += 9; dst.y += 6;}
            SDL_BlitSurface(imageSpin, &src, gpScreen, &dst);
            gpJeu->testEpee(dst.x+phg[0],dst.y+phg[1],src.w,src.h,EPEE_SPIN,epee*2,dir);
            
            src.x = 33; src.y = 15; src.w = 1; src.h = 1;
            if (direction==N) { dst.x += 3; dst.y -= 9;}
            if (direction==S) dst.y += 10;
            if (direction==O) { dst.x -= 8; dst.y -= 1;}
            if (direction==E) { dst.x += 12; dst.y += 2;}
            SDL_BlitSurface(imageSpin, &src, gpScreen, &dst);
            gpJeu->testEpee(dst.x+phg[0],dst.y+phg[1],src.w,src.h,EPEE_SPIN,epee*2,dir);
            
            if (direction==N) { dst.x -= 33; dst.y += 19;}
            if (direction==S) { dst.x += 35; dst.y -= 13;}
            if (direction==O) { dst.x += 16; dst.y += 34;}
            if (direction==E) { dst.x -= 19; dst.y -= 33;}
            SDL_BlitSurface(imageSpin, &src, gpScreen, &dst);
            break;
        case 22 :
            src.x = 0; src.y = 8; src.w = 8; src.h = 8;
            if (direction==N) { dst.x += 2; dst.y += 29; dir=S;}
            if (direction==S) { dst.x += 10; dst.y -= 11; dir=N;}
            if (direction==O) { dst.x += 23; dst.y += 12; dir=E;}
            if (direction==E) { dst.x -= 15; dst.y += 6; dir=O;}
            SDL_BlitSurface(imageSpin, &src, gpScreen, &dst);
            gpJeu->testEpee(dst.x+phg[0],dst.y+phg[1],src.w,src.h,EPEE_SPIN,epee*2,dir);
            
            src.x = 25; src.y = 14; src.w = 7; src.h = 7;
            if (direction==N) { dst.x += 10; dst.y -= 1;}
            if (direction==S) dst.x -= 10;
            if (direction==O) { dst.x -= 1; dst.y -= 9;}
            if (direction==E) { dst.x += 2; dst.y += 11;}
            SDL_BlitSurface(imageSpin, &src, gpScreen, &dst);
            gpJeu->testEpee(dst.x+phg[0],dst.y+phg[1],src.w,src.h,EPEE_SPIN,epee*2,dir);
            
            src.x = 0; src.y = 16; src.w = 5; src.h = 5;
            if (direction==N) { dst.x += 8; dst.y -= 4; dir=E;}
            if (direction==S) { dst.x -= 8; dst.y += 6; dir=O;}
            if (direction==O) { dst.x -= 4; dst.y -= 7; dir=N;}
            if (direction==E) { dst.x += 6; dst.y += 8; dir=S;}
            SDL_BlitSurface(imageSpin, &src, gpScreen, &dst);
            gpJeu->testEpee(dst.x+phg[0],dst.y+phg[1],src.w,src.h,EPEE_SPIN,epee*2,dir);
            
            src.x = 33; src.y = 15; src.w = 1; src.h = 1;
            if (direction==N) { dst.x += 7; dst.y -= 8;}
            if (direction==S) { dst.x -= 3; dst.y += 8;}
            if (direction==O) { dst.x -= 6; dst.y -= 4;}
            if (direction==E) { dst.x += 11; dst.y += 6;}
            SDL_BlitSurface(imageSpin, &src, gpScreen, &dst);
            gpJeu->testEpee(dst.x+phg[0],dst.y+phg[1],src.w,src.h,EPEE_SPIN,epee*2,dir);
            break;
        case 23 :
            src.x = 0; src.y = 8; src.w = 8; src.h = 8;
            if (direction==N) { dst.x -= 5; dst.y += 27; dir=S;}
            if (direction==S) { dst.x += 17; dst.y -= 7; dir=N;}
            if (direction==O) { dst.x += 20; dst.y += 19; dir=E;}
            if (direction==E) { dst.x -= 13; dst.y -= 1; dir=O;}
            SDL_BlitSurface(imageSpin, &src, gpScreen, &dst);
            gpJeu->testEpee(dst.x+phg[0],dst.y+phg[1],src.w,src.h,EPEE_SPIN,epee*2,dir);
            
            src.x = 25; src.y = 14; src.w = 7; src.h = 7;
            if (direction==N) { dst.x += 9; dst.y += 3;}
            if (direction==S) { dst.x -= 9; dst.y -= 4;}
            if (direction==O) { dst.x += 3; dst.y -= 8;}
            if (direction==E) { dst.x -= 2; dst.y += 10;}
            SDL_BlitSurface(imageSpin, &src, gpScreen, &dst);
            gpJeu->testEpee(dst.x+phg[0],dst.y+phg[1],src.w,src.h,EPEE_SPIN,epee*2,dir);
            
            src.x = 0; src.y = 16; src.w = 5; src.h = 5;
            if (direction==N) { dst.x += 10; dst.y -= 1;}
            if (direction==S) { dst.x -= 9; dst.y += 2;}
            if (direction==O) { dst.x -= 1; dst.y -= 9;}
            if (direction==E) { dst.x += 3; dst.y += 10;}
            SDL_BlitSurface(imageSpin, &src, gpScreen, &dst);
            gpJeu->testEpee(dst.x+phg[0],dst.y+phg[1],src.w,src.h,EPEE_SPIN,epee*2,dir);
            
            src.x = 33; src.y = 15; src.w = 1; src.h = 1;
            if (direction==N) { dst.x += 10; dst.y -= 6; dir=E;}
            if (direction==S) { dst.x -= 6; dst.y += 6; dir=O;}
            if (direction==O) { dst.x -= 3; dst.y -= 6; dir=N;}
            if (direction==E) { dst.x += 9; dst.y += 9; dir=S;}
            SDL_BlitSurface(imageSpin, &src, gpScreen, &dst);
            gpJeu->testEpee(dst.x+phg[0],dst.y+phg[1],src.w,src.h,EPEE_SPIN,epee*2,dir);
            
            src.x = 18; src.y = 14; src.w = 7; src.h = 7;
            if (direction==N) { dst.x -= 40; dst.y -= 13;}
            if (direction==S) { dst.x += 31; dst.y += 16;}
            if (direction==O) { dst.x -= 18; dst.y += 33;}
            if (direction==E) { dst.x += 8; dst.y -= 39;}
            SDL_BlitSurface(imageSpin, &src, gpScreen, &dst);
            break;
        case 24 :
            src.x = 10; src.y = 14; src.w = 8; src.h = 8;
            if (direction==N) { dst.x -= 11; dst.y += 22; dir=S;}
            if (direction==S) { dst.x += 22; dst.y -= 1; dir=N;}
            if (direction==O) { dst.x += 14; dst.y += 25; dir=E;}
            if (direction==E) { dst.x -= 8; dst.y -= 7; dir=O;}
            SDL_BlitSurface(imageSpin, &src, gpScreen, &dst);
            gpJeu->testEpee(dst.x+phg[0],dst.y+phg[1],src.w,src.h,EPEE_SPIN,epee*2,dir);
            
            src.x = 18; src.y = 14; src.w = 7; src.h = 7;
            if (direction==N) { dst.x += 7; dst.y += 6;}
            if (direction==S) { dst.x -= 7; dst.y -= 7;}
            if (direction==O) { dst.x += 7; dst.y -= 7;}
            if (direction==E) { dst.x -= 5; dst.y += 8;}
            SDL_BlitSurface(imageSpin, &src, gpScreen, &dst);
            gpJeu->testEpee(dst.x+phg[0],dst.y+phg[1],src.w,src.h,EPEE_SPIN,epee*2,dir);
            
            src.x = 5; src.y = 16; src.w = 5; src.h = 5;
            if (direction==N) { dst.x += 11; dst.y += 3;}
            if (direction==S) { dst.x -= 8; dst.y -= 2;}
            if (direction==O) { dst.x += 3; dst.y -= 8;}
            if (direction==E) { dst.x -= 1; dst.y += 11;}
            SDL_BlitSurface(imageSpin, &src, gpScreen, &dst);
            gpJeu->testEpee(dst.x+phg[0],dst.y+phg[1],src.w,src.h,EPEE_SPIN,epee*2,dir);
            
            src.x = 33; src.y = 15; src.w = 1; src.h = 1;
            if (direction==N) { dst.x += 11; dst.y -= 2;}
            if (direction==S) { dst.x -= 8; dst.y += 3;}
            if (direction==O) dst.y -= 8;
            if (direction==E) { dst.x += 5; dst.y += 10;}
            SDL_BlitSurface(imageSpin, &src, gpScreen, &dst);
            gpJeu->testEpee(dst.x+phg[0],dst.y+phg[1],src.w,src.h,EPEE_SPIN,epee*2,dir);
            
            src.x = 5; src.y = 16; src.w = 5; src.h = 5;
            if (direction==N) { dst.x -= 33; dst.y -= 18;}
            if (direction==S) { dst.x += 26; dst.y += 21;}
            if (direction==O) { dst.x -= 22; dst.y += 28;}
            if (direction==E) { dst.x += 15; dst.y -= 32;}
            SDL_BlitSurface(imageSpin, &src, gpScreen, &dst);
            break;
        case 25 :
            src.x = 0; src.y = 8; src.w = 8; src.h = 8;
            if (direction==N) { dst.x -= 15; dst.y += 15; dir=O;}
            if (direction==S) { dst.x += 24; dst.y += 6; dir=E;}
            if (direction==O) { dst.x += 7; dst.y += 28; dir=S;}
            if (direction==E) { dst.x -= 6; dst.y -= 9; dir=N;}
            SDL_BlitSurface(imageSpin, &src, gpScreen, &dst);
            gpJeu->testEpee(dst.x+phg[0],dst.y+phg[1],src.w,src.h,EPEE_SPIN,epee*2,dir);
            
            src.x = 25; src.y = 14; src.w = 7; src.h = 7;
            if (direction==N) { dst.x += 5; dst.y += 9; dir=S;}
            if (direction==S) { dst.x -= 3; dst.y -= 8; dir=N;}
            if (direction==O) { dst.x += 9; dst.y -= 4; dir=E;}
            if (direction==E) { dst.x -= 7; dst.y += 9; dir=O;}
            SDL_BlitSurface(imageSpin, &src, gpScreen, &dst);
            gpJeu->testEpee(dst.x+phg[0],dst.y+phg[1],src.w,src.h,EPEE_SPIN,epee*2,dir);
            
            src.x = 0; src.y = 16; src.w = 5; src.h = 5;
            if (direction==N) { dst.x += 9; dst.y += 6;}
            if (direction==S) { dst.x -= 7; dst.y -= 6;}
            if (direction==O) { dst.x += 6; dst.y -= 6;}
            if (direction==E) { dst.x -= 1; dst.y += 10;}
            SDL_BlitSurface(imageSpin, &src, gpScreen, &dst);
            gpJeu->testEpee(dst.x+phg[0],dst.y+phg[1],src.w,src.h,EPEE_SPIN,epee*2,dir);
            
            src.x = 33; src.y = 15; src.w = 1; src.h = 1;
            if (direction==N) { dst.x += 12; dst.y += 2;}
            if (direction==S) { dst.x -= 7; dst.y -= 1;}
            if (direction==O) { dst.x += 4; dst.y -= 9;}
            if (direction==E) { dst.x += 4; dst.y += 10;}
            SDL_BlitSurface(imageSpin, &src, gpScreen, &dst);
            gpJeu->testEpee(dst.x+phg[0],dst.y+phg[1],src.w,src.h,EPEE_SPIN,epee*2,dir);
            
            if (direction==N) { dst.x -= 24; dst.y -= 20;}
            if (direction==S) { dst.x += 20; dst.y += 24;}
            if (direction==O) { dst.x -= 22; dst.y += 22;}
            if (direction==E) { dst.x += 18; dst.y -= 28;}
            SDL_BlitSurface(imageSpin, &src, gpScreen, &dst);
            break;
        case 26 :
            src.x = 0; src.y = 8; src.w = 8; src.h = 8;
            if (direction==N) { dst.x -= 16; dst.y += 7; dir=O;}
            if (direction==S) { dst.x += 23; dst.y += 14; dir=E;}
            if (direction==O) { dst.x -= 1; dst.y += 28; dir=S;}
            if (direction==E) { dst.x += 7; dst.y -= 12; dir=N;}
            SDL_BlitSurface(imageSpin, &src, gpScreen, &dst);
            gpJeu->testEpee(dst.x+phg[0],dst.y+phg[1],src.w,src.h,EPEE_SPIN,epee*2,dir);
            
            src.x = 25; src.y = 14; src.w = 7; src.h = 7;
            if (direction==N) { dst.x += 2; dst.y += 11;}
            if (direction==S) { dst.x += 1; dst.y -= 9;}
            if (direction==O) dst.x += 10;
            if (direction==E) { dst.x -= 10; dst.y += 3;}
            SDL_BlitSurface(imageSpin, &src, gpScreen, &dst);
            gpJeu->testEpee(dst.x+phg[0],dst.y+phg[1],src.w,src.h,EPEE_SPIN,epee*2,dir);
            
            src.x = 0; src.y = 16; src.w = 5; src.h = 5;
            if (direction==N) { dst.x += 6; dst.y += 8; dir=S;}
            if (direction==S) { dst.x -= 4; dst.y -= 8; dir=N;}
            if (direction==O) { dst.x += 9; dst.y -= 4; dir=E;}
            if (direction==E) { dst.x -= 6; dst.y += 6; dir=O;}
            SDL_BlitSurface(imageSpin, &src, gpScreen, &dst);
            gpJeu->testEpee(dst.x+phg[0],dst.y+phg[1],src.w,src.h,EPEE_SPIN,epee*2,dir);
            
            src.x = 33; src.y = 15; src.w = 1; src.h = 1;
            if (direction==N) { dst.x += 11; dst.y += 6;}
            if (direction==S) { dst.x -= 5; dst.y -= 4;}
            if (direction==O) { dst.x += 7; dst.y -= 7;}
            if (direction==E) { dst.x -= 3; dst.y += 10;}
            SDL_BlitSurface(imageSpin, &src, gpScreen, &dst);
            gpJeu->testEpee(dst.x+phg[0],dst.y+phg[1],src.w,src.h,EPEE_SPIN,epee*2,dir);
            break;
        case 27 :
            src.x = 10; src.y = 14; src.w = 8; src.h += 8;
            if (direction==N) { dst.x -= 14; dir=O;}
            if (direction==S) { dst.x += 19; dst.y += 22; dir=E;}
            if (direction==O) { dst.x -= 8; dst.y += 25; dir=S;}
            if (direction==E) { dst.x += 14; dst.y -= 10; dir=N;}
            SDL_BlitSurface(imageSpin, &src, gpScreen, &dst);
            gpJeu->testEpee(dst.x+phg[0],dst.y+phg[1],src.w,src.h,EPEE_SPIN,epee*2,dir);
            
            src.x = 18; src.y = 14; src.w = 7; src.h = 7;
            if (direction==N) { dst.x -= 2; dst.y += 10;}
            if (direction==S) { dst.x += 5; dst.y -= 8;}
            if (direction==O) { dst.x += 9; dst.y += 4;}
            if (direction==E) { dst.x -= 9; dst.y -= 1;}
            SDL_BlitSurface(imageSpin, &src, gpScreen, &dst);
            gpJeu->testEpee(dst.x+phg[0],dst.y+phg[1],src.w,src.h,EPEE_SPIN,epee*2,dir);
            
            src.x = 5; src.y = 16; src.w = 5; src.h = 5;
            if (direction==N) { dst.x += 3; dst.y += 10;}
            if (direction==S) dst.y -= 9;
            if (direction==O) { dst.x += 11; dst.y -= 1;}
            if (direction==E) { dst.x -= 8; dst.y += 3;}
            SDL_BlitSurface(imageSpin, &src, gpScreen, &dst);
            gpJeu->testEpee(dst.x+phg[0],dst.y+phg[1],src.w,src.h,EPEE_SPIN,epee*2,dir);
            
            src.x = 33; src.y = 15; src.w = 1; src.h = 1;
            if (direction==N) { dst.x += 9; dst.y += 9; dir=S;}
            if (direction==S) { dst.x -= 3; dst.y -= 7; dir=N;}
            if (direction==O) { dst.x += 9; dst.y -= 4; dir=E;}
            if (direction==E) { dst.x -= 6; dst.y += 8; dir=O;}
            SDL_BlitSurface(imageSpin, &src, gpScreen, &dst);
            gpJeu->testEpee(dst.x+phg[0],dst.y+phg[1],src.w,src.h,EPEE_SPIN,epee*2,dir);
            
            src.x = 18; src.y = 14; src.w = 7; src.h = 7;
            if (direction==N) { dst.x -= 5; dst.y -= 34;}
            if (direction==S) { dst.x -= 8; dst.y += 30;}
            if (direction==O) { dst.x -= 35; dst.y -= 4;}
            if (direction==E) { dst.x += 29; dst.y += 4;}
            SDL_BlitSurface(imageSpin, &src, gpScreen, &dst);
            break;
        case 28 :
            src.x = 52; src.y = 0; src.w = 14; src.h = 14;
            if (direction==N) { dst.x -= 10; dst.y += 22; dir=S;}
            if (direction==S) { dst.x += 14; dst.y -= 8; dir=N;}
            if (direction==O) { dst.x += 17; dst.y += 17; dir=E;}
            if (direction==E) { dst.x -= 14; dst.y -= 1; dir=O;}
            SDL_BlitSurface(imageSpin, &src, gpScreen, &dst);
            gpJeu->testEpee(dst.x+phg[0],dst.y+phg[1],src.w,src.h,EPEE_SPIN,epee*2,dir);
            break;
        case 29 :
            src.x = 0; src.y = 8; src.w = 8; src.h = 8;
            if (direction==N) { dst.x -= 7; dst.y += 25; dir=S;}
            if (direction==S) { dst.x += 17; dst.y -= 5; dir=N;}
            if (direction==O) { dst.x += 20; dst.y += 20; dir=E;}
            if (direction==E) { dst.x -= 11; dst.y += 2; dir=O;}
            SDL_BlitSurface(imageSpin, &src, gpScreen, &dst);
            gpJeu->testEpee(dst.x+phg[0],dst.y+phg[1],src.w,src.h,EPEE_SPIN,epee*2,dir);
            break;
        case 30 :
            src.x = 0; src.y = 16; src.w = 5; src.h = 5;
            if (direction==N) { dst.x -= 6; dst.y += 27; dir=S;}
            if (direction==S) { dst.x += 18; dst.y -= 3; dir=N;}
            if (direction==O) { dst.x += 21; dst.y += 22; dir=E;}
            if (direction==E) { dst.x -= 10; dst.y += 4; dir=O;}
            SDL_BlitSurface(imageSpin, &src, gpScreen, &dst);
            gpJeu->testEpee(dst.x+phg[0],dst.y+phg[1],src.w,src.h,EPEE_SPIN,epee*2,dir);
            break;
    }
    
}

void Joueur::collisionEpee() {
    int i=0, j=0, a=0, b=0;
    switch (direction) {
        case N :
            switch(anim) {
                case 0 : i=9; j=7; a=14; b=6; if (epee > 3) { j=6; a=16; b=7;} break;
                case 1 : i=9; j=-6; a=14; b=18; if (epee > 3) { i=3; j=-13; a=22; b=24;} break;
                case 2 : i=0; j=-8; a=16; b=16; if (epee > 3) { j=-14; a=16; b=24;} break;
                case 3 : i=-4; j=-6; a=14; b=14; if (epee > 3) { i=-14; a=24; b=16;} break;
                case 4 : i=-12; j=-6; a=14; b=22; if (epee > 3) { i=-14; a=16;} break;
            }
            break;
        case S :
            switch(anim) {
                case 0 : i=-5; j=12; a=12; b=6; if (epee > 3) { i=-7; j=14; a=14; b=8;} break;
                case 1 : i=-6; j=12; a=14; b=14; if (epee > 3) { i=-5; j=14; b=21;} break;
                case 2 : i=0; j=21; a=16; b=12; if (epee > 3) { j=20; b=19;} break;
                case 3 : i=9; j=17; a=15; b=14; if (epee > 3) { b=16;} break;
                case 4 : i=16; j=15; a=12; b=14; if (epee > 3) { a=14; b=13;} break;
            }
            break;
        case O :
            switch(anim) {
                case 0 : i=-2; j=1; a=6; b=12; if (epee > 3) { i=-3; j=-1; a=8; b=14;} break;
                case 1 : i=-10; j=0; a=12; b=14; if (epee > 3) { i=-14; a=18; b=14;} break;
                case 2 : i=-13; j=0; a=16; b=16; if (epee > 3) { i=-20; a=24; b=20;} break;
                case 3 : i=-12; j=13; a=14; b=10; if (epee > 3) { i=-14; j=12; a=16; b=18;} break;
                case 4 : i=-7; j=16; a=14; b=18; if (epee > 3) { j=16; b=19;} break;
            }
            break;
        case E :
            switch(anim) {
                case 0 : i=12; j=1; a=6; b=12; if (epee > 3) { i=11; j=-1; a=8; b=14;} break;
                case 1 : i=14; j=0; a=12; b=14; if (epee > 3) { i=12; a=18; b=14;} break;
                case 2 : i=13; j=0; a=16; b=16; if (epee > 3) { i=12; a=24; b=20;} break;
                case 3 : i=14; j=13; a=14; b=10; if (epee > 3) { i=14; j=12; a=16; b=18;} break;
                case 4 : i=9; j=16; a=14; b=18; if (epee > 3) { i=7; j=16; b=19;} break;
            }
            break;
    }
    gpJeu->testEpee(x+i,y+j,a,b,COUP_EPEE,epee,direction);
}

void Joueur::collisionSpin() {
    int i=0, j=0, a=0, b=0;
    Direction dir=N;
    switch (direction) {
        case N :
            if (anim==0 || anim==5 || (anim>=21 && anim<=26)) {i=-5; j=17; a=8; b=14; dir=S; break;}
            if (anim>=1 && anim<=4) {i=3; j=20; a=8; b=14; dir=S;break;}
            if (anim==6) {i=-14; j=15; a=14; b=14; dir=O; break;}
            if (anim==7 || anim==8) {i=-14; j=7; a=14; b=14; dir=O; break;}
            if (anim==9 || anim==10) {i=-14; j=-6; a=22; b=14; dir=N; break;}
            if (anim==11 || anim==12) {i=4; j=-6; a=14; b=14; dir=N; break;}
            if (anim==13 || anim==14) {i=16; j=-6; a=14; b=21; dir=E; break;}
            if (anim==15 || anim==16) {i=16; j=10; a=14; b=14; dir=E; break;}
            if (anim==17 || anim==18) {i=7; j=21; a=14; b=14; dir=S; break;}
            if (anim==19 || anim==20) {i=-2; j=21; a=14; b=14; dir=S; break;}
            return;
        case S :
            if (anim==0 || anim==5 || (anim>=21 && anim<=26)) {i=13; j=-4; a=8; b=14; dir=N; break;}
            if (anim>=1 && anim<=4) {i=6; j=-8; a=8; b=14; dir=N;break;}
            if (anim==6) {i=16; j=1; a=14; b=14; dir=E; break;}
            if (anim==7 || anim==8) {i=16; j=10; a=14; b=14; dir=E; break;}
            if (anim==9 || anim==10) {i=7; j=21; a=22; b=14; dir=S; break;}
            if (anim==11 || anim==12) {i=-2; j=21; a=14; b=14; dir=S; break;}
            if (anim==13 || anim==14) {i=-14; j=15; a=14; b=21; dir=O; break;}
            if (anim==15 || anim==16) {i=-14; j=7; a=14; b=14; dir=O; break;}
            if (anim==17 || anim==18) {i=-6; j=-6; a=14; b=14; dir=N; break;}
            if (anim==19 || anim==20) {i=11; j=16; a=14; b=14; dir=N; break;}
            return;
        case O :
            if (anim==0 || anim==5 || (anim>=21 && anim<=26)) {i=11; j=16; a=14; b=8; dir=E; break;}
            if (anim>=1 && anim<=4) {i=13; j=9; a=8; b=14; dir=E;break;}
            if (anim==6) {i=7; j=20; a=14; b=14; dir=S; break;}
            if (anim==7 || anim==8) {i=-2; j=20; a=14; b=14; dir=S; break;}
            if (anim==9 || anim==10) {i=-14; j=15; a=14; b=20; dir=O; break;}
            if (anim==11 || anim==12) {i=-14; j=7; a=14; b=14; dir=O; break;}
            if (anim==13 || anim==14) {i=-6; j=-6; a=14; b=21; dir=N; break;}
            if (anim==15 || anim==16) {i=4; j=-6; a=14; b=14; dir=N; break;}
            if (anim==17 || anim==18) {i=16; j=1; a=14; b=14; dir=E; break;}
            if (anim==19 || anim==20) {i=16; j=10; a=14; b=14; dir=E; break;}
            break;
        case E :
            if (anim==0 || anim==5 || (anim>=21 && anim<=26)) {i=-10; j=5; a=14; b=8; dir=O; break;}
            if (anim>=1 && anim<=4) {i=-10; j=12; a=14; b=8; dir=O;break;}
            if (anim==6) {i=-6; j=-6; a=14; b=14; dir=N; break;}
            if (anim==7 || anim==8) {i=4; j=-6; a=14; b=14; dir=N; break;}
            if (anim==9 || anim==10) {i=16; j=-6; a=14; b=21; dir=E; break;}
            if (anim==11 || anim==12) {i=16; j=10; a=14; b=14; dir=E; break;}
            if (anim==13 || anim==14) {i=7; j=21; a=21; b=14; dir=S; break;}
            if (anim==15 || anim==16) {i=-2; j=21; a=14; b=14; dir=S; break;}
            if (anim==17 || anim==18) {i=-14; j=15; a=14; b=14; dir=O; break;}
            if (anim==19 || anim==20) {i=-14; j=7; a=14; b=14; dir=O; break;}
            break;
    }
    gpJeu->testEpee(x+i,y+j,a,b,EPEE_SPIN,epee*2,dir);
}

void Joueur::drawArc(SDL_Surface* gpScreen) {
    int anim2 = (int)((anim+1)/2);
    int phg[2];
    phg[0] = gpJeu->getPhg(0);
    phg[1] = gpJeu->getPhg(1);
    
    SDL_Rect src;
    SDL_Rect dst;
    
    if (direction==N) {
        src.w = 16; src.h = 16;
        dst.x = x - phg[0]-2 - 3*((anim2+1)/2); dst.y = y - phg[1]+7+3*((anim2+1)/2);
        src.x = 16*direction; src.y = 16*((anim2+1)/2);
        SDL_BlitSurface(imageObjets, &src, gpScreen, &dst);
    }
    
    src.w = w; src.h = h;
    dst.x = x - phg[0]; dst.y = y - phg[1];
    
    src.x = 128+w*direction; src.y = 120+h*anim2;
    SDL_BlitSurface(image, &src, gpScreen, &dst);
    
    if (direction==N) return;
    
    src.x = 16*direction; src.y = 16*((anim2+1)/2); src.w = 16; src.h = 16;
    switch (direction) {
        case S : dst.x = x - phg[0]+10-8*((anim2+1)/2); dst.y = y - phg[1]+10+5*((anim2+1)/2); break;
        case O : dst.x = x - phg[0]-1-2*((anim2+1)/2); dst.y = y - phg[1]+11-4*((anim2+1)/2); break;
        case E : dst.x = x - phg[0]+4+8*((anim2+1)/2); dst.y = y - phg[1]+11-4*((anim2+1)/2); break;
        default : break;
    }
    SDL_BlitSurface(imageObjets, &src, gpScreen, &dst);
    
}

void Joueur::drawGrappin(SDL_Surface* gpScreen) {
    int phg[2];
    phg[0] = gpJeu->getPhg(0);
    phg[1] = gpJeu->getPhg(1);
    
    //boucl->setZone(new Zone(NULL));
    
    SDL_Rect src;
    SDL_Rect dst;
    SDL_Rect* bcl;
    int xbcl, ybcl;
    
    src.w = w; src.h = h;
    dst.x = x - phg[0]; dst.y = y - phg[1];
    
    src.x = 128+w*direction; src.y = 192;
    SDL_BlitSurface(image, &src, gpScreen, &dst);
    
    // Bouclier
    if (bouclier) {
        switch (direction) {
            case N :
                bcl = new SDL_Rect();
                bcl->x = 16 + dst.x; bcl->y = 9 + dst.y; 
                bcl->w = 4; bcl->h = 10; xbcl = 20; ybcl = 0;
                boucl->draw(gpScreen, xbcl, ybcl, new ZoneRect(bcl), E);
                break;
            case S :
                bcl = new SDL_Rect();
                bcl->x = -4 + dst.x; bcl->y = 10 + dst.y; 
                bcl->w = 4; bcl->h = 10; xbcl = 16; ybcl = 0;
                boucl->draw(gpScreen, xbcl, ybcl, new ZoneRect(bcl), O);
                break;
            default : break;
        }
    }
    
    if (direction==N || typeanim==LANTERNE) return;
    
    src.x = 56; src.y = 74; src.w = 8; src.h = 4;
    switch (direction) {
        case S : dst.x = x - phg[0]+ 6; dst.y = y - phg[1]+17; src.w = 4; src.h = 8; break;
        case O : dst.x = x - phg[0]-7; dst.y = y - phg[1]+14; src.y += 8; break;
        case E : dst.x = x - phg[0]+15; dst.y = y - phg[1]+14; src.y += 12; break;
        default : break;
    }
    SDL_BlitSurface(imageObjets, &src, gpScreen, &dst);
    
}

void Joueur::drawBaguette(SDL_Surface* gpScreen) {
    int anim2 = (int)((anim+1)/2);
    int phg[2];
    phg[0] = gpJeu->getPhg(0);
    phg[1] = gpJeu->getPhg(1);
    
    //boucl->setZone(new Zone(NULL));
    
    SDL_Rect src;
    SDL_Rect dst;
    SDL_Rect* bcl;
    int xbcl, ybcl;
    
    src.w = w; src.h = h;
    dst.x = x - phg[0]; dst.y = y - phg[1];
    
    src.x = 192+w*direction; src.y = h*anim2;
    SDL_BlitSurface(image, &src, gpScreen, &dst);
    
    // Bouclier
    if (bouclier) {
        switch (direction) {
            case N :
                bcl = new SDL_Rect();
                bcl->x = 15 + dst.x; bcl->y = 9 + dst.y; 
                bcl->w = 4; bcl->h = 10; xbcl = 20; ybcl = 0;
                boucl->draw(gpScreen, xbcl, ybcl, new ZoneRect(bcl), E);
                break;
            case S :
                bcl = new SDL_Rect();
                bcl->x = -4 + dst.x; bcl->y = 10 + dst.y; 
                bcl->w = 4; bcl->h = 10; xbcl = 16; ybcl = 0;
                boucl->draw(gpScreen, xbcl, ybcl, new ZoneRect(bcl), O);
                break;
            default : break;
        }
    }
    
    src.x = 14*direction; src.y = 32+anim2*14; src.w = 14; src.h = 14;
    if (typeanim==BGLACE) src.x+=56;
    if (typeanim==MARTEAU) src.y+=42;
    switch (direction) {
        case N : dst.x = x - phg[0]-2; dst.y = y - phg[1]-5; 
            if (anim2==1) dst.y+=3; if (anim2==2) dst.y+=7; break;
        case S : dst.x = x - phg[0]+ 10; dst.y = y - phg[1]-3;
            if (anim2==1) dst.y+=13; if (anim2==2) {dst.x--; dst.y+=23;} break;
        case O : dst.x = x - phg[0]; dst.y = y - phg[1]-1;
            if (anim2==1) {dst.x-=8;dst.y+=5;} if (anim2==2) {dst.x-=10; dst.y+=15;} break;
        case E : dst.x = x - phg[0]+8; dst.y = y - phg[1]-1;
            if (anim2==1) {dst.x+=5;dst.y+=5;} if (anim2==2) {dst.x+=4; dst.y+=15;} break;
    }
    SDL_BlitSurface(imageObjets, &src, gpScreen, &dst);
    
    if (typeanim == MARTEAU && anim2 == 2) {
        int i=0; int j=0;
        switch(direction) {
            case N : i = x; j = y - 8; break;
            case S : i = x; j = y + 24; break;
            case O : i = x-16; j = y + 8; break;
            case E : i = x+16; j = y + 8; break;
        }
        gpJeu->testEpee(i,j,16,16,COUP_MARTEAU,3,direction);
    }
}

void Joueur::drawMort(SDL_Surface* gpScreen) {
    int phg[2];
    phg[0] = gpJeu->getPhg(0);
    phg[1] = gpJeu->getPhg(1);
    
    SDL_Rect src;
    SDL_Rect dst;
    
    src.w = 24; src.h = 16; src.x=192; src.y = 200; 
    dst.y = y+8 - phg[1]; dst.x = x - phg[0]-4;
    SDL_BlitSurface(image, &src, gpScreen, &dst);
}

void Joueur::drawFlotte(SDL_Surface* gpScreen) {
    int anim2 = anim/3;
    int phg[2];
    phg[0] = gpJeu->getPhg(0);
    phg[1] = gpJeu->getPhg(1);
    
    SDL_Rect src;
    SDL_Rect dst;
    
    src.w = 16; src.h = 24; src.x=192+16*direction; src.y = 72+h*anim2;
    dst.y = y - phg[1]; dst.x = x - phg[0];
    
    if(direction == O) {dst.x--; src.w=24;}
    if(direction == E) {dst.x-=7; src.w=24; src.x+=8;}
    
    if (typeanim == NAGE) src.y+=h*2;
    if (src.y==192) src.y-=48;
    
    SDL_BlitSurface(image, &src, gpScreen, &dst);
}

void Joueur::drawSouleve(SDL_Surface* gpScreen) {
    int anim2 = anim/3;
    int phg[2];
    phg[0] = gpJeu->getPhg(0);
    phg[1] = gpJeu->getPhg(1);
    
    SDL_Rect src;
    SDL_Rect dst;
    
    if (typeanim == EMMENE) anim2 = (anim2+1)%4;
    
    src.w = 16; src.h = 24; src.x=272+16*direction; src.y = h*anim2;
    dst.y = y - phg[1]; dst.x = x - phg[0];
    
    if(direction == O) {src.w=24;}
    if(direction == E) {dst.x-=8; src.w=24; src.x+=8;}
    
    if (typeanim == LOURD && anim2==2) src.y=0;
    if (typeanim == PORTE) src.y=48;
    if (typeanim == EMMENE) src.y+=48;
    if (typeanim == LANCE) src.y=144;
    
    if (typeanim == EMMENE && direction == O && anim2 == 2) dst.x--;
    if (typeanim == EMMENE && direction == E && anim2 == 2) dst.x++;
    
    SDL_BlitSurface(image, &src, gpScreen, &dst);
    
    if (porte) {
        src.w=16; src.h=16; src.x=112; src.y=16*(porte-1);
        dst.y = y - phg[1]-8; dst.x = x - phg[0];
        if (anim2%2 && (direction == N || direction == S)) dst.y++;
        if ((anim2==1 || anim2==3) && (direction == O || direction == E)) dst.y++;
        if (anim2==2 && (direction == O || direction == E)) dst.y+=2;
        if (direction == O) dst.x-=2;
        if (direction == E) dst.x+=2;
        SDL_BlitSurface(imageObjets, &src, gpScreen, &dst);
    }
}

void Joueur::drawTouche(SDL_Surface* gpScreen) {
    int phg[2];
    phg[0] = gpJeu->getPhg(0);
    phg[1] = gpJeu->getPhg(1);
    
    SDL_Rect src;
    SDL_Rect dst;
    
    src.w = 16; src.h = 24; src.x=64+16*direction; src.y = 192;
    dst.y = y - phg[1]; dst.x = x - phg[0];
    
    SDL_BlitSurface(image, &src, gpScreen, &dst);
}

void Joueur::drawBoire(SDL_Surface* gpScreen) {
    int anim2 = anim/3;
    int phg[2];
    phg[0] = gpJeu->getPhg(0);
    phg[1] = gpJeu->getPhg(1);
    
    SDL_Rect src;
    SDL_Rect dst;
    
    src.w = 16; src.h = 24; src.x=256; src.y = 0;
    dst.y = y - phg[1]; dst.x = x - phg[0];
    
    SDL_BlitSurface(image, &src, gpScreen, &dst);
    
    src.w=8; src.h=7; src.x=96 + 8 * anim2; src.y=8*(bouteille[objet-9]-1);
    if (src.x==112) src.y=0;
    if (src.y==0) src.x=96;
    dst.x += 4; dst.y += 12;
    if (!anim2) dst.y++;
    
    SDL_BlitSurface(imageObjets, &src, gpScreen, &dst);
}

void Joueur::drawPousse(SDL_Surface* gpScreen) {
    int anim2 = anim/3;
    int phg[2];
    phg[0] = gpJeu->getPhg(0);
    phg[1] = gpJeu->getPhg(1);
    
    SDL_Rect src;
    SDL_Rect dst;
    
    src.w = 16; src.h = 24; src.x=352+w*direction; src.y = h*anim2;
    dst.y = y - phg[1]; dst.x = x - phg[0];
    
    switch (direction) {
        case N : dst.y--; break;
        case S : dst.y++; break;
        case O : dst.x--; break;
        case E : dst.x++; break;
    }
    
    SDL_BlitSurface(image, &src, gpScreen, &dst);
}

void Joueur::drawChute(SDL_Surface* gpScreen) {
    int anim2 = anim/3;
    int phg[2];
    phg[0] = gpJeu->getPhg(0);
    phg[1] = gpJeu->getPhg(1);
    
    SDL_Rect src;
    SDL_Rect dst;
    
    dst.x = x - phg[0]-3; dst.y = y - phg[1]-1+8-2;
    
    src.w = 22; src.h = 22; src.x=216+src.w*anim2; src.y = 194;
    
    SDL_BlitSurface(image, &src, gpScreen, &dst);
}

void Joueur::drawTrouve(SDL_Surface* gpScreen) {
    int phg[2];
    phg[0] = gpJeu->getPhg(0);
    phg[1] = gpJeu->getPhg(1);
    
    SDL_Rect src;
    SDL_Rect dst;
    SDL_Rect* bcl;
    
    src.w = 16; src.h = 24; src.x=256; 
    if (trouve==C_BOUCLIER || trouve==C_TRIFORCE || trouve==C_MIMAGIE) src.y = 24;
    else src.y = 48;
    dst.y = y - phg[1]; dst.x = x - phg[0];
    
    SDL_BlitSurface(image, &src, gpScreen, &dst);
    
    //bouclier
    if (src.y == 48 && bouclier) {
        bcl = new SDL_Rect();
        bcl->x = 0 + dst.x; bcl->y = 12 + dst.y; 
        bcl->w = 4; bcl->h = 10;
        boucl->draw(gpScreen, 16, 0, new ZoneRect(bcl), O);
    }
    
    //objet trouvé
    switch (trouve) {
        case C_BOUCLIER : src.w = 8; src.h = 10; src.x=16; src.y=75; dst.x+=4; break;
        case C_EPEE : src.w = 6; src.h = 13; src.x=9; src.y=72; dst.x+=11; break;
        case C_QUART_COEUR : src.w = 16; src.h = 16; src.x=80; src.y=0; dst.x+=6; break;
        case C_BOUTEILLE : src.w = 16; src.h = 16; src.x=24; src.y=69; dst.x+=6; break;
        case C_CARTE : src.w = 16; src.h = 16; src.x=48; src.y=0; dst.x+=6; break;
        case C_BOUSSOLE : src.w = 16; src.h = 16; src.x=96; src.y=0; dst.x+=6; break;
        case C_CLE_BOSS : src.w = 16; src.h = 16; src.x=16; src.y=0; dst.x+=6; break;
        case C_CLE : src.w = 16; src.h = 16; src.x=32; src.y=0; dst.x+=6; break;
        case C_GANTS : src.w = 16; src.h = 16; src.x=112; src.y=17; dst.x+=6; break;
        case C_CRISTAL : src.w = 16; src.h = 16; src.x=112; src.y=0; dst.x+=6; break;
        case C_GRAPPIN : src.w = 16; src.h = 16; src.x=112; src.y=34; dst.x+=6; break;
        case C_LANTERNE : src.w = 16; src.h = 16; src.x=128; src.y=0; dst.x+=6; break;
        case C_PALMES : src.w = 16; src.h = 16; src.x=128; src.y=17; dst.x+=6; break;
        case C_MARTEAU : src.w = 16; src.h = 16; src.x=128; src.y=34; dst.x+=6; break;
        case C_BFEU : src.w = 16; src.h = 16; src.x=144; src.y=0; dst.x+=6; break;
        case C_BGLACE : src.w = 16; src.h = 16; src.x=144; src.y=17; dst.x+=6; break;
        case C_EXCALIBUR : src.w = 8; src.h = 16; src.x=0; src.y=69; dst.x+=10; break;
        case C_TRIFORCE : src.w = 26; src.h = 25; src.x=166; src.y=26; dst.x-=5; break;
        case C_LIVRE : src.w = 16; src.h = 16; src.x=176; src.y=0; dst.x+=6; break;
        case C_MEDAILLON : 
            src.w = 16; src.h = 16; src.x=192; src.y=17*(gpJeu->getZone()-24); dst.x+=6; break;
        case C_POTION_ROUGE : src.w = 16; src.h = 16; src.x=40; src.y=69; dst.x+=6; break;
        case C_POTION_VERTE : src.w = 16; src.h = 16; src.x=56; src.y=69; dst.x+=6; break;
        case C_POTION_BLEUE : src.w = 16; src.h = 16; src.x=72; src.y=69; dst.x+=6; break;
        case C_ARC : src.w = 16; src.h = 16; src.x=144; src.y=34; dst.x+=6; break;
        case C_EPEE_R : src.w = 6; src.h = 13; src.x=136; src.y=72; dst.x+=11; break;
        case C_GANTS_R : src.w = 16; src.h = 16; src.x=208; src.y=0; dst.x+=6; break;
        case C_ARC_FEE : src.w = 16; src.h = 16; src.x=164; src.y=52; dst.x+=6; break;
        default : return;
    }
    dst.y-=(src.h-2);
    SDL_BlitSurface(gpJeu->getImageObjets(), &src, gpScreen, &dst);
}

/*

    Zelda Return of the Hylian

    Copyright (C) 2005-2008  Vincent Jouillat

    Please send bugreports with examples or suggestions to www.zeldaroth.fr

*/

#ifndef __JOUEUR_H__
#define __JOUEUR_H__

#include "Personnage.h"
#include "Bouclier.h"

class Jeu;

class Joueur : public Personnage {
    public :
        Joueur(Jeu* jeu, int nmsv);
        ~Joueur();
        void saveSemiEmpl();
        void restoreSemiEmpl();
        void draw(SDL_Surface* gpScreen);
        void moveX(int i, int nbdir);
        void moveY(int i, int nbdir);
        void moveX(int i);
        void moveY(int i);
        void setTypeAnim(int t);
        int getTypeAnim();
        void setBouclier(int b);
        void setMagie(int b);
        void setMagieMax(int b);
        void setEpee(int e);
        void setRubis(int i);
        void setFleche(int i);
        void setBombe(int i);
        void setRubisMax(int i);
        void setFlecheMax(int i);
        void setBombeMax(int i);
        void calculDef();
        void chargeSpin();
        void dechargeSpin();
        int getCharge();
        int nbQuarts();
        int nbCristaux();
        bool hasCristal(int i);
        void setCristal(int i);
        bool hasMedaillon(int i);
        void trouveMedaillon(int i);
        int hasObjet(int i);
        int hasBouteille(int i);
        void setBouteille(int i, int v);
        int getMagie();
        int getMagieMax();
        int getMiMagie();
        int getForce();
        int getEpee();
        int getDefense();
        int getBouclier();
        int getRubis();
        int getFleche();
        int getBombe();
        int getRubisMax();
        int getFlecheMax();
        int getBombeMax();
        int getObjet();
        int getCle();
        void setObjet(int i);
        bool getOnilink();
        bool getOni();
        int getOnijauge();
        int getOnimax();
        void setOnilink(bool b);
        void setOni(bool b);
        void setOnijauge(int i);
        void setOnimax(int i);
        bool verifZone();
        bool isDauphin();
        void savePrec();
        void setVie(int i);
        void setVieMax(int i);
        void bigPlouf();
        void antiPlouf();
        void revie();
        int getBoostVie();
        void setBoostVie(int i);
        int getBoostMagie();
        void setBoostMagie(int i);
        int getBoostRubis();
        void setBoostRubis(int i);
        int getPousse();
        void setPousse(int i);
        void setTrouve(int i);
        int getCoeur(int i);
        void setCoeur(int i);
        bool getCoffre(int i, int j);
        void setCoffre(int i, int j);
        int getCle(int i, int j);
        void setCle(int i, int j, int val=1);
        int getAvancement();
        void setAvancement(int i);
        int getTemps(int i);
        void trouveObjet(int obj, int val=1);
        bool getExplore(int z, int i, int j);
        void setExplore(int z, int i, int j);
        void visible();
        bool isLapin();
        void setLapin();
        void save();
        void load();
        void setImmo(bool b);
        bool getImmo();
        int nbEnnemis();
        void setEnnemi(int i);
        int getEnnemi(int i);
        bool getLoader();
        int getMort();
    private :
        void tireFleche();
        void tireGrappin();
        void tireMagie();
        void tireLanterne();
        void poseBombe();
        void lance(int v);
        void boit();
        void glisser();
        void drawChute(SDL_Surface* gpScreen);
        void drawImmoMarche(SDL_Surface* gpScreen);
        void drawCharge(SDL_Surface* gpScreen);
        void drawEpee(SDL_Surface* gpScreen);
        void drawSpin(SDL_Surface* gpScreen);
        void drawArc(SDL_Surface* gpScreen);
        void drawGrappin(SDL_Surface* gpScreen);
        void drawBaguette(SDL_Surface* gpScreen);
        void drawFlotte(SDL_Surface* gpScreen);
        void drawSouleve(SDL_Surface* gpScreen);
        void drawBoire(SDL_Surface* gpScreen);
        void drawPousse(SDL_Surface* gpScreen);
        void drawTrouve(SDL_Surface* gpScreen);
        void drawTouche(SDL_Surface* gpScreen);
        void drawMort(SDL_Surface* gpScreen);
        void collisionEpee();
        void collisionSpin();
        
        int numSave;
        int temps; //temps depuis la sauvegarde chargée
        int xdep2;
        int ydep2;
        Direction dirdep2;
        int magie;
        int magiemax;
        int typeanim;
        int minanim;
        int fleche;
        int bombe;
        int rubis;
        int objet;
        Bouclier* boucl;
        int bouclier;
        int epee;
        Jeu* gpJeu;
        int charge;
        int flechemax;
        int bombemax;
        int rubismax;
        bool lapin;
        bool onilink;  //risque de transformation
        bool oni;      //transformé
        int onijauge; //jauge transformation
        int onimax;   //capacité jauge
        int plouf;  //si entre dans l'eau
        int mort; //nombre de mort
        int porte; //objet porté
        int boostVie;
        int boostMagie;
        int boostRubis;
        int pousse;
        int trouve;
        int avancement;
        bool invisible;
        int dirglisse;          //direction pour glisser
        int dirglace;           //direction dans laquel link a commencer à glisser
        int glace;
        int glisse;      // si sur surface glissante et à quel point
        int vitesse;
        int oldxg; //x précédent
        int oldyg; //y précédent
        bool immo;
        bool loader; //si les données ont été chargées depuis une sauvegarde
        int defense;
        int coeur[44]; //8 coeurs + 36 quarts de coeur
        int cristaux[7];
        int medaillon[3];
        int objets[13];
        int bouteille[3];
        int ennemis[53]; //ennemis vaincus
        int cle[9][4]; // clés dans donjon i, 0 : carte, 1 : boussole, 2 : clé boss, 3 : clés
        int coffre[10][15]; //coffres extérieurs + 9 donjons
        bool explore[9][16][10]; //lieux explorés dans les donjons
        int oldx[8]; //x précédents
        int oldy[8]; //y précédents
        SDL_Surface* imageEpee;
        SDL_Surface* imageSpin;
        SDL_Surface* imageObjets;
        Uint32 TimeB; //temps de la dernière sauvegarde chargée
};

#endif  // Joueur.h

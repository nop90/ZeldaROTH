/*

    Zelda Return of the Hylian

    Copyright (C) 2005-2008  Vincent Jouillat

    Please send bugreports with examples or suggestions to www.zeldaroth.fr

*/

#ifndef __JEU_H__
#define __JEU_H__

//#include <SDL/SDL.h>
#include "3ds/os_3ds.h"

#include "Audio.h"
#include "Statut.h"
#include "Objet.h"
#include "Caisse.h"
#include "Ennemi.h"
#include "Snipe.h"
#include "Pnj.h"
#include "Joueur.h"
#include "Monde.h"
#include "Texte.h"
#include "Menu.h"
#include "Projectile.h"
#include "Generique.h"
class Keyboard;

class Jeu {
    public :
        Jeu(Audio* audio);
        ~Jeu();
        void init(int save);
        void reinit(int save);
        void draw(SDL_Surface* screen);
        void drawFreeze(SDL_Surface* screen);
		void drawMenu(SDL_Surface* screen);
        Joueur* getJoueur();
        Monde* getMonde();
        Audio* getAudio();
        Texte* getTexte();
        Statut* getStatut();
        Generique* getGenerique();
        Keyboard* getKeyboard();
        int getZone();
        void setZone(int i);
        int getPhg(int i);
        void setPhg(int i, int j);
        bool getStop();
        bool getText();
        bool getTransition();
        bool verifZoneJoueur(int x, int y, int w, int h, int nbdir); // regarde si Link peut y aller
        bool testEpee(int x, int y, int w, int h, int type, int force, Direction direction); //blesse monstres
        bool testDegat(int x, int y, int w, int h, int type, int force, Direction direction); //blesse Link
        bool getMenu();
        void setMenu(bool b);
        void setStop(bool b);
        void setTransition(bool b);
        void setText(bool b);
        void ecrit(int id, bool cadre=true, bool anim=true, int x=8, int y=152, int w=304, int h=80);
        void affiche(SDL_Surface* gpScreen, std::string s, int x, int y);
        bool isDonjon();
        bool isDehors();
        bool isGrotte();
        void ajouteProjectile(int type, Direction dir, int a, int b, int v);
        void ajouteObjet(int type, int i, int j, int num=0, int p=0);
        void ajouteSnipe(int type, double i, double j, double dirx, double diry);
        void ajouteCaisse(int type, int i, int j);
        void ajouteEnnemi(int type, int i, int j, bool p=true);
        void ajoutePiege(int type, int i, int j, bool p=true);
        void ajoutePnj(int type, int i, int j, int m);
        bool isNotFull(int x, int y, int w, int h);
        bool isNotFull(int x, int y, int w, int h, int vol, int nage, int spectre);
        bool isWater(int x, int y, int w, int h);
        bool isVide(int x, int y, int w, int h);
        bool isGlace(int x, int y, int w, int h);
        bool isRamassable(int x, int y, int g);
        void ramasse();
        int enleve();
        void testFin();
        void videListes();
        void setNewZone(int nz);
        void setNewX(int nx);
        void setNewY(int ny);
        void testAnim();
        void finZone();
        SDL_Surface* getImageObjets();
        void lire();
        bool pousse(int i, int j, Direction dir);
        bool isMarchable(int i, int j, int vol, int nage, int spectre);
        bool finTexte(int id);
        void trouve(int id);
        bool isEnnemi(int x, int y, int w, int h, int id=0);
        bool isPnj(int x, int y, int w, int h, int id=0);
        bool isLink(int x, int y, int w, int h);
        Ennemi* existEnnemi(int type);
        int getVueHorz();
        int getVueVert();
        void setVueHorz(int i);
        void setVueVert(int i);
        void replace(); //replace les ennemis, pnj, etc.
        void poule(); //énerve les poules
        void permuteBloc(int val);
        void setKeyboard(Keyboard* kb);
        void setGenerique(Generique* gen);
    private :
        void testRamasse(int x, int y, int w, int h);
        bool coupeBuisson(int i, int j, int type);
        bool applatitBloc(int i, int j);
        bool allumeTorche(int i, int j);
        bool etteintTorche(int i, int j);
        bool exploseMur(int i, int j);
        bool tueEnnemi(int i, int j, int type, int force, Direction direction);
        bool renvoieProj(int i, int j, Direction dir);
        bool coupeRideau(int i, int j);
        void donneObjet(Ennemi* ennemi);
        void scrolling();
        void ouvrePorteMaison(int x, int y);
        void ouvrePorteDonjon(int x, int y);
        void testInterrupteur(int x, int y);
        bool isDangereux(int x,int y);
        int nbEnnemis();
        
        Audio* gpAudio;
        Generique* gpGenerique;
        Keyboard* gpKeyboard;
        int zone;
        bool stop;
        bool transition;
        bool menu;
        bool text;
        int vueHorz;
        int vueVert;
        int phg[2];
        int newzone; //lors d'un changement de zone
        int newx;
        int newy;
        Monde* gpMonde;
        Joueur* gpJoueur;
        Texte* gpTexte;
        Menu* gpMenu;
        Statut* gpStatut;
        Projectile* gpProjectile;
        Objet* gpObjet;
        Caisse* gpCaisse;
        Ennemi* gpEnnemi;
        Ennemi* gpPiege; //ennemis au premier plan
        Snipe* gpSnipe;
        Pnj* gpPnj;
        SDL_Surface* imageObjets;
};

#endif  // Jeu.h

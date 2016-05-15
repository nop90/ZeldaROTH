/*

    Zelda Return of the Hylian

    Copyright (C) 2005-2008  Vincent Jouillat

    Please send bugreports with examples or suggestions to www.zeldaroth.fr

*/

#ifndef __COMMON_H__
#define __COMMON_H__

#define CASE    16          // taille d'une case en pixel

#define ANIM_REFRESH    120     // intervale entre les anims

#include <sstream>
#include <iostream>
#include <fstream>

//#include <SDL/SDL.h>
//#include <SDL/SDL_image.h>
#include "3ds/os_3ds.h"

using namespace std;

enum Direction {N, S, O, E};

enum Collision { VIDE = -1, LIBRE = 0, PLEIN = 1, SEMI_PLEIN = 2, BAS = 3, 
    HAUT = 4, DROITE = 5, GAUCHE = 6, BAS_DROITE = 7, BAS_GAUCHE = 8, 
    HAUT_DROITE = 9, HAUT_GAUCHE = 10, BOIS = 11, EAU = 12, GLACE = 13};
    
enum Type_Anim {AUCUNE, MARCHE, EPEE, SPIN, NAGE, ARC, GRAPPIN, BOMBE, BFEU, BGLACE,
    LANTERNE, MARTEAU, BOIRE, NOYADE, FLOTTE, LOURD, SOULEVE, PORTE, EMMENE, TOUCHE,
    LANCE, POUSSE, CHUTE, MORT};

enum Type_Attaque {COUP_EPEE, EPEE_SPIN, FLECHE, COUP_GRAPPIN, FEU, COUP_GLACE, PROJECTILE,
    EXPLOSION,COUP_MARTEAU, POT, CAISSE, MAGIE, ENNEMI, LAPIN, RETOUR, DEMI};

enum Type_Def {CRAINT, X2, RESISTE, INVUL};

enum Type_Objets {O_ARC, O_GRAPPIN, O_SAC_BOMBES, O_BFEU, O_BGLACE, O_LANTERNE,
    O_MARTEAU, O_PALMES, O_GANTS, O_LIVRE, O_BOTTES, O_CARTE, O_ENCYCL, O_OCARINA, O_MASQUE};

enum Type_Items {I_RIEN, I_RUBIS_VERT, I_RUBIS_BLEU, I_RUBIS_ROUGE, I_PETIT_COEUR, 
    I_FLECHE, I_BOMBE, I_MAGIE_PEU, I_MAGIE_BCP, I_QUART_COEUR, I_EPEE, I_CRISTAL,
    I_EXCALIBUR, I_PETITE_CLE, I_TRIFORCE, I_ARC};

enum Type_Contenu {C_RIEN = 0, C_BOUCLIER = 1, C_EPEE = 2, C_QUART_COEUR = 3, 
    C_BOUTEILLE = 33, C_CARTE=67, C_BOUSSOLE=68, C_CLE_BOSS=69, C_CLE=70, C_GANTS=71, 
    C_CRISTAL=72, C_GRAPPIN=80, C_LANTERNE=81, C_PALMES=82, C_MARTEAU=83, C_BFEU=84,
    C_BGLACE=85, C_EXCALIBUR=86, C_TRIFORCE=97, C_LIVRE=98, C_MEDAILLON=102, C_MIMAGIE=104,
    C_POTION_ROUGE=109, C_POTION_VERTE=110, C_POTION_BLEUE=111,C_ARC=130,C_PAIE=152,
    C_EPEE_R=168, C_GANTS_R=171, C_ARC_FEE=174};

enum Type_IA {IA_RIEN, IA_FONCE, IA_SNIPE, IA_ATTEND, IA_RANDOM, IA_SUIT, IA_HAUT_BAS, 
    IA_SPECIAL, IA_STATUE};

enum Type_Ennemi {E_SIMPLE, E_MONO_DIR, E_PROFIL};

enum Type_Pnj {P_SIMPLE, P_MONO_DIR, P_PROFIL};
    
#endif  // Common.h

/*

    Zelda Return of the Hylian

    Copyright (C) 2005-2008  Vincent Jouillat

    Please send bugreports with examples or suggestions to www.zeldaroth.fr

*/

#include <sstream>
#include <fstream>
#include <iostream>

//#include <SDL/SDL.h>
#include <3ds.h>
#include "Audio.h"

#include "3ds/os_3ds.h"
#include "3ds/sound_3ds.h"

Audio::Audio() : volume(0), musiqueId(0), specialId(0) {
    SOUND = FSOUND_Init(48000, 16, 0);
    music = NULL;
    
    if (SOUND) {
        previous_volume = -1;
        previous_volson = FSOUND_GetSFXMasterVolume();
        loadSounds();
    }
}

Audio::~Audio() {
    if (SOUND) {
        freeSounds();
        FMUSIC_StopSong(music);
        FMUSIC_SetMasterVolume(music, previous_volume);
        FMUSIC_FreeSong(music);
        FSOUND_SetSFXMasterVolume(previous_volson);
        FSOUND_Close();
    }
}

void Audio::setVolume(int vol) {
	volume=vol;
//    if (previous_volume == -1) previous_volume = FMUSIC_GetMasterVolume(music);
    FMUSIC_SetMasterVolume(music, volume);}
void Audio::setVolson(int volson) {FSOUND_SetSFXMasterVolume(volson);
   // for (int i = 0; i < 40; i++) Mix_VolumeChunk(sons[i], volson);
}

void Audio::loadSounds() {
    sons = new FSOUND_SAMPLE*[40];
    
    sons[0] = FSOUND_Sample_Load(FSOUND_FREE, "romfs:/sound/text.raw",0,0,0); // lettres
    sons[1] = FSOUND_Sample_Load(FSOUND_FREE, "romfs:/sound/menu1.raw",0,0,0); // menu 1
    sons[2] = FSOUND_Sample_Load(FSOUND_FREE, "romfs:/sound/menu2.raw",0,0,0); // menu 2
    sons[3] = FSOUND_Sample_Load(FSOUND_FREE, "romfs:/sound/menu3.raw",0,0,0); // menu 3
    sons[4] = FSOUND_Sample_Load(FSOUND_FREE, "romfs:/sound/menu4.raw",0,0,0); // menu 4
    sons[5] = FSOUND_Sample_Load(FSOUND_FREE, "romfs:/sound/timewarp.raw",0,0,0); // time retour
    sons[6] = FSOUND_Sample_Load(FSOUND_FREE, "romfs:/sound/tombe.raw",0,0,0); // tombe (ennemi)
    sons[7] = FSOUND_Sample_Load(FSOUND_FREE, "romfs:/sound/hitenemy.raw",0,0,0); //shot 1
    sons[8] = FSOUND_Sample_Load(FSOUND_FREE, "romfs:/sound/killenemy.raw",0,0,0); // shot 2
    sons[9] = FSOUND_Sample_Load(FSOUND_FREE, "romfs:/sound/surprise.raw",0,0,0); // surprise
    sons[10] = FSOUND_Sample_Load(FSOUND_FREE, "romfs:/sound/monte.raw",0,0,0); // monte
    sons[11] = FSOUND_Sample_Load(FSOUND_FREE, "romfs:/sound/descend.raw",0,0,0); // descend
    sons[12] = FSOUND_Sample_Load(FSOUND_FREE, "romfs:/sound/chute.raw",0,0,0); // chute
    sons[13] = FSOUND_Sample_Load(FSOUND_FREE, "romfs:/sound/item.raw",0,0,0); // item
    sons[14] = FSOUND_Sample_Load(FSOUND_FREE, "romfs:/sound/rupee.raw",0,0,0); // rubis
    sons[15] = FSOUND_Sample_Load(FSOUND_FREE, "romfs:/sound/heart.raw",0,0,0); // coeur
    sons[16] = FSOUND_Sample_Load(FSOUND_FREE, "romfs:/sound/bomb.raw",0,0,0); // bombe
    sons[17] = FSOUND_Sample_Load(FSOUND_FREE, "romfs:/sound/textnext.raw",0,0,0); // suite texte
    sons[18] = FSOUND_Sample_Load(FSOUND_FREE, "romfs:/sound/textend.raw",0,0,0); // fin texte
    sons[19] = FSOUND_Sample_Load(FSOUND_FREE, "romfs:/sound/happy.raw",0,0,0); // trouve objet
    sons[20] = FSOUND_Sample_Load(FSOUND_FREE, "romfs:/sound/door.raw",0,0,0); // ouvre porte
    sons[21] = FSOUND_Sample_Load(FSOUND_FREE, "romfs:/sound/pics.raw",0,0,0); // pics contre mur
    sons[22] = FSOUND_Sample_Load(FSOUND_FREE, "romfs:/sound/sword.raw",0,0,0); // épée
    sons[23] = FSOUND_Sample_Load(FSOUND_FREE, "romfs:/sound/SwordCharging.raw",0,0,0); // chargée
    sons[24] = FSOUND_Sample_Load(FSOUND_FREE, "romfs:/sound/Sword360.raw",0,0,0); // spin
    sons[25] = FSOUND_Sample_Load(FSOUND_FREE, "romfs:/sound/shoot.raw",0,0,0); // flèche
    sons[26] = FSOUND_Sample_Load(FSOUND_FREE, "romfs:/sound/hookshot.raw",0,0,0); // grappin
    sons[27] = FSOUND_Sample_Load(FSOUND_FREE, "romfs:/sound/stamp.raw",0,0,0); // pose bombe
    sons[28] = FSOUND_Sample_Load(FSOUND_FREE, "romfs:/sound/magic.raw",0,0,0); // magie
    sons[29] = FSOUND_Sample_Load(FSOUND_FREE, "romfs:/sound/burn.raw",0,0,0); // brûle
    sons[30] = FSOUND_Sample_Load(FSOUND_FREE, "romfs:/sound/hammer.raw",0,0,0); // marteau
    sons[31] = FSOUND_Sample_Load(FSOUND_FREE, "romfs:/sound/plouf.raw",0,0,0); // plouf
    sons[32] = FSOUND_Sample_Load(FSOUND_FREE, "romfs:/sound/danger.raw",0,0,0); // danger
    sons[33] = FSOUND_Sample_Load(FSOUND_FREE, "romfs:/sound/hurt.raw",0,0,0); // link se blesse
    sons[34] = FSOUND_Sample_Load(FSOUND_FREE, "romfs:/sound/porte.raw",0,0,0); // porte objet
    sons[35] = FSOUND_Sample_Load(FSOUND_FREE, "romfs:/sound/lance.raw",0,0,0); // lance objet
    sons[36] = FSOUND_Sample_Load(FSOUND_FREE, "romfs:/sound/casse.raw",0,0,0); // casse objet
    sons[37] = FSOUND_Sample_Load(FSOUND_FREE, "romfs:/sound/charge.raw",0,0,0); // charge magie
    sons[38] = FSOUND_Sample_Load(FSOUND_FREE, "romfs:/sound/buisson.raw",0,0,0); // coupe buisson
    sons[39] = FSOUND_Sample_Load(FSOUND_FREE, "romfs:/sound/pousse.raw",0,0,0); // pousse caisse
    
}

void Audio::freeSounds() {
    if (SOUND) {
        for (int i = 0; i < 40; i++) FSOUND_Sample_Free(sons[i]);
        delete[] sons;
    }
}

void Audio::playSound(int id, int chl) {
    if (SOUND) //Mix_PlayChannel(chl,sons[id],0);
        FSOUND_PlaySound(chl, sons[id]);
}

void Audio::playMusic(int id) {
    if (SOUND) {
        if (musiqueId != id) {
            musiqueId = id;            
            if (specialId == 0) {
                FMUSIC_StopSong(music);
                FMUSIC_FreeSong(music);
                music = choixMusique(id);
                if (previous_volume == -1) previous_volume = FMUSIC_GetMasterVolume(music);
                FMUSIC_SetMasterVolume(music, volume);
                FMUSIC_SetLooping(music, 1);
                FMUSIC_PlaySong(music);
                specialId = 0;
            }
		}
    }
}

void Audio::stopMusic() {
    if (SOUND) FMUSIC_StopSong(music);
}

void Audio::replayMusic() {
    if (SOUND) FMUSIC_PlaySong(music);
}

FMUSIC_MODULE* Audio::choixMusique(int id) {
    switch (id) {
        case 1 : return FMUSIC_LoadSong("romfs:/music/Foret.raw");
        case 2 : return FMUSIC_LoadSong("romfs:/music/Plaine.raw");
        case 3 : return FMUSIC_LoadSong("romfs:/music/Lac.raw");
        case 4 : return FMUSIC_LoadSong("romfs:/music/Mont.raw");
        case 5 : return FMUSIC_LoadSong("romfs:/music/Desert.raw");
        case 6 : return FMUSIC_LoadSong("romfs:/music/Ombre.raw");
        case 7 : return FMUSIC_LoadSong("romfs:/music/Feu.raw");
        case 8 : return FMUSIC_LoadSong("romfs:/music/Cocorico.raw");
        case 9 : return FMUSIC_LoadSong("romfs:/music/Chateau.raw");
        case 10 : case 11 : return FMUSIC_LoadSong("romfs:/music/Secret.raw");
        case 12 : case 13 : case 14 : return FMUSIC_LoadSong("romfs:/music/Donjon1.raw");
        case 15 : return FMUSIC_LoadSong("romfs:/music/DDesert.raw");
        case 16 : case 17 : case 18 : return FMUSIC_LoadSong("romfs:/music/Donjon2.raw");
        case 19 : return FMUSIC_LoadSong("romfs:/music/DSecret.raw");
        case 20 : return FMUSIC_LoadSong("romfs:/music/DFinal.raw");
        case 21 : return FMUSIC_LoadSong("romfs:/music/Home.raw");
        case 22 : case 23 : return FMUSIC_LoadSong("romfs:/music/Cave.raw");
        case 24 : case 25 : case 26 : case 27 : return FMUSIC_LoadSong("romfs:/music/Fee.raw"); break;
//case 28 : case 32 : case 35 : case 36 : case 38 : case 41 : case 42 : case 43 : case 44 :
//    return FMUSIC_LoadSong("romfs:/music/Maison.raw"); break;
        case 29 : case 37 : case 40 : return FMUSIC_LoadSong("romfs:/music/Potion.raw"); break;
        case 30 : return FMUSIC_LoadSong("romfs:/music/Bar.raw"); break;
        case 31 : return FMUSIC_LoadSong("romfs:/music/Magasin.raw"); break;
        case 33 : case 34 : case 39 : return FMUSIC_LoadSong("romfs:/music/Jeu.raw"); break;
        case 45 : return FMUSIC_LoadSong("romfs:/music/Titre.raw"); break;
        case 46 : return FMUSIC_LoadSong("romfs:/music/Selection.raw"); break;
        case 47 : return FMUSIC_LoadSong("romfs:/music/Debut.raw"); break;
        default : return FMUSIC_LoadSong("romfs:/music/Maison.raw");
    }
 return 0;
}

void Audio::playSpecial(int id) {
    if (SOUND) {
        if (specialId != id) {
            FMUSIC_StopSong(music);
            FMUSIC_FreeSong(music);
            music = choixSpecial(id);
            if (previous_volume == -1) previous_volume = FMUSIC_GetMasterVolume(music);
            FMUSIC_SetMasterVolume(music, volume);
            FMUSIC_SetLooping(music, 1);
            FMUSIC_PlaySong(music);
            specialId=id;
        }    
    }
}

void Audio::stopSpecial() {
    if (!specialId) return;
    int tmp = musiqueId;
    musiqueId = 0;
    specialId = 0;
    playMusic(tmp);
}

FMUSIC_MODULE* Audio::choixSpecial(int id) {

    switch (id) {
        case 1 : return FMUSIC_LoadSong("romfs:/music/Boss.raw");
        case 2 : return FMUSIC_LoadSong("romfs:/music/Mort.raw");
        case 3 : return FMUSIC_LoadSong("romfs:/music/Epee.raw");
        case 4 : return FMUSIC_LoadSong("romfs:/music/BossF.raw");
        case 5 : return FMUSIC_LoadSong("romfs:/music/Fin.raw");
        default : return FMUSIC_LoadSong("romfs:/music/Boss.raw");
    }
 return NULL;
}


/*

    Zelda Return of the Hylian

    Copyright (C) 2005-2008  Vincent Jouillat

    Please send bugreports with examples or suggestions to www.zeldaroth.fr

*/

//#include <SDL/SDL.h>
//#include <SDL/SDL_image.h>
#include "os_3ds.h"

#include "Texte.h"
#include "Menu.h"
#include "Joueur.h"
#include "Monde.h"
#include "Projectile.h"
#include "Jeu.h"
#include "3ds/gfx_3ds.h"

Texte::Texte(Jeu* jeu) : gpJeu(jeu), vitesse(40), av(0), x(0), y(0), w(0), h(0),
id(0), idsuiv(0), def(false), cadre(false), texte(""), buffer("") {
    lastAnimTime = SDL_GetTicks();
    imageFont = IMG_Load("romfs:/images/texte/font.png");
    SDL_SetColorKey(imageFont,SDL_SRCCOLORKEY,SDL_MapRGB(imageFont->format,0,0,255));
    imageCoeur = IMG_Load("romfs:/images/menu/coeur.png");
    SDL_SetColorKey(imageCoeur,SDL_SRCCOLORKEY,SDL_MapRGB(imageCoeur->format,0,0,255));
}

Texte::~Texte() {
    SDL_FreeSurface(imageFont);
    SDL_FreeSurface(imageCoeur);
}

void Texte::chercheText() {
    ostringstream os;
    int tmp;
	
switch(getLanguage()) {

case 2: // French

    switch (id) {
        case 1  : texte = "Tu as trouvé un bouclier !!!**Ta défense augmente d'un point !"; break;
        case 2  : texte = "Tu as trouvé une épée !!!**Tu vas pouvoir te défendre face aux monstres avec la touche B !!!*Maintiens B enfoncé pour charger une attaque cyclone !!!"; break;
        case 3  : 
            tmp = 4-(gpJeu->getJoueur()->nbQuarts()%4);
            os << tmp;
            texte = "Tu as trouvé un quart de coeur !!!";
            if (tmp<4) buffer = "Encore "+os.str()+" et tu gagneras un coeur supplémentaire !!!";
            else buffer = "Ton nombre de coeurs vient d'augmenter !!!";
            break;
        case 11 : texte = "N : Maison de Link*O : Plaine d'Hyrule*E : Temple de la Forêt"; break;
        case 12 : texte = "Temple de la Montagne**Déconseillé aux cardiaques, peureux, et aux âmes sensibles en général."; break;
        case 13 : texte = "O : Entrée du Désert*N : Temple de la Montagne"; break;
        case 14 : texte = "N : Temple de la Montagne*S : Plaine d'Hyrule"; break;
        case 15 : texte = "N : Mont du Péril"; break;
        case 16 : texte = "E : Entrée de la Forêt"; break;
        case 17 : texte = "S : Lac Hylia*E : Cimetière Hanté"; break;
        case 18 : texte = "S : Vers le Village Cocorico"; break;
        case 19 : texte = "N : Village Cocorico"; break;
        case 20 : texte = "N : Plaine d'Hyrule"; break;
        case 21 : texte = "O : Terres de Feu"; break;
        case 22 : texte = "E : Village de l'Ombre*- Déconseillé aux vivants -"; break;
        case 23 : texte = "Temple des Ténèbres**Si vous êtes un fantôme et que vous cherchez un job, venez plutôt chômer ici."; break;
        case 24 : texte = "N : Village de l'Ombre*O : Lac Hylia"; break;
        case 25 : texte = "N : Cimetière Hanté**Entrée interdite"; break;
        case 26 : texte = "Village de l'Ombre"; break;
        case 27 : texte = "Je vends un flacon pour 100 rubis, ça t'intéresse ?*              OUI ?            non  "; 
            if ((gpJeu->getJoueur()->hasBouteille(0) && gpJeu->getZone()==8)
            || (gpJeu->getJoueur()->hasBouteille(1) && gpJeu->getZone()==5)
            || (gpJeu->getJoueur()->hasBouteille(2) && gpJeu->getZone()==6)) {id=32; chercheText();break;}
            if (gpJeu->getJoueur()->getRubis() + gpJeu->getJoueur()->getBoostRubis()<100) idsuiv=29;
            else idsuiv=30;
            break;
        case 28 : texte = "Je vends un flacon pour 100 rubis, ça t'intéresse ?*              oui              NON ?"; idsuiv=31; break;
        case 29 : texte = "Désolé, mais tu n'as pas assez de rubis."; break;
        case 30 : texte = "Merci beaucoup, voilà ton flacon."; break;
        case 31 : texte = "Reviens me voir si tu changes d'avis."; break;
        case 32 : texte = "Désolé, c'était mon seul flacon."; break;
        case 33 : texte = "Tu as trouvé un flacon vide !!!**Tu vas pouvoir t'en servir pour stocker des potions !!!"; break;
        case 35 : texte = "Les habitants de ce village ont un dialecte très particulier, je n'y comprends rien..."; break;
        case 36 : texte = "@+@+@+@+@+@+@@@+*@=++=@=+@=+@=+=@*+@+@+@+=+="; break;
        case 37 : texte = "Obtiens une autorisation du chef si tu veux vraiment passer!!!"; break;
        case 38 : texte = "Le chef t'as autorisé à passer ??? Grrrr... Alors écarte toi!!!"; break;
        case 39 : texte = "Ne traîne pas dans mes pattes !!!"; break;
        case 40 : texte = "On n'a pas l'habitude de voir des visiteurs par ici..."; break;
        case 41 : texte = "Les monstres du coin ne craignent pas grand chose, heureusement qu'ils n'attaquent que les humains."; break;
        case 42 : texte = "C'est vrai ?*Tu comprends ce que je dis ?"; break;
        case 43 : texte = "N : Désert et Village Gerudo"; break;
        case 44 : texte = "S : Lac Hylia*O : Château d'Hyrule"; break;
        case 45 : texte = "Village Cocorico"; break;
        case 46 : texte = "O : Château d'Hyrule"; break;
        case 47 : texte = "Quelle belle journée !!!"; break;
        case 48 : texte = "Pourquoi ai-je accepté de m'occuper de ces saletés ???*J'ai horreur des poules !!!"; break;
        case 49 : texte = "On trouve des tas de choses dans les boutiques du village."; break;
        case 50 : texte = "S : Village Cocorico*O : Désert*E : Mont du Péril"; break;
        case 51 : texte = "Village Gerudo"; break;
        case 52 : texte = "Perdu dans le désert ?*Vous êtes ici :*                        X"; break;
        case 54 : texte = "S : Vers le Château d'Hyrule"; break;
        case 55 : texte = "Temple du Désert**Venez essayer nos bains.*(eau non-potable)"; break;
        case 56 : texte = "Temple de la Forêt**Sauvez les arbres, mangez du castor!!!"; break;
        case 57 : texte = "Temple du Lac**Bonnet de bain obligatoire."; break;
        case 58 : texte = "Temple de Glace**Le personnel tient à rappeler qu'un donjon n'est pas une patinoire."; break;
        case 59 : texte = "As-tu été voir le forgeron de ce village ?*On dit qu'il est très doué pour améliorer l'équipement des aventuriers, beaucoup de monde lui rend visite."; break;
        case 60 : texte = "Le temple se trouve au fin fond du désert, à la place d'une ancienne oasis."; break;
        case 61 : texte = "S : Terres de Feu*E : Village Cocorico"; break;
        case 62 : texte = "Château d'Hyrule"; break;
        case 63 : texte = "E : Village Cocorico"; break;
        case 64 : texte = "O : Roc de la Tortue*E : Lac Hylia"; break;
        case 65 : texte = "Temple Caché**Ici repose la lame purificatrice, Excalibur."; break;
        case 66 : texte = "N : Vers le cimetière hanté"; break;
        case 67 : texte = "Tu as trouvé la carte du donjon !!!*Appuie sur L et Y pour voir le plan."; break;
        case 68 : texte = "Tu as trouvé la boussole !!!*Tu peux localiser le boss et les coffres sur le plan."; break;
        case 69 : texte = "Tu as trouvé la clé du boss !!!"; break;
        case 70 : texte = "Tu as trouvé une petite clé !!!*Approche une porte pour l'ouvrir."; break;
        case 71 : texte = "Tu as trouvé les gants !!!*Utilise-les pour soulever des objets en les équipant ou en appuyant sur X."; break;
        case 72 : 
            texte = "Tu as trouvé un cristal magique !!!"; 
            tmp = 7-gpJeu->getJoueur()->nbCristaux();
            os << tmp;
            if (tmp==6 && !gpJeu->getJoueur()->getAvancement()) buffer = "A quoi pourra-t-il bien servir ?";
            else if (tmp>2) buffer = "Il en reste encore "+os.str()+" à trouver !!!";
            else if (tmp==2) buffer = "Plus que 2 à trouver !!!";
            else if (tmp==1) buffer = "Il ne t'en manque plus qu'un seul !!!";
            else if (tmp==0) buffer = "Tu as tous les cristaux, cours au château sauver Zelda !!!";
            break;
        case 80 : texte = "Tu as trouvé le grappin !!!*Utilise-le pour franchir des obstacles."; break;
        case 81 : texte = "Tu as trouvé la lanterne !!!*Utilise-la pour faire jaillir des flammes."; break;
        case 82 : texte = "Tu as trouvé les palmes !!!*Equipe-les pour aller dans l'eau."; break;
        case 83 : texte = "Tu as trouvé le marteau magique !!!*Utilise-le pour écraser les obstacles."; break;
        case 84 : texte = "Tu as trouvé le baton de feu !!!*Tu peux désormais lancer de puissantes flammes à distance."; break;
        case 85 : texte = "Tu as trouvé le baton de glace !!!*Utilise-le pour tout congeler à distance."; break;
        case 86 : texte = "Tu as trouvé Excalibur !!!*Même Ganon ne devrait pas pouvoir résister à sa puissance !!! (en théorie)"; break;
        case 87 : texte = "Félicitation Link, tu as réussi à venir me délivrer!!!***Allons vite trouver Ganon, il faut impérativement lui reprendre la Triforce !"; break;
        case 89 : texte = "Le passage secret derrière la salle du trône mène à Ganon. Dépêchons-nous !"; break;
        case 90 : texte = "Nous y sommes presque, suis-moi !"; break;
        case 91 : texte = "J'ai bien peur que tu ne puisses jamais vaincre Ganon avec tes armes actuelles...*Va parler au chef du village Cocorico, je suis sûre qu'il trouvera une solution."; break;
        case 92 : texte = "Ganon est juste derrière cette porte, je vais soigner tes blessures."; break;
        case 93 : texte = "Ganon est toujours quelque part dans le château."; break;
        case 94 : texte = "Tu devrais attendre la princesse Zelda !!!"; break;
        case 95 : texte = "Tu n'aurais pas l'étrange sentiment d'oublier quelqu'un par hasard ???"; break;
        case 96 : texte = "Zelda t'attend!!!"; break;
        case 97 : texte = "Tu as trouvé la Triforce!!!"; break;
        case 98 : texte = "Tu as trouvé le livre de Mudora!!!**Tu comprends désormais le Hylien ancien!!!"; break;
        case 99 : texte = "Félicitation Link, tu as réussi à me trouver. En récompense, je t'offre le médaillon de Din, il augmente ta défense d'un point."; break;
        case 100 : texte = "Félicitation Link, tu as réussi à me trouver. En récompense, je t'offre le médaillon de Nayru, il augmente ta défense de deux points!!!"; break;
        case 101 : texte = "..."; break;
        case 102 : texte = "Tu as obtenu un médaillon magique !!!**Ta défense vient d'augmenter!!!"; break;
        case 103 : texte = "Félicitation Link, tu as réussi à me trouver. Pour te récompenser, je vais doubler la capacité de ta réserve de magie!!!"; break;
        case 104 : texte = "Ta réserve de magie a doublé!!!"; break;
        case 105 : texte = "Reviens me voir quand tu auras un flacon vide  et je te vendrai une potion rouge qui restaure l'énergie."; break;
        case 106 : texte = "Une potion rouge pour 60 rubis, ça t'intéresse?*              OUI ?            non  "; 
            if (gpJeu->getJoueur()->hasBouteille(0)!=1
            && gpJeu->getJoueur()->hasBouteille(1)!=1
            && gpJeu->getJoueur()->hasBouteille(2)!=1) {id=105; chercheText();break;}
            if (gpJeu->getJoueur()->getRubis() + gpJeu->getJoueur()->getBoostRubis()<60) idsuiv=29;
            else idsuiv=108; break;
        case 107 : texte = "Une potion rouge pour 60 rubis, ça t'intéresse?*              oui              NON ?"; break;
        case 108 : texte = "Merci, voilà ta potion.";break;
        case 109 : texte = "Tu as obtenu une potion rouge !!!*Bois-la pour restaurer ton énergie!!!"; break;
        case 110 : texte = "Tu as obtenu une potion verte !!!*Bois-la pour restaurer ta magie!!!"; break;
        case 111 : texte = "Tu as obtenu une potion bleue !!!*Bois-la pour restaurer ton énergie et ta magie!!!"; break;
        case 112 : texte = "Bonjour, je vous sers quelque chose ?"; break;
        case 113 : texte = "Hic! On raconte qu'une fée donnerait des objets enchantés... Hic! ...aux aventuriers qui la  trouveraient... Hic!!!"; break;
        case 114 : texte = "Un coeur pour 10 rubis, ok ?**              OUI ?            non  "; 
            if (gpJeu->getJoueur()->getVie()+gpJeu->getJoueur()->getBoostVie()
            == gpJeu->getJoueur()->getVieMax()) {id=128; chercheText();break;}
            if (gpJeu->getJoueur()->getRubis() + gpJeu->getJoueur()->getBoostRubis()<10) idsuiv=29;
            else idsuiv=129; break;
        case 115 : texte = "Un coeur pour 10 rubis, ok ?**              oui              NON ?"; break;
        case 116 : texte = "Un peu de magie pour 20 rubis, ok ?**              OUI ?            non  "; 
            if (gpJeu->getJoueur()->getMagie()+gpJeu->getJoueur()->getBoostMagie()
            == gpJeu->getJoueur()->getMagieMax() || !gpJeu->getJoueur()->hasObjet(O_LANTERNE)) {
                id=128; chercheText();break;}
            if (gpJeu->getJoueur()->getRubis() + gpJeu->getJoueur()->getBoostRubis()<20) idsuiv=29;
            else idsuiv=129; break;
        case 117 : texte = "Un peu de magie pour 20 rubis, ok ?**              oui              NON ?"; break;
        case 118 : texte = "Beaucoup de magie pour 30 rubis, ok ?**              OUI ?            non  "; 
            if (gpJeu->getJoueur()->getMagie()+gpJeu->getJoueur()->getBoostMagie()
            == gpJeu->getJoueur()->getMagieMax() || !gpJeu->getJoueur()->hasObjet(O_LANTERNE)) {
                id=128; chercheText();break;}
            if (gpJeu->getJoueur()->getRubis() + gpJeu->getJoueur()->getBoostRubis()<30) idsuiv=29;
            else idsuiv=129; break;
        case 119 : texte = "Beaucoup de magie pour 30 rubis, ok ?**              oui              NON ?"; break;
        case 120 : texte = "5 flèches pour 30 rubis, ok ?**              OUI ?            non  "; 
            if (gpJeu->getJoueur()->getFleche() == gpJeu->getJoueur()->getFlecheMax() 
            || !gpJeu->getJoueur()->hasObjet(O_ARC)) {id=128; chercheText();break;}
            if (gpJeu->getJoueur()->getRubis() + gpJeu->getJoueur()->getBoostRubis()<30) idsuiv=29;
            else idsuiv=129; break;
        case 121 : texte = "5 flèches pour 30 rubis, ok ?**              oui              NON ?"; break;
        case 122 : texte = "Une bombe pour 30 rubis, ok ?**              OUI ?            non  "; 
            if (gpJeu->getJoueur()->getBombe() == gpJeu->getJoueur()->getBombeMax() 
            || !gpJeu->getJoueur()->hasObjet(O_SAC_BOMBES)) {id=128; chercheText();break;}
            if (gpJeu->getJoueur()->getRubis() + gpJeu->getJoueur()->getBoostRubis()<30) idsuiv=29;
            else idsuiv=129; break;
        case 123 : texte = "Une bombe pour 30 rubis, ok ?**              oui              NON ?"; break;
        case 124 : texte = "Un arc pour 1000 rubis, ok ?**              OUI ?            non  "; 
            idsuiv=29; break;
        case 125 : texte = "Un arc pour 1000 rubis, ok ?**              oui              NON ?"; break;
        case 126 : texte = "Un arc pour 50 rubis, ok ?**              OUI ?            non  "; 
            if (gpJeu->getJoueur()->getRubis() + gpJeu->getJoueur()->getBoostRubis()<50) idsuiv=29;
            else idsuiv=129; break;
        case 127 : texte = "Un arc pour 50 rubis, ok ?**              oui              NON ?"; break;
        case 128 : texte = "Tu n'as pas besoin de ça maintenant."; break;
        case 129 : texte = "Merci."; break;
        case 130 : texte = "Tu as obtenu l'arc !!!*Utilise-le pour atteindre des cibles éloignées."; break;
        case 131 : texte = "Choisis ce que tu veux."; break;
        case 132 : texte = "Hein ???*Tu es en mission pour le chef ???*Bon alors je vais te faire une réduction sur l'arc..."; break;
        case 133 : texte = "Bonjour Link, je suis le chef de ce village, je me doutais que tu passerais."; idsuiv=134; break;
        case 134 : texte = "Depuis ce matin des monstres sont apparus un peu partout à Hyrule, j'ai voulu aller en parler à la princesse Zelda, mais comme tu l'as peut-être déjà vu, un puissant sortilège bloque l'accès au château..."; idsuiv=136; break;
        case 136 : texte = "Tu dis que Ganon est responsable de tout ça ? Qu'il a volé la Triforce et qu'il détient la princesse Zelda prisonnière ?"; idsuiv=137; break;
        case 137 : texte = "Mmmm... la situation est plus grave que je le croyais..."; idsuiv=138; break;
        case 138 : texte = "Il faut agir au plus vite, tu dois affronter Ganon à nouveau!"; idsuiv=139; break;
        case 139 : texte = "Comment entrer dans le château ? J'ai peut-être une idée..."; idsuiv=140; break;
        case 140 : texte = "Comme tu le sais, le pouvoir des 7 sages a été enfermé dans 7 cristaux par le sorcier Aghanim alors qu'il tentait de lever le sceau qui confinait Ganon au monde des ténèbres."; idsuiv=141; break;
        case 141 : texte = "Or, bien que tu aies ressuscité les descendants des sages après avoir vaincu Ganon et trouvé la Triforce, ces cristaux ont conservé leur puissance."; idsuiv=142; break;
        case 142 : texte = "Ils ont été cachés au fin fond des 7 temples, réunis-les tous et tu pourras passer outre le sortilège de Ganon."; idsuiv=143; break;
        case 143 : texte = "Ce ne sera toutefois pas chose aisé, Ganon a certainement dû envoyer ses meilleurs éléments protéger ces cristaux..."; idsuiv=144; break;
        case 144 : texte = "Une dernière chose, tu n'iras pas loin avec ton équipement actuel. Va voir le marchand d'armes et dis-lui que tu viens de ma part. Il fera sûrement un geste pour toi."; break;
        case 145 : texte = "Va voir le marchand d'armes et dis-lui que tu viens de ma part, il fera certainement un geste pour toi."; break;
        case 146 : texte = "Il te faut rassembler les 7 cristaux pour avoir une chance de sauver Zelda et Hyrule..."; break;
        case 147 : texte = "Tu as trouvé les 7 cristaux ? Alors rends-toi au château, tu dois avant tout délivrer la princesse, elle saura mieux que moi comment sauver Hyrule."; break;
        case 148 : texte = "Tes armes sont sans effet sur Ganon ? Dans ce cas tu dois trouver Excalibur. On raconte qu'elle a été dissimulée dans un temple caché.*On dit aussi que le chemin menant à la lame purificatrice passe par un village peuplé de monstres...*Je me demande si un tel village existe..."; break;
        case 150 : texte = "Toutefois j'ai bien peur que ton équipement actuel soit insuffisant pour accéder à ce temple, tu devrais peut-être passer voir le forgeron du village Gerudo..."; break;
        case 151 : texte = "Il est temps d'affronter Ganon et de lui reprendre la Triforce!"; break;
        case 152 : texte = "Hey! Il faut payer pour ouvrir un de mes coffres!!!"; break;
        case 153 : texte = "Un de ces coffres contient un quart de coeur, tu veux tenter ta chance pour 10 rubis ?*Tu ne pourras ouvrir qu'un seul coffre, ok ?*              OUI ?            non  "; 
            if (gpJeu->getJoueur()->getRubis() + gpJeu->getJoueur()->getBoostRubis()<10) idsuiv=29; break;
        case 154 : texte = "Un de ces coffres contient un quart de coeur, tu veux tenter ta chance pour 10 rubis ?*Tu ne pourras ouvrir qu'un seul coffre, ok ?*              oui              NON ?"; break;
        case 155 : texte = "Choisis un coffre."; break;
        case 156 : texte = "Je n'ai plus de prix à mettre en jeu, désolé."; break;
        case 157 : texte = "Perdu! Ce coffre est vide."; break;
        case 158 : texte = "Bonjour! Si vous êtes venu voir le forgeron, il habite un peu plus loin."; break;
        case 159 : texte = "Bonjour Link, je suis le chef de ce village.*Tu devrais aller voir le forgeron et lui présenter ce que tu veux qu'il reforge dans ton équipement.";
            buffer = "J'ai été informé de ta quête, alors je me suis arrangé avec lui.*Tout sera gratuit pour toi."; break;
        case 160 : texte = "Tu n'as pas besoin de boire cette potion maintenant."; break;
        case 161 : texte = "Une potion verte pour 40 rubis, ça t'intéresse?*              OUI ?            non  "; 
            if (gpJeu->getJoueur()->hasBouteille(0)!=1
            && gpJeu->getJoueur()->hasBouteille(1)!=1
            && gpJeu->getJoueur()->hasBouteille(2)!=1) {id=163; chercheText();break;}
            if (gpJeu->getJoueur()->getRubis() + gpJeu->getJoueur()->getBoostRubis()<40) idsuiv=29;
            else idsuiv=108; break;
        case 162 : texte = "Une potion verte pour 40 rubis, ça t'intéresse?*              oui              NON ?"; break;
        case 163 : texte = "Reviens me voir quand tu auras un flacon vide et je te vendrai une potion verte qui restaure la magie."; break;
        case 164 : texte = "Reviens plus tard, je suis débordé!"; break;
        case 165 : texte = "Puis-je faire quelque chose pour toi ?";
            if (gpJeu->getJoueur()->getEpee()==1) {id=166; chercheText();break;}
            if (gpJeu->getJoueur()->hasObjet(O_GANTS)==1 && gpJeu->getJoueur()->getObjet()==8) {
                id=169; chercheText();break;}
            if (gpJeu->getJoueur()->hasObjet(O_ARC)==2 && gpJeu->getJoueur()->getObjet()==0 
            && gpJeu->getJoueur()->nbCristaux()==7) {
                id=172; chercheText();break;}
            break;
        case 166 : texte = "Veux-tu que je reforge ton épée ?**              OUI ?            non  "; break;
        case 167 : texte = "Veux-tu que je reforge ton épée ?**              oui              NON ?"; break;
        case 168 : texte = "Ton épée a triplé de puissance et peut désormais renvoyer certaines attaques magiques, trop bien !!!*"; break;
        case 169 : texte = "Veux-tu que j'améliore tes gants ?**              OUI ?            non  "; break;
        case 170 : texte = "Veux-tu que j'améliore tes gants ?**              oui              NON ?"; break;
        case 171 : texte = "La puissance de tes gants va te permettre de soulever de lourds rochers."; break;
        case 172 : texte = "Veux-tu que j'améliore ton arc ?**              OUI ?            non  "; break;
        case 173 : texte = "Veux-tu que j'améliore ton arc ?**              oui              NON ?"; break;
        case 174 : texte = "Ton arc tire désormais des flèches d'argent d'une puissance dévastatrice!!!"; break;
        case 175 : texte = "Reviens me voir quand tu auras un flacon vide et je te vendrai une potion bleue qui restaure l'énergie et la magie."; break;
        case 176 : texte = "Une potion bleue pour 160 rubis, ça t'intéresse?*              OUI ?            non  "; 
            if (gpJeu->getJoueur()->hasBouteille(0)!=1
            && gpJeu->getJoueur()->hasBouteille(1)!=1
            && gpJeu->getJoueur()->hasBouteille(2)!=1) {id=175; chercheText();break;}
            if (gpJeu->getJoueur()->getRubis() + gpJeu->getJoueur()->getBoostRubis()<160) idsuiv=29;
            else idsuiv=108; break;
        case 177 : texte = "Une potion bleue pour 160 rubis, ça t'intéresse?*              oui              NON ?"; break;
        case 178 : texte = "Notre village a été fondé pour empêcher les aventuriers imprudents d'accéder au temple, je ne donne pas l'autorisation de passer à n'importe qui."; break;
        case 179 : texte = "Mmmm... Tu as déjà trouvé 4 cristaux ?*Je dois avouer que tu m'impressionnes...";
            buffer = "Bon c'est entendu, tu as l'autorisation de passer par le cimetière, à tes risques et périls."; break;
        case 181 : texte = "Va parler au garde au nord du village, il te laissera passer."; break;
        case 182 : texte = "Je vois que tu es toujours vivant...*Etonnant."; break;
        case 183 : texte = "Le temple de l'ombre constitue le point de départ du chemin menant à un objet de légende. Ce village ne sert qu'à dissuader les aventuriers de trop s'en approcher."; break;
        case 184 : texte = "Ainsi tu as trouvé l'Epée ?*Mais ne crois pas qu'elle soit à toi pour autant.";
            buffer = "Excalibur fait partie du patrimoine le plus précieux d'Hyrule avec la Triforce, tu devras la ramener dans son sanctuaire une fois ta mission achevée."; break;
        case 186 : texte = "Les habitants de ce village sont tous des esprits ayant répondu à l'appel de la princesse Zelda."; break;
        case 187 : texte = "Lorsque tu as ramené la paix sur Hyrule et réunifié le monde de la lumière avec la Terre d'Or, Zelda a secrètement demandé aux esprits volontaires de créer ce village pour bloquer";
            buffer = "l'accès à un objet de très grande valeur."; break;
        
        case 189 : texte = "Bienvenue Link!*Laisse-moi t'expliquer ce que je fais ici."; idsuiv=190; break;
        case 190 : texte = "Comme tu l'as certainement remarqué, des monstres sont apparus partout dans Hyrule. C'est la première fois que je vois des monstres depuis que tu as vaincu Ganon."; idsuiv=191; break;
        case 191 : texte = "Et bien j'ai décidé de tous les répertorier."; idsuiv=192; break;
        case 192 : texte = "L'ennui c'est que je suis bien trop peureux pour aller à leur rencontre, alors j'ai besoin de ton aide."; idsuiv=193; break;
        case 193 : texte = "Il faut que tu battes chaque type de monstre et que tu viennes me faire ton rapport."; idsuiv=194; break;
        case 194 : texte = "Je te récompenserai tous les 7 nouveaux types d'ennemis vaincus."; idsuiv=195; break;
        case 195 : texte = "Si tu veux savoir quels ennemis tu as déjà vaincu, appuye sur L et B."; break;
        
        
        case 196 : 
            if (gpJeu->getJoueur()->getCoeur(43)) {
                if (gpJeu->getJoueur()->nbEnnemis() >= 46) {id=205; chercheText();return;}
                else {id=204; chercheText();return;}
            }
            for (int i = 38; i < 44; i++) 
                if (!gpJeu->getJoueur()->getCoeur(i) 
                && (gpJeu->getJoueur()->nbEnnemis()/7)+37>=i) {
                    id=203; chercheText();return;
                }
            tmp = 7-(gpJeu->getJoueur()->nbEnnemis()%7);
            os << tmp;
            if (tmp>1) texte = "Encore "+os.str()+" ennemis différents avant la prochaine récompense.";
            else texte = "Plus qu'un nouvel ennemi avant la prochaine récompense!!!";
            break;
        case 203 : texte = "Accepte ce cadeau pour ta contribution à mon projet :"; break;
        case 204 : texte = "Je suis désolé mais je n'ai plus rien à t'offrir..."; break;
        case 205 : texte = "Je n'en crois pas mes yeux, tu as réussi à vaincre chaque type d'ennemis !!!*Félicitation Link!!!"; break;
        
        case 206 : texte = "                   Niveau 1 :**               Temple de la forêt"; break;
        case 207 : texte = "                   Niveau 2 :**                 Temple de l'eau"; break;
        case 208 : texte = "                   Niveau 3 :**              Temple de la montagne"; break;
        case 209 : texte = "                   Niveau 4 :**                Temple du désert"; break;
        case 210 : texte = "                   Niveau 5 :**                Temple de l'ombre"; break;
        case 211 : texte = "                   Niveau 6 :**                  Temple du feu"; break;
        case 212 : texte = "                   Niveau 7 :**                Temple de la glace"; break;
        case 213 : texte = "                   Niveau ? :**                   Temple secret"; break;
        case 214 : texte = "                 Niveau Final :**                Château d'Hyrule"; break;
        
        case 215 :texte = "                Déjà fatigué ?                  ?                CONTINUER                  ?             Sauvegarder et quitter                        Quitter sans sauvegarder           "; break;
        case 216 :texte = "                Déjà fatigué ?                                   Continuer                     ?          SAUVEGARDER ET QUITTER           ?            Quitter sans sauvegarder           "; break;
        case 217 :texte = "                Déjà fatigué ?                                   Continuer                                Sauvegarder et quitter              ?         QUITTER SANS SAUVEGARDER          ?"; break;
        
        case 223 : texte = "A l'instant où Link toucha la Triforce, les monstres disparurent et la paix revint.**La princesse Zelda prit alors une grande décision: elle toucha à son tour la Triforce pour exaucer son voeu...**La Triforce fut depuis toujours à l'origine des problèmes survenus au Royaume, tôt ou tard un autre être malfaisant aurait fini par la trouver.**A la seconde où la princesse toucha la relique, elle disparue à jamais d'Hyrule.";
            buffer = "Depuis ce jour, le peuple d'Hyrule vécut en paix, sans craindre de nouveaux désastres.**Ainsi s'achève la légende de Zelda, celle de la  Triforce et de la Terre d'Or.**On dit que l'épée Excalibur repose en sûreté quelque part, et qu'elle constitue le dernier souvenir de la quête de Link..."; break;
        case 224 : texte = "Félicitation Link, tu as réussi à me trouver. En récompense, je t'offre le médaillon de Farore, il augmente ta défense d'un point."; break;
        case 225 : texte = "Souhaites-tu sauvegarder ta partie ?**                    ? OUI ?*                      non  "; break;
        case 226 : texte = "Souhaites-tu sauvegarder ta partie ?**                      oui  *                    ? NON ?"; break;
        case 227 : texte = "Partie sauvegardée."; break;
        
        case 228 : texte = "Le Royaume d'Hyrule était en paix depuis que Link, dernier chevalier hylien, avait vaincu le maléfique Ganon et lui avait repris la précieuse Triforce."; idsuiv = 229; break;
        case 229 : texte = "Nul ne sait quel voeu fit Link en touchant la Triforce, mais il eut comme effet de réunifier le monde de la lumière et celui des ténèbres, et ramena à la vie les descendants des 7 sages."; idsuiv = 230; break;
        case 230 : texte = "Link remit ensuite à la princesse Zelda la Triforce et l'épée Excalibur, et le peuple se mit à croire que la paix durerait.*Mais le peuple se trompait..."; idsuiv=231; break;
        case 231 : texte = "Le souhait de Link à la Triforce eu hélas de fâcheux effets. Ganon et ses sbires furent ainsi ressuscités et s'apprêtèrent à attaquer."; idsuiv=232; break;
        case 232 : texte = "Quelque part dans la forêt d'Hyrule, Link dort sans se douter que Ganon est passé à l'attaque, jusqu'à ce qu'une voix familière lui parle dans son sommeil..."; break;
        case 233 : texte = "A l'aide!*A l'aide!*C'est moi! C'est Zelda!*Je te parle par télépathie."; idsuiv = 234; break;
        case 234 : texte = "Je suis retenue prisonnière dans le donjon du château! J'ai besoin de ton aide!*Ganon est de retour, et il a certainement dû trouver la Triforce à l'heure qu'il est..."; idsuiv=235; break;
        case 235 : texte = "Viens vite au château Link, tu es mon seul espoir..."; break;
        case 236 : texte = "AIDE : Appuie sur L et X pour consulter l'aide."; break;
    }

	break; //French

case 3: // German

    switch (id) {
        case 1  : texte = "Du hast ein Schild gefunden!!!**Deine Rüstung steigert sich um einen Punkt!"; break;
        case 2  : texte = "Du hast ein Schwert gefunden!!!**Du kannst jetzt Monster angreifen (drück B)!!!*Halte B um den Spezialhieb aufzuladen!!!"; break;
        case 3  :
            tmp = 4-(gpJeu->getJoueur()->nbQuarts()%4);
            os << tmp;
            texte = "Du hast ein Herzfragment gefunden!!!";
            if (tmp<4) buffer = "Noch "+os.str()+" für ein neues Herz nötig!!!";
            else buffer = "Deine Herzanzahl hat sich gerade erhöht!!!";
            break;
        case 11 : texte = "N: Link's zu Hause*W: Hyrule-Gebiet*O: Wald-Tempel"; break;
        case 12 : texte = "Berg-Tempel**Nichts für Herzkranke, Angsthasen und Schlappschwänze."; break;
        case 13 : texte = "W: Wüste Gerudo*N: Berg-Tempel"; break;
        case 14 : texte = "N: Berg-Tempel*S: Hyrule Gebiet"; break;
        case 15 : texte = "N: Todesschlucht"; break;
        case 16 : texte = "O: Zum Wald"; break;
        case 17 : texte = "S: See Hylia*O: Spuk-Friedhof"; break;
        case 18 : texte = "S: Zum Dorf Kakariko"; break;
        case 19 : texte = "N: Dorf Kakariko"; break;
        case 20 : texte = "N: Hyrule-Gebiet"; break;
        case 21 : texte = "W: Feuer-Land"; break;
        case 22 : texte = "O: Dorf der Schatten*- Nicht für Lebendige -"; break;
        case 23 : texte = "Dunkler Tempel**Bist du ein Geist, der Arbeit sucht? Dann solltest du dich bei uns melden."; break;
        case 24 : texte = "N: Dorf der Schatten*W: See Hylia"; break;
        case 25 : texte = "N: Spuk-Friedhof**Kein Zutritt"; break;
        case 26 : texte = "Dorf der Schatten"; break;
        case 27 : texte = "Ich verkaufe eine Flasche für 100 Rupien, abgemacht?*               JA ?          nein  ";
            if ((gpJeu->getJoueur()->hasBouteille(0) && gpJeu->getZone()==8)
            || (gpJeu->getJoueur()->hasBouteille(1) && gpJeu->getZone()==5)
            || (gpJeu->getJoueur()->hasBouteille(2) && gpJeu->getZone()==6)) {id=32; chercheText();break;}
            if (gpJeu->getJoueur()->getRubis() + gpJeu->getJoueur()->getBoostRubis()<100) idsuiv=29;
            else idsuiv=30;
            break;
        case 28 : texte = "Ich verkaufe eine Flasche für 100 Rupien, abgemacht?*               ja            NEIN ?"; idsuiv=31; break;
        case 29 : texte = "Bedaure, du hast leider nicht genug Rupien"; break;
        case 30 : texte = "Danke, hier ist die Flasche."; break;
        case 31 : texte = "Komm wieder, falls sich deine Meinung geändert hat."; break;
        case 32 : texte = "Das war leider meine einzige Flasche."; break;
        case 33 : texte = "Du hast eine leere Flasche gekauft!!!**Darin kannst du Getränke aufbewahren!!!"; break;
        case 35 : texte = "Die Einwohner dieses Dorfes sprechen einen ungewöhnlichen Dialekt, nix versteh'n..."; break;
        case 36 : texte = "@+@+@+@+@+@+@@@+*@=++=@=+@=+@=+=@*+@+@+@+=+="; break;
        case 37 : texte = "Falls du wirklich hier hindurch willst, musst du erst den Dorfältesten fragen!!!"; break;
        case 38 : texte = "Der Dorfälteste erlaubt dir also zu passieren??? Grrrr... Dann geh doch!!!"; break;
        case 39 : texte = "Geh mir aus dem Weg!!!"; break;
        case 40 : texte = "Normalerweise kommen keine Touristen hier her."; break;
        case 41 : texte = "Einheimische Monster fürchten sich kaum vor etwas. Zum Glück überfallen sie nur Menschen."; break;
        case 42 : texte = "Wirklich?*Verstehst du, was ich sage?"; break;
        case 43 : texte = "N: Wüste und Dorf Gerudo"; break;
        case 44 : texte = "S: See Hylia*W: Schloss Hyrule"; break;
        case 45 : texte = "Dorf Kakariko"; break;
        case 46 : texte = "W: Schloss Hyrule"; break;
        case 47 : texte = "Was für ein schöner Tag!!!"; break;
        case 48 : texte = "Aber warum soll ich auf dieses Viehzeug aufpassen???*Ich hasse Hühner!!!"; break;
        case 49 : texte = "Im Dorfladen kannst du viele nützliche Dinge kaufen."; break;
        case 50 : texte = "S: Dorf Kakariko*W: Wüste Gerudo*O: Todesschlucht"; break;
        case 51 : texte = "Dorf Gerudo"; break;
        case 52 : texte = "Verirrt in der Wüste?*Du bist hier:*                        X"; break;
        case 54 : texte = "S: Zum Schloss Hyrule"; break;
        case 55 : texte = "Wüsten-Tempel**Tritt ein für ein Bad.*(Das Wasser ist nicht zum Trinken da!)"; break;
        case 56 : texte = "Wald-Tempel**Schütze die Bäume, fang die Biber!!!"; break;
        case 57 : texte = "See-Tempel**Badekappen vorgeschrieben!"; break;
        case 58 : texte = "Eis-Tempel**Das Personal möchte dich daran erinnern, dass ein Tempel keine Eissporthalle ist."; break;
        case 59 : texte = "Warst du schon beim Dorfschmied?*Man sagt, er sei sehr begabt, die Ausrüstung für Abenteurer aufzuwerten. Er hat viele Kunden."; break;
        case 60 : texte = "Der Tempel ist in der tiefsten Wüste, in einer antiken Oase."; break;
        case 61 : texte = "S: Feuerland*O: Dorf Kakariko"; break;
        case 62 : texte = "Schloss Hyrule"; break;
        case 63 : texte = "O: Dorf Kakariko"; break;
        case 64 : texte = "W: Schildkrötfelsen*O: See Hylia"; break;
        case 65 : texte = "Geheim-Tempel**Hier ruht das Schwert Excalibur."; break;
        case 66 : texte = "N: Zum Spuk-Friedhof"; break;
        case 67 : texte = "Du hast den Tempelplan gefunden!!!*Drück L und Y, um den Plan anzusehen."; break;
        case 68 : texte = "Du hast den Kompass gefunden!!!*Du kannst nun den Boss und die Truhen auf dem Plan sehen."; break;
        case 69 : texte = "Du hast den Boss-Schlüssel gefunden!!!"; break;
        case 70 : texte = "Du hast einen Schlüssel gefunden!!!*Geh auf eine Tür zu, um diese zu öffnen."; break;
        case 71 : texte = "Du hast die magischen Handschuhe gefunden!!!*Benutz diese (oder drück X) zum Heben/Schieben schwerer Objekte."; break;
        case 72 :
            texte = "Du hast einen Kristall gefunden!!!";
            tmp = 7-gpJeu->getJoueur()->nbCristaux();
            os << tmp;
            if (tmp==6 && !gpJeu->getJoueur()->getAvancement()) buffer = "Für was mag der wohl gut sein...?";
            else if (tmp>2) buffer = "Es müssen noch "+os.str()+" gefunden werden!!!";
            else if (tmp==2) buffer = "Nur noch 2 Exemplare fehlen noch!!!";
            else if (tmp==1) buffer = "Jetzt fehlt nur noch einer!!!";
            else if (tmp==0) buffer = "Du hast jetzt alle 7 Kristalle. Lauf schnell zum Schloss und rette Zelda!!!";
            break;
        case 80 : texte = "Du hast einen Hakenwerfer gefunden!!!*Damit kannst du einige Hindernisse überwinden."; break;
        case 81 : texte = "Du hast die magische Laterne gefunden!!!*Damit kannst du Feuer entfachen."; break;
        case 82 : texte = "Du hast Schwimmflossen gefunden!!!*Damit kannst du endlich im See baden gehen."; break;
        case 83 : texte = "Du hast einen Hammer gefunden!!!*Damit kannst du Hindernisse zu Brei schlagen."; break;
        case 84 : texte = "Du hast den magischen Feuerstab gefunden!!!*Damit kannst du mächtige Feuerbälle schleudern."; break;
        case 85 : texte = "Du hast den magischen Eisstab gefunden!!!*Damit kannst du etwas beliebiges Einfrieren."; break;
        case 86 : texte = "Du hast das Schwert Excalibur gefunden!!!*Selbst Ganon kann nicht seiner Macht standhalten!!! (theoretisch)"; break;
        case 87 : texte = "Endlich Link, du hast mich befreit!!!**Lass uns schnell Ganon finden. Wir müssen unbedingt das Triforce zurück bekommen!"; break;
        case 89 : texte = "Der Geheimgang hinter dem Thron führt zu Ganon. Beeil dich!"; break;
        case 90 : texte = "Wir sind nahe dran, folge mir!"; break;
        case 91 : texte = "Ich befürchte, dass du mit den gegenwärtigen Waffen Ganon nicht besiegen kannst...*Geh nach Karariko und sprich mit dem Dorfältesten. Er findet sicher eine Lösung."; break;
        case 92 : texte = "Ganon ist hinter dieser Tür da.*Ich habe soeben alle deine Wunden geheilt!"; break;
        case 93 : texte = "Ganon ist irgendwo hier im Schloss."; break;
        case 94 : texte = "Du solltest auf Prinzessin Zelda warten!!!"; break;
        case 95 : texte = "Würdest du nicht dieses mulmige Gefühl manchmal loswerden wollen???"; break;
        case 96 : texte = "Zelda wartet auf dich!!!"; break;
        case 97 : texte = "Du hast das Triforce gefunden!!!"; break;
        case 98 : texte = "Du hast das Buch Mudora gefunden!!!**Jetzt kannst du endlich die Sprache der antiken Hylian verstehen!!!"; break;
        case 99 : texte = "Du hast mich gefunden, Link. Zur Belohnung schenke ich dir das Din-Amulett. Dies steigert deine Rüstung um einen Punkt."; break;
        case 100 : texte = "Du hast mich gefunden, Link. Zur Belohnung schenke ich dir das Nayru-Amulett. Dies steigert deine Rüstung um zwei Punkte!!!"; break;
        case 101 : texte = "..."; break;
        case 102 : texte = "Du hast ein magisches Amulett bekommen!!!**Dies steigert deine Rüstung!!!"; break;
        case 103 : texte = "Du hast mich gefunden, Link. Zur Belohnung werde ich deine Zauberkraft verdoppeln!!!"; break;
        case 104 : texte = "Deine Zauberkraft hat sich verdoppelt!!!"; break;
        case 105 : texte = "Komm wieder, wenn du eine leere Flasche hast, dann verkauf ich dir einen roten Trank, der alle Wunden heilt."; break;
        case 106 : texte = "Den roten Heiltrank für 60 Rupien, abgemacht?*               JA ?          nein  ";
            if (gpJeu->getJoueur()->hasBouteille(0)!=1
            && gpJeu->getJoueur()->hasBouteille(1)!=1
            && gpJeu->getJoueur()->hasBouteille(2)!=1) {id=105; chercheText();break;}
            if (gpJeu->getJoueur()->getRubis() + gpJeu->getJoueur()->getBoostRubis()<60) idsuiv=29;
            else idsuiv=108; break;
        case 107 : texte = "Den roten Heiltrank für 60 Rupien, abgemacht?*               ja            NEIN ?"; break;
        case 108 : texte = "Danke. Hier ist der Trank."; break;
        case 109 : texte = "Du hast einen roten Trank bekommen!!!*(Stellt die volle Gesundheit wieder her)"; break;
        case 110 : texte = "Du hast einen grünen Trank bekommen!!!*(Stellt die magischen Kräfte wieder her)"; break;
        case 111 : texte = "Du hast einen blauen Trank bekommen!!!*(Stellt volle Gesundheit und die magischen Kräfte wieder her)"; break;
        case 112 : texte = "Hallo, was möchtest du gerne trinken?"; break;
        case 113 : texte = "-Hick!- Eine Fee flüsterte mir ins Ohr... -Hick!- ...derjenige, der mich findet, bekommt etwas besonderes... -Hick!!!-"; break;
        case 114 : texte = "Ein Herz für 10 Rupien, abgemacht?**               JA ?          nein  ";
            if (gpJeu->getJoueur()->getVie()+gpJeu->getJoueur()->getBoostVie()
            == gpJeu->getJoueur()->getVieMax()) {id=128; chercheText();break;}
            if (gpJeu->getJoueur()->getRubis() + gpJeu->getJoueur()->getBoostRubis()<10) idsuiv=29;
            else idsuiv=129; break;
        case 115 : texte = "Ein Herz für 10 Rupien, abgemacht?**               ja            NEIN ?"; break;
        case 116 : texte = "Kleiner Zaubertrank für 20 Rupien, abgemacht?**               JA ?          nein  ";
            if (gpJeu->getJoueur()->getMagie()+gpJeu->getJoueur()->getBoostMagie()
            == gpJeu->getJoueur()->getMagieMax() || !gpJeu->getJoueur()->hasObjet(O_LANTERNE)) {
                id=128; chercheText();break;}
            if (gpJeu->getJoueur()->getRubis() + gpJeu->getJoueur()->getBoostRubis()<20) idsuiv=29;
            else idsuiv=129; break;
        case 117 : texte = "Kleiner Zaubertrank für 20 Rupien, abgemacht?**               ja            NEIN ?"; break;
        case 118 : texte = "Großer Zaubertrank für 30 Rupien, abgemacht?**               JA ?          nein  ";
            if (gpJeu->getJoueur()->getMagie()+gpJeu->getJoueur()->getBoostMagie()
            == gpJeu->getJoueur()->getMagieMax() || !gpJeu->getJoueur()->hasObjet(O_LANTERNE)) {
                id=128; chercheText();break;}
            if (gpJeu->getJoueur()->getRubis() + gpJeu->getJoueur()->getBoostRubis()<30) idsuiv=29;
            else idsuiv=129; break;
        case 119 : texte = "Großer Zaubertrank für 30 Rupien, abgemacht?**               ja            NEIN ?"; break;
        case 120 : texte = "5 Pfeile für 30 Rupien, abgemacht?**               JA ?          nein  ";
            if (gpJeu->getJoueur()->getFleche() == gpJeu->getJoueur()->getFlecheMax()
            || !gpJeu->getJoueur()->hasObjet(O_ARC)) {id=128; chercheText();break;}
            if (gpJeu->getJoueur()->getRubis() + gpJeu->getJoueur()->getBoostRubis()<30) idsuiv=29;
            else idsuiv=129; break;
        case 121 : texte = "5 Pfeile für 30 Rupien, abgemacht?**               ja            NEIN ?"; break;
        case 122 : texte = "Eine Bombe für 30 Rupien, abgemacht?**               JA ?          nein  ";
            if (gpJeu->getJoueur()->getBombe() == gpJeu->getJoueur()->getBombeMax()
            || !gpJeu->getJoueur()->hasObjet(O_SAC_BOMBES)) {id=128; chercheText();break;}
            if (gpJeu->getJoueur()->getRubis() + gpJeu->getJoueur()->getBoostRubis()<30) idsuiv=29;
            else idsuiv=129; break;
        case 123 : texte = "Eine Bombe für 30 Rupien, abgemacht?**               ja            NEIN ?"; break;
        case 124 : texte = "Ein Bogen für 1000 Rupien, abgemacht?**               JA ?          nein  ";
            idsuiv=29; break;
        case 125 : texte = "Ein Bogen für 1000 Rupien, abgemacht?**               ja            NEIN ?"; break;
        case 126 : texte = "Ein Bogen für 50 Rupien, abgemacht?**               JA ?          nein  ";
            if (gpJeu->getJoueur()->getRubis() + gpJeu->getJoueur()->getBoostRubis()<50) idsuiv=29;
            else idsuiv=129; break;
        case 127 : texte = "Ein Bogen für 50 Rupien, abgemacht?**               ja            NEIN ?"; break;
        case 128 : texte = "Das brauchst du jetzt nicht."; break;
        case 129 : texte = "Danke."; break;
        case 130 : texte = "Du hast den Bogen bekommen!!!*Damit kannst du auf entfernte Ziele schießen."; break;
        case 131 : texte = "Wähle, was du begehrst."; break;
        case 132 : texte = "Was???*Du hast einen Auftrag vom Dorfältesten???*Nun gut, dann gebe ich dir einen Preisnachlass auf den Bogen."; break;
        case 133 : texte = "Hallo Link. Ich bin der Dorfälteste. Ich dachte mir schon, dass du zu mir kommen wirst."; idsuiv=134; break;
        case 134 : texte = "Seit heute früh treiben sich Monster in Hyrule herum. Ich versuchte darüber mit Zelda zu sprechen, aber wie du bereits sagtest, blockiert ein Zauberbann den Eingang zum Schloss..."; idsuiv=136; break;
        case 136 : texte = "Du sagst, Ganon sei Schuld für all dies? Er soll das Triforce gestohlen und Prinzessin Zelda gefangen halten?"; idsuiv=137; break;
        case 137 : texte = "Mmmm... Die Situation ist ernster, als ich dachte..."; idsuiv=138; break;
        case 138 : texte = "Wir müssen rasch handeln. Du musst erneut gegen Ganon kämpfen!"; idsuiv=139; break;
        case 139 : texte = "Wie kommt man ins Schloss? Ich hätte da eine Idee..."; idsuiv=140; break;
        case 140 : texte = "Wie bekannt, wurden die Kräfte der 7 Weisen von Zauberer Aghanim in 7 Kristalle gesperrt, als er versuchte den Pfad zur Dunklen Welt zu öffnen, wo Ganon sich aufhält."; idsuiv=141; break;
        case 141 : texte = "Jetzt, nachdem du Ganon besiegt, das Triforce gefunden und damit die 7 Weisen wiederbelebt hast, haben diese Kristalle ihre magischen Kräfte immer noch erhalten."; idsuiv=142; break;
        case 142 : texte = "Sie sind tief im Inneren der 7 Tempel versteckt worden. Hol sie dir alle, dann kannst du Ganon's Zauberbann am Schlosseingang lösen."; idsuiv=143; break;
        case 143 : texte = "Das wird jedoch nicht einfach werden, denn Ganon hat sicher seine besten Einheiten zum Schutz dieser Kristalle eingesetzt..."; idsuiv=144; break;
        case 144 : texte = "Zu guter Letzt: Du wirst mit deiner jetzigen Ausrüstung nicht weit kommen. Geh zum Waffenhändler und sage ihm, dass du in meinem Auftrag kommst. Er wird sich dann wohl erkenntlich zeigen."; break;
        case 145 : texte = "Geh zum Waffenhändler und sage ihm, dass du in meinem Auftrag kommst! Er wird sich dann wohl erkenntlich zeigen."; break;
        case 146 : texte = "Du brauchst alle 7 Kristalle, um Zelda und Hyrule retten zu können."; break;
        case 147 : texte = "Du besitzt alle 7 Kristalle? Geh nun geradewegs zum Schloss und befrei' erst die Prinzessin. Sie wird besser als ich wissen, wie Hyrule gerettet werden kann."; break;
        case 148 : texte = "Sind deine Waffen gegen Ganon null und nichtig? Deswegen musst du das Schwert Excalibur finden. Man sagt, es sei gut versteckt in einem Geheim-Tempel."
            "*Nebenbei: Der Weg zum Schwert Excalibur führt mitten durch ein Dorf, nur von Monstern bewohnt... Ich frag mich, ob es so ein Dorf gibt..."; break;
        case 150 : texte = "Nun, ich befürchte dass deine Ausrüstung unzulänglich ist, um diesen Tempel zu erreichen. Du solltest zum Schmied von Gerudo gehen..."; break;
        case 151 : texte = "Es ist jetzt an der Zeit, Ganon ins Auge zu blicken und ihm das Triforce abzunehmen!"; break;
        case 152 : texte = "Heh! Du musst erst bezahlen, wenn du eine meiner Truhen öffnen willst!!!"; break;
        case 153 : texte = "Eine dieser Truhen enthält ein Herzfragment. Willst du für 10 Rupien dein Glück versuchen? Du darfst nur eine Truhe öffnen, abgemacht?*                JA ?          nein";
            if (gpJeu->getJoueur()->getRubis() + gpJeu->getJoueur()->getBoostRubis()<10) idsuiv=29; break;
        case 154 : texte = "Eine dieser Truhen enthält ein Herzfragment. Willst du für 10 Rupien dein Glück versuchen? Du darfst nur eine Truhe öffnen, abgemacht?*                ja            NEIN ?"; break;
        case 155 : texte = "Wähle eine Truhe."; break;
        case 156 : texte = "Ich habe leider keine Preise mehr für dieses Spiel, bedaure."; break;
        case 157 : texte = "Du hast verloren! Diese Truhe war leer. Versuch es noch einmal!"; break;
        case 158 : texte = "Hallo! Falls du den Schmied suchst, er wohnt etwas abseits."; break;
        case 159 : texte = "Hallo Link, ich bin der Dorfälteste hier.*Du solltest zum Schmied gehen und ihm zeigen, was du brauchst, damit er deine Ausrüstung verbessern kann.";
            buffer = "Ich kannte dein Anliegen schon, deshalb habe ich bereits mit ihm gesprochen.*Alles wird für dich kostenlos erledigt."; break;
        case 160 : texte = "Du brauchst diesen Trank jetzt nicht zu benutzen."; break;
        case 161 : texte = "Grüner Zaubertrank für 40 Rupien, einverstanden?*               JA ?          nein  ";
            if (gpJeu->getJoueur()->hasBouteille(0)!=1
            && gpJeu->getJoueur()->hasBouteille(1)!=1
            && gpJeu->getJoueur()->hasBouteille(2)!=1) {id=163; chercheText();break;}
            if (gpJeu->getJoueur()->getRubis() + gpJeu->getJoueur()->getBoostRubis()<40) idsuiv=29;
            else idsuiv=108; break;
        case 162 : texte = "Grüner Zaubertrank für 40 Rupien, einverstanden?*               ja            NEIN ?"; break;
        case 163 : texte = "Komm wieder, wenn du eine leere Flasche hast, dann verkauf ich dir einen grünen Trank, welcher die Zauberkraft wiederherstellt."; break;
        case 164 : texte = "Komm später wieder, ich bin zu sehr beschäftigt!"; break;
        case 165 : texte = "Kann ich etwas für dich tun?";
            if (gpJeu->getJoueur()->getEpee()==1) {id=166; chercheText();break;}
            if (gpJeu->getJoueur()->hasObjet(O_GANTS)==1 && gpJeu->getJoueur()->getObjet()==8) {
                id=169; chercheText();break;}
            if (gpJeu->getJoueur()->hasObjet(O_ARC)==2 && gpJeu->getJoueur()->getObjet()==0
            && gpJeu->getJoueur()->nbCristaux()==7) {
                id=172; chercheText();break;}
            break;
        case 166 : texte = "Soll ich dein Schwert härten?**               JA ?          nein  "; break;
        case 167 : texte = "Soll ich dein Schwert härten?**               ja            NEIN ?"; break;
        case 168 : texte = "Dein Schwert hat seine Kraft verdreifacht. Außerdem kann es jetzt magische Hiebe schleudern, da guckst du!!!*"; break;
        case 169 : texte = "Soll ich deine Handschuhe aufwerten?**               JA ?          nein  "; break;
        case 170 : texte = "Soll ich deine Handschuhe aufwerten?**               ja            NEIN ?"; break;
        case 171 : texte = "Die Kraft deiner Handschuhe ermöglicht dir nun, schwere Brocken zu heben."; break;
        case 172 : texte = "Soll ich deinen Bogen aufwerten?**               JA ?          nein  "; break;
        case 173 : texte = "Soll ich deinen Bogen aufwerten?**               ja            NEIN ?"; break;
        case 174 : texte = "Dein Bogen verwendet nun Silberpfeile mit kataklystischer Energie!!!"; break;
        case 175 : texte = "Komm wieder, wenn du eine leere Flasche hast, dann verkauf ich dir den blauen Trank, der Gesundheit und Zauberkraft wiederherstellt."; break;
        case 176 : texte = "Der blaue Zaubertrank für 160 Rupien, einverstanden?*               JA ?          nein  ";
            if (gpJeu->getJoueur()->hasBouteille(0)!=1
            && gpJeu->getJoueur()->hasBouteille(1)!=1
            && gpJeu->getJoueur()->hasBouteille(2)!=1) {id=175; chercheText();break;}
            if (gpJeu->getJoueur()->getRubis() + gpJeu->getJoueur()->getBoostRubis()<160) idsuiv=29;
            else idsuiv=108; break;
        case 177 : texte = "Der blaue Zaubertrank für 160 Rupien, einverstanden?*               ja            NEIN ?"; break;
        case 178 : texte = "Unser Dorfältester hat festgelegt, unvorsichtige Abenteurer vom Tempel fernzuhalten. Ich erlaube niemandem zu passieren."; break;
        case 179 : texte = "Mmmm... Du hast bereits 4 Kristalle gefunden?*Ich muss zugeben, dass du mich beeindruckst...";
            buffer = "Nun gut, du darfst das Gräberfeld überqueren, aber auf eigenes Risiko."; break;
        case 181 : texte = "Sag dem Wächter im Norden des Dorfes, dass er dich durchlassen soll."; break;
        case 182 : texte = "Ach soo, du lebst immer noch...*Erstaunlich."; break;
        case 183 : texte = "Der Dunkle Tempel ist der Startpunkt des Weges zum Erreichen eines legendären Objektes. Dieses Dorf existiert nur deshalb, um Abenteurer davon fern zu halten."; break;
        case 184 : texte = "Du hast also das Schwert Excalibur gefunden?*Aber glaub' nicht, dass du es für immer behalten kannst.";
            buffer = "Das Schwert Excalibur ist eines der wertvollsten Kulturgüter von Hyrule, zusammen mit dem Triforce. Du wirst es am Ende deiner Mission in den Altarraum zurück legen!"; break;
        case 186 : texte = "Alle Bewohner dieses Dorfes sind Geister, die Prinzessin Zelda's Anweisungen treu befolgen."; break;
        case 187 : texte = "Als der Frieden in Hyrule wieder einzog und die Lichtwelt mit dem Goldenen Land wieder vereinigt war, bat Zelda (heimlich) einige Geister diese Stadt zu errichten,";
            buffer = "um den Zugang zu einem sehr wertvollen Objekt zu blockieren."; break;
        case 189 : texte = "Willkommen Link!*Lass mich dir erklären, was ich hier mache."; idsuiv=190; break;
        case 190 : texte = "Wie du sicher bemerkt hast, sind Monster in Hyrule aufgetaucht. Es ist das erste mal, seit deinem Sieg über Ganon, dass ich Monster beobachte."; idsuiv=191; break;
        case 191 : texte = "Nun habe ich beschlossen, alle Monster zu katalogisieren."; idsuiv=192; break;
        case 192 : texte = "Unglücklicherweise bin ich wirklich zu ängstlich, diesen persönlich zu begegnen. Deshalb brauche ich deine Hilfe."; idsuiv=193; break;
        case 193 : texte = "Du musst jede unbekannte Monster-Art besiegen und mir im Anschluss davon berichten."; idsuiv=194; break;
        case 194 : texte = "Jedes mal, wenn du 7 neue Arten von Monstern besiegt hast, gebe ich dir ein Herzfragment dafür."; idsuiv=195; break;
        case 195 : texte = "Willst du wissen, welche Monster du bereits besiegt hast, dann drück L und B."; break;
        case 196 :
            if (gpJeu->getJoueur()->getCoeur(43)) {
                if (gpJeu->getJoueur()->nbEnnemis() >= 46) {id=205; chercheText();return;}
                else {id=204; chercheText();return;}
            }
            for (int i = 38; i < 44; i++)
                if (!gpJeu->getJoueur()->getCoeur(i)
                && (gpJeu->getJoueur()->nbEnnemis()/7)+37>=i) {
                    id=203; chercheText();return;
                }
            tmp = 7-(gpJeu->getJoueur()->nbEnnemis()%7);
            os << tmp;
            if (tmp>1) texte = "Abermals "+os.str()+" unbekannte Monster bis zur nächsten Belohnung.";
            else texte = "Du benötigst nur noch ein unbekanntes Monster bis zur nächsten Belohnung!!!";
            break;
        case 203 : texte = "Nimm dieses Geschenk für deine tatkräftige Unterstützung meines Projekts:"; break;
        case 204 : texte = "Tut mir leid, aber ich habe keine Geschenke mehr für dich..."; break;
        case 205 : texte = "Ich kann es nicht fassen. Dir gelang es, alle Monster-Arten zu besiegen!!!*Gratulation, Link!!!"; break;
        case 206 : texte = "                    Level 1:**                  Wald-Tempel"; break;
        case 207 : texte = "                    Level 2:**                 Wasser-Tempel"; break;
        case 208 : texte = "                    Level 3:**                  Berg-Tempel"; break;
        case 209 : texte = "                    Level 4:**                 Wüsten-Tempel"; break;
        case 210 : texte = "                    Level 5:**                 Dunkler Tempel"; break;
        case 211 : texte = "                    Level 6:**                  Feuer-Tempel"; break;
        case 212 : texte = "                    Level 7:**                   Eis-Tempel"; break;
        case 213 : texte = "                    Level ?:**                 Geheim-Tempel"; break;
        case 214 : texte = "                 Letzter Level:**                 Schloss Hyrule"; break;

        case 215 :texte = "                  Schon müde?                   ?               WEITER MACHEN               ?               Sichern und beenden                          Beenden ohne zu sichern           "; break;
        case 216 :texte = "                  Schon müde?                                   weiter machen                  ?            SICHERN UND BEENDEN            ?             Beenden ohne zu sichern           "; break;
        case 217 :texte = "                  Schon müde?                                   weiter machen                               Sichern und beenden               ?          BEENDEN OHNE ZU SICHERN          ?"; break;

        case 223 : texte = "Als Link das Triforce berührte, verschwanden auch alle Monster und Frieden zog ein.**"
          "Dann fasste Prinzessin Zelda einen mutigen Entschluss: Sie berührte kreisend das Triforce und wünschte sich etwas.**"
          "Das Triforce hat schon genug Probleme im Königreich verursacht. Früher oder später wird erneut ein Bösewicht das Triforce finden.**"
          "Im gleichen Moment, als die Prinzessin die Reliquie berührte, verschwand diese für immer von Hyrule.";
          buffer = "Seit diesem Tag lebten die Bewohner von Hyrule in Frieden, ohne Angst vor neuem Unheil.**"
          "Deshalb ist die Geschichte von Zelda, dem Triforce und dem Goldenen Land Hyrule hiermit beendet.**"
          "Das Schwert Excalibur gilt als sicher verwahrt (-irgendwo an einem geheimen Ort-) und ist Link's jüngstes Beutestück..."; break;
        case 224 : texte = "Du hast mich gefunden, Link. Zur Belohnung schenke ich dir das Farore-Amulett. Dies steigert deine Rüstung um einen Punkt..."; break;
        case 225 : texte = "Möchtest du dein Spiel sichern?**                    ?  JA  ?*                      nein  "; break;
        case 226 : texte = "Möchtest du dein Spiel sichern?**                       ja   *                    ? NEIN ?"; break;
        case 227 : texte = "Spielstand gesichert."; break;
        case 228 : texte = "Das Königreich Hyrule lebte in Frieden, seit Link, der letzte Ritter von Hyrule, den heimtückischen Ganon besiegte und das wertvolle Triforce von ihm zurückgewann."; idsuiv = 229; break;
        case 229 : texte = "Niemand kennt die Worte von Link's Wunsch an das Triforce, allerdings hat dieser die Wiedervereinigung der Hellen mit der Dunklen Welt bewirkt, sowie *die Auferstehung der 7 Weisen."; idsuiv = 230; break;
        case 230 : texte = "Schließlich übergab Link das Triforce und das Schwert Excalibur an Prinzessin Zelda, und die Leute begannen zu glauben, dass nun für immer Frieden herrschen wird.                        Aber die Leute lagen falsch..."; idsuiv=231; break;
        case 231 : texte = "... denn unglücklicherweise hatte Link's Wunsch auch negative Auswirkungen. Ganon und seine Schergen sind wiederauferstanden und machen sich kampfbereit."; idsuiv=232; break;
        case 232 : texte = "Irgendwo im Wald von Hyrule schläft Link und ahnt noch nicht, dass Ganon bereits den Kampf begonnen hat,";
            buffer="bis eine wohlbekannte Stimme zu ihm spricht, während er weiter fest schläft..."; break;
        case 233 : texte = "Hilf mir!*Hilf mir!*Ich bin's, Zelda!*Ich spreche zu dir über Telepathie."; idsuiv = 234; break;
        case 234 : texte = "Ich bin Gefangene im Kerker des Schlosses!*Ich brauche deine Hilfe!*Ganon ist zurückgekehrt, und er hat bestimmt schon das Triforce gefunden..."; idsuiv=235; break;
        case 235 : texte = "Komm schnell zu mir ins Schloss, Link, du bist meine einzige Hoffnung..."; break;
        case 236 : texte = "Brauchst du HILFE, dann drück L und X."; break;
    }

break; // German

case 4: // Italian

    switch (id) {
        case 1  : texte = "Hai trovato uno scudo!**La tua difesa aumenta di un punto!"; break;
        case 2  : texte = "Hai trovato una spada!**Ora puoi combattere i monstri con B .*Lascia premuto B per caricare un attacco rotante!"; break;
        case 3  : 
            tmp = 4-(gpJeu->getJoueur()->nbQuarts()%4);
            os << tmp;
            texte = "Hai trovato una parte di cuore!!!";
            if (tmp<4) buffer = "te ne servono "+os.str()+" ancora per un nuvo cuore.";
            else buffer = "Il tuo numero di cuori è aumentato!";
            break;
        case 11 : texte = "N: Casa di Link*W: Terra di Hyrule*E: Tempio della foresta"; break;
        case 12 : texte = "Tempio della montagna**Non per cardiopatici, paurosi, o deboli di cuore."; break;
        case 13 : texte = "W: Entrata del Deserto*N: Tempio della montagna"; break;
        case 14 : texte = "N: Tempio della montagna*S: terra di Hyrule"; break;
        case 15 : texte = "N: Mountagna della morte"; break;
        case 16 : texte = "E: Entrata della foresta"; break;
        case 17 : texte = "S: Lago Hylia*E: Cimitero Infestato"; break;
        case 18 : texte = "S: Al villaggio Kakariko"; break;
        case 19 : texte = "N: Villaggio Kakariko"; break;
        case 20 : texte = "N: Terra di Hyrule"; break;
        case 21 : texte = "W: terra di Fuoco"; break;
        case 22 : texte = "E: Villaggio Ombra*- Non per i vivi -"; break;
        case 23 : texte = "Tempio Oscuro**Se sei un fantasma e cerchi un lavoro, vieni a trovarci."; break;
        case 24 : texte = "N: Villaggio Ombra*W: Lago Hylia"; break;
        case 25 : texte = "N: Cimitero infestato**Non entrare"; break;
        case 26 : texte = "Villaggio Ombra"; break;
        case 27 : texte = "Vendo una bottiglia per 100 rupie, sei interessato?*              SI' ?            no  "; 
            if ((gpJeu->getJoueur()->hasBouteille(0) && gpJeu->getZone()==8)
            || (gpJeu->getJoueur()->hasBouteille(1) && gpJeu->getZone()==5)
            || (gpJeu->getJoueur()->hasBouteille(2) && gpJeu->getZone()==6)) {id=32; chercheText();break;}
            if (gpJeu->getJoueur()->getRubis() + gpJeu->getJoueur()->getBoostRubis()<100) idsuiv=29;
            else idsuiv=30;
            break;
        case 28 : texte = "Vendo una bottiglia per 100 rupie, sei interessato?*              sì             NO ?"; idsuiv=31; break;
        case 29 : texte = "Spiacente, non hai abbastanza rupie"; break;
        case 30 : texte = "Grazie, ecco la tua bottiglia."; break;
        case 31 : texte = "Torna ancora se vambi idea."; break;
        case 32 : texte = "Spiecente, era la mia unica bottiglia."; break;
        case 33 : texte = "Hai trovato una bottiglia vuota!**Ti aiuterà a conservare le pozioni."; break;
        case 35 : texte = "Gli abitanti del villaggio hanno uno strano dialetto. Non capisco nulla..."; break;
        case 36 : texte = "@+@+@+@+@+@+@@@+*@=++=@=+@=+@=+=@*+@+@+@+=+="; break;
        case 37 : texte = "Chiedi il permesso al capo se vuoi veramente passare!"; break;
        case 38 : texte = "Il capo ti ha concesso di passare? Grrrr... Allora vai!"; break;
        case 39 : texte = "Sparisci dalla mia vista!"; break;
        case 40 : texte = "Non abbiamo spesso visitatori da queste parti."; break;
        case 41 : texte = "I mostri locali temono poche cose. Fortunatamente, attaccano solo gli umani."; break;
        case 42 : texte = "Davvero?*Capisci quello che sto dicendo?"; break;
        case 43 : texte = "N: Deserto e Villaggio Gerudo"; break;
        case 44 : texte = "S: Lago Hylia*W: Castello di Hyrule"; break;
        case 45 : texte = "Villaggio Kakariko"; break;
        case 46 : texte = "W: Castello di Hyrule"; break;
        case 47 : texte = "Che bella giornata!"; break;
        case 48 : texte = "perchè ho accettato di prendermi cura di quella robaccia?*Io odio le galline!"; break;
        case 49 : texte = "Puoi trovare molte cose nel negozio del villaggio."; break;
        case 50 : texte = "S: Villaggio Kakariko*W: Deserto*E: Montagna della Morte"; break;
        case 51 : texte = "Villagio Gerudo"; break;
        case 52 : texte = "Ti sei perso nel deserto?*Tu sei qui:*                        X"; break;
        case 54 : texte = "S: Al Castello di Hyrule"; break;
        case 55 : texte = "Tempio del deserto**Vieni a provare le nostre terme.*(L'acqua non è potabile)"; break;
        case 56 : texte = "Tempio della foresta**Salva gli alberi, mangia un castoro!"; break;
        case 57 : texte = "Tempio del Lago**Cuffia da bagno obligatoria"; break;
        case 58 : texte = "Tempio di Ghiaccio**Il personale vi ricorda che un dungeon non è una pista di ghiaccio."; break;
        case 59 : texte = "Hai visto il fabbro del villaggio?*Si dice che sia molyo abile nel migliorare l'equipaggiamento degli avventurieri. Miolte persone gli fanno visita."; break;
        case 60 : texte = "Il tempio è in pieno desert, in una antica oasi."; break;
        case 61 : texte = "S: terra del fuoco*E: Villaggio Kakarico"; break;
        case 62 : texte = "Castello di Hyrule"; break;
        case 63 : texte = "E: Villaggio Kakariko"; break;
        case 64 : texte = "W: Roccia della tartaruga*E: Lago Hylia"; break;
        case 65 : texte = "Tempio celato**Qui riposa la spada purificatrice, Excalibur.."; break;
        case 66 : texte = "N: Al cimitero infestato"; break;
        case 67 : texte = "Hai trovato la mappa del tempio!*Premi L e Y per vedere la mappa."; break;
        case 68 : texte = "Hai trovato la bussola!*Puoi localizzare i bauli e il bosssulla mappa."; break;
        case 69 : texte = "Hai trovato la chiave per il Boss!"; break;
        case 70 : texte = "Hai trovato una piccola chiave!*Avvicinati ad una porta per aprirla."; break;
        case 71 : texte = "Hai trovato i guanti!*Usali per alzare gli oggetti con il bottone X."; break;
        case 72 : 
            texte = "Hai trovato un Cristallo Magico!"; 
            tmp = 7-gpJeu->getJoueur()->nbCristaux();
            os << tmp;
            if (tmp==6 && !gpJeu->getJoueur()->getAvancement()) buffer = "A cosa potrà servire...?";
            else if (tmp>2) buffer = "Ce ne sono "+os.str()+" ancora da trovare!";
            else if (tmp==2) buffer = "Ne rimangono solo due da trovare!";
            else if (tmp==1) buffer = "Te ne manca uno solo!";
            else if (tmp==0) buffer = "Hai tutti i cristalli, corri al castello e salva Zelda!";
            break;
        case 80 : texte = "You found the Hookshot!*Use it to overcome obstacles."; break;
        case 81 : texte = "You found the Lantern!*Use it to set things on fire."; break;
        case 82 : texte = "You found the Flippers!*Select them in the item menu to swim in the water."; break;
        case 83 : texte = "You found the Magic Hammer!*Use it to squash obstacles."; break;
        case 84 : texte = "You found the Fire Rod!*From now on you are able to shoot out powerful flames."; break;
        case 85 : texte = "You found the Ice Rod!*Use it to freeze anything from a distance."; break;
        case 86 : texte = "You found the Master Sword!*Even Ganon could not stand up to its power... in theory!"; break;
        case 87 : texte = "Thank you for saving me, Link.***Let's find Ganon quickly; we have to reclaim the Triforce!"; break;
        case 89 : texte = "The secret passage at the back of the throne room leads to Ganon. Hurry up!"; break;
        case 90 : texte = "We are very near, follow me!"; break;
        case 91 : texte = "I'm afraid you are not able to defeat Ganon with your current weapons...*Go and speak to the chief of Kakariko Village, I am sure he will find a solution."; break;
        case 92 : texte = "Ganon is just behind that door. I will cure your wounds."; break;
        case 93 : texte = "Ganon is still somewhere in the castle."; break;
        case 94 : texte = "You should wait for Princess Zelda!"; break;
        case 95 : texte = "Don't you have the strange feeling that you are forgetting someone, by any chance?"; break;
        case 96 : texte = "Zelda is waiting for you!"; break;
        case 97 : texte = "You found the Triforce!"; break;
        case 98 : texte = "You found the Book of Mudora!**From now on, you understand the ancient Hylian language!"; break;
        case 99 : texte = "Congratulations for finding me, Link. As a reward, I give you Din's Pendant. It increases your defense by one point."; break;
        case 100 : texte = "Congratulations for finding me, Link. As a reward, I give you Nayru's Pendant. It increases your defense by two points."; break;
        case 101 : texte = "..."; break;
        case 102 : texte = "You obtain a magic pendant!**Your defense increased!"; break;
        case 103 : texte = "Congratulation for finding me, Link. As a reward, I will double the capacity of your magic meter!"; break;
        case 104 : texte = "Your magic meter has been doubled!"; break;
        case 105 : texte = "Come back when you have an empty bottle, and I will sell you a red potion which restores your health."; break;
        case 106 : texte = "A red potion for 60 rupees. Are you interested?*              YES ?            no  "; 
            if (gpJeu->getJoueur()->hasBouteille(0)!=1
            && gpJeu->getJoueur()->hasBouteille(1)!=1
            && gpJeu->getJoueur()->hasBouteille(2)!=1) {id=105; chercheText();break;}
            if (gpJeu->getJoueur()->getRubis() + gpJeu->getJoueur()->getBoostRubis()<60) idsuiv=29;
            else idsuiv=108; break;
        case 107 : texte = "A red potion for 60 rupees. Are you interested?*              yes              NO ?"; break;
        case 108 : texte = "Thank you, here is your potion.";break;
        case 109 : texte = "You got a red potion!*Drink it to restore your health."; break;
        case 110 : texte = "You got a green potion!*Drink it to restore your magic meter."; break;
        case 111 : texte = "You got a blue potion!*Drink it to restore your health and magic meter."; break;
        case 112 : texte = "Hello, what would you like to drink?"; break;
        case 113 : texte = "-Hiccup!- A fairy is said to give enchanted objects... -Hiccup!- ...to the adventurers who would find her... -Hiccup!!!-"; break;
        case 114 : texte = "One heart for 10 rupees. Are you interested?**              YES ?            no  "; 
            if (gpJeu->getJoueur()->getVie()+gpJeu->getJoueur()->getBoostVie()
            == gpJeu->getJoueur()->getVieMax()) {id=128; chercheText();break;}
            if (gpJeu->getJoueur()->getRubis() + gpJeu->getJoueur()->getBoostRubis()<10) idsuiv=29;
            else idsuiv=129; break;
        case 115 : texte = "One heart for 10 rupees. Are you interested?**              yes              NO ?"; break;
        case 116 : texte = "A little bit of magic for 20 rupees. Are you interested?**              YES ?            no  "; 
            if (gpJeu->getJoueur()->getMagie()+gpJeu->getJoueur()->getBoostMagie()
            == gpJeu->getJoueur()->getMagieMax() || !gpJeu->getJoueur()->hasObjet(O_LANTERNE)) {
                id=128; chercheText();break;}
            if (gpJeu->getJoueur()->getRubis() + gpJeu->getJoueur()->getBoostRubis()<20) idsuiv=29;
            else idsuiv=129; break;
        case 117 : texte = "A little bit of magic for 20 rupees. Are you interested?**              yes              NO ?"; break;
        case 118 : texte = "Some magic for 30 rupees. Are you interested?**              YES ?            no  "; 
            if (gpJeu->getJoueur()->getMagie()+gpJeu->getJoueur()->getBoostMagie()
            == gpJeu->getJoueur()->getMagieMax() || !gpJeu->getJoueur()->hasObjet(O_LANTERNE)) {
                id=128; chercheText();break;}
            if (gpJeu->getJoueur()->getRubis() + gpJeu->getJoueur()->getBoostRubis()<30) idsuiv=29;
            else idsuiv=129; break;
        case 119 : texte = "Some magic for 30 rupees. Are you interested?**              yes              NO ?"; break;
        case 120 : texte = "5 arrows for 30 rupees. Are you interested?**              YES ?            no  "; 
            if (gpJeu->getJoueur()->getFleche() == gpJeu->getJoueur()->getFlecheMax() 
            || !gpJeu->getJoueur()->hasObjet(O_ARC)) {id=128; chercheText();break;}
            if (gpJeu->getJoueur()->getRubis() + gpJeu->getJoueur()->getBoostRubis()<30) idsuiv=29;
            else idsuiv=129; break;
        case 121 : texte = "5 arrows for 30 rupees. Are you interested?**              yes              NO ?"; break;
        case 122 : texte = "One bomb for 30 rupees. Are you interested?**              YES ?            no  "; 
            if (gpJeu->getJoueur()->getBombe() == gpJeu->getJoueur()->getBombeMax() 
            || !gpJeu->getJoueur()->hasObjet(O_SAC_BOMBES)) {id=128; chercheText();break;}
            if (gpJeu->getJoueur()->getRubis() + gpJeu->getJoueur()->getBoostRubis()<30) idsuiv=29;
            else idsuiv=129; break;
        case 123 : texte = "One bomb for 30 rupees. Are you interested?**              yes              NO ?"; break;
        case 124 : texte = "A bow for 1000 rupees. Are you interested?**              YES ?            no  "; 
            idsuiv=29; break;
        case 125 : texte = "A bow for 1000 rupees. Are you interested?**              yes              NO ?"; break;
        case 126 : texte = "A bow for 50 rupees. Are you interested?**              YES ?            no  "; 
            if (gpJeu->getJoueur()->getRubis() + gpJeu->getJoueur()->getBoostRubis()<50) idsuiv=29;
            else idsuiv=129; break;
        case 127 : texte = "A bow for 50 rupees. Are you interested?**              yes              NO ?"; break;
        case 128 : texte = "You don't need this right now."; break;
        case 129 : texte = "Thank you."; break;
        case 130 : texte = "You got the bow!*Use it to shoot at distant targets."; break;
        case 131 : texte = "Pick what you want."; break;
        case 132 : texte = "What?*You are on a mission for the chief?*Okay, so I'm going to give you a discount on the bow"; break;
        case 133 : texte = "Hello Link, I am the chief of this village. I suspected you were coming."; idsuiv=134; break;
        case 134 : texte = "Since this morning, monsters stride along Hyrule. I tried to talk to Zelda about it, but as you've perhaps already seen, a powerful spell blocks the access to the castle."; idsuiv=136; break;
        case 136 : texte = "You say Ganon is responsible for all of this? He stole the Triforce and he holds Princess Zelda captive?"; idsuiv=137; break;
        case 137 : texte = "Mmmm... The situation is more serious than I thought..."; idsuiv=138; break;
        case 138 : texte = "We have to act very quickly. You must face Ganon again!"; idsuiv=139; break;
        case 139 : texte = "How do we enter the castle? I may have an idea..."; idsuiv=140; break;
        case 140 : texte = "As you know, the power of the 7 wise men was locked up in 7 crystals by Aghanim when he was trying to open up the path to the Dark World, where Ganon was."; idsuiv=141; break;
        case 141 : texte = "Yet, even if you raised the descendants of the 7 wise men from the dead after you defeated Ganon and found the Triforce, these crystals have kept their power."; idsuiv=142; break;
        case 142 : texte = "They have been hidden deep in the 7 temples. Gather them all, and you'll be able to get over Ganon's spell."; idsuiv=143; break;
        case 143 : texte = "That won't be easy, however. Ganon will surely send his best troops to protect these crystals."; idsuiv=144; break;
        case 144 : texte = "One last thing. You won't go very far with your current equipment. Go and see the arms dealer and tell him that you're coming on my behalf. He will probably make a gesture for you."; break;
        case 145 : texte = "Go and see the arms dealer and tell him that you're coming on my behalf. He will probably make a gesture for you."; break;
        case 146 : texte = "You have to gather the 7 crystals to have a chance at saving Zelda and Hyrule."; break;
        case 147 : texte = "You found the 7 crystals? Go to the castle! First, you have to save the princess. She will know better than I how to save Hyrule."; break;
        case 148 : texte = "Your weapons are null and void against Ganon? You have to find the Master Sword. It is said to have been concealed in a hidden temple.*The way leading to the Sword of Evils Bane is said to cross over a village populated by monsters...*I wonder if such a village exists."; break;
        case 150 : texte = "However, I'm afraid that your current equipment is inadequate for reaching this temple. You should go see the blacksmith of Gerudo village."; break;
        case 151 : texte = "It's about time to face Ganon and take the Triforce back!"; break;
        case 152 : texte = "Hey! You have to pay to open one of my chests!"; break;
        case 153 : texte = "One of these chests contains a piece of heart. Do you want to try for 10 rupees?*You will only be allowed to open one chest, ok?               YES ?            no  "; 
            if (gpJeu->getJoueur()->getRubis() + gpJeu->getJoueur()->getBoostRubis()<10) idsuiv=29; break;
        case 154 : texte = "One of these chests contains a piece of heart. Do you want to try for 10 rupees?*You will only be allowed to open one chest, ok?               yes              NO ?"; break;
        case 155 : texte = "Pick a chest."; break;
        case 156 : texte = "I don't have anymore prizes to bring into play, sorry."; break;
        case 157 : texte = "You lose! This chest is empty. Try again!"; break;
        case 158 : texte = "Hello! If you are looking for the blacksmith, he lives a little further ahead."; break;
        case 159 : texte = "Hello Link, I am the chief of this village.*You should go to see the blacksmith and show him what equipment you want him to improve.";
            buffer = "I have heard of your quest, so I have arranged everything with him.*All will be free for you."; break;
        case 160 : texte = "You don't need to drink this potion right now."; break;
        case 161 : texte = "A green potion for 40 rupees. Are you interested?*              YES ?            no  "; 
            if (gpJeu->getJoueur()->hasBouteille(0)!=1
            && gpJeu->getJoueur()->hasBouteille(1)!=1
            && gpJeu->getJoueur()->hasBouteille(2)!=1) {id=163; chercheText();break;}
            if (gpJeu->getJoueur()->getRubis() + gpJeu->getJoueur()->getBoostRubis()<40) idsuiv=29;
            else idsuiv=108; break;
        case 162 : texte = "A green potion for 40 rupees. Are you interested?*              yes              NO ?"; break;
        case 163 : texte = "Come back when you have an empty bottle, and I will sell you a green potion which restores your magic meter."; break;
        case 164 : texte = "Come back later. I am overbooked!"; break;
        case 165 : texte = "May I do something for you?";
            if (gpJeu->getJoueur()->getEpee()==1) {id=166; chercheText();break;}
            if (gpJeu->getJoueur()->hasObjet(O_GANTS)==1 && gpJeu->getJoueur()->getObjet()==8) {
                id=169; chercheText();break;}
            if (gpJeu->getJoueur()->hasObjet(O_ARC)==2 && gpJeu->getJoueur()->getObjet()==0 
            && gpJeu->getJoueur()->nbCristaux()==7) {
                id=172; chercheText();break;}
            break;
        case 166 : texte = "Do you want me to temper your sword?**              YES ?            no  "; break;
        case 167 : texte = "Do you want me to temper your sword?**              yes              NO ?"; break;
        case 168 : texte = "Your sword triples in power and you can now shoot magic beams. So good!*"; break;
        case 169 : texte = "Do you want me to upgrade your gloves?**              YES ?            no  "; break;
        case 170 : texte = "Do you want me to upgrade your gloves?**              yes              NO ?"; break;
        case 171 : texte = "The power of your gloves now helps you lift heavy rocks!"; break;
        case 172 : texte = "Do you want me to upgrade your bow?**              YES ?            no  "; break;
        case 173 : texte = "Do you want me to upgrade your bow?**              yes              NO ?"; break;
        case 174 : texte = "Your bow now shoots silver arrows of cataclysmic power!"; break;
        case 175 : texte = "Come back when you have an empty bottle, and I will sell you a blue potion which restores your health and your magic meter."; break;
        case 176 : texte = "A blue potion for 160 rupees. Are you interested?*              YES ?            no  "; 
            if (gpJeu->getJoueur()->hasBouteille(0)!=1
            && gpJeu->getJoueur()->hasBouteille(1)!=1
            && gpJeu->getJoueur()->hasBouteille(2)!=1) {id=175; chercheText();break;}
            if (gpJeu->getJoueur()->getRubis() + gpJeu->getJoueur()->getBoostRubis()<160) idsuiv=29;
            else idsuiv=108; break;
        case 177 : texte = "A blue potion for 160 rupees. Are you interested?*              yes              NO ?"; break;
        case 178 : texte = "Our village has been built here with the purpose of preventing careless adventurers from reaching the temple. I won't allow just anyone to pass."; break;
        case 179 : texte = "Mmmm... You already found 4 crystals?*I have got to admit, you impress me...";
            buffer = "All right, you are allowed to cross over the graveyard, at your own risk."; break;
        case 181 : texte = "Talk to the guard at the north of the village. He will let you pass."; break;
        case 182 : texte = "I see you're still alive...*Surprising."; break;
        case 183 : texte = "The dark temple is the starting point of the way leading to a legendary object. This village only exists to dissuade adventurers from approaching."; break;
        case 184 : texte = "So you found the Sword?*But... don't believe it is yours.";
            buffer = "The Master Sword is among the most treasured heritage of Hyrule along with the Triforce. You will have to bring it back to its sanctuary once your mission is completed."; break;
        case 186 : texte = "All the inhabitants of this village are spirits who answered to Princess Zelda's call."; break;
        case 187 : texte = "When you returned peace to Hyrule and reunified the Light World with the Golden Land, Zelda secretly asked volunteer spirits to create this village to block";
            buffer = "the access to a very high valued object."; break;
        
        case 189 : texte = "Welcome Link!*Let me explain to you what I am doing here."; idsuiv=190; break;
        case 190 : texte = "As you certainly have noticed, monsters appeared in Hyrule. It's the first time I can see monsters since your victory on Ganon."; idsuiv=191; break;
        case 191 : texte = "I have decided to list all of them."; idsuiv=192; break;
        case 192 : texte = "Unfortunately, I'm too fearful to meet them, so I need your help."; idsuiv=193; break;
        case 193 : texte = "You have to fight each kind of monster and come to give me your report."; idsuiv=194; break;
        case 194 : texte = "Each time you have defeated 7 new kinds of enemies, I will give you a piece of heart."; idsuiv=195; break;
        case 195 : texte = "If you want to see what enemies you've already defeated, press the L and B buttons."; break;
        
        
        case 196 : 
            if (gpJeu->getJoueur()->getCoeur(43)) {
                if (gpJeu->getJoueur()->nbEnnemis() >= 46) {id=205; chercheText();return;}
                else {id=204; chercheText();return;}
            }
            for (int i = 38; i < 44; i++) 
                if (!gpJeu->getJoueur()->getCoeur(i) 
                && (gpJeu->getJoueur()->nbEnnemis()/7)+37>=i) {
                    id=203; chercheText();return;
                }
            tmp = 7-(gpJeu->getJoueur()->nbEnnemis()%7);
            os << tmp;
            if (tmp>1) texte = "You need "+os.str()+" more different enemies before your next reward.";
            else texte = "You only need one more enemy before your next reward!";
            break;
        case 203 : texte = "Take this gift as a thank you for your contribution to my project:"; break;
        case 204 : texte = "I'm sorry, but I don't have anything for you anymore."; break;
        case 205 : texte = "I can't take it in, you succeeded in defeating all kinds of enemies!*Congratulations, Link!"; break;
        
        case 206 : texte = "                    Level 1:**                  Forest Temple"; break;
        case 207 : texte = "                    Level 2:**                  Water Temple"; break;
        case 208 : texte = "                    Level 3:**                 Mountain Temple"; break;
        case 209 : texte = "                    Level 4:**                  Desert Temple"; break;
        case 210 : texte = "                    Level 5:**                   Dark Temple"; break;
        case 211 : texte = "                    Level 6:**                   Fire Temple"; break;
        case 212 : texte = "                    Level 7:**                   Ice Temple"; break;
        case 213 : texte = "                    Level ?:**                  Hidden Temple"; break;
        case 214 : texte = "                  Last Level:**                 Hyrule Castle"; break;
        
        case 215 :texte = "                 Already tired?                  ?                 CONTINUE                 ?                   Save and quit                               Quit without saving            "; break;
        case 216 :texte = "                 Already tired?                                    Continue                     ?               SAVE AND QUIT              ?                Quit without saving            "; break;
        case 217 :texte = "                 Already tired?                                    Continue                                     Save and quit                  ?            QUIT WITHOUT SAVING           ?"; break;
        
        case 223 : texte = "At the exact same time Link touched the Triforce, the monsters disappeared,and peace returned.**Princess Zelda then made a great decision: she touched the Triforce and made her wish.**The Triforce had always been the origin of troubles in the Kingdom. Sooner or later, another evil creature would find it.**When Princess Zelda touched the relic, it disappeared from Hyrule... forever.";
            buffer = "From this day, the people of Hyrule have lived in peace, without fear of new disasters.**Thus the Legend of Zelda, of the Triforce, and of the Golden Land, is achieved.**The Master Sword is said to rest safely somewhere and to be the last heirloom of Link's quest."; break;
        case 224 : texte = "Congratulations for finding me, Link. As a reward, I give you Farore's Pendant. It increases your defense by one point."; break;
        case 225 : texte = "Do you want to save the game?**                    ? YES ?*                      no  "; break;
        case 226 : texte = "Do you want to save the game?**                      yes  *                    ? NO ?"; break;
        case 227 : texte = "Game saved."; break;
        
        case 228 : texte = "The Kingdom of Hyrule has been in peace since Link, the last knight of Hyrule, had defeated the malicious Ganon and reclaimed the precious Triforce from him."; idsuiv = 229; break;
        case 229 : texte = "Nobody knows what Link's wish to the Triforce was, but it had the effect of reunifying the Light and Dark World, and resurrecting the 7 wise men's descendants."; idsuiv = 230; break;
        case 230 : texte = "Link then handed the Triforce and the Master Sword over to Princess Zelda, and people started believing that peace would last.*But the people were wrong..."; idsuiv=231; break;
        case 231 : texte = "Unfortunately, Link's wish also had negative effects. Ganon and his henchmen were resurrected and got ready to attack."; idsuiv=232; break;
        case 232 : texte = "Somewhere in Hyrule Forest, Link is sleeping without suspecting that Ganon has already moved into the attack, until a"; 
            buffer="well-known voice talked to him in his sleep..."; break;
        case 233 : texte = "Help me!*Help me!*It's me! Zelda!*I'm talking to you via telepathy."; idsuiv = 234; break;
        case 234 : texte = "I am a prisoner in the dungeon of the castle!*I need your help!*Ganon is back, and he surely has already found the Triforce..."; idsuiv=235; break;
        case 235 : texte = "Come, quickly, to the castle, Link! You are my only hope..."; break;
        case 236 : texte = "HELP: Press L and X to view the help menu."; break;
    }
	break; // Italian

case 5: // Spanish

    switch (id) {
        case 1  : texte = "Has encontrado un escudo !!!**Tu defensa aumenta de un punto!"; break;
        case 2  : texte = "Has encontrado una espada !!!**Ahora puedes defenderte contra los monstros con la tecla Z (o W) !!!*Manten Z para descargar el giratorio !"; break;
        case 3  : 
            tmp = 4-(gpJeu->getJoueur()->nbQuarts()%4);
            os << tmp;
            texte = "Has encontrado un cuarto de corazón !!!";
            if (tmp<4) buffer = "Con "+os.str()+" mas obtendras un corazón adicional !!!";
            else buffer = "Tu numero de corazónes acaba de aumentar !!!";
            break;
        case 11 : texte = "N : Casa de Link*O : Finca de Hyrule*E : Templo del Bosque"; break;
        case 12 : texte = "Templo de la Montaña**No aconsejado para cardiacos, miedosos, y emotivos en general."; break;
        case 13 : texte = "O : Entrada del desierto*N : Templo de la Montaña"; break;
        case 14 : texte = "N : Templo de la Montaña*S : Finca de Hyrule"; break;
        case 15 : texte = "N : Montaña de la Muerte"; break;
        case 16 : texte = "E : Entrada del Bosque"; break;
        case 17 : texte = "S : Lago de Hylia*E : Cementerio de los Espíritus"; break;
        case 18 : texte = "S : Hacia el pueblo Kakariko"; break;
        case 19 : texte = "N : Pueblo Kakariko"; break;
        case 20 : texte = "N : Finca de Hyrule"; break;
        case 21 : texte = "O : Tierras de Fuego"; break;
        case 22 : texte = "E : Pueblo de la Sombra*- Prohibido a los seres vivos -"; break;
        case 23 : texte = "Templo de las tinieblas**Si eres un fantasma y que buscas un trabajo, más bien vente a estar parado aquí."; break;
        case 24 : texte = "N : Pueblo de la Sombra*O : Lago de Hylia"; break;
        case 25 : texte = "N : Cementerio de los Espíritus**Entrada prohibida"; break;
        case 26 : texte = "Pueblo de la Sombra"; break;
        case 27 : texte = "Vendo una botella por 100 rupias, quieres ?*              SI ?            no  "; 
            if ((gpJeu->getJoueur()->hasBouteille(0) && gpJeu->getZone()==8)
            || (gpJeu->getJoueur()->hasBouteille(1) && gpJeu->getZone()==5)
            || (gpJeu->getJoueur()->hasBouteille(2) && gpJeu->getZone()==6)) {id=32; chercheText();break;}
            if (gpJeu->getJoueur()->getRubis() + gpJeu->getJoueur()->getBoostRubis()<100) idsuiv=29;
            else idsuiv=30;
            break;
        case 28 : texte = "Vendo una botella por 100 rupias, quieres ?*              si              NO ?"; idsuiv=31; break;
        case 29 : texte = "Lo siento, pero no tienes bastantes rupias."; break;
        case 30 : texte = "Muchas gracias, he aquí tu botella."; break;
        case 31 : texte = "Vuelve si cambias de opinión."; break;
        case 32 : texte = "Lo siento, no tengo más botellas."; break;
        case 33 : texte = "Has encontrado una botella vacía !!!**Podrás utilizarla para almacenar pozimas !!!"; break;
        case 35 : texte = "Los habitantes de este pueblo tienen un dialecto muy especial, no entiendo nada..."; break;
        case 36 : texte = "@+@+@+@+@+@+@@@+*@=++=@=+@=+@=+=@*+@+@+@+=+="; break;
        case 37 : texte = "Obten una autorization del jefe si quieres pasar de verdad !!!"; break;
        case 38 : texte = "El jefe te autorizó ??? Grrrr... Pues apartate!!!"; break;
        case 39 : texte = "Fuera de aquí !!!"; break;
        case 40 : texte = "No solemos ver muchos visitantes por aquí..."; break;
        case 41 : texte = "Los monstruos de aquí no temen muchas cosas, por suerte atacan solo los humanos."; break;
        case 42 : texte = "De verdad ?*Entiendes lo que digo ?"; break;
        case 43 : texte = "N : Desierto y Pueblo Gerudo"; break;
        case 44 : texte = "S : Lago de Hylia*O : Castillo de Hyrule"; break;
        case 45 : texte = "Pueblo Kakariko"; break;
        case 46 : texte = "O : Castillo de Hyrule"; break;
        case 47 : texte = "Que día tan bueno !!!"; break;
        case 48 : texte = "Pero porque acepté cuidar estos bichos ???*Odio las gallinas !!!"; break;
        case 49 : texte = "Se encuentran muchas cosas en las tiendas de este pueblo."; break;
        case 50 : texte = "S : Pueblo Kakariko*O : Desierto*E : Montaña de la Muerte"; break;
        case 51 : texte = "Pueblo Gerudo"; break;
        case 52 : texte = "Perdido en el desierto ?*Estas aquí :*                        X"; break;
        case 54 : texte = "S : Hacia el Castillo de Hyrule"; break;
        case 55 : texte = "Templo del Desierto**Ven a probar nuestros baños.*(agua no potable)"; break;
        case 56 : texte = "Templo del Bosque**Salve los árboles, coma castor!!!"; break;
        case 57 : texte = "Templo del Lago**Gorro de baño obligatorio."; break;
        case 58 : texte = "Templo de Hielo**El personal insiste en recordar que un calabozo no es una pista de patinaje."; break;
        case 59 : texte = "Has visto el herrero del pueblo ?*Dicen que es muy avispado para mejorar el equipo de los aventureros, mucha gente le visita."; break;
        case 60 : texte = "El templo esta en el fondo del desierto, en puesto de una antigua oasis."; break;
        case 61 : texte = "S : Tierras de Fuego*E : Pueblo Kakariko"; break;
        case 62 : texte = "Castillo de Hyrule"; break;
        case 63 : texte = "E : Pueblo Kakariko"; break;
        case 64 : texte = "O : Tortuga de Roca*E : Lago de Hylia"; break;
        case 65 : texte = "Templo Oculto**Aquí descansa la Espada del Golpe Contra el Mal, la Espada Maestra."; break;
        case 66 : texte = "N : Hacia el Cimenterio encantado"; break;
        case 67 : texte = "Has encontrado el mapa del calabozo !!!*Pulsa L y Y para verlo."; break;
        case 68 : texte = "Has encontrado la brújula !!!*Puedes localizar el jefe y las arcas sobre el mapa."; break;
        case 69 : texte = "Has encontrado la llave del jefe !!!"; break;
        case 70 : texte = "Has encontrado una llave pequeña !!!*Acercate de una puerta para abrirla."; break;
        case 71 : texte = "Has encontrado los Guantes !!!*Utilizalos para levantar objetos equipandolos o pulsando X."; break;
        case 72 : 
            texte = "Has encontrado un cristal magico !!!"; 
            tmp = 7-gpJeu->getJoueur()->nbCristaux();
            os << tmp;
            if (tmp==6 && !gpJeu->getJoueur()->getAvancement()) buffer = "Cómo podría ser útil... ?";
            else if (tmp>2) buffer = "Te quedan aún "+os.str()+" por encontrar !!!";
            else if (tmp==2) buffer = "Solo 2 que encontrar !!!";
            else if (tmp==1) buffer = "Te falta solo uno !!!";
            else if (tmp==0) buffer = "Tienes todo los cristales, corre hasta el castillo a salvar a Zelda !!!";
            break;
        case 80 : texte = "Has encontrado el lanzagarfios !!! *Utilizalo para atravesar obstáculos."; break;
        case 81 : texte = "Has encontrado la lampara !!! *Utilizala para hacer surgir llamas."; break;
        case 82 : texte = "Has encontrado las aletas !!! *Equipalas para ir en el agua."; break;
        case 83 : texte = "Has encontrado el martillo mágico !!! *Utilizalo para hundir obstáculos."; break;
        case 84 : texte = "Has encontrado la varilla de fuego !!! *De ahora en adelante puedes lanzar potentes llamas remotas"; break;
        case 85 : texte = "Has encontrado la varilla de hielo !!! *Utilizala para congelar todo a distancia."; break;
        case 86 : texte = "Has encontrado la Espada Maestra !!! *Incluso Ganon no debería ser capaz de resistir a su potencia !!! (en teoría)"; break;
        case 87 : texte = "Felicitaciones Link, has conseguido venir a liberarme!!!***Vamonos de prisa a encontrar a Ganon, tenemos que retomar la Trifuerza a toda costa !"; break;
        case 89 : texte = "El paso secreto detrás de la sala del trono lleva a Ganon. Vamos rapido !"; break;
        case 90 : texte = "Casi hemos llegado, sigueme !"; break;
        case 91 : texte = "Tengo miedo que no puedas nunca vencer a Ganon con tus armas actuales...*Ve a hablar con el jefe del pueblo Kakariko, estoy segura que él va a encontrar una solución."; break;
        case 92 : texte = "Ganon esta ya detrás de esta puerta, voy a curarte las heridas."; break;
        case 93 : texte = "Ganon todavía esta en algún sitio del castillo."; break;
        case 94 : texte = "Tendrías que esperar a la princesa Zelda !!!"; break;
        case 95 : texte = "No tienes la extraña impresión de olvidar a alguien por casualidad ???"; break;
        case 96 : texte = "Zelda te esta esperando!!!"; break;
        case 97 : texte = "Has encontrado la Trifuerza!!!"; break;
        case 98 : texte = "Has encontrado el libro de Mudora!!! **Ahora entiendes el hyliano antiguo!!!"; break;
        case 99 : texte = "Felicitaciones Link, has conseguido encontrarme. En recompensa, te ofrezco el pendiente de Din, aumenta tu defensa de un punto."; break;
        case 100 : texte = "Felicitaciones Link, has conseguido encontrarme. En recompensa, te ofrezco el pendiente de Nayru, aumenta tu defensa de dos puntos!!!"; break;
        case 101 : texte = "..."; break;
        case 102 : texte = "Has obtenido un pendiente mágico !!!**Tu defensa acaba de aumentar!!!"; break;
        case 103 : texte = "Felicitaciones Link, has conseguido encontrarme. Para recompensarte, voy a doblar la capacidad de tu reserva de magia!!!"; break;
        case 104 : texte = "Tu reserva de magia ha doblado!!!"; break;
        case 105 : texte = "Vuelve cuando tengas una botella vacía  y te venderé una pozima roja que te devuelve la energia."; break;
        case 106 : texte = "Una pozima roja por 60 rupias, la quieres ?*              SI ?            no  "; 
            if (gpJeu->getJoueur()->hasBouteille(0)!=1
            && gpJeu->getJoueur()->hasBouteille(1)!=1
            && gpJeu->getJoueur()->hasBouteille(2)!=1) {id=105; chercheText();break;}
            if (gpJeu->getJoueur()->getRubis() + gpJeu->getJoueur()->getBoostRubis()<60) idsuiv=29;
            else idsuiv=108; break;
        case 107 : texte = "Una pozima roja para 60 rupias, la quieres ?*              si              NO ?"; break;
        case 108 : texte = "Gracias, toma tu pozima.";break;
        case 109 : texte = "Has obtenido una pozima roja !!!*Bebela para renovar tu energia!!!"; break;
        case 110 : texte = "Has obtenido una pozima verde !!!*Bebela para renovar tu magia!!!"; break;
        case 111 : texte = "Has obtenido una pozima azul !!!*Bebela para renovar tu energia y tu magia!!!"; break;
        case 112 : texte = "Ola, en que puedo ayudarle ?"; break;
        case 113 : texte = "Hic! Dicen que una hada daría objetos encantados... Hic! ...a los aventureros que la encuentran... Hic!!!"; break;
        case 114 : texte = "Un corazón por 10 rupias, vale ?**              SI ?            no  "; 
            if (gpJeu->getJoueur()->getVie()+gpJeu->getJoueur()->getBoostVie()
            == gpJeu->getJoueur()->getVieMax()) {id=128; chercheText();break;}
            if (gpJeu->getJoueur()->getRubis() + gpJeu->getJoueur()->getBoostRubis()<10) idsuiv=29;
            else idsuiv=129; break;
        case 115 : texte = "Un corazón por 10 rupias, vale ?**             si              NO ?"; break;
        case 116 : texte = "Un poco de magia por 20 rupias, vale ?**              SI ?            no  "; 
            if (gpJeu->getJoueur()->getMagie()+gpJeu->getJoueur()->getBoostMagie()
            == gpJeu->getJoueur()->getMagieMax() || !gpJeu->getJoueur()->hasObjet(O_LANTERNE)) {
                id=128; chercheText();break;}
            if (gpJeu->getJoueur()->getRubis() + gpJeu->getJoueur()->getBoostRubis()<20) idsuiv=29;
            else idsuiv=129; break;
        case 117 : texte = "Un poco de magia por 20 rupias, vale ?**              si              NO ?"; break;
        case 118 : texte = "Mucho magia por 30 rupias, vale ?**              SI ?            no  "; 
            if (gpJeu->getJoueur()->getMagie()+gpJeu->getJoueur()->getBoostMagie()
            == gpJeu->getJoueur()->getMagieMax() || !gpJeu->getJoueur()->hasObjet(O_LANTERNE)) {
                id=128; chercheText();break;}
            if (gpJeu->getJoueur()->getRubis() + gpJeu->getJoueur()->getBoostRubis()<30) idsuiv=29;
            else idsuiv=129; break;
        case 119 : texte = "Mucho magia por 30 rupias, vale ?**              si              NO ?"; break;
        case 120 : texte = "5 flechas por 30 rupias, vale ?**              SI ?            no  "; 
            if (gpJeu->getJoueur()->getFleche() == gpJeu->getJoueur()->getFlecheMax() 
            || !gpJeu->getJoueur()->hasObjet(O_ARC)) {id=128; chercheText();break;}
            if (gpJeu->getJoueur()->getRubis() + gpJeu->getJoueur()->getBoostRubis()<30) idsuiv=29;
            else idsuiv=129; break;
        case 121 : texte = "5 flechas por 30 rupias, vale ?**              si              NO ?"; break;
        case 122 : texte = "Una bomba por 30 rupias, vale ?**              SI ?            no  "; 
            if (gpJeu->getJoueur()->getBombe() == gpJeu->getJoueur()->getBombeMax() 
            || !gpJeu->getJoueur()->hasObjet(O_SAC_BOMBES)) {id=128; chercheText();break;}
            if (gpJeu->getJoueur()->getRubis() + gpJeu->getJoueur()->getBoostRubis()<30) idsuiv=29;
            else idsuiv=129; break;
        case 123 : texte = "Una bomba por 30 rupias, vale ?**              si              NO ?"; break;
        case 124 : texte = "Un arco por 1000 rupias, vale ?**              SI ?            no  "; 
            idsuiv=29; break;
        case 125 : texte = "Un arco por 1000 rupias, vale ?**              si              NO ?"; break;
        case 126 : texte = "Un arco por 50 rupias, vale ?**              SI ?            no  "; 
            if (gpJeu->getJoueur()->getRubis() + gpJeu->getJoueur()->getBoostRubis()<50) idsuiv=29;
            else idsuiv=129; break;
        case 127 : texte = "Un arco por 50 rupias, vale ?**              si              NO ?"; break;
        case 128 : texte = "No necesitas eso ahora."; break;
        case 129 : texte = "Gracias."; break;
        case 130 : texte = "Has obtenido el arco !!!*Utilizalo para alcanzar objetivos alejados."; break;
        case 131 : texte = "Elige lo que quieras."; break;
        case 132 : texte = "Que ???*Estas en contratado por el jefe ???*Pues vale, te hago une reducción sobre el arco..."; break;
        case 133 : texte = "Buenos días Link, soy el jefe de este pueblo, me figuraba que vendriás."; idsuiv=134; break;
        case 134 : texte = "Desde esta mañana monstruos aparecen en muchas partes de Hyrule, he intentado ir a hablar de eso con la princesa Zelda, pero como quizás ya lo has notado, un poderoso hechizo bloquea el acceso al castillo..."; idsuiv=136; break;
        case 136 : texte = "Dices que Ganon es responsable de todo esto ? Que ha robado la Trifuerza y que encarcela la princesa Zelda en el castillo ?"; idsuiv=137; break;
        case 137 : texte = "Pues... la situación es mucho más grave que lo que creía..."; idsuiv=138; break;
        case 138 : texte = "Tenemos que obrar rapidamente, debes enfrentar a Ganon de nuevo!"; idsuiv=139; break;
        case 139 : texte = "Cómo entrar en el castillo ? Quizás tenga una idea..."; idsuiv=140; break;
        case 140 : texte = "Como lo sabes, el poder de los 7 Hombres Sabios fue encerrado en 7 cristales por el mago Aghanim mientras intentaba quitar el hechizo que confinaba Ganon en el Dark World."; idsuiv=141; break;
        case 141 : texte = "Ahora bien, aunque resucitaste las descendientes de los hombres sabios despues de vencer a Ganon y de encontrar la Trifuerza, estos cristales conservaron sus potencia."; idsuiv=142; break;
        case 142 : texte = "Estan escondidos en los 7 templos, reúnelos todos y podrás ir más allá del sortilego de Ganon."; idsuiv=143; break;
        case 143 : texte = "Sin embargo, será una tarea muy dura, Ganon sin duda ha debido de enviar sus mejores unidades a proteger esos cristales..."; idsuiv=144; break;
        case 144 : texte = "Una ultima cosa, no irás muy lejos con tu equipo actual. Ve a ver al vendedor de armas y dile que vienes de mi parte. Seguro que tu ayudará."; break;
        case 145 : texte = "Ve a ver el vendedor de armas y dile que vienes de mi parte. Seguro que tu ayudará."; break;
        case 146 : texte = "Tienes que reunir los 7 cristales si quieres tener una posibilidad de salvar a Zelda y Hyrule..."; break;
        case 147 : texte = "Has encontrado los 7 cristales ?  Ve al castillo, primero debes liberar a la princesa, ella sabrá mejor que yo cómo salvar Hyrule."; break;
        case 148 : texte = "Tus armas no tienen efecto sobre Ganon ? En este caso tienes que encontrar la Espada Maestra. Se dice que fue disimulada en un templo secreto.*Se dice tambien que el camino que conduce a la Espada del Golpe Contra el Mal pasa por un pueblo lleno de monstros...*Me pregunto si tal pueblo existe..."; break;
        case 150 : texte = "No obstante tengo miedo que tu equipo actual sea insuficiente para acceder a este templo, a lo mejor deberías ir a ver al herrero del pueblo Gerudo..."; break;
        case 151 : texte = "Ya es hora de enfrentarse a Ganon y de recuperar la Trifuerza!"; break;
        case 152 : texte = "Hey! Hay que pagar para abrir una de mis arcas!!!"; break;
        case 153 : texte = "Una de estas arcas contiene un cuarto de corazón, quieres probar tu fortuna por 10 rupias ? Podrás abrir solo una, vale ?*              SI ?            no  "; 
            if (gpJeu->getJoueur()->getRubis() + gpJeu->getJoueur()->getBoostRubis()<10) idsuiv=29; break;
        case 154 : texte = "Una de estas arcas contiene un cuarto de corazón, quieres probar tu fortuna por 10 rupias ? Podrás abrir solo una, vale ?*              si              NO ?"; break;
        case 155 : texte = "Elige un arca."; break;
        case 156 : texte = "No tengo más premios que poner en juego, lo siento."; break;
        case 157 : texte = "Has perdido! Este arca esta vacía."; break;
        case 158 : texte = "Buenos días! Si estas aquí para ver el herrero, vive un poco más lejos."; break;
        case 159 : texte = "Ola Link, soy el jefe de este pueblo.*Deberías ir a ver el herrero y monstrarle lo que quieres que mejore en tu equipo.";
            buffer = "He sido informado de tu búsqueda, entonces me puse de acuerdo con el.*Todo será gratis para ti."; break;
        case 160 : texte = "No necesitas beber esta pozima ahora."; break;
        case 161 : texte = "Una pozima verde por 40 rupias, la quieres ?*              SI ?            no  "; 
            if (gpJeu->getJoueur()->hasBouteille(0)!=1
            && gpJeu->getJoueur()->hasBouteille(1)!=1
            && gpJeu->getJoueur()->hasBouteille(2)!=1) {id=163; chercheText();break;}
            if (gpJeu->getJoueur()->getRubis() + gpJeu->getJoueur()->getBoostRubis()<40) idsuiv=29;
            else idsuiv=108; break;
        case 162 : texte = "Una pozima verde por 40 rupias, la quieres ?*              si              NO ?"; break;
        case 163 : texte = "Vuelve cuando tengas una botella vacía y te venderé una pozima verde que rellena la magia."; break;
        case 164 : texte = "Vuelve más tarde, estoy agobiado!"; break;
        case 165 : texte = "Puedo hacer algo para ti ?";
            if (gpJeu->getJoueur()->getEpee()==1) {id=166; chercheText();break;}
            if (gpJeu->getJoueur()->hasObjet(O_GANTS)==1 && gpJeu->getJoueur()->getObjet()==8) {
                id=169; chercheText();break;}
            if (gpJeu->getJoueur()->hasObjet(O_ARC)==2 && gpJeu->getJoueur()->getObjet()==0 
            && gpJeu->getJoueur()->nbCristaux()==7) {
                id=172; chercheText();break;}
            break;
        case 166 : texte = "Quieres que mejore tu espada ?**              SI ?            no  "; break;
        case 167 : texte = "Quieres que mejore tu espada ?**              si              NO ?"; break;
        case 168 : texte = "Tu espada ha triplicado de potencia y puede ahora devolver algunos ataques magicos, guay!!!"; break;
        case 169 : texte = "Quieres que mejore tus guantes ?**              SI ?            no  "; break;
        case 170 : texte = "Quieres que mejore tus guantes ?**              si              NO ?"; break;
        case 171 : texte = "La potencia de tus guantes te permite ahora levantar rocas pesadas."; break;
        case 172 : texte = "Quieres que mejore tu arco ?**              SI ?            no  "; break;
        case 173 : texte = "Quieres que mejore tu arco ?**              si              NO ?"; break;
        case 174 : texte = "Tu arco tira ahora flechas de plata de una potencia devastadora !!!"; break;
        case 175 : texte = "Vuelve cuando tengas una botella vacía y te venderé una pozima azul que rellena la energia y la magia."; break;
        case 176 : texte = "Una pozima azul por 160 rupias, la quieres ?*              SI ?            no  "; 
            if (gpJeu->getJoueur()->hasBouteille(0)!=1
            && gpJeu->getJoueur()->hasBouteille(1)!=1
            && gpJeu->getJoueur()->hasBouteille(2)!=1) {id=175; chercheText();break;}
            if (gpJeu->getJoueur()->getRubis() + gpJeu->getJoueur()->getBoostRubis()<160) idsuiv=29;
            else idsuiv=108; break;
        case 177 : texte = "Una pozima azul por 160 rupias, la quieres ?*              si              NO ?"; break;
        case 178 : texte = "Nuestro pueblo fue fundado para impedir a los aventureros imprudentes que tengan acceso al templo, no da el permiso para pasar a cualquiera."; break;
        case 179 : texte = "Mmmm... Ya has encontrado 4 cristales ?*Tengo que confesar que me impresionas...";
            buffer = "Pues entendido, tienes el permiso para pasar por el cementerio, por tu cuenta y riesgo."; break;
        case 181 : texte = "Ve a hablar al guardia al norte del pueblo, te dejará pasar."; break;
        case 182 : texte = "Veo que siempre estas vivo...*Sorprendente."; break;
        case 183 : texte = "El templo de las tinieblas es el punto de partida del camino que conduce a un objeto de leyenda. Este pueblo solo sirve para disuadir a los aventureros de acercarse demasiado."; break;
        case 184 : texte = "Has encontrado la Espada ?*Pero no creas que es tuya por eso.";
            buffer = "La Espada Maestra forma parte del patrimonio el más valioso de Hyrule con la Trifuerza, tendrás que devolverla en su santuario en cuanto acabes tu misión."; break;
        case 186 : texte = "Los habitantes de este pueblo son todos espíritus que han contestado a la llamada de la princesa Zelda."; break;
        case 187 : texte = "Cuando devolviste la paz sobre Hyrule y reunificaste el Light World con el Golden Land, Zelda preguntó en secreto a los espíritus voluntarios que crean este pueblo para bloquear";
            buffer = "el acceso a un objeto muy valioso."; break;
        
        case 189 : texte = "Bienvenido Link!*Dejame explicarte lo que hago aquí."; idsuiv=190; break;
        case 190 : texte = "Como ya lo has notado sin duda, monstruos han aparecido por todas partes en Hyrule. Es la primera vez que veo monstruos despues que venciste a Ganon."; idsuiv=191; break;
        case 191 : texte = "Pues he decidido catalogarlos todos."; idsuiv=192; break;
        case 192 : texte = "El problema es que soy demasiado miedoso para ir al encuentro de ellos, necesito tu ayuda."; idsuiv=193; break;
        case 193 : texte = "Tienes que golpear cada tipo de monstruo y venir a hacerme tu informe."; idsuiv=194; break;
        case 194 : texte = "Te recompensaré cada vez que mates 7 nuevos tipos de enemigo."; idsuiv=195; break;
        case 195 : texte = "Si quieres ver que enemigos ya has vencido, pulsa L y B."; break;
        
        
        case 196 : 
            if (gpJeu->getJoueur()->getCoeur(43)) {
                if (gpJeu->getJoueur()->nbEnnemis() >= 46) {id=205; chercheText();return;}
                else {id=204; chercheText();return;}
            }
            for (int i = 38; i < 44; i++) 
                if (!gpJeu->getJoueur()->getCoeur(i) 
                && (gpJeu->getJoueur()->nbEnnemis()/7)+37>=i) {
                    id=203; chercheText();return;
                }
            tmp = 7-(gpJeu->getJoueur()->nbEnnemis()%7);
            os << tmp;
            if (tmp>1) texte = "Con "+os.str()+" mas enemigos diferentes tendras la proxima recompensa.";
            else texte = "Te falta solo uno nuevo enemigo antes de la proxima recompensa!!!";
            break;
        case 203 : texte = "Acepta este regalo por tu contribución en mi proyecto :"; break;
        case 204 : texte = "Lo siento pero no tengo nada más que regalarte..."; break;
        case 205 : texte = "No me le puedo creer, has conseguido vencer cada tipo de enemigo !!!*Felicitaciones Link !!!"; break;
        
        case 206 : texte = "                    Nivel 1 :**               Templo del Bosque"; break;
        case 207 : texte = "                    Nivel 2 :**                Templo del Agua"; break;
        case 208 : texte = "                    Nivel 3 :**              Templo de la Montaña"; break;
        case 209 : texte = "                    Nivel 4 :**              Templo del Desierto"; break;
        case 210 : texte = "                    Nivel 5 :**            Templo de las Tinieblas"; break;
        case 211 : texte = "                    Nivel 6 :**                Templo del Fuego"; break;
        case 212 : texte = "                    Nivel 7 :**                Templo de Hielo"; break;
        case 213 : texte = "                    Nivel ? :**                 Templo Oscuro"; break;
        case 214 : texte = "                  Nivel Final :**               Castillo de Hyrule"; break;
        
        case 215 :texte = "               Ya estas cansado ?               ?                   SEGUIR                  ?                  Grabar y dejar                                Dejar sin grabar              "; break;
        case 216 :texte = "               Ya estas cansado ?                                   Seguir                     ?               GRABAR Y DEJAR              ?                 Dejar sin grabar              "; break;
        case 217 :texte = "               Ya estas cansado ?                                   Seguir                                     Grabar y dejar                 ?              DEJAR SIN GRABAR             ?"; break;
        
        case 223 : texte = "En el momento en que Link tocó la Trifuerza, los monstruos desaparecieron y la paz volvió.**La princesa Zelda tomó entonces una gran decisión: tocó a su vez la Trifuerza para formular su deseo...**La Trifuerza siempre fue el origen de los problemas sobrevenidos en el Reino,  tarde o temprano una otra criatura dañina habría acabado encontrandola.**Al mismo tiempo que la princesa tocó la reliquia, desapareció para siempre de Hyrule.";
            buffer = "Desde ese día, el pueblo de Hyrule vivió en paz, sin temer nuevos desastres.**Asi se acaba la leyenda de Zelda, de la  Trifuerza y del Golden Land.**Se dice que la Espada Maestra descansa en seguridad en cualquier lugar, y que es el ultimo recuerdo de la búsqueda de Link..."; break;
        case 224 : texte = "Felicitaciones Link, has conseguido encontrarme. En recompensa, te ofrezco el pendiente de Farore, aumenta tu defensa de un punto."; break;
        case 225 : texte = "Quieres grabar tu juego ?**                      ? SI ?*                        no  "; break;
        case 226 : texte = "Quieres grabar tu juego ?**                        si  *                      ? NO ?"; break;
        case 227 : texte = "Juego grabado."; break;
        
        case 228 : texte = "El Reino de Hyrule estaba en paz desde que Link, ultimo cavallero hyliano, había vencido el maléfico Ganon y había recuperado la valiosa Trifuerza."; idsuiv = 229; break;
        case 229 : texte = "Nadie sabe que deseo hizo Link tocando la Trifuerza, pero tuvo como efecto de reunificar el Light World y el Dark World, y volvió a los descendientes de los 7 Hombres Sabios a la vida."; idsuiv = 230; break;
        case 230 : texte = "Despues Link devolvió a la princesa Zelda la Trifuerza y la Espada Maestra, y el pueblo se puso a creer que la paz duraria.*Pero el pueblo se equivocaba..."; idsuiv=231; break;
        case 231 : texte = "El deseo de Link a la Trifuerza igualmente tuvo lamentables efectos. Ganon y sus esbirros fuen asi resucitados y se dispusieron a atacar."; idsuiv=232; break;
        case 232 : texte = "En alguna parte del bosque de Hyrule, Link duerme sin sospechar que Ganon ya ha pasado al ataque, hasta que una voz familiar le habla en su sueño..."; break;
        case 233 : texte = "Ayuda!*Ayuda!*Soy yo! Zelda!*Te hablo por telepatía."; idsuiv = 234; break;
        case 234 : texte = "Estoy prisionera en el torreón del castillo! Necesito tu ayuda!*Ganon esta de vuelta, y sin duda ya ha encontrado la Trifuerza a estas horas..."; idsuiv=235; break;
        case 235 : texte = "Corre al castillo Link, eres mi única esperanza..."; break;
        case 236 : texte = "AYUDA : Pulsa L y X para consultar la ayuda."; break;
    }

break; // Spanish

default:	
    switch (id) {
        case 1  : texte = "You found a shield!**Your defense increases by one point!"; break;
        case 2  : texte = "You found a sword!**You can now fight monsters with the B .*Hold B to charge a spin attack!"; break;
        case 3  : 
            tmp = 4-(gpJeu->getJoueur()->nbQuarts()%4);
            os << tmp;
            texte = "You found a piece of heart!!!";
            if (tmp<4) buffer = "You need "+os.str()+" more for a new heart container.";
            else buffer = "Your heart level increased!";
            break;
        case 11 : texte = "N: Link's Home*W: Hyrule Field*E: Forest Temple"; break;
        case 12 : texte = "Mountain Temple**Not for cardiacs, scaredy-cats, and the generally faint of heart."; break;
        case 13 : texte = "W: Desert entrance*N: Mountain Temple"; break;
        case 14 : texte = "N: Mountain Temple*S: Hyrule Field"; break;
        case 15 : texte = "N: Death Mountain"; break;
        case 16 : texte = "E: Forest Entrance"; break;
        case 17 : texte = "S: Lake Hylia*E: Haunted Graveyard"; break;
        case 18 : texte = "S: To Kakariko Village"; break;
        case 19 : texte = "N: Kakariko Village"; break;
        case 20 : texte = "N: Hyrule Field"; break;
        case 21 : texte = "W: Fire Land"; break;
        case 22 : texte = "E: Shadow Village*- Not for the living -"; break;
        case 23 : texte = "Dark Temple**If you are a ghost, looking for a job, you'd better come here to hang out with us."; break;
        case 24 : texte = "N: Shadow Village*W: Lake Hylia"; break;
        case 25 : texte = "N: Haunted Graveyard**No entry"; break;
        case 26 : texte = "Shadow Village"; break;
        case 27 : texte = "I am selling a bottle for 100 rupees, are you interested?*              YES ?            no  "; 
            if ((gpJeu->getJoueur()->hasBouteille(0) && gpJeu->getZone()==8)
            || (gpJeu->getJoueur()->hasBouteille(1) && gpJeu->getZone()==5)
            || (gpJeu->getJoueur()->hasBouteille(2) && gpJeu->getZone()==6)) {id=32; chercheText();break;}
            if (gpJeu->getJoueur()->getRubis() + gpJeu->getJoueur()->getBoostRubis()<100) idsuiv=29;
            else idsuiv=30;
            break;
        case 28 : texte = "I am selling a bottle for 100 rupees, are you interested?*              yes             NO ?"; idsuiv=31; break;
        case 29 : texte = "Sorry, you don't have enough rupees"; break;
        case 30 : texte = "Thank you, here is your bottle."; break;
        case 31 : texte = "Come again if you change your mind."; break;
        case 32 : texte = "Sorry, that was my only bottle."; break;
        case 33 : texte = "You find an empty bottle!**It will help you to store potions."; break;
        case 35 : texte = "The inhabitants of that village have a dialect out of the ordinary. I don't understand anything..."; break;
        case 36 : texte = "@+@+@+@+@+@+@@@+*@=++=@=+@=+@=+=@*+@+@+@+=+="; break;
        case 37 : texte = "Wangle chief's permit if you really want to pass!"; break;
        case 38 : texte = "The chief allowed you to pass? Grrrr... So move along!"; break;
        case 39 : texte = "Get out of my way!"; break;
        case 40 : texte = "We don't often have visitors around here."; break;
        case 41 : texte = "Local monsters don't fear many things. Fortunately, they only attack humans."; break;
        case 42 : texte = "Really?*You understand what I'm saying?"; break;
        case 43 : texte = "N: Desert and Gerudo Village"; break;
        case 44 : texte = "S: Lake Hylia*W: Hyrule Castle"; break;
        case 45 : texte = "Kakariko Village"; break;
        case 46 : texte = "W: Hyrule Castle"; break;
        case 47 : texte = "What a good day!"; break;
        case 48 : texte = "Why did I accept to take care of that rubbish?*I loathe hens!"; break;
        case 49 : texte = "You can find a lot of things in this village's shop."; break;
        case 50 : texte = "S: Kakariko Village*W: Desert*E: Death Mountain"; break;
        case 51 : texte = "Gerudo Village"; break;
        case 52 : texte = "Lost in the desert?*You are here:*                        X"; break;
        case 54 : texte = "S: To Hyrule Castle"; break;
        case 55 : texte = "Desert Temple**Come to try our baths.*(The water is not for drinking)"; break;
        case 56 : texte = "Forest Temple**Save the trees, eat beaver!"; break;
        case 57 : texte = "Lake Temple**Bathing cap mandatory."; break;
        case 58 : texte = "Ice Temple**The staff would like to remind you that a dungeon is not a ice rink."; break;
        case 59 : texte = "Did you see the blacksmith of this village?*He is said to be very gifted in the way of improving adventurers' equipment. A lot of people visit him."; break;
        case 60 : texte = "The Temple is in the deepest desert, in an ancient oasis."; break;
        case 61 : texte = "S: Fire Land*E: Kakariko Village"; break;
        case 62 : texte = "Hyrule Castle"; break;
        case 63 : texte = "E: Kakariko Village"; break;
        case 64 : texte = "W: Turtle Rock*E: Lake Hylia"; break;
        case 65 : texte = "Hidden Temple**Here rests the Sword of Evils Bane, the Master Sword."; break;
        case 66 : texte = "N: To the Haunted Graveyard"; break;
        case 67 : texte = "You found the Dungeon Map!*Press the L and Y button to view the map."; break;
        case 68 : texte = "You found the Compass!*You can locate the boss and chests on the plan."; break;
        case 69 : texte = "You found the Boss Key!"; break;
        case 70 : texte = "You found a small key!*Walk up to a door to open it."; break;
        case 71 : texte = "You found the Gloves!*Use them to lift objects with the X button."; break;
        case 72 : 
            texte = "You found a Magic Crystal!"; 
            tmp = 7-gpJeu->getJoueur()->nbCristaux();
            os << tmp;
            if (tmp==6 && !gpJeu->getJoueur()->getAvancement()) buffer = "What could it be used for...?";
            else if (tmp>2) buffer = "There are "+os.str()+" left to find!";
            else if (tmp==2) buffer = "Only two left to find!";
            else if (tmp==1) buffer = "You are only missing one!";
            else if (tmp==0) buffer = "You have all the crystals, run to the castle and save Zelda!";
            break;
        case 80 : texte = "You found the Hookshot!*Use it to overcome obstacles."; break;
        case 81 : texte = "You found the Lantern!*Use it to set things on fire."; break;
        case 82 : texte = "You found the Flippers!*Select them in the item menu to swim in the water."; break;
        case 83 : texte = "You found the Magic Hammer!*Use it to squash obstacles."; break;
        case 84 : texte = "You found the Fire Rod!*From now on you are able to shoot out powerful flames."; break;
        case 85 : texte = "You found the Ice Rod!*Use it to freeze anything from a distance."; break;
        case 86 : texte = "You found the Master Sword!*Even Ganon could not stand up to its power... in theory!"; break;
        case 87 : texte = "Thank you for saving me, Link.***Let's find Ganon quickly; we have to reclaim the Triforce!"; break;
        case 89 : texte = "The secret passage at the back of the throne room leads to Ganon. Hurry up!"; break;
        case 90 : texte = "We are very near, follow me!"; break;
        case 91 : texte = "I'm afraid you are not able to defeat Ganon with your current weapons...*Go and speak to the chief of Kakariko Village, I am sure he will find a solution."; break;
        case 92 : texte = "Ganon is just behind that door. I will cure your wounds."; break;
        case 93 : texte = "Ganon is still somewhere in the castle."; break;
        case 94 : texte = "You should wait for Princess Zelda!"; break;
        case 95 : texte = "Don't you have the strange feeling that you are forgetting someone, by any chance?"; break;
        case 96 : texte = "Zelda is waiting for you!"; break;
        case 97 : texte = "You found the Triforce!"; break;
        case 98 : texte = "You found the Book of Mudora!**From now on, you understand the ancient Hylian language!"; break;
        case 99 : texte = "Congratulations for finding me, Link. As a reward, I give you Din's Pendant. It increases your defense by one point."; break;
        case 100 : texte = "Congratulations for finding me, Link. As a reward, I give you Nayru's Pendant. It increases your defense by two points."; break;
        case 101 : texte = "..."; break;
        case 102 : texte = "You obtain a magic pendant!**Your defense increased!"; break;
        case 103 : texte = "Congratulation for finding me, Link. As a reward, I will double the capacity of your magic meter!"; break;
        case 104 : texte = "Your magic meter has been doubled!"; break;
        case 105 : texte = "Come back when you have an empty bottle, and I will sell you a red potion which restores your health."; break;
        case 106 : texte = "A red potion for 60 rupees. Are you interested?*              YES ?            no  "; 
            if (gpJeu->getJoueur()->hasBouteille(0)!=1
            && gpJeu->getJoueur()->hasBouteille(1)!=1
            && gpJeu->getJoueur()->hasBouteille(2)!=1) {id=105; chercheText();break;}
            if (gpJeu->getJoueur()->getRubis() + gpJeu->getJoueur()->getBoostRubis()<60) idsuiv=29;
            else idsuiv=108; break;
        case 107 : texte = "A red potion for 60 rupees. Are you interested?*              yes              NO ?"; break;
        case 108 : texte = "Thank you, here is your potion.";break;
        case 109 : texte = "You got a red potion!*Drink it to restore your health."; break;
        case 110 : texte = "You got a green potion!*Drink it to restore your magic meter."; break;
        case 111 : texte = "You got a blue potion!*Drink it to restore your health and magic meter."; break;
        case 112 : texte = "Hello, what would you like to drink?"; break;
        case 113 : texte = "-Hiccup!- A fairy is said to give enchanted objects... -Hiccup!- ...to the adventurers who would find her... -Hiccup!!!-"; break;
        case 114 : texte = "One heart for 10 rupees. Are you interested?**              YES ?            no  "; 
            if (gpJeu->getJoueur()->getVie()+gpJeu->getJoueur()->getBoostVie()
            == gpJeu->getJoueur()->getVieMax()) {id=128; chercheText();break;}
            if (gpJeu->getJoueur()->getRubis() + gpJeu->getJoueur()->getBoostRubis()<10) idsuiv=29;
            else idsuiv=129; break;
        case 115 : texte = "One heart for 10 rupees. Are you interested?**              yes              NO ?"; break;
        case 116 : texte = "A little bit of magic for 20 rupees. Are you interested?**              YES ?            no  "; 
            if (gpJeu->getJoueur()->getMagie()+gpJeu->getJoueur()->getBoostMagie()
            == gpJeu->getJoueur()->getMagieMax() || !gpJeu->getJoueur()->hasObjet(O_LANTERNE)) {
                id=128; chercheText();break;}
            if (gpJeu->getJoueur()->getRubis() + gpJeu->getJoueur()->getBoostRubis()<20) idsuiv=29;
            else idsuiv=129; break;
        case 117 : texte = "A little bit of magic for 20 rupees. Are you interested?**              yes              NO ?"; break;
        case 118 : texte = "Some magic for 30 rupees. Are you interested?**              YES ?            no  "; 
            if (gpJeu->getJoueur()->getMagie()+gpJeu->getJoueur()->getBoostMagie()
            == gpJeu->getJoueur()->getMagieMax() || !gpJeu->getJoueur()->hasObjet(O_LANTERNE)) {
                id=128; chercheText();break;}
            if (gpJeu->getJoueur()->getRubis() + gpJeu->getJoueur()->getBoostRubis()<30) idsuiv=29;
            else idsuiv=129; break;
        case 119 : texte = "Some magic for 30 rupees. Are you interested?**              yes              NO ?"; break;
        case 120 : texte = "5 arrows for 30 rupees. Are you interested?**              YES ?            no  "; 
            if (gpJeu->getJoueur()->getFleche() == gpJeu->getJoueur()->getFlecheMax() 
            || !gpJeu->getJoueur()->hasObjet(O_ARC)) {id=128; chercheText();break;}
            if (gpJeu->getJoueur()->getRubis() + gpJeu->getJoueur()->getBoostRubis()<30) idsuiv=29;
            else idsuiv=129; break;
        case 121 : texte = "5 arrows for 30 rupees. Are you interested?**              yes              NO ?"; break;
        case 122 : texte = "One bomb for 30 rupees. Are you interested?**              YES ?            no  "; 
            if (gpJeu->getJoueur()->getBombe() == gpJeu->getJoueur()->getBombeMax() 
            || !gpJeu->getJoueur()->hasObjet(O_SAC_BOMBES)) {id=128; chercheText();break;}
            if (gpJeu->getJoueur()->getRubis() + gpJeu->getJoueur()->getBoostRubis()<30) idsuiv=29;
            else idsuiv=129; break;
        case 123 : texte = "One bomb for 30 rupees. Are you interested?**              yes              NO ?"; break;
        case 124 : texte = "A bow for 1000 rupees. Are you interested?**              YES ?            no  "; 
            idsuiv=29; break;
        case 125 : texte = "A bow for 1000 rupees. Are you interested?**              yes              NO ?"; break;
        case 126 : texte = "A bow for 50 rupees. Are you interested?**              YES ?            no  "; 
            if (gpJeu->getJoueur()->getRubis() + gpJeu->getJoueur()->getBoostRubis()<50) idsuiv=29;
            else idsuiv=129; break;
        case 127 : texte = "A bow for 50 rupees. Are you interested?**              yes              NO ?"; break;
        case 128 : texte = "You don't need this right now."; break;
        case 129 : texte = "Thank you."; break;
        case 130 : texte = "You got the bow!*Use it to shoot at distant targets."; break;
        case 131 : texte = "Pick what you want."; break;
        case 132 : texte = "What?*You are on a mission for the chief?*Okay, so I'm going to give you a discount on the bow"; break;
        case 133 : texte = "Hello Link, I am the chief of this village. I suspected you were coming."; idsuiv=134; break;
        case 134 : texte = "Since this morning, monsters stride along Hyrule. I tried to talk to Zelda about it, but as you've perhaps already seen, a powerful spell blocks the access to the castle."; idsuiv=136; break;
        case 136 : texte = "You say Ganon is responsible for all of this? He stole the Triforce and he holds Princess Zelda captive?"; idsuiv=137; break;
        case 137 : texte = "Mmmm... The situation is more serious than I thought..."; idsuiv=138; break;
        case 138 : texte = "We have to act very quickly. You must face Ganon again!"; idsuiv=139; break;
        case 139 : texte = "How do we enter the castle? I may have an idea..."; idsuiv=140; break;
        case 140 : texte = "As you know, the power of the 7 wise men was locked up in 7 crystals by Aghanim when he was trying to open up the path to the Dark World, where Ganon was."; idsuiv=141; break;
        case 141 : texte = "Yet, even if you raised the descendants of the 7 wise men from the dead after you defeated Ganon and found the Triforce, these crystals have kept their power."; idsuiv=142; break;
        case 142 : texte = "They have been hidden deep in the 7 temples. Gather them all, and you'll be able to get over Ganon's spell."; idsuiv=143; break;
        case 143 : texte = "That won't be easy, however. Ganon will surely send his best troops to protect these crystals."; idsuiv=144; break;
        case 144 : texte = "One last thing. You won't go very far with your current equipment. Go and see the arms dealer and tell him that you're coming on my behalf. He will probably make a gesture for you."; break;
        case 145 : texte = "Go and see the arms dealer and tell him that you're coming on my behalf. He will probably make a gesture for you."; break;
        case 146 : texte = "You have to gather the 7 crystals to have a chance at saving Zelda and Hyrule."; break;
        case 147 : texte = "You found the 7 crystals? Go to the castle! First, you have to save the princess. She will know better than I how to save Hyrule."; break;
        case 148 : texte = "Your weapons are null and void against Ganon? You have to find the Master Sword. It is said to have been concealed in a hidden temple.*The way leading to the Sword of Evils Bane is said to cross over a village populated by monsters...*I wonder if such a village exists."; break;
        case 150 : texte = "However, I'm afraid that your current equipment is inadequate for reaching this temple. You should go see the blacksmith of Gerudo village."; break;
        case 151 : texte = "It's about time to face Ganon and take the Triforce back!"; break;
        case 152 : texte = "Hey! You have to pay to open one of my chests!"; break;
        case 153 : texte = "One of these chests contains a piece of heart. Do you want to try for 10 rupees?*You will only be allowed to open one chest, ok?               YES ?            no  "; 
            if (gpJeu->getJoueur()->getRubis() + gpJeu->getJoueur()->getBoostRubis()<10) idsuiv=29; break;
        case 154 : texte = "One of these chests contains a piece of heart. Do you want to try for 10 rupees?*You will only be allowed to open one chest, ok?               yes              NO ?"; break;
        case 155 : texte = "Pick a chest."; break;
        case 156 : texte = "I don't have anymore prizes to bring into play, sorry."; break;
        case 157 : texte = "You lose! This chest is empty. Try again!"; break;
        case 158 : texte = "Hello! If you are looking for the blacksmith, he lives a little further ahead."; break;
        case 159 : texte = "Hello Link, I am the chief of this village.*You should go to see the blacksmith and show him what equipment you want him to improve.";
            buffer = "I have heard of your quest, so I have arranged everything with him.*All will be free for you."; break;
        case 160 : texte = "You don't need to drink this potion right now."; break;
        case 161 : texte = "A green potion for 40 rupees. Are you interested?*              YES ?            no  "; 
            if (gpJeu->getJoueur()->hasBouteille(0)!=1
            && gpJeu->getJoueur()->hasBouteille(1)!=1
            && gpJeu->getJoueur()->hasBouteille(2)!=1) {id=163; chercheText();break;}
            if (gpJeu->getJoueur()->getRubis() + gpJeu->getJoueur()->getBoostRubis()<40) idsuiv=29;
            else idsuiv=108; break;
        case 162 : texte = "A green potion for 40 rupees. Are you interested?*              yes              NO ?"; break;
        case 163 : texte = "Come back when you have an empty bottle, and I will sell you a green potion which restores your magic meter."; break;
        case 164 : texte = "Come back later. I am overbooked!"; break;
        case 165 : texte = "May I do something for you?";
            if (gpJeu->getJoueur()->getEpee()==1) {id=166; chercheText();break;}
            if (gpJeu->getJoueur()->hasObjet(O_GANTS)==1 && gpJeu->getJoueur()->getObjet()==8) {
                id=169; chercheText();break;}
            if (gpJeu->getJoueur()->hasObjet(O_ARC)==2 && gpJeu->getJoueur()->getObjet()==0 
            && gpJeu->getJoueur()->nbCristaux()==7) {
                id=172; chercheText();break;}
            break;
        case 166 : texte = "Do you want me to temper your sword?**              YES ?            no  "; break;
        case 167 : texte = "Do you want me to temper your sword?**              yes              NO ?"; break;
        case 168 : texte = "Your sword triples in power and you can now shoot magic beams. So good!*"; break;
        case 169 : texte = "Do you want me to upgrade your gloves?**              YES ?            no  "; break;
        case 170 : texte = "Do you want me to upgrade your gloves?**              yes              NO ?"; break;
        case 171 : texte = "The power of your gloves now helps you lift heavy rocks!"; break;
        case 172 : texte = "Do you want me to upgrade your bow?**              YES ?            no  "; break;
        case 173 : texte = "Do you want me to upgrade your bow?**              yes              NO ?"; break;
        case 174 : texte = "Your bow now shoots silver arrows of cataclysmic power!"; break;
        case 175 : texte = "Come back when you have an empty bottle, and I will sell you a blue potion which restores your health and your magic meter."; break;
        case 176 : texte = "A blue potion for 160 rupees. Are you interested?*              YES ?            no  "; 
            if (gpJeu->getJoueur()->hasBouteille(0)!=1
            && gpJeu->getJoueur()->hasBouteille(1)!=1
            && gpJeu->getJoueur()->hasBouteille(2)!=1) {id=175; chercheText();break;}
            if (gpJeu->getJoueur()->getRubis() + gpJeu->getJoueur()->getBoostRubis()<160) idsuiv=29;
            else idsuiv=108; break;
        case 177 : texte = "A blue potion for 160 rupees. Are you interested?*              yes              NO ?"; break;
        case 178 : texte = "Our village has been built here with the purpose of preventing careless adventurers from reaching the temple. I won't allow just anyone to pass."; break;
        case 179 : texte = "Mmmm... You already found 4 crystals?*I have got to admit, you impress me...";
            buffer = "All right, you are allowed to cross over the graveyard, at your own risk."; break;
        case 181 : texte = "Talk to the guard at the north of the village. He will let you pass."; break;
        case 182 : texte = "I see you're still alive...*Surprising."; break;
        case 183 : texte = "The dark temple is the starting point of the way leading to a legendary object. This village only exists to dissuade adventurers from approaching."; break;
        case 184 : texte = "So you found the Sword?*But... don't believe it is yours.";
            buffer = "The Master Sword is among the most treasured heritage of Hyrule along with the Triforce. You will have to bring it back to its sanctuary once your mission is completed."; break;
        case 186 : texte = "All the inhabitants of this village are spirits who answered to Princess Zelda's call."; break;
        case 187 : texte = "When you returned peace to Hyrule and reunified the Light World with the Golden Land, Zelda secretly asked volunteer spirits to create this village to block";
            buffer = "the access to a very high valued object."; break;
        
        case 189 : texte = "Welcome Link!*Let me explain to you what I am doing here."; idsuiv=190; break;
        case 190 : texte = "As you certainly have noticed, monsters appeared in Hyrule. It's the first time I can see monsters since your victory on Ganon."; idsuiv=191; break;
        case 191 : texte = "I have decided to list all of them."; idsuiv=192; break;
        case 192 : texte = "Unfortunately, I'm too fearful to meet them, so I need your help."; idsuiv=193; break;
        case 193 : texte = "You have to fight each kind of monster and come to give me your report."; idsuiv=194; break;
        case 194 : texte = "Each time you have defeated 7 new kinds of enemies, I will give you a piece of heart."; idsuiv=195; break;
        case 195 : texte = "If you want to see what enemies you've already defeated, press the L and B buttons."; break;
        
        
        case 196 : 
            if (gpJeu->getJoueur()->getCoeur(43)) {
                if (gpJeu->getJoueur()->nbEnnemis() >= 46) {id=205; chercheText();return;}
                else {id=204; chercheText();return;}
            }
            for (int i = 38; i < 44; i++) 
                if (!gpJeu->getJoueur()->getCoeur(i) 
                && (gpJeu->getJoueur()->nbEnnemis()/7)+37>=i) {
                    id=203; chercheText();return;
                }
            tmp = 7-(gpJeu->getJoueur()->nbEnnemis()%7);
            os << tmp;
            if (tmp>1) texte = "You need "+os.str()+" more different enemies before your next reward.";
            else texte = "You only need one more enemy before your next reward!";
            break;
        case 203 : texte = "Take this gift as a thank you for your contribution to my project:"; break;
        case 204 : texte = "I'm sorry, but I don't have anything for you anymore."; break;
        case 205 : texte = "I can't take it in, you succeeded in defeating all kinds of enemies!*Congratulations, Link!"; break;
        
        case 206 : texte = "                    Level 1:**                  Forest Temple"; break;
        case 207 : texte = "                    Level 2:**                  Water Temple"; break;
        case 208 : texte = "                    Level 3:**                 Mountain Temple"; break;
        case 209 : texte = "                    Level 4:**                  Desert Temple"; break;
        case 210 : texte = "                    Level 5:**                   Dark Temple"; break;
        case 211 : texte = "                    Level 6:**                   Fire Temple"; break;
        case 212 : texte = "                    Level 7:**                   Ice Temple"; break;
        case 213 : texte = "                    Level ?:**                  Hidden Temple"; break;
        case 214 : texte = "                  Last Level:**                 Hyrule Castle"; break;
        
        case 215 :texte = "                 Already tired?                  ?                 CONTINUE                 ?                   Save and quit                               Quit without saving            "; break;
        case 216 :texte = "                 Already tired?                                    Continue                     ?               SAVE AND QUIT              ?                Quit without saving            "; break;
        case 217 :texte = "                 Already tired?                                    Continue                                     Save and quit                  ?            QUIT WITHOUT SAVING           ?"; break;
        
        case 223 : texte = "At the exact same time Link touched the Triforce, the monsters disappeared,and peace returned.**Princess Zelda then made a great decision: she touched the Triforce and made her wish.**The Triforce had always been the origin of troubles in the Kingdom. Sooner or later, another evil creature would find it.**When Princess Zelda touched the relic, it disappeared from Hyrule... forever.";
            buffer = "From this day, the people of Hyrule have lived in peace, without fear of new disasters.**Thus the Legend of Zelda, of the Triforce, and of the Golden Land, is achieved.**The Master Sword is said to rest safely somewhere and to be the last heirloom of Link's quest."; break;
        case 224 : texte = "Congratulations for finding me, Link. As a reward, I give you Farore's Pendant. It increases your defense by one point."; break;
        case 225 : texte = "Do you want to save the game?**                    ? YES ?*                      no  "; break;
        case 226 : texte = "Do you want to save the game?**                      yes  *                    ? NO ?"; break;
        case 227 : texte = "Game saved."; break;
        
        case 228 : texte = "The Kingdom of Hyrule has been in peace since Link, the last knight of Hyrule, had defeated the malicious Ganon and reclaimed the precious Triforce from him."; idsuiv = 229; break;
        case 229 : texte = "Nobody knows what Link's wish to the Triforce was, but it had the effect of reunifying the Light and Dark World, and resurrecting the 7 wise men's descendants."; idsuiv = 230; break;
        case 230 : texte = "Link then handed the Triforce and the Master Sword over to Princess Zelda, and people started believing that peace would last.*But the people were wrong..."; idsuiv=231; break;
        case 231 : texte = "Unfortunately, Link's wish also had negative effects. Ganon and his henchmen were resurrected and got ready to attack."; idsuiv=232; break;
        case 232 : texte = "Somewhere in Hyrule Forest, Link is sleeping without suspecting that Ganon has already moved into the attack, until a"; 
            buffer="well-known voice talked to him in his sleep..."; break;
        case 233 : texte = "Help me!*Help me!*It's me! Zelda!*I'm talking to you via telepathy."; idsuiv = 234; break;
        case 234 : texte = "I am a prisoner in the dungeon of the castle!*I need your help!*Ganon is back, and he surely has already found the Triforce..."; idsuiv=235; break;
        case 235 : texte = "Come, quickly, to the castle, Link! You are my only hope..."; break;
        case 236 : texte = "HELP: Press L and X to view the help menu."; break;
    }
	break; // default language : English
}
    
    
}

void Texte::affiche(SDL_Surface* gpScreen, std::string s, int a, int b) {
    for (int i = 0; i < (int)s.length(); i++) {
        afficheLettre(gpScreen, s.at(i),a,b);
        a+=6;
    }
}

void Texte::draw(SDL_Surface* gpScreen) {
    
    if (cadre) drawCadre(gpScreen);
	
	std::string comptexte;
	
switch(getLanguage()) {

case 2: // French
	comptexte = "Tu as trouvé un quart de coeur !!!";
break;

case 3: // German
	comptexte = "Du hast ein Herzfragment gefunden!!!";
break;

case 4: // Italian
	comptexte = "Hai trovato una parte di cuore !!!";
break;

case 5: // Spanish
	comptexte = "Has encontrado un cuarto de corazón !!!";
break;

default:
	comptexte = "You found a piece of heart!!!";
break;	
}    
//    if (id==3 && texte == "You find a piece of heart!!!") {
    if (id==3 && texte == comptexte) {
        SDL_Rect src; SDL_Rect dst;
        src.x=16*(gpJeu->getJoueur()->nbQuarts()%4);
        if(src.x==0)src.x=16*4;
        src.y=0; src.w=16; src.h=16; dst.x=160-8; dst.y=120-8+16*5;
        SDL_BlitSurface(imageCoeur, &src, gpScreen, &dst);
    }
    
    int a = x+8; int b = y+8;
    for (int i = 0; i < av; i++) {
        afficheLettre(gpScreen, texte.at(i),a,b);
        a+=6;
        if (a > x+w-16) {a=x+8; b+=16;}
    }
    
    if(SDL_GetTicks() > lastAnimTime + vitesse && def && av < (int)texte.length()) {
        lastAnimTime = SDL_GetTicks();
        do av++;
        while (av < (int)texte.length() && texte.at(av-1) == ' ');
        if (texte.at(av-1) != ' ') gpJeu->getAudio()->playSound(0,1);
    }
}

bool Texte::isFinished() {return (av==(int)texte.length());}

int Texte::getId() {return id;}

void Texte::changeId(int i) {
    id=i; idsuiv=0; buffer="";
    chercheText();
    decoupeText();
    if (av>(int)texte.length()) av=(int)texte.length();
}

void Texte::drawCadre(SDL_Surface* gpScreen) {
    SDL_Rect src;
    SDL_Rect dst;
    
    src.w=8; src.h=8; src.x = 103; src.y = 100; dst.x = x; dst.y = y;
    SDL_BlitSurface(imageFont, &src, gpScreen, &dst);
    
    src.x = 112;
    for (int i = 8; i < w-8; i+=16) {
        dst.x = x+i; src.w=16;
        while (dst.x + src.w > x+w && src.w>0) src.w--;
        if (src.w>0) SDL_BlitSurface(imageFont, &src, gpScreen, &dst);
    }
    
    src.w=8; src.x = 129; dst.x = x+w-8;
    SDL_BlitSurface(imageFont, &src, gpScreen, &dst);
    
    src.y = 109; src.w=8;
    for (int j = 8; j < h-8; j+=16) {
        dst.y = y + j;
        src.x = 103; dst.x = x; src.h=16;
        while (dst.y + src.h > y+h && src.h>0) src.h--;
        if (src.h>0) SDL_BlitSurface(imageFont, &src, gpScreen, &dst);
    
        src.x = 129; dst.x = x+w-8;
        if (src.h>0)SDL_BlitSurface(imageFont, &src, gpScreen, &dst);
    }
    
    src.h=8; src.x = 103; src.y = 126; dst.x = x; dst.y = y+h-8;
    SDL_BlitSurface(imageFont, &src, gpScreen, &dst);
    
    src.x = 112;
    for (int i = 8; i < w-8; i+=16) {
        dst.x = x+i; src.w=16;
        while (dst.x + src.w > x+w && src.w>0) src.w--;
        if (src.w>0) SDL_BlitSurface(imageFont, &src, gpScreen, &dst);
    }
    
    src.w=8; src.x = 129; dst.x = x+w-8;
    SDL_BlitSurface(imageFont, &src, gpScreen, &dst);
}

void Texte::setTexte(int idTxt, int vx, int vy, int vw, int vh, bool cadr, bool defil, int vit) {
    if (idTxt == 0) return;
    id = idTxt; 
    idsuiv = 0;
    buffer = "";
    chercheText();
    
    x = vx; y = vy; w = vw; h = vh;
    decoupeText();
        
    def=defil;
    if (def) av = 0;
    else av = texte.length();
    
    cadre = cadr;
    
    vitesse = vit;
}

void Texte::decoupeText() {
    //compte le nombre de caractères possibles et largeur et en hauteur
    int nbcol = (w-16)/6 -1;
    int nblig = (h-16)/16;
    int tailleMax = nbcol * nblig;
    int taille;
    
    //parcours du texte à afficher; à chaque début de mot, 
    //vérifie que le mot peut tenir sur la ligne
    for (int i = 0; i < (int)texte.length(); i++) {
        
        //supprime les espaces isolés en début de ligne
        if (texte.at(i)==' ' && texte.at(i+1)!=' ' && i%nbcol == 0) texte.erase(i,1);
        //recherche du début du prochain mot
        while(texte.at(i)==' ' && i < (int)texte.length()-1) i++;
        
        //saute une ligne si trouve une étoile
        if (texte.at(i)=='*') {
            texte.erase(i,1);//replace(i, 1, " ");
            int nb = (nbcol)-(i%(nbcol));
            for (int j = 0; j < nb; j++) texte.insert(i," ");
            continue;
        }
        
        //si le mot dépasse
        taille = tailleMot(i);
        if ((i%nbcol)+taille>nbcol) {
            if  (i < tailleMax) {
                //si le mot ne tient pas sur une ligne, on le coupe avec des tirets
                if (taille>nbcol) {
                    texte.insert(((i/nbcol)+1)*nbcol-1,"--");
                    i = 1+((i/nbcol)+1)*nbcol;
                }
                //sinon, on ajoute des espaces pour faire commencer le mot à la ligne
                else while((i%nbcol) != 0) {texte.insert(i," "); i++;}
            }
        }
        
    }
    
    // si le texte est trop grand, on le coupe en deux
    if ((int)texte.length() > tailleMax) {
        buffer = texte.substr(tailleMax);
        texte = texte.substr(0, tailleMax);
    }
}

int Texte::tailleMot(int deb) {
    int i = deb;
    int total = 0;
    while (texte.at(i)!=' ') {total++; i++; if (i >= (int)texte.length()) return total;}
    return total;
}

void Texte::afficheLettre(SDL_Surface* gpScreen, char c, int vx, int vy) {
    SDL_Rect src;
    SDL_Rect dst;
    
    int val = (int)c;
    
    dst.x=vx; dst.y=vy;
    src.h=16;src.w=8;
    
    if(val==32) return;
    
    // /
    if(val==47) {src.x=52;src.y=151;}
    
    // @ hylien
    if(val==64) {src.x=4;src.y=151;}
            
    // + hylien
    if(val==43) {src.x=20;src.y=151;}
            
    // = hylien
    if(val==61) {src.x=36;src.y=151;}
            
    //minuscules a-z
    if(val>=97 && val<=122) {src.x=4+16*((val-97)%10); src.y=52+16*((val-97)/10);}
            
    //majuscules A-Z
    if(val>=65 && val<=90) {src.x=6+16*((val-65)%10); src.y=2+16*((val-65)/10);}   
    // ç
    if(val==-25) {src.x=148;src.y=34;}
    // é
    if(val==-23) {src.x=100;src.y=84;}
    // ê
    if(val==-22) {src.x=116;src.y=84;}
    // è
    if(val==-24) {src.x=132;src.y=84;}
    // ë
    if(val==-21) {src.x=132;src.y=151;}
    // à
    if(val==-32) {src.x=148;src.y=84;}
    // â
    if(val==-30) {src.x=148;src.y=103;}
    // ä
    if(val==-28) {src.x=148;src.y=135;}
    // î
    if(val==-18) {src.x=84;src.y=119;}
    // ï
    if(val==-17) {src.x=116;src.y=151;}
    // û
    if(val==-5) {src.x=84;src.y=103;}
    // ù
    if(val==-7) {src.x=148;src.y=151;}
    // ü
    if(val==-4) {src.x=116;src.y=135;}
    // ö
    if(val==-10) {src.x=132;src.y=135;}
    // ô
    if(val==-12) {src.x=148;src.y=119;}
            
    //ponctuation
    // -
    if(val==45) {src.x=102;src.y=34;}
    // .
    if(val==46) {src.x=118;src.y=34;}
    // ,
    if(val==44) {src.x=134;src.y=34;}
    // !
    if(val==33) {src.x=3;src.y=135;}
    // ?
    if(val==63) {src.x=19;src.y=135;}
    // (
    if(val==40) {src.x=35;src.y=135;}
    // )
    if(val==41) {src.x=51;src.y=135;}            
    // ' ( avec @ )
    if(val==39) {src.x=67;src.y=135;}
    // :
    if(val==58) {src.x=83;src.y=135;}
    // ... ( avec % )
    if(val==37) {src.x=101;src.y=135;}
    // >
    if(val==62) {src.x=100;src.y=151;}
    // <
    if(val==60) {src.x=84;src.y=151;}
            
    //chiffres            
    if(val>=48 && val<=57) {src.x=3+16*((val-48)%5); src.y=103+16*((val-48)/5);}
    
    SDL_BlitSurface(imageFont, &src, gpScreen, &dst);
}

bool Texte::hasNext() {
    return (buffer != "" || idsuiv > 0);
}

bool Texte::suite() {
    if (av < (int)texte.length()) {
        av = texte.length();
        return true;
    }
    if (!hasNext()) {
        gpJeu->getAudio()->playSound(18);
        return gpJeu->finTexte(id);
    }
    if (buffer != "") {
        texte = buffer;
        buffer = "";
    }
    else {
        id = idsuiv;
        idsuiv = 0;
        chercheText();
    }
    decoupeText();
    if (def) av = 0;
    else av = texte.length();
    gpJeu->getAudio()->playSound(17);
    return true;
}

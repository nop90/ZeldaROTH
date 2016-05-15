/*

    Zelda Return of the Hylian

    Copyright (C) 2005-2008  Vincent Jouillat

    Please send bugreports with examples or suggestions to www.zeldaroth.fr

*/

#ifndef __LISTABLE_H__
#define __LISTABLE_H__

class Listable {
    public :
        Listable();
        ~Listable();
        int size();
        void ajout(Listable* l); // ajout d'un élément seul
        void enleve(Listable* l); //enlève et libère l'élément
        bool operator==(const Listable* l); // compare les id
        int getListId();
    protected :
        Listable* suivant;
    private :
        int id;             // identifiant de l'objet, toujours > au précédent
};

#endif  // Listable.h

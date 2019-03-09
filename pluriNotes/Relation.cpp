#include "Relation.h"
#include <iostream>

int Relation::nbRelation=0;

/**
 * @brief Relation::display Pour le developpement
 */
void Relation::display()const{
    std::cout<<"Relation "<<getId()<<", titre "<<getTitre()<<std::endl;
}

/**
 * @brief Relation::addCouple Ajoute un couple a la relation
 * @param n
 */
void Relation::addCouple(Couple &n){
    coupleList.push_back(n);
}

/**
 * @brief Relation::deleteCouple gère la suppression du ième couple dans la liste coupleList
 * @param indiceCouple
 */
void Relation::deleteCouple(unsigned int indiceCouple){
    if(indiceCouple < coupleList.size()) {
        coupleList.erase(coupleList.begin()+indiceCouple);
        std::cout<<"[Relation.cpp]Couple supprime ("<<indiceCouple<<")"<<endl;
    } else {
        std::cout<<"[Relation.cpp]Suppression impossible: indice du couple "<<indiceCouple<<"(indice supérieur à la taille de la liste de couple)"<<endl;
    }
}

/**
 * @brief Relation::removeCouple supprime tous les couples de la relation qui concerne la note d'id idNote
 * @param idNote id de la note pour laquelle nous voulons supprimer les couples
 */
void Relation::removeCouple(int idNote){
    for(unsigned int i = 0 ; i < coupleList.size() ; i++){
        if(coupleList.at(i).isInCouple(idNote)){
            deleteCouple(i);
            i--;
        }
    }
}

/**
 * @brief Relation::removeCouple supprime le couple (idNoteA,idNoteB) (l'ordre est important)
 * @param idNoteA
 * @param idNoteB
 */
void Relation::removeCoupleWithIds(int idNoteA, int idNoteB){
    for(unsigned int i = 0 ; i < coupleList.size() ; i++){
        if(coupleList.at(i).getNoteA() == idNoteA && coupleList.at(i).getNoteB() == idNoteB){
            deleteCouple(i);
            i--;
        }
    }
}

#include "RelationListManager.h"
#include <iostream>

/**
 * @brief RelationListManager::RelationListManager
 * @param fen
 */
RelationListManager::RelationListManager(FenPrincipale* fen){
    observerRelations = new RelationListObserver(&relationList, fen);
}

/**
 * @brief RelationListManager::notify Permet le renvoie des evenements notifiés à l'observeur des relations
 */
void RelationListManager::notify(){
    observerRelations->notify();
}

/**
 * @brief RelationListManager::addRelation Ajout une relation au manager
 * @param r
 */
void RelationListManager::addRelation(Relation &r){
    relationList.push_back(r);
    notify();
}

void RelationListManager::deleteRelation(int idRelation){
    unsigned int i = 0;
    bool isFind = false;

    while(i < relationList.size() && !isFind){
        if(relationList[i].getId() == idRelation){
            isFind = true;
        } else{
            i++;
        }
    }

    if(i < relationList.size()) {
        std::cout<<"Suppression de la relation "<<idRelation<<endl;
        relationList.erase(relationList.begin()+i);
    } else {
        std::cout<<"Suppression impossible: id "<<idRelation<<" ne correspond à aucune relation"<<endl;
    }
    notify();
}

/**
 * @brief RelationListManager::getRelation
 * @param id Id de la relation recherchée
 * @return un pointeur de Relation si l'id est trouvé null sinon
 */
Relation* RelationListManager::getRelation(int id){
    unsigned int i = 0;
    bool isFind = false;
    Relation *r;

    while(i < relationList.size() && !isFind){
        if(relationList[i].getId() == id){
            r = &(relationList[i]);
            isFind = true;
        }
        i++;
    }
    return r;
}

/**
 * @brief RelationListManager::display Pour le developpement
 */
void RelationListManager::display(){
    for(unsigned int i = 0 ; i < relationList.size() ; i++){
        relationList[i].display();
    }
}

/**
 * @brief RelationListManager::listeDescendantsParNote permet de récupérer un tableau de vecteur.
 * A l'emplacement tab[i], on aura un vecteur contenant la liste des descendants directs de la Note ayant l'id i
 * @return
 */
vector<int>* RelationListManager::listeDescendantsParNote(){
    // todo: optimisation (récupérer dernier id existant, ou créer vecteur de vecteur)
    vector<int> *listeDescendants = new vector<int>[100];
    for(unsigned int i = 0 ; i < relationList.size() ; i++){
        Relation relation = relationList.at(i);
        for(unsigned int j = 0 ; j < relation.getcoupleList().size() ; j++){
            Couple c = relation.getcoupleList().at(j);
            listeDescendants[c.getNoteB()].push_back(c.getNoteA());
        }
    }
    return listeDescendants;
}

/**
 * @brief RelationListManager::listeAscendantsParNote permet de récupérer un tableau de vecteur.
 * A l'emplacement tab[i], on aura un vecteur contenant la liste des ascendants directs de la Note ayant l'id i
 * @return
 */
vector<int>* RelationListManager::listeAscendantsParNote(){
    // todo: optimisation (récupérer dernier id existant, ou créer vecteur de vecteur)
    vector<int> *listeAscendants = new vector<int>[100];
    for(unsigned int i = 0 ; i < relationList.size() ; i++){
        Relation relation = relationList.at(i);
        for(unsigned int j = 0 ; j < relation.getcoupleList().size() ; j++){
            Couple c = relation.getcoupleList().at(j);
            listeAscendants[c.getNoteA()].push_back(c.getNoteB());
        }
    }
    return listeAscendants;
}

/**
 * @brief RelationListManager::getIdRelationStartedByIdNote retourne l'id de la relation qui concernant la note d'id passé en paramètre
 * @param id : id de la note
 * @return id de la note, -1 si n'existe pas de relation pour cette note
 */
int RelationListManager::getIdRelationStartedByIdNote(int id){
    int i = 0;
    bool isFind = false;
    // on regarde si une relation reference est déja existante pour une note

    // pour chaque relation
    while(i < Relation::nbRelation && !isFind){ // todo changer le static en relationList.size() ?
        unsigned int j = 0;
        Relation relation = relationList.at(i);
        std::vector<Couple> couples = relation.getcoupleList();

        // pour chaque couple
        while(j < couples.size() && !isFind){
            // si relation est une référence
            if(relation.getIsReference()==true){
                // s'il existe un couple avec l'ID a = à l'id du label consulté
                if (couples.at(j).getNoteA() == id){
                    isFind = true;
                }
            }else{
                j++;
            }
        }
        i++;
    }
    return isFind ? i : -1;
}

/**
 * @brief RelationListManager::removeCouple appelle pour chaque relation la fonction permettant de supprimer les couples id
 * @param id de la note pour laquelle on veut supprimer les couples associées
 */
void RelationListManager::removeCouple(int id){
    for(unsigned int i = 0 ; i < relationList.size() ; i++){
        relationList.at(i).removeCouple(id);
    }
    notify();
}

/**
 * @brief RelationListManager::isReferenced retourne un boolean permettant de savoir si la note d'id idNote est référencée
 * @param idNote
 * @return
 */
bool RelationListManager::isReferenced(int idNote){
    bool isReferenced = false;
    unsigned int i = 0;
    // on parcourt les relations existantes
    while(i < relationList.size() && !isReferenced){
        // si est une référence
        if(relationList.at(i).getIsReference()){
            std::vector<Couple> couples = relationList.at(i).getcoupleList();
            unsigned int j = 0;
            // on parcourt les couples
            while(j < couples.size() && !isReferenced){
                if(couples.at(j).getNoteB() == idNote){
                    isReferenced = true;
                }
                j++;
            }
        }
        i++;
    }
    return isReferenced;
}

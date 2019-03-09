#ifndef RELATIONLISTMANAGER_H
#define RELATIONLISTMANAGER_H

#include "RelationListObserver.h"
#include "Relation.h"

using namespace std;
class FenPrincipale;

/**
 * @brief The RelationListManager class gère la liste des relations présentent dans l'application. Elle fait le lien entre la vue (FenPrincipale) et l'Observeur (RelationListObserveur).
 */
class RelationListManager{
private:
    vector<Relation> relationList;
    RelationListObserver* observerRelations;

public:
    RelationListManager(FenPrincipale*);
    void addRelation(Relation&);
    void deleteRelation(int idRelation);
    vector<Relation> *getRelationList(){return &relationList;}
    vector<int>* listeDescendantsParNote();
    vector<int>* listeAscendantsParNote();
    Relation* getRelation(int);
    void display();
    void notify();
    int getIdRelationStartedByIdNote(int);
    void removeCouple(int);
    bool isReferenced(int);
};

#endif // RELATIONLISTMANAGER_H

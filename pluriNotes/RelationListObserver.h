#ifndef RELATIONLISTOBSERVER_H
#define RELATIONLISTOBSERVER_H

#include <Relation.h>
#include "ui_FenPrincipale.h"

class FenPrincipale;


/**
 * @brief The RelationListObserver class impl�mente le design pattern Obserser. Elle permet de g�rer les mise � jour des liste de relations dans l'UI.
 */
class RelationListObserver{
private:
    std::vector<Relation> *relationList;
    FenPrincipale* window;

public:
    RelationListObserver(std::vector<Relation> *relations, FenPrincipale* w):relationList(relations),window(w){}
    void notify();
};

#endif // RELATIONLISTOBSERVER_H

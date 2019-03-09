#include "RelationListObserver.h"
#include <iostream>
#include "ui_FenPrincipale.h"
#include "FenPrincipale.h"
#include <Relation.h>


/**
 * @brief RelationListObserver::notify Recoit et gere les changements envoyés par le relationManager : permet l'actualisation automatique des listes de relations dans l'UI
 */
void RelationListObserver::notify(){
    Ui::FenPrincipale* fen = window->getWindow();

    fen->relationSimpleList->clear();
    fen->referenceList->clear();

    unsigned int i;
    for(i = 0 ; i < relationList->size() ; i++){
        Relation relation = (*relationList).at(i);

        QListWidgetItem* newItem = new QListWidgetItem;
        newItem->setText(QString::fromStdString(relation.getTitre()));
        newItem->setData(Qt::UserRole, relation.getId());

        // ajout dans la liste des relations (second view)
        if(relation.getIsReference() == false) {
            fen->relationSimpleList->insertItem(i, newItem);
        } else {
            fen->referenceList->insertItem(i, newItem);
        }
    }
}

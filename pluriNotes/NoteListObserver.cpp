#include "NoteListObserver.h"
#include <iostream>
#include <QListWidget>
#include <QString>
#include <QListWidgetItem>
#include <QListData>
#include "Task.h"
#include "TimeLibrary.h"
#include "FenPrincipale.h"

/**
 * @brief NoteListObserver::clearTaskList supprime les éléments de TaskList (QTreeWidgetItem)
 * et ajoute les éléments directeurs
 */
void NoteListObserver::clearTaskList(){
    window->getWindow()->taskList->clear();

    if(parameters->getDisplayTaskByPriority()){
        (new QTreeWidgetItem(window->getWindow()->taskList))->setText(0,"Haute");
        (new QTreeWidgetItem(window->getWindow()->taskList))->setText(0,"Moyenne");
        (new QTreeWidgetItem(window->getWindow()->taskList))->setText(0,"Basse");
        (new QTreeWidgetItem(window->getWindow()->taskList))->setText(0,"Indefinie");
    }
}

/**
 * @brief NoteListObserver::notify Recoit et gere les changements envoyés par le noteManager : permet l'actualisation automatique des listes de notes dans l'UI
 */
void NoteListObserver::notify(){
    window->getWindow()->activeNoteList->clear();
    window->getWindow()->archivedNoteList->clear();
    window->getWindow()->inTrashNoteList->clear();
    window->getWindow()->ComboBoxNoteACouple->clear();
    window->getWindow()->ComboBoxNoteBCouple->clear();

    clearTaskList();

    unsigned int i;
    for(i = 0 ; i < noteList->size() ; i++){
        Note note = (*noteList)[i];
        const VersionManager& versionManager = note.getVersionManager();
        // Si la note est une tâche et qu'elle est active
        if(!versionManager.isEmpty() && versionManager.getTypeNote() == Enumeration::TASK && note.getStatus() == Enumeration::ACTIVE){
            Task* task = static_cast<Task*>(versionManager.getCurrentVersion().getaNote());

            // Get the good priority parent
            QTreeWidgetItem* parent;
            if(parameters->getDisplayTaskByPriority()){
                // By priority
                QString priorite = EnumerationLibrary::priorityToQString(task->getPriority());
                parent = window->getWindow()->taskList->findItems(priorite,Qt::MatchContains | Qt::MatchRecursive).at(0);
            } else {
                // By dates
                QString deadLine = QString::fromStdString(TimeLibrary::getString(task->getDeadline(), false));
                if(window->getWindow()->taskList->findItems(deadLine,Qt::MatchContains | Qt::MatchRecursive).size() > 0){
                    // Getting category with the date
                    parent = window->getWindow()->taskList->findItems(deadLine,Qt::MatchContains | Qt::MatchRecursive).at(0);
                } else {
                    // Create category with the date
                    parent = new QTreeWidgetItem(window->getWindow()->taskList);
                    parent->setText(0,deadLine);
                }
            }

            // Create new item and add as child item
            QTreeWidgetItem* item = new QTreeWidgetItem(parent);
            item->setText(0, QString::fromStdString(task->getAction()));
            item->setText(1, QString::fromStdString(TimeLibrary::getString(task->getDeadline(), false)));
            //ajout du +1 pour gerer les items "Haute", "Moyenne", ... au double clic sur les item du QTreeWidget
            item->setData(0, Qt::UserRole, note.getId()+1);
            item->setData(1, Qt::UserRole, note.getId()+1);
        }

        QListWidgetItem* newItem = new QListWidgetItem;
        newItem->setData(Qt::UserRole, note.getId());
        newItem->setText(QString::fromStdString(note.getTitle()));

        // ajout de la note selon son status
        if(note.getStatus() == Enumeration::ACTIVE){
            window->getWindow()->activeNoteList->insertItem(i+1, newItem);
            window->getWindow()->ComboBoxNoteACouple->addItem(newItem->text(), QVariant(newItem->data(Qt::UserRole)));
            window->getWindow()->ComboBoxNoteBCouple->addItem(newItem->text(), QVariant(newItem->data(Qt::UserRole)));
        } else if(note.getStatus() == Enumeration::ARCHIVED){
            window->getWindow()->archivedNoteList->insertItem(i+1, newItem);
        } else if(note.getStatus() == Enumeration::IN_TRASH){
            window->getWindow()->inTrashNoteList->insertItem(i+1, newItem);
        }
    }
}

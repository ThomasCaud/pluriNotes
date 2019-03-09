/**
  * \file application.h
  * \brief Entete pour la class Application
  * \authors QuentinS ThomasC
  * \version 0.1
*/

#ifndef APPLICATION_H
#define APPLICATION_H

#include "Parameter.h"
#include "NoteListManager.h"
#include "RelationListManager.h"

/**
 * @brief The Application class est une interface gérant les différents Manager du logiciel
 */
class Application{
private:
    Parameter parameters;
    NoteListManager* noteListManager;
    RelationListManager* relationListManager;

    ~Application();

public:
    Application(NoteListManager*, RelationListManager*);
    Parameter* getParameters(){return &parameters;}
    NoteListManager* getNoteListManager(){return noteListManager;}
    RelationListManager* getRelationListManager(){return relationListManager;}
    void setNoteListManager(NoteListManager* m){noteListManager=m;}
    Note* getNote(int id){return noteListManager->getNote(id);}
    void setRelationListManager(RelationListManager* rm){relationListManager=rm;}
};

#endif // APPLICATION_H

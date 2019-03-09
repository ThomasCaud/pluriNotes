#ifndef NOTELISTOBSERVER_H
#define NOTELISTOBSERVER_H
#include "Note.h"
#include "ui_FenPrincipale.h"
#include "Parameter.h"

class FenPrincipale;

/**
 * @brief The NoteListObserver class implémente le design pattern Obserser. Elle permet de gérer les mise à jour des liste de notes dans l'UI.
 */
class NoteListObserver{
private:
    std::vector<Note> *noteList;
    FenPrincipale* window;
    Parameter* parameters;
    void clearTaskList();

public:
    NoteListObserver(std::vector<Note> *notes, FenPrincipale* w):noteList(notes),window(w){}
    void notify();
    void setParameter(Parameter* p){parameters=p;}
};

#endif // NOTELISTOBSERVER_H

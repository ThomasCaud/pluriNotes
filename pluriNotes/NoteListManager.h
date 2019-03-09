#ifndef NOTELISTMANAGER_H
#define NOTELISTMANAGER_H

#include "NoteListObserver.h"
#include "Note.h"
#include "Parameter.h"

using namespace std;
class FenPrincipale;

/**
 * @brief The NoteListManager class gère la liste des notes présentent dans l'application. Elle fait le lien entre la vue (FenPrincipale) et l'Observeur (NoteListObserveur).
 */
class NoteListManager{
private:
    vector<Note> noteList;
    NoteListObserver* observerNotes;
    void deleteNoteIndice(unsigned int);

public:
    NoteListManager(FenPrincipale*);
    void addNote(Note&);
    void deleteNote(int);
    vector<Note> *getNoteList(){return &noteList;}
    void setStatus(Note*, Enumeration::noteStatus);
    Note* getNote(int);
    void display();
    void notify();
    void setParameter(Parameter* p){observerNotes->setParameter(p);}
    void clear();
    bool existTrashedNote();
    void throwInTrash(int);
    void archivedNote(int);
    void activeNote(int idNote);
};

#endif // NOTELISTMANAGER_H

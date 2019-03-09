#include "NoteListManager.h"

/**
 * @brief NoteListManager::NoteListManager
 * @param fen
 */
NoteListManager::NoteListManager(FenPrincipale* fen){
    observerNotes = new NoteListObserver(&noteList, fen);
}

/**
 * @brief NoteListManager::notify
 */
void NoteListManager::notify(){
    observerNotes->notify();
}

/**
 * @brief NoteListManager::addNote
 * @param n
 */
void NoteListManager::addNote(Note &n){
    noteList.push_back(n);
    notify();
}

/**
 * @brief NoteListManager::getNote
 * @param id
 * @return
 */
Note* NoteListManager::getNote(int id){
    unsigned int i = 0;
    bool isFind = false;
    Note *n;

    while(i < noteList.size() && !isFind){
        if(noteList[i].getId() == id){
            n = &(noteList[i]);
            isFind = true;
        }
        i++;
    }

    return n;
}

/**
 * @brief NoteListManager::setStatus
 * @param n
 * @param status
 */
void NoteListManager::setStatus(Note* n, Enumeration::noteStatus status){
    n->setStatus(status);
    notify();
}

/**
 * @brief NoteListManager::display Pour le developpement
 */
void NoteListManager::display(){
    for(unsigned int i = 0 ; i < noteList.size() ; i++){
        noteList[i].display();
    }
}

/**
 * @brief NoteListManager::clear Vide la list de note et remet le nombre de note a 0
 */
void NoteListManager::clear(){
    noteList.clear();
    Note::nbNote = 0;
}

/**
 * @brief NoteListManager::deleteNote supprime la (indiceDansList)ieme Note dans la liste
 * @param indiceDansList indice dans la liste de note de la note à supprimer
 */
void NoteListManager::deleteNoteIndice(unsigned int indiceDansList){
    if(indiceDansList < noteList.size()) {
        noteList.erase(noteList.begin()+indiceDansList);
    } else {
        std::cout<<"[NoteListManager.cpp]Suppression impossible: id "<<indiceDansList<<" ne correspond à aucune note"<<endl;
    }
}

/**
 * @brief NoteListManager::deleteNote Gere la suppression d'une note dans le vecteur des Notes du manager
 * @param idNote
 */
void NoteListManager::deleteNote(int idNote){
    unsigned int i = 0;
    bool isFind = false;

    while(i < noteList.size() && !isFind){
        if(noteList[i].getId() == idNote){
            isFind = true;
        } else{
            i++;
        }
    }

    deleteNoteIndice(i);
    notify();
}

/**
 * @brief NoteListManager::existTrashedNote
 * @return true si il y a des notes dans la corbeille, false sinon
 */
bool NoteListManager::existTrashedNote(){
    bool isTrash = false;
    unsigned int i = 0;

    while(i < noteList.size() && !isTrash){
        if(noteList[i].getStatus() == Enumeration::IN_TRASH){
            isTrash = true;
        } else{
            i++;
        }
    }
    return isTrash;
}

/**
 * @brief NoteListManager::throwInTrash permet la mise en corbeille de la note d'id idNote
 * @param idNote
 */
void NoteListManager::throwInTrash(int idNote){
    getNote(idNote)->throwInTrash();
    notify();
}

/**
 * @brief NoteListManager::archivedNote permet l'archivage de la note d'id idNote
 * @param idNote
 */
void NoteListManager::archivedNote(int idNote){
    getNote(idNote)->archived();
    notify();
}

void NoteListManager::activeNote(int idNote){
    getNote(idNote)->setStatus(Enumeration::ACTIVE);
    notify();
}


#include "Note.h"
#include <QString>
#include <iostream>

int Note::nbNote=0;

/**
 * @brief Note::display Pour le developpement
 */
void Note::display()const{
    std::cout<<"Note "<<getId()<<", titre "<<getTitle()<<" créée le "<<getCreation()<<", modifiée le "<<getLastModification()<<", status: "<<getStatus()<<std::endl;
    versions.display();
}

/**
 * @brief Note::addVersion Ajoute une version a la liste des versions de la note
 * @param a
 */
void Note::addVersion(AbstractNote* a){
    versions.addVersion(a);
}

/**
 * @brief Note::setStatus
 * @param s
 */
void Note::setStatus(Enumeration::noteStatus s){
    status = s;
}

Note::~Note(){
    //Appelle le desqtructeur de VersionManager
    //delete versions;
    //delete this;
}

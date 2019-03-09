#include "VersionManager.h"
#include <iostream>
#include <algorithm>
#include <iterator>

/**
 * @brief VersionManager::addVersion
 * @param a
 */
void VersionManager::addVersion(AbstractNote *a){
    iterator->addVersionIterator(a);
    typeNote = a->getTypeNote();
}

/**
 * @brief VersionManager::display
 */
void VersionManager::display()const{
    iterator->display();
}

/**
 * @brief VersionManager::isEmpty
 * @return
 */
bool VersionManager::isEmpty()const{
    return iterator->size() == 0;
}

/**
 * @brief VersionManager::getCurrentVersion
 * @return
 */
Version VersionManager::getCurrentVersion()const{
    return iterator->current();
}

/**
 * @brief VersionManager::getNameTypeNote
 * @return
 */
QString VersionManager::getNameTypeNote()const{
    return getCurrentVersion().getaNote()->getNameTypeNote();
}

/**
 * @brief VersionManager::Iterator::next
 */
void VersionManager::Iterator::next(){
    if(list.size() == 1){
        indice = 0; // première élément
    } else if(indice < (int)list.size()){
        indice++;
    } else {
        cout<<"erreur pas d'element suivant (indice:" << indice << ", size:" << list.size()<<endl;
        throw "erreur";
    }
}

/**
 * @brief VersionManager::Iterator::previous
 */
void VersionManager::Iterator::previous(){
    if(indice > 0){
        indice--;
    } else {
        cout<<"erreur pas d'element precedent (indice = 0)"<<endl;
        throw "erreur pas d'element precedent (indice = 0)";
    }
}

/**
 * @brief VersionManager::Iterator::current
 * @return
 */
Version VersionManager::Iterator::current()const{
    if(indice >= 0){
        return list.at(indice);
    } else{
        cout<<"Erreur current : indice = "<<indice<<endl;
        throw "Erreur current : indice <0 (pas d'élément défini)";
    }
}

/**
 * @brief VersionManager::Iterator::display
 */
void VersionManager::Iterator::display()const{
    cout<<"indice: "<<indice<<", size list: "<<list.size()<<", capacity: "<<list.capacity()<<endl;
}

/**
 * @brief VersionManager::Iterator::addVersionIterator
 * @param a
 */
void VersionManager::Iterator::addVersionIterator(AbstractNote* a){
    list.push_back(Version(a, list.size()));
    next();
}


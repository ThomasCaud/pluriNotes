#ifndef VERSIONMANAGER_H
#define VERSIONMANAGER_H
#include "Version.h"
#include <vector>
#include <iostream>

using namespace std;

/**
 * @brief The VersionManager class gère la liste de l'ensemble des versions présentent dans l'application. Elle fait le lien entre la vue (FenPrincipale) et la class Note.
 */
class VersionManager{
public:
    /**
     * @brief The Iterator class permet de parcourir les objets de l'instance de VersionManager
     */
    class Iterator{
    private:
        friend class VersionManager;
        vector<Version> list;
        int indice;

    public:
        Iterator():indice(-1){}
        ~Iterator(){}
        void next();
        void previous();
        void goToEnd(){indice = list.size()-1;}
        Version current()const;
        Version getVersion(int i){return list.at(i);}
        void display()const;
        void displayAddrVector(){cout<<"Iterator addr lis: "<<&list<<endl;}
        void addVersionIterator(AbstractNote*);
        size_t size(){return list.size();}
    };

    VersionManager(){iterator = new VersionManager::Iterator();}
    ~VersionManager(){}
    void addVersion(AbstractNote*);
    void display()const;
    Version getCurrentVersion()const;
    Enumeration::typeNote getTypeNote()const{return typeNote;}
    bool isEmpty()const;
    QString getNameTypeNote()const;
    VersionManager::Iterator* getIterator()const{return iterator;}

private:
    Enumeration::typeNote typeNote;
    VersionManager::Iterator *iterator;
};

#endif // VERSIONMANAGER_H

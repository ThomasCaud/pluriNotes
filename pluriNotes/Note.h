#ifndef NOTE_H
#define NOTE_H

#include <ctime>
#include <Enumeration.h>
#include <VersionManager.h>
#include <AbstractNote.h>

/**
 * @brief The Note class est la fonction gérant les "documents". Elle définit l'ensemble des méthodes et attributs communes aux Notes. Elle est l'interface de gestion des relations et des versions.
 */
class Note {
public:
    static int nbNote;
    Note(Enumeration::noteStatus st, std::string tit):id(Note::nbNote),title(tit),status(st),creation(time(0)),lastModification(time(0)){Note::nbNote++;}
    Note(Enumeration::noteStatus st, std::string tit, time_t crea, time_t lastMod):id(Note::nbNote),title(tit),status(st),creation(crea),lastModification(lastMod){Note::nbNote++;}
    Note():id(Note::nbNote),title("notitle"),status(Enumeration::ACTIVE),creation(0),lastModification(0){Note::nbNote++;}
    void addVersion(AbstractNote*);
    void display()const;

    // getters
    int getId()const{return id;}
    std::string getTitle()const{return title;}
    Enumeration::noteStatus getStatus()const{return status;}
    time_t getCreation()const{return creation;}
    time_t getLastModification()const{return lastModification;}
    const VersionManager& getVersionManager()const{return versions;}
    VersionManager::Iterator* getIterator(){return versions.getIterator();}

    // setters
    void setTitle(std::string t){title = t;}
    void setStatus(Enumeration::noteStatus s);
    void setVersionManager(VersionManager& v){versions = v;}
    void setCreation(time_t d){creation=d;}
    void setLastModification(time_t d){lastModification=d;}
    time_t updateLastModification(){lastModification=time(0); return lastModification;}
    QString getNameTypeNote()const{return versions.getNameTypeNote();}
    Version getCurrentVersion()const{return versions.getCurrentVersion();}
    Enumeration::typeNote getTypeNote()const{return versions.getTypeNote();}
    int getNombreDeVersion(){return versions.getIterator()->size();}
    QString getAllQString(){return getCurrentVersion().getaNote()->getAllQString()+QString::fromStdString(title);}
    void throwInTrash(){status=Enumeration::IN_TRASH;}
    void archived(){status=Enumeration::ARCHIVED;}

    ~Note();

private:
    int id;
    std::string title;
    Enumeration::noteStatus status;
    time_t creation;
    time_t lastModification;
    VersionManager versions;
};

#endif // NOTE_H

#ifndef TASK_H
#define TASK_H

#include "AbstractNote.h"
#include <ctime>
#include "Enumeration.h"
#include <string>

using namespace std;

/**
 * @brief The Task class hérite d'Abstract Note. Elle gère les Notes de type Tache
 */
class Task : public AbstractNote
{
public:
    Task(const string& titleANote, const string& actionArticle, Enumeration::priority prio):AbstractNote(titleANote), action(actionArticle), priority(prio), status(Enumeration::TODO){}
    Task(const string& titleANote, const string& actionArticle, Enumeration::priority prio, time_t deadL, Enumeration::taskStatus stat):AbstractNote(titleANote), action(actionArticle), priority(prio), deadline(deadL), status(stat){}

    Enumeration::typeNote getTypeNote()const;
    void display()const;

    string getAction()const{return action;}

    Enumeration::priority getPriority()const{return priority;}
    time_t getDeadline()const{return deadline;}
    Enumeration::taskStatus getStatus()const{return status;}

    void setPriority(Enumeration::priority p){priority = p;}
    void setDeadline(time_t t){deadline = t;}
    void setAction(const std::string & a){action=a;}
    void setStatus(Enumeration::taskStatus s){status = s;}
    QString getNameTypeNote()const{return "Tache";}
    QString getAllQString()const{return QString::fromStdString(getTitle())+QString::fromStdString(action);}
    ~Task(){}

private:
    string action;
    Enumeration::priority priority;
    time_t deadline;
    Enumeration::taskStatus status;
};

#endif // TASK_H

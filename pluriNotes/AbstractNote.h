#ifndef ABSTRACTNOTE_H
#define ABSTRACTNOTE_H
#include <string>
#include <Enumeration.h>

/**
 * @brief The AbstractNote class est une classe abstraite permettant de gérer les attributs et méthodes communs aux différents types de notes
 */
class AbstractNote
{
public:
    AbstractNote():title(""){}
    AbstractNote(const std::string& t):title(t){}
    const std::string getTitle()const{return title;}
    void setTitle(const std::string& t){title=t;}
    virtual void display()const=0;
    virtual Enumeration::typeNote getTypeNote()const=0;
    virtual QString getNameTypeNote()const=0;
    virtual QString getAllQString()const=0;
    virtual ~AbstractNote(){}

private:
    std::string title;
};

#endif // ABSTRACTNOTE_H

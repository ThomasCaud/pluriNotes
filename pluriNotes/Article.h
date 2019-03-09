#ifndef ARTICLE_H
#define ARTICLE_H
#include <string>
#include "AbstractNote.h"

using namespace std;

/**
 * @brief The Article class h�rite d'Abstract Note. Elle g�re les articles
 */
class Article : public AbstractNote
{
public:
    Article(const string& titleANote, const string& textArticle):AbstractNote(titleANote), text(textArticle){}
    string getText()const{return text;}
    void setText(const std::string & t){text=t;}
    void display()const;
    Enumeration::typeNote getTypeNote()const;

    QString getNameTypeNote()const{return "Article";}
    QString getAllQString()const;
    ~Article(){}

private:
    string text;
};

#endif // ARTICLE_H

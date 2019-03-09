#include "Article.h"
#include <iostream>

/**
 * @brief Article::display Pour le developpement
 */
void Article::display()const{
    std::cout<<"Note title: "<<getTitle()<<", note article: "<<getText()<<endl;
}

/**
 * @brief Article::getAllQString
 * @return un �l�ment QString contenant tout les informations de l'article (concat�nation)
 */
QString Article::getAllQString()const{
    return QString::fromStdString(getTitle())+QString::fromStdString(text);
}

/**
 * @brief Article::getTypeNote
 * @return
 */
Enumeration::typeNote Article::getTypeNote()const{
    return Enumeration::ARTICLE;
}

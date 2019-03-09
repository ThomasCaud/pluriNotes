#ifndef ARTICLEFACTORY_H
#define ARTICLEFACTORY_H
#include "Article.h"

/**
 * @brief The ArticleFactory class impl�mentant le design pattern Factory. Elle permet de cr�er concretement des objets Articles
 */
class ArticleFactory{
public:
    ArticleFactory(){}
    Article* createArticle(const string& title){return new Article(title,"");}
};

#endif // ARTICLEFACTORY_H

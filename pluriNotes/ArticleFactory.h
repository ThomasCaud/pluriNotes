#ifndef ARTICLEFACTORY_H
#define ARTICLEFACTORY_H
#include "Article.h"

/**
 * @brief The ArticleFactory class implémentant le design pattern Factory. Elle permet de créer concretement des objets Articles
 */
class ArticleFactory{
public:
    ArticleFactory(){}
    Article* createArticle(const string& title){return new Article(title,"");}
};

#endif // ARTICLEFACTORY_H

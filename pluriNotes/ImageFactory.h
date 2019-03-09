#ifndef IMAGEFACTORY_H
#define IMAGEFACTORY_H
#include "Image.h"

/**
 * @brief The ImageFactory class implémente le design pattern Factory. Elle permet de créer concretement des objets Image
 */
class ImageFactory{
public:
    ImageFactory(){}
    Image* createImage(const string& title){return new Image(title);}
};

#endif // IMAGEFACTORY_H

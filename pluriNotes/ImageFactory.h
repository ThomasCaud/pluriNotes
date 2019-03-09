#ifndef IMAGEFACTORY_H
#define IMAGEFACTORY_H
#include "Image.h"

/**
 * @brief The ImageFactory class impl�mente le design pattern Factory. Elle permet de cr�er concretement des objets Image
 */
class ImageFactory{
public:
    ImageFactory(){}
    Image* createImage(const string& title){return new Image(title);}
};

#endif // IMAGEFACTORY_H

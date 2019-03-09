#ifndef VIDEOFACTORY_H
#define VIDEOFACTORY_H
#include "Video.h"

/**
 * @brief The VideoFactory class implémente le design pattern Factory. Elle permet de créer concretement des objets Video
 */
class VideoFactory{
public:
    VideoFactory(){}
    Video* createVideo(const string& title){return new Video(title);}
};

#endif // VIDEOFACTORY_H

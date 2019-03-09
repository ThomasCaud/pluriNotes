#ifndef VIDEOFACTORY_H
#define VIDEOFACTORY_H
#include "Video.h"

/**
 * @brief The VideoFactory class impl�mente le design pattern Factory. Elle permet de cr�er concretement des objets Video
 */
class VideoFactory{
public:
    VideoFactory(){}
    Video* createVideo(const string& title){return new Video(title);}
};

#endif // VIDEOFACTORY_H

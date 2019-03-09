#ifndef AUDIOFACTORY_H
#define AUDIOFACTORY_H
#include "Audio.h"

/**
 * @brief The AudioFactory class impl�mente le design pattern Factory. Elle permet de cr�er concretement des objets Audio
 */
class AudioFactory{
public:
    AudioFactory(){}
    Audio* createAudio(const string& title){return new Audio(title);}
};

#endif // AUDIOFACTORY_H

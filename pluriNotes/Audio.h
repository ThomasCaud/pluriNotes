#ifndef AUDIO_H
#define AUDIO_H
#include "Media.h"

/**
 * @brief The Audio class hérite de la classe Media. Elle gère les notes Audio.
 */
class Audio : public Media{
public:
    Audio(const string& titleANote):Media(titleANote){}
    Audio(const string& titleANote, const string& url, const string& description):Media(titleANote, url, description){}

    void displayMedia()const;
    Enumeration::typeNote getTypeNote()const;
    QString getNameTypeNote()const{return "Audio";}
};

#endif // AUDIO_H

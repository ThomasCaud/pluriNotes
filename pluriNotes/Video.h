#ifndef VIDEO_H
#define VIDEO_H
#include "Media.h"

/**
 * @brief The Video class h�rite de la classe Media. Elle g�re les notes Video.
 */
class Video : public Media{
public:
    Video(const string& titleANote):Media(titleANote){}
    Video(const string& titleANote, const string& url, const string& description):Media(titleANote, url, description){}

    void displayMedia()const;
    Enumeration::typeNote getTypeNote()const;
    QString getNameTypeNote()const{return "Video";}
};

#endif // VIDEO_H

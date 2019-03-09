#ifndef IMAGE_H
#define IMAGE_H
#include "Media.h"

/**
 * @brief The Image class hérite de la classe Media. Elle gère les notes Image.
 */
class Image : public Media{
public:
    Image(const string& titleANote):Media(titleANote){}
    Image(const string& titleANote, const string& url, const string& description):Media(titleANote, url, description){}

    void displayMedia()const{}
    Enumeration::typeNote getTypeNote()const{return Enumeration::IMAGE;}
    QString getNameTypeNote()const{return "Image";}
};

#endif // IMAGE_H

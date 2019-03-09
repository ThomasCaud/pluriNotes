#ifndef MEDIA_H
#define MEDIA_H

#include <string>
#include "AbstractNote.h"
#include <iostream>

using namespace std;

/**
 * @brief The Media class est une classe abstraite. Elle permet de spécialiser les AbstractNote, et permet de définir les méthodes et attributs communs aux Notes multimedias.
 */
class Media : public AbstractNote
{
public:
    Media(const string& titleANote):AbstractNote(titleANote), urlMedia("noUrl"), description("noDescription"){}
    Media(const string& titleANote, const string& url, const string& description):AbstractNote(titleANote), urlMedia(url), description(description){}

    string getUrlMedia()const{return urlMedia;}
    string getDescription()const{return description;}

    void setUrlMedia(const std::string& url){urlMedia=url;}
    void setDescription(const std::string& desc){description=desc;}

    void display()const{
        std::cout<<getNameTypeNote().toStdString()<<", urlMedia: "<<urlMedia<<", description"<<description<<endl;
    }
    virtual void displayMedia()const=0;
    QString getAllQString()const{return QString::fromStdString(getTitle())+QString::fromStdString(description);}
    ~Media(){}

private:
    string urlMedia;
    string description;
};

#endif // MEDIA_H

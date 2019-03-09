#ifndef XMLMANAGER_H
#define XMLMANAGER_H

#include <QtXml>
#include <iostream>
#include "FormatDataStrategy.h"

/**
 * @brief The writterAndFileStream struct permet de récuperer un QFile et un QXmlStreamWriter permettant l'écriture dans le fichier XML
 */
struct writterAndFileStream{
    QFile* file;
    QXmlStreamWriter* stream;
};

/**
 * @brief The readerAndFileStream struct struct permet de récuperer un QFile et un QXmlStreamReader permettant la lecture dans le fichier XML
 */
struct readerAndFileStream{
    QFile* file;
    QXmlStreamReader* stream;
};

/**
 * @brief The XMLManager class implémentation du pattern Strategy afin de sauvegarder/charger en XML
 */
class XMLManager: public FormatDataStrategy{

private:
    static XMLManager* instance;
    static std::string filenameNotes;
    static std::string filenameParameters;
    static std::string filenameRelations;
    static std::string filenameContexte;
    XMLManager();
    ~XMLManager(){}
    QString filename;
    writterAndFileStream* getWritterStream(const std::string&)const;
    readerAndFileStream* getReaderStream(const std::string&)const;    

public:
    static XMLManager& getInstance();
    void save(const Note&, writterAndFileStream*);
    void save(Parameter);
    void save(Application&);
    void save(RelationListManager*);
    void load(Parameter*);
    void load(Application&);
    void load(RelationListManager*);
    void saveContext(Ui::FenPrincipale*);
    void loadContext(FenPrincipale*);
};

#endif // XMLMANAGER_H

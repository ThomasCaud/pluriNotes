/**
  * \file XMLNoteManager.cpp
  * \brief Permet récupération des données depuis un fichier XML
  * \author Thomas Caudrelier
  * \version 0.1
*/
#include "RelationListManager.h"
#include "FenPrincipale.h"
#include "XMLManager.h"
#include <iostream>
#include <string>
#include <vector>
#include "Article.h"
#include "Task.h"
#include "Audio.h"
#include "Video.h"
#include "Image.h"

using namespace std;

XMLManager* XMLManager::instance = 0;
string XMLManager::filenameNotes = "../XML/notes.xml";
string XMLManager::filenameParameters = "../XML/parameters.xml";
string XMLManager::filenameRelations = "../XML/relations.xml";
string XMLManager::filenameContexte = "../XML/contexte.xml";

XMLManager::XMLManager(){

}

/**
 * @brief XMLManager::getInstance cette classe est un singleton car elle doit être instanciée une seule et unique fois
 * @return
 */
XMLManager& XMLManager::getInstance(){
    if (!instance){
        instance = new XMLManager;
    }
    return *instance;
}

/**
 * Factorisation de l'ouverture d'un fichier en écriture
 * @return a writterAndFileStream which permits to write and close a file
 */
writterAndFileStream* XMLManager::getWritterStream(const string& filename)const{
    writterAndFileStream* streamWritter = new writterAndFileStream;
    streamWritter->file = new QFile(QString::fromStdString(filename));
    if(!streamWritter->file->exists()){
        throw new std::string("file " + filename + " doesn't exist!");
    }

    streamWritter->file->open(QIODevice::WriteOnly | QIODevice::Text);
    if(!streamWritter->file->isWritable()){
        throw new std::string("file " + filename + " isn't writtable");
    }

    streamWritter->stream = new QXmlStreamWriter(streamWritter->file);
    if(streamWritter->stream->hasError()){
        throw new std::string("QXMLStreamWriter error on file " + filename);
    }
    streamWritter->stream->setAutoFormatting(true);

    return streamWritter;
}

/**
 * Factorisation de l'ouverture d'un fichier en lecture
 * @return a readerAndFileStream which permits to read and close a file
 */
readerAndFileStream* XMLManager::getReaderStream(const std::string& filename)const{
    readerAndFileStream* streamReader = new readerAndFileStream;
    streamReader->file = new QFile(QString::fromStdString(filename));
    if(!streamReader->file->exists()){
        throw new std::string("file " + filename + " doesn't exist!");
    }

    if(!streamReader->file->open(QFile::ReadOnly | QFile::Text)){
        throw new std::string("cannot read file " + filename);
    }

    streamReader->stream = new QXmlStreamReader(streamReader->file);
    if(streamReader->stream->hasError()){
        throw new std::string("QXMLStreamReader error on file " + filename);
    }

    return streamReader;
}

/**
 * @brief XMLManager::save permet de sauvegarder au format XML les Paramètres de l'application
 * @param parameters
 */
void XMLManager::save(Parameter parameters){
    writterAndFileStream *xmlWriter = getWritterStream(XMLManager::filenameParameters);
    xmlWriter->stream->writeStartDocument();
        xmlWriter->stream->writeStartElement("Parameters");
            xmlWriter->stream->writeTextElement("removeAuto", QString::number(parameters.getRemoveAuto()));
            xmlWriter->stream->writeTextElement("displayTaskByPriority", QString::number(parameters.getDisplayTaskByPriority()));
            xmlWriter->stream->writeTextElement("saveAuto", QString::number(parameters.getSaveAuto()));
            xmlWriter->stream->writeEndElement();
        xmlWriter->stream->writeEndDocument();
    xmlWriter->file->close();
}

/**
 * @brief XMLManager::save permet de sauvegarder au format XML une Note
 * @param note
 * @param xmlWriter afin de continuer à écrire dans le même fichier que application * @brief XMLManager::save permet de sauvegarder au format XML l'applications
 */
void XMLManager::save(const Note& note, writterAndFileStream* xmlWriter){
    QXmlStreamWriter* xmlWriterStream = xmlWriter->stream;

    xmlWriterStream->writeStartElement("Note");
    xmlWriterStream->writeTextElement("id",QString::number(note.getId()));
    xmlWriterStream->writeTextElement("title", QString::fromStdString(note.getTitle()));
    xmlWriterStream->writeTextElement("creation", QString::number(note.getCreation()));
    xmlWriterStream->writeTextElement("lastModification", QString::number(note.getLastModification()));
    xmlWriterStream->writeTextElement("status",QString::number(note.getStatus()));
    xmlWriterStream->writeStartElement("Versions");

    VersionManager::Iterator* it = note.getVersionManager().getIterator();
    AbstractNote* aNote;

    //Sauvegarde des versions
    for(unsigned int i = 0 ; i < it->size() ; i++){
        xmlWriterStream->writeStartElement("version");
            xmlWriterStream->writeTextElement("noVersion",QString::number(it->getVersion(i).getNoVersion()));

            aNote = it->getVersion(i).getaNote();
            xmlWriterStream->writeStartElement("aNote");
                xmlWriterStream->writeTextElement("title",QString::fromStdString(aNote->getTitle()));

                Enumeration::typeNote typeNote = aNote->getTypeNote();
                if(typeNote == Enumeration::ARTICLE){
                    xmlWriterStream->writeStartElement("article");
                        Article* article = static_cast<Article*>(aNote);
                        xmlWriterStream->writeTextElement("text",QString::fromStdString(article->getText()));
                    xmlWriterStream->writeEndElement();
                } else if(typeNote == Enumeration::TASK){
                    xmlWriterStream->writeStartElement("task");
                        Task* task = static_cast<Task*>(aNote);
                        xmlWriterStream->writeTextElement("action",QString::fromStdString(task->getAction()));
                        xmlWriterStream->writeTextElement("priority",QString::number(task->getPriority()));
                        xmlWriterStream->writeTextElement("deadline",QString::number(task->getDeadline()));
                        xmlWriterStream->writeTextElement("status",QString::number(task->getStatus()));
                    xmlWriterStream->writeEndElement();
                } else if(typeNote == Enumeration::AUDIO || typeNote == Enumeration::VIDEO || typeNote == Enumeration::IMAGE){
                    QString nomElement;
                    if(typeNote == Enumeration::AUDIO){
                        nomElement="audio";
                    } else if(typeNote == Enumeration::VIDEO){
                        nomElement="video";
                    }
                    else if(typeNote == Enumeration::IMAGE){
                        nomElement="image";
                    } else {
                        cout<<"Pas audio ni video ni image??"<<endl;
                    }
                    xmlWriterStream->writeStartElement(nomElement);
                        Audio* audio = static_cast<Audio*>(aNote);
                        xmlWriterStream->writeTextElement("urlMedia",QString::fromStdString(audio->getUrlMedia()));
                        xmlWriterStream->writeTextElement("description",QString::fromStdString(audio->getDescription()));
                    xmlWriterStream->writeEndElement();
                } else {
                    std::cout<<"Pas un type de note sauvegardable?"<<endl;
                }
            xmlWriterStream->writeEndElement();
        xmlWriterStream->writeEndElement();
    }

    xmlWriterStream->writeEndElement();
    xmlWriterStream->writeEndElement();
}

/**
 * @brief XMLManager::save permet de sauvegarder au format XML l'application
 * @param app
 */
void XMLManager::save(Application &app){
    // Sauvegarde des paramètres
    Parameter *params = app.getParameters();
    save(*params);

    save(app.getRelationListManager());

    writterAndFileStream *xmlWriter = getWritterStream(XMLManager::filenameNotes);

    xmlWriter->stream->writeStartDocument();
        xmlWriter->stream->writeStartElement("Application");

            // Sauvegarde des Notes
            const std::vector<Note> noteList = *(app.getNoteListManager()->getNoteList());
            for(unsigned int i = 0 ; i < noteList.size() ; i++){
                save(noteList[i], xmlWriter);
            }
        xmlWriter->stream->writeEndElement();
    xmlWriter->stream->writeEndDocument();
    xmlWriter->file->close();
}

/**
 * @brief XMLManager::save permet la sauvegarde des relations
 */
void XMLManager::save(RelationListManager* relationManager){
    writterAndFileStream *xmlWriter = getWritterStream(XMLManager::filenameRelations);
    QXmlStreamWriter * writer = xmlWriter->stream;
    writer->writeStartDocument();
        writer->writeStartElement("Relations");
            // sauvegarde de chaque relations
            vector<Relation> relations = *relationManager->getRelationList();
            for(unsigned int i = 0 ; i < relations.size() ; i++){
                // sauvegarde d'une relation
                Relation relation = relations.at(i);

                writer->writeStartElement("Relation");
                    writer->writeTextElement("id",QString::number(relation.getId()));
                    writer->writeTextElement("titre",QString::fromStdString(relation.getTitre()));
                    writer->writeTextElement("description",QString::fromStdString(relation.getDescription()));
                    writer->writeTextElement("isOriented", QString::number(relation.getIsOriented()));
                    writer->writeTextElement("isReference", QString::number(relation.getIsReference()));

                    // sauvegarde des couples
                    vector<Couple> couples = relation.getcoupleList();
                    writer->writeStartElement("coupleList");
                        for(unsigned int j = 0 ; j < couples.size() ; j++){
                            // sauvegarde de chaque couple
                            writer->writeStartElement("couple");
                                writer->writeTextElement("label", QString::fromStdString(couples.at(j).getLabel()));
                                // on ne sauvegarde que l'ID
                                writer->writeTextElement("id1", QString::number(couples.at(j).getNoteA()));
                                writer->writeTextElement("id2", QString::number(couples.at(j).getNoteB()));
                            writer->writeEndElement();
                        }
                    writer->writeEndElement();
                writer->writeEndElement();
            }
        writer->writeEndElement();
    writer->writeEndDocument();
    xmlWriter->file->close();
}

/**
 * @brief XMLManager::load permet de charger les paramètres de l'application à partir du document XML
 * @param parameters
 */
void XMLManager::load(Parameter* parameters){
    readerAndFileStream *xmlReader = getReaderStream(XMLManager::filenameParameters);
    QXmlStreamReader* xmlReaderStream = xmlReader->stream;

    if (xmlReaderStream->readNextStartElement()) {
        if(xmlReaderStream->name() == "Parameters"){
            // Récupération des paramètres
            while(xmlReaderStream->readNextStartElement()){
                if(xmlReaderStream->name() == "removeAuto"){
                    parameters->setRemoveAuto(xmlReaderStream->readElementText().toInt());
                } else if (xmlReaderStream->name() == "displayTaskByPriority"){
                    parameters->setDisplayTaskByPriority(xmlReaderStream->readElementText().toInt());
                } else if (xmlReaderStream->name() == "saveAuto"){
                    parameters->setSaveAuto(xmlReaderStream->readElementText().toInt());
                } else{
                    xmlReaderStream->skipCurrentElement();
                }
            }
        } else {
            std::cout<<"Pas des parametres?("<<xmlReaderStream->name().toString().toStdString()<<")"<<endl;
        }
    }

    xmlReader->file->close();
}

/**
 * @brief XMLManager::load permet de charger l'application à partir du document XML
 * @param application
 */
void XMLManager::load(Application &application){
    application.getNoteListManager()->clear();
        readerAndFileStream *xmlReader = getReaderStream(XMLManager::filenameNotes);
        QXmlStreamReader* xmlReaderStream = xmlReader->stream;
        if (xmlReaderStream->readNextStartElement()) {
            if(xmlReaderStream->name() == "Application"){

                // Récupération de la liste des notes
                while(xmlReaderStream->readNextStartElement()){
                     if(xmlReaderStream->name() == "Note"){
                         // création de la nouvelle note
                         Note note;
                         // récupération des éléments d'une note

                          while(xmlReaderStream->readNextStartElement()){
                              if(xmlReaderStream->name() == "title"){
                                note.setTitle(xmlReaderStream->readElementText().toStdString());
                              } else if(xmlReaderStream->name() == "creation"){
                                note.setCreation(xmlReaderStream->readElementText().toInt());
                              } else if(xmlReaderStream->name() == "lastModification"){
                                note.setLastModification(xmlReaderStream->readElementText().toInt());
                              } else if(xmlReaderStream->name() == "status"){
                                note.setStatus(static_cast<Enumeration::noteStatus>(xmlReaderStream->readElementText().toInt()));
                              }

                              // Récupération des versions
                              else if(xmlReaderStream->name() == "Versions"){
                                while(xmlReaderStream->readNextStartElement()){
                                    if(xmlReaderStream->name() == "version"){
                                        //Récupération de la version
                                        string titleANote;
                                        AbstractNote *aNote;
                                        while(xmlReaderStream->readNextStartElement()){
                                            if(xmlReaderStream->name() == "noVersion"){
                                                xmlReaderStream->skipCurrentElement();
                                            }else if(xmlReaderStream->name() == "aNote"){
                                                while(xmlReaderStream->readNextStartElement()){
                                                    if(xmlReaderStream->name() == "title"){
                                                        titleANote = xmlReaderStream->readElementText().toStdString();
                                                    } else if (xmlReaderStream->name() == "article"){
                                                        // Recuperation d'un article
                                                        string textArticle;
                                                        //Parcours des éléments d'un Article
                                                        while(xmlReaderStream->readNextStartElement()){
                                                            if(xmlReaderStream->name() == "text"){
                                                                textArticle = xmlReaderStream->readElementText().toStdString();
                                                            } else{
                                                                xmlReaderStream->skipCurrentElement();
                                                            }
                                                        }
                                                        aNote = new Article(titleANote, textArticle);
                                                    } else if(xmlReaderStream->name() == "task"){
                                                        // Récupération d'une tâche
                                                        string action;
                                                        Enumeration::priority priority;
                                                        time_t deadline;
                                                        Enumeration::taskStatus status;
                                                        //Parcours des éléments d'une tâche
                                                        while(xmlReaderStream->readNextStartElement()){
                                                            if(xmlReaderStream->name() == "action"){
                                                                action = xmlReaderStream->readElementText().toStdString();
                                                            } else if(xmlReaderStream->name() == "priority"){
                                                                priority = static_cast<Enumeration::priority>(xmlReaderStream->readElementText().toInt());
                                                            } else if(xmlReaderStream->name() == "deadline"){
                                                                deadline = xmlReaderStream->readElementText().toInt();
                                                            } else if(xmlReaderStream->name() == "status"){
                                                                status = static_cast<Enumeration::taskStatus>(xmlReaderStream->readElementText().toInt());
                                                            } else{
                                                                xmlReaderStream->skipCurrentElement();
                                                            }
                                                        }
                                                        aNote = new Task(titleANote, action, priority, deadline, status);

                                                    } else if(xmlReaderStream->name() == "audio" || xmlReaderStream->name() == "video" || xmlReaderStream->name() == "image"){
                                                        string urlMedia;
                                                        string description;

                                                        //Parcours des éléments d'un Media
                                                        while(xmlReaderStream->readNextStartElement()){
                                                            if(xmlReaderStream->name() == "urlMedia"){
                                                                urlMedia = xmlReaderStream->readElementText().toStdString();
                                                            } else if(xmlReaderStream->name() == "description"){
                                                                description = xmlReaderStream->readElementText().toStdString();
                                                            } else{
                                                                xmlReaderStream->skipCurrentElement();
                                                            }
                                                        }
                                                        if(xmlReaderStream->name() == "audio"){
                                                            aNote = new Audio(titleANote, urlMedia, description);
                                                        } else if(xmlReaderStream->name() == "video"){
                                                            aNote = new Video(titleANote, urlMedia, description);
                                                        } else if(xmlReaderStream->name() == "image"){
                                                            aNote = new Image(titleANote, urlMedia, description);
                                                        } else {
                                                            throw "Recuperation: Pas audio ni video ni image?";
                                                        }

                                                    } else {
                                                        xmlReaderStream->skipCurrentElement();
                                                    }
                                                }
                                            }
                                            else{
                                                xmlReaderStream->skipCurrentElement();
                                            }
                                        }
                                        // Ajout de la version à la liste
                                        note.addVersion(aNote);
                                    }else{
                                        std::cout<<"Pas une version?("<<xmlReaderStream->name().toString().toStdString()<<")"<<endl;
                                    }
                                }
                                // Ajout de la note à l'application
                                application.getNoteListManager()->addNote(note);
                              } else {
                                  xmlReaderStream->skipCurrentElement();
                              }
                          }
                     } else {
                        std::cout<<"Pas une Note?("<<xmlReaderStream->name().toString().toStdString()<<")"<<endl;
                     }
                }
            }
        }
        xmlReader->file->close();

        // Chargement des paramètres
        Parameter *params = application.getParameters();
        load(params);
        load(application.getRelationListManager());
}

/**
 * @brief XMLManager::load
 * @param relationsManager
 * @param notesManager
 */
void XMLManager::load(RelationListManager* relationsManager){
        readerAndFileStream *xmlReader = getReaderStream(XMLManager::filenameRelations);
        QXmlStreamReader* xmlReaderStream = xmlReader->stream;

        if (xmlReaderStream->readNextStartElement()) {
            if(xmlReaderStream->name() == "Relations"){
                // Récupération des relations
                while(xmlReaderStream->readNextStartElement()){
                    if(xmlReaderStream->name() == "Relation"){
                        // création de la nouvelle relation
                        Relation relation;
                        // récupération des éléments d'une relation

                         while(xmlReaderStream->readNextStartElement()){
                             // on ne récupère pas l'id : il se gère lui même
                            if (xmlReaderStream->name() == "titre"){
                                relation.setTitre(xmlReaderStream->readElementText().toStdString());
                            } else if (xmlReaderStream->name() == "description"){
                                relation.setDescription(xmlReaderStream->readElementText().toStdString());
                            } else if (xmlReaderStream->name() == "isOriented"){
                                relation.setIsOriented(xmlReaderStream->readElementText().toInt());
                            } else if (xmlReaderStream->name() == "isReference"){
                                relation.setIsReference(xmlReaderStream->readElementText().toInt());
                            } else if(xmlReaderStream->name() == "coupleList"){
                                // récupération des couples
                                Couple* c;
                                while(xmlReaderStream->readNextStartElement()){
                                    if(xmlReaderStream->name() == "couple"){
                                        // récupération d'un couple
                                        c = new Couple();
                                        while(xmlReaderStream->readNextStartElement()){
                                            if(xmlReaderStream->name() == "label"){
                                                c->setLabel(xmlReaderStream->readElementText().toStdString());
                                            } else if(xmlReaderStream->name() == "id1"){
                                                c->setNoteA(xmlReaderStream->readElementText().toInt());
                                            } else if(xmlReaderStream->name() == "id2"){
                                                c->setNoteB(xmlReaderStream->readElementText().toInt());
                                            } else {
                                                cout<<"Ne devrait pas passer par la 110 ("<<xmlReaderStream->readElementText().toStdString()<<")"<<endl;
                                                xmlReaderStream->skipCurrentElement();
                                            }
                                         }
                                         relation.addCouple(*c);
                                     } else {
                                        cout<<"Ne devrait pas passer par là ("<<xmlReaderStream->readElementText().toStdString()<<")"<<endl;
                                        xmlReaderStream->skipCurrentElement();
                                     }
                                 }
                            } else {
                                // skip l'id
                                xmlReaderStream->skipCurrentElement();
                            }
                        }
                        relationsManager->addRelation(relation);
                    } else {
                        cout<<"Ne devrait pas passer par la (111)"<<xmlReaderStream->name().toString().toStdString()<<endl;
                        xmlReaderStream->skipCurrentElement();
                    }
                }
            } else {
                cout<<"Ne devrait pas passer par la (112)"<<xmlReaderStream->name().toString().toStdString()<<endl;
                xmlReaderStream->skipCurrentElement();
            }
        }
        xmlReader->file->close();
}

/**
 * @brief XMLManager::loadContext récupère le contexte et le charge
 */
void XMLManager::loadContext(FenPrincipale* fen){
    Ui::FenPrincipale *ui = fen->getWindow();
    readerAndFileStream *xmlReader = getReaderStream(XMLManager::filenameContexte);
    QXmlStreamReader* xmlReaderStream = xmlReader->stream;

    if (xmlReaderStream->readNextStartElement()) {
        if(xmlReaderStream->name() == "Contexte"){
            // Récupération des paramètres
            while(xmlReaderStream->readNextStartElement()){
                if(xmlReaderStream->name() == "TabFocus"){
                    ui->tabWidget->setCurrentIndex(xmlReaderStream->readElementText().toInt());
                } else if(xmlReaderStream->name() == "CurrentRowNote") {
                    int currentRowNote = xmlReaderStream->readElementText().toInt();
                    if(currentRowNote >= 0 && currentRowNote < Note::nbNote){
                        // il existe un item qui était selectionnée
                        ui->activeNoteList->setCurrentRow(currentRowNote);
                        fen->simuleClickedActiveNoteList(ui->activeNoteList->item(currentRowNote));
                    }
                }
                else {
                    xmlReaderStream->skipCurrentElement();
                }
            }
        } else {
            std::cout<<"Pas du contexte ?("<<xmlReaderStream->name().toString().toStdString()<<")"<<endl;
        }
    }

    xmlReader->file->close();
}

/**
 * @brief XMLManager::saveContext permet de sauvegarder l'état de l'application (le contexte)
 */
void XMLManager::saveContext(Ui::FenPrincipale* ui){
    writterAndFileStream *xmlWriter = getWritterStream(XMLManager::filenameContexte);
    QXmlStreamWriter * writer = xmlWriter->stream;
    writer->writeStartDocument();
        writer->writeStartElement("Contexte");
            writer->writeTextElement("TabFocus", QString::number(ui->tabWidget->currentIndex()));
            writer->writeTextElement("CurrentRowNote", QString::number(ui->activeNoteList->currentRow()));
        writer->writeEndElement();
    writer->writeEndDocument();
    xmlWriter->file->close();
}


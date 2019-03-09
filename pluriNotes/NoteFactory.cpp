#include "NoteFactory.h"

/**
 * @brief NoteFactory::getNote Interface pour la création d'un objet de type AbstractNote
 * @param typeN
 * @param title
 * @return
 */
AbstractNote* NoteFactory::getNote(Enumeration::typeNote typeN, const string& title){
    if(typeN == Enumeration::ARTICLE){
        return articleF.createArticle(title);
    } else if(typeN == Enumeration::TASK){
        return taskF.createTask(title);
    } else if(typeN == Enumeration::AUDIO){
        return audioF.createAudio(title);
    }  else if(typeN == Enumeration::VIDEO){
        return videoF.createVideo(title);
    }  else if (typeN == Enumeration::IMAGE){
        return imageF.createImage(title);
    }   else{
        throw "Ne correspond à aucun typeNote";
    }
}

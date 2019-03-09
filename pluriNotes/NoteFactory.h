#ifndef NOTEFACTORY_H
#define NOTEFACTORY_H
#include "AbstractNote.h"
#include "AudioFactory.h"
#include "ArticleFactory.h"
#include "ImageFactory.h"
#include "TaskFactory.h"
#include "VideoFactory.h"
#include "Enumeration.h"
#include <string>

/**
 * @brief The NoteFactory class implémente le design pattern Factory. Elle est la fabrique "maitre" des autres Factory (AudioFactory, TaskFactory, ...) et gère ainsi l'ensemble des méthodes communes (création générique d'objet) aux fabriques concrètes.
 */
class NoteFactory{
public:
    NoteFactory(){}
    AbstractNote* getNote(Enumeration::typeNote, const string&);

private:
    AudioFactory audioF;
    ArticleFactory articleF;
    ImageFactory imageF;
    TaskFactory taskF;
    VideoFactory videoF;
};

#endif // NOTEFACTORY_H

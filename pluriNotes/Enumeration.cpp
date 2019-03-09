#include "Enumeration.h"

/**
 * @brief EnumerationLibrary::priorityToQString Permet la conversion d'une priority (enum) en QString
 * @param p
 * @return QString
 */
QString EnumerationLibrary::priorityToQString(Enumeration::priority p){
    if(p == Enumeration::HIGH) return "Haute";
    if(p == Enumeration::MEDIUM) return "Moyenne";
    if(p == Enumeration::LOW) return "Basse";
    if(p == Enumeration::UNDEFINEDPRIORITY) return "Indefinie";

    else return "not a priority?";
}

/**
 * @brief EnumerationLibrary::qstringToTypeNote Permet la conversion d'un QString en typeNote (enum)
 * @param s
 * @return Enumeration::typeNote
 */
Enumeration::typeNote EnumerationLibrary::qstringToTypeNote(QString s){
    if(s == "Image"){
        return Enumeration::IMAGE;
    } else if(s == "Audio"){
        return Enumeration::AUDIO;
    } else if(s == "Video"){
        return Enumeration::VIDEO;
    } else if(s == "Task"){
        return Enumeration::TASK;
    } else if(s == "Article") {
        return Enumeration::ARTICLE;
    } else {
        throw "Not a type?";
    }
}

/**
 * @brief EnumerationLibrary::qstringToPriority Permet la conversion d'un QString en priority (enum)
 * @param s
 * @return Enumeration::priority
 */
Enumeration::priority EnumerationLibrary::qstringToPriority(QString s){
    if(s == "Haute") return Enumeration::HIGH;
    if(s == "Moyenne") return Enumeration::MEDIUM;
    if(s == "Basse") return Enumeration::LOW;
    return Enumeration::UNDEFINEDPRIORITY;
}

/**
 * @brief EnumerationLibrary::qstringToTaskStatus Permet la conversion d'un QString en taskStatus (enum)
 * @param s
 * @return Enumeration::taskStatus
 */
Enumeration::taskStatus EnumerationLibrary::qstringToTaskStatus(QString s){
    if(s == "A faire") return Enumeration::TODO;
    if(s == "En cours") return Enumeration::DOING;
    if(s == "Terminee") return Enumeration::DONE;
    return Enumeration::UNDEFINEDSTATUS;
}

/**
 * @brief EnumerationLibrary::taskStatusToQstring Permet la conversion d'un taskStatus (enum) en QString
 * @param ts
 * @return QString
 */
QString EnumerationLibrary::taskStatusToQstring(Enumeration::taskStatus ts){
    if(ts == Enumeration::TODO) return "A faire";
    if(ts == Enumeration::DOING) return "En cours";
    if(ts == Enumeration::DONE) return "Terminée";
    return "Indefinie";
}

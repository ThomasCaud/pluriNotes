#ifndef ENUMERATION_H
#define ENUMERATION_H
#include <QString>

namespace Enumeration {
    typedef enum {
        IN_TRASH,
        ARCHIVED,
        ACTIVE
    } noteStatus;

    typedef enum {
        TASK,
        ARTICLE,
        AUDIO,
        VIDEO,
        IMAGE
    } typeNote;

    typedef enum {
        TODO,
        DOING,
        DONE,
        UNDEFINEDSTATUS
    } taskStatus;

    typedef enum {
        LOW,
        MEDIUM,
        HIGH,
        UNDEFINEDPRIORITY
    } priority;
}

/**
* @brief The Enumeration class permet de faciliter les conversions enum <=> int <=> string
*/
class EnumerationLibrary{
public:
   static QString priorityToQString(Enumeration::priority);
   static Enumeration::typeNote qstringToTypeNote(QString);
   static Enumeration::priority qstringToPriority(QString);
   static Enumeration::taskStatus qstringToTaskStatus(QString);
   static QString taskStatusToQstring(Enumeration::taskStatus);
};

#endif // ENUMERATION_H

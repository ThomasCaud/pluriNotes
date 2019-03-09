#ifndef VERSION_H
#define VERSION_H
#include "AbstractNote.h"

/**
 * @brief The Version class permet de stocker une version de Note
 */
class Version
{
public:
    Version():aNote(0),noVersion(0){}
    Version(AbstractNote *n, int v):aNote(n),noVersion(v){}

    int getNoVersion()const{return noVersion;}
    void display()const;
    AbstractNote* getaNote()const{return aNote;}

private:
    AbstractNote *aNote;
    int noVersion;
};

#endif // VERSION_H

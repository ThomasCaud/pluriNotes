#ifndef COUPLE_H
#define COUPLE_H
#include "Note.h"
#include <string>

using namespace std;

/**
 * @brief The Couple class permet de gérer les couples en jeu dans une Relation. Un couple est composé de deux Notes et d'un label, optionnel.
 */
class Couple {
public:
    Couple(int na, int nb, const string& l = "noLabel"):idNote1(na),idNote2(nb),label(l){}
    Couple(){idNote1=-1;idNote2=-1;label="default";}
    int getNoteA(){return idNote1;}
    int getNoteB(){return idNote2;}
    string getLabel(){return label;}

    void setNoteA(int id){idNote1=id;}
    void setNoteB(int id){idNote2=id;}
    void setLabel(string l){label=l;}
    bool isInCouple(int idNote){return (idNote1==idNote)||(idNote2==idNote);}

private:
    int idNote1;
    int idNote2;
    string label;
};

#endif // COUPLE_H

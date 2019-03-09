#ifndef RELATION_H
#define RELATION_H
#include "Couple.h"
#include <string>

/**
 * @brief The Relation class gère les relations liées aux notes. Une relation est composée d'un couple de Notes, peut etre orientée ou non. Elle possède divers autres attributs comme un titre ou une description.
 */
class Relation
{
public:
    static int nbRelation;
    Relation(const string& t = "noTitle", const string& d = "noDescription"):id(Relation::nbRelation), titre(t), description(d), isOriented(true), isReference(false){Relation::nbRelation++;}

    void display()const;

    int getId()const{return id;}
    bool getIsOriented()const{return isOriented;}
    bool getIsReference()const{return isReference;}
    string getTitre()const{return titre;}
    string getDescription()const{return description;}
    std::vector<Couple> getcoupleList(){return coupleList;}

    void setTitre(const std::string & t){titre=t;}
    void setDescription(const std::string & d){description=d;}
    void setIsOriented(bool b){isOriented=b;}
    void setIsReference(bool b){isReference=b;}
    void cleanCoupleList(){coupleList.clear();}
    void removeCouple(int idNote);

    void addCouple(Couple&);
    void removeCoupleWithIds(int idNoteA, int idNoteB);
private:
    int id;
    string titre;
    string description;
    bool isOriented;
    bool isReference;
    std::vector<Couple> coupleList;
    void deleteCouple(unsigned int indiceCouple);
};

#endif // RELATION_H

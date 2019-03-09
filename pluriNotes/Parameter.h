#ifndef PARAMETER_H
#define PARAMETER_H

/**
 * @brief The Parameter class gère l'ensemble des paramètres de l'application, comme la gestion de la corbeille, l'interface d'affichage des taches ou encore la sauvegarde automatique
 */
class Parameter{
public:
    Parameter():removeAuto(false),saveAuto(true),displayTaskByPriority(true){}
    bool getRemoveAuto()const{return removeAuto;}
    void setRemoveAuto(bool b){removeAuto=b;}
    bool getSaveAuto()const{return saveAuto;}
    void setSaveAuto(bool b){saveAuto=b;}
    bool getDisplayTaskByPriority()const{return displayTaskByPriority;}
    void setDisplayTaskByPriority(bool b){displayTaskByPriority=b;}
    void display()const;

private:
    bool removeAuto;
    bool saveAuto;
    bool displayTaskByPriority;
};

#endif // PARAMETER_H

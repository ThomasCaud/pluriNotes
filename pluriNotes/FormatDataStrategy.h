/**
  * \file FormatDataStrategy.h
  * \brief Entete pour la class FormatDataStrategy
  * \authors QuentinS ThomasC
  * \version 0.1
*/
#ifndef FORMATDATASTRATEGY_H
#define FORMATDATASTRATEGY_H
#include "Application.h"

class FenPrincipale;
/**
 * @brief The FormatDataStrategy class permet de changer facilement de changement de format de sauvegarde/chargement des fichiers
 */
class FormatDataStrategy{
public:
    virtual void save(Application&)=0;
    virtual void load(Application&)=0;
    virtual void saveContext(Ui::FenPrincipale*)=0;
    virtual void loadContext(FenPrincipale*)=0;
    virtual ~FormatDataStrategy(){}
};

#endif // FORMATDATASTRATEGY_H

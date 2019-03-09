/*
  * \file application.cpp
  * \brief Application gère les ressources majeures de pluriNotes
  * \details L'unique instance Application est responsable de l'ensemble des listes (notes, relations) est gère les suppressions des elements de celles-ci. On l'implémente via un singleton.
  * \author QuentinS
  * \version 0.1
*/

#include <iostream>
#include <QApplication>
#include "Application.h"

using namespace std;

/**
 * @brief Application::Application
 * @param nListManager
 * @param rListManager
 */
Application::Application(NoteListManager* nListManager, RelationListManager* rListManager){
    noteListManager = nListManager;
    noteListManager->setParameter(&parameters);
    relationListManager = rListManager;
}

/**
 * @brief Application::~Application
 */
Application::~Application(){

}

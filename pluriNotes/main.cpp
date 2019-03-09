/**
  * \file main.cpp
  * \brief Fichier main standard
  * \authors QuentinS ThomasC
  * \version 0.1
*/
#include "FenPrincipale.h"
#include <QApplication>
#include <iostream>
#include "Note.h"
#include "Enumeration.h"
#include "NoteFactory.h"
#include "Task.h"

using namespace std;

/*! \mainpage Accueil - Documentation pluriNotes
 *
 * @authors Caudrelier Thomas - Quentin Sutkowski
 *
 * \section intro_sec Informations
 *
 * Documentation du projet pluriNotes dans le cadre de l'UV LO21
 *
 * \section description_sec LO21 - Printemps 2017 - Projet PluriNotes
 *  Dans ce projet, il s?agit de concevoir et développer l?application PLURINOTES, destinée à éditer et gérer un ensemble de notes (des mémos) qui peuvent correspondre à du texte ou des images. Une note peut par exemple correspondre au compte-rendu d?une réunion, à des notes prises lors d?une séance de cours, ou encore à des tâches à e?ectuer.
 *
 */

int main(int argc, char* argv[]){
    QApplication a(argc, argv);
    FenPrincipale* window = FenPrincipale::getInstance();
    window->display();
    return a.exec();
}

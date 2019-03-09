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
 *  Dans ce projet, il s?agit de concevoir et d�velopper l?application PLURINOTES, destin�e � �diter et g�rer un ensemble de notes (des m�mos) qui peuvent correspondre � du texte ou des images. Une note peut par exemple correspondre au compte-rendu d?une r�union, � des notes prises lors d?une s�ance de cours, ou encore � des t�ches � e?ectuer.
 *
 */

int main(int argc, char* argv[]){
    QApplication a(argc, argv);
    FenPrincipale* window = FenPrincipale::getInstance();
    window->display();
    return a.exec();
}

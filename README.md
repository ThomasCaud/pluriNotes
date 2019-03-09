# LO21 - Projet UTC

## Liens
* Drive: https://docs.google.com/spreadsheets/d/1GrTnVnbx3tBSSjyIq_h0Cup7TY3SAygDxTM1qBKXVZA/edit#gid=0
* Maquettage: https://cacoo.com/diagrams/Vz0tPODreOJvmHlT
* Diagramme de classe: https://scontent.xx.fbcdn.net/v/t34.0-12/18555054_10211123436214810_144502566_n.png?oh=8642fd1de5a2faf098d11357cd68cfb3&oe=591FF2A6

## Bonnes pratiques
* Réduire au minimum vital les inclusions dans les fichiers d’en-tête (.h) en les déportant dans les fichiers d’implémentation (.cpp)
* Aucune directive #include ne doit apparaître dans un .h, sauf si une de ces conditions est satisfaite :
	* ce fichier .h déclare une classe qui hérite d’une autre classe
	* il nécessite la définition de constantes (au sens #define) ou de structures définies dans un autre .h.
	* la classe qu’il déclare comporte comme attribut un objet d’autre classe (et non pas un pointeur ou une référence vers celui-ci)
* Méthodes const dès que l'on peut
* Pas de code mort
* Ne pas commit sur master
* Lower camel case : voiciUnNomDeVariable
* Une tâche = une branche. Nom de la branche : taskXXX (XXX = num branche sur le backlog)
* Pas de duplication de code

## Commandes
* git status //Connaître l'état de votre repertoire
* git pull //Récupérer les datas distante
* git commit -m "Indiquer ce que votre commit fait" //Exemple "Ajoute un bouton pour créer une note"
* git branch //Connaitre votre branche courante
* git checkout #NomBrancheExistante //Se placer sur une branche qui existe déjà
* git checkout -b #NomNouvelleBrance //Créer et aller sur une nouvelle branche
* git push origin #NomDeVotreBranche //Pousse vos data sur le git
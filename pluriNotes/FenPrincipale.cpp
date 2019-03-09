#include "FenPrincipale.h"
#include <iostream>
#include <QListWidget>
#include <QString>
#include <QtWidgets>
#include <QAbstractListModel>
#include <QStringListModel>
#include <Note.h>
#include <Enumeration.h>
#include <Application.h>
#include <QLayoutItem>
#include "XMLManager.h"
#include <TimeLibrary.h>
#include <QMessageBox>
#include <stdlib.h>
#include <Relation.h>
#include <RelationListObserver.h>
#include <QTableWidget>
#include <QTableWidgetItem>
#include <QWidgetItem>

/**
 * @brief FenPrincipale::FenPrincipale Creation de la fenetre, initialisations graphiques + chargements des elements necessaires
 * @param parent
 */
FenPrincipale::FenPrincipale(QWidget *parent) : QMainWindow(parent),ui(new Ui::FenPrincipale){
    ui->setupUi(this);
    //a optimiser par un parcours des element des enums associes (typeNote et priority)
    ui->typeNoteComboBox->addItems(QStringList()<<"Article"<<"Tache"<<"Audio"<<"Video"<<"Image");
    ui->priorityComboBox->addItems(QStringList()<<"Basse"<<"Moyenne"<<"Haute"<<"Indefinie");
    ui->taskStatusComboBox->addItems(QStringList()<<"A faire"<<"En cours"<<"Terminee"<<"Indefinie");
    ui->descriptionText->clear();

    ui->noteResumGroup->setHidden(true);
    ui->DescriptionNoteArea->setHidden(true);
    ui->relationResumGroup->setHidden(true);
    ui->groupBoxCouple->setHidden(true);
    ui->idRelation->setHidden(true);

    NoteListManager* noteLManager = new NoteListManager(this);
    RelationListManager* relationLManager = new RelationListManager(this);

    app = new Application(noteLManager, relationLManager);

    connect(ui->actionQuitter, SIGNAL(triggered()), this, SLOT(close()));
    connect(ui->actionSauvegarder, SIGNAL(triggered()), this, SLOT(saveApplication()));
    connect(ui->actionCharger_XML, SIGNAL(triggered()), this, SLOT(loadData()));
    connect(qApp,SIGNAL(aboutToQuit()),this,SLOT(leftApplication()));

    ui->trashInformationNote->setHidden(true);
    database = &(XMLManager::getInstance());
    load();
    displayParameterData();

    ui->arborescenceNote->setHidden(true);
    ui->showArborescenceButton->setHidden(true);
}

FenPrincipale* FenPrincipale::f_instance = NULL;

/**
 * @brief FenPrincipale::getInstance singleton de la classe
 * @return
 */
FenPrincipale* FenPrincipale::getInstance(){
    if(!f_instance){
        f_instance = new FenPrincipale();
    }
    return f_instance;
}

/**
 * @brief FenPrincipale::display affiche la fenêtre
 */
void FenPrincipale::display(){
    show();
}

/**
 * @brief FenPrincipale::~FenPrincipale Destructeur de la fenetre principale
 */
FenPrincipale::~FenPrincipale(){
    delete ui;
}

/**
 * @brief FenPrincipale::leftApplication save? and left the application
 */
void FenPrincipale::leftApplication(){
    if(app->getNoteListManager()->existTrashedNote()){
        if(app->getParameters()->getRemoveAuto()){
            on_buttonViderCorbeille_clicked();
        } else{
            QMessageBox::StandardButton reply;
            reply = QMessageBox::question(this, "Avant de quitter...", "Vider la corbeille?",QMessageBox::Yes|QMessageBox::No);

            if (reply == QMessageBox::Yes){
                on_buttonViderCorbeille_clicked();
            }
        }
    }
    if(app->getParameters()->getSaveAuto()){
        saveApplication();
    } else {
        QMessageBox::StandardButton reply;
        reply = QMessageBox::question(this, "Avant de quitter...", "Sauvegarder ?",QMessageBox::Yes|QMessageBox::No);

        if (reply == QMessageBox::Yes){
            saveApplication();
        }
    }
}

/**
 * @brief FenPrincipale::saveApplication lance la sauvegarde de l'application
 */
void FenPrincipale::saveApplication(){
    try{
        database->save(*(app));
        QMessageBox::information(this,tr("Information"),tr("Sauvegarde effectuee"));
        database->saveContext(ui);
    } catch(char const* error){
        QMessageBox::information(this,tr("Information"), QString::fromStdString(error));
    } catch(std::string* error){
        QMessageBox::information(this,tr("Information"), QString::fromStdString(*error));
    }
}

/**
 * @brief FenPrincipale::loadApplicationXML charge les donnees stockes en XML
 */
void FenPrincipale::loadData(){
    try{
        database->load(*(app));
        QMessageBox::information(this,tr("Information"),tr("Chargement des notes...OK"));
        database->loadContext(this);
   } catch(std::string& error){
        QMessageBox::information(this,tr("Information"), QString::fromStdString(error));
   } catch(std::string* error){
        QMessageBox::information(this,tr("Information"), QString::fromStdString(*error));
   }
}

/*********
 * PREMIER ONGLET : GESTION NOTE
 *
 *
 * *******/

/**
 * @brief FenPrincipale::updateButtonsNaviguationVersions gère les boutons permettant la navigation des versions
 * @param noVersion
 * @param sizeVersions
 */
void FenPrincipale::updateButtonsNaviguationVersions(int noVersion, int sizeVersions){
    ui->buttonPreviousVersion->setDisabled(noVersion == 0);
    ui->buttonNextVersion->setDisabled(noVersion == sizeVersions-1);
    ui->buttonRestaure->setDisabled(noVersion == sizeVersions-1); // empêche restauration de la dernière version
    ui->nbVersionActual->setText(QString::number(noVersion));
}

/**
 * @brief FenPrincipale::displayPreviousVersion utilise iterator pour afficher la version precedente de la note affichee
 */
void FenPrincipale::on_buttonPreviousVersion_clicked(){
    int id = ui->idNoteLabel->text().toInt();
    Note* currentNote = app->getNote(id);
    currentNote->getIterator()->previous();
    displayAbstractNote(currentNote);
    updateButtonsNaviguationVersions(currentNote->getIterator()->current().getNoVersion(), currentNote->getNombreDeVersion());
}

/**
 * @brief FenPrincipale::displayNextVersion utilise iterator pour afficher la version suivante de la note affichee
 */
void FenPrincipale::on_buttonNextVersion_clicked(){
    int id = ui->idNoteLabel->text().toInt();
    Note* currentNote = app->getNote(id);
    currentNote->getIterator()->next();
    displayAbstractNote(currentNote);
    updateButtonsNaviguationVersions(currentNote->getIterator()->current().getNoVersion(), currentNote->getNombreDeVersion());
}

/**
 * @brief FenPrincipale::clearChampsText Fonction de nettoyage des champs textes pour la premiere vue
 */
void FenPrincipale::clearChampsText(){
    ui->dateTimeEdit->setText("");
    ui->titleNoteText->setText("");
    ui->titleVersion->setText("");
    ui->descriptionText->setPlainText("");
}



/**
 * @brief FenPrincipale::displayDateDerniereModif permet d'afficher ou cacher le label concernant la dernière modif
 * @param b
 */
void FenPrincipale::displayDateDerniereModif(bool b){
    ui->dateModifLabel->setVisible(b);
    ui->dateTimeEdit->setVisible(b);
}

/**
 * @brief FenPrincipale::displayMainAndRightLayout permet d'afficher ou cacher la vue du milieu (Consulation de note + arborescences)
 * @param b
 */
void FenPrincipale::displayMainAndRightLayout(bool b){
    // on cache le layout de la creation/consultation de note
    ui->noteResumGroup->setVisible(b);
    ui->DescriptionNoteArea->setVisible(b);

    // on cache le layout affichant les arborescences
    ui->arborescenceNote->setVisible(b);
}

/**
 * @brief FenPrincipale::on_deleteNoteBut_clicked permet de gerer la suppression/archivage d'une note
 */
void FenPrincipale::on_deleteNoteBut_clicked(){
    int idNote = ui->idNoteLabel->text().toInt();
    QString messageBox;

    // si est referencee
    if(app->getRelationListManager()->isReferenced(idNote)){
        app->getNoteListManager()->archivedNote(idNote);
        messageBox = "Note archivee (reference)";
    } else {
        // suppression des couples associees
        app->getRelationListManager()->removeCouple(idNote);

        // mise a la corbeille de la note
        app->getNoteListManager()->throwInTrash(idNote);

        messageBox = "La note a ete mise en corbeille";
        std::vector<Note>* notes = app->getNoteListManager()->getNoteList();
        for(unsigned int i = 0 ; i < notes->size() ; i++){
            if(notes->at(i).getStatus() == Enumeration::ARCHIVED){
                int id = notes->at(i).getId();
                if(!app->getRelationListManager()->isReferenced(id)){
                    int reponse = QMessageBox::question(this, "Question", "Note d'id " + QString::number(id) + " n'est plus referencee. Supprimer ?", QMessageBox ::Yes | QMessageBox::No);
                    if (reponse == QMessageBox::Yes){
                        app->getNoteListManager()->deleteNote(id);
                    }
                }
            }
        }
    }

    displayMainAndRightLayout(false);
    QMessageBox::information(this,tr("Information"),messageBox);
}

/**
 * @brief FenPrincipale::on_createNoteButton_clicked Fonction appelee lors d'une demande de creation de note. Gestion des affichages lies.
 */
void FenPrincipale::on_createNoteButton_clicked(){
    ui->deleteNoteBut->setHidden(true);
    ui->arborescenceNote->setHidden(true);
    ui->showArborescenceButton->setHidden(true);
    displayNumVersion(false);

    ui->noteResumGroup->setHidden(false);
    ui->typeNoteComboBox->setEnabled(true);
    ui->createOrEditBut->setEnabled(true);
    ui->createOrEditBut->setText("Ajouter");

    // afficher milieu de l'ecran
    ui->noteResumGroup->setVisible(true);
    ui->DescriptionNoteArea->setVisible(true);

    ui->noteResumGroup->setTitle("Mode creation");
    ui->idNoteLabel->setHidden(true);
    ui->label_21->setHidden(true);

    displayDateDerniereModif(false);
    clearChampsText();

    ui->typeNoteComboBox->setDisabled(false);
}

/**
 * @brief FenPrincipale::editNote s'occupe de creer une nouvelle version pour l'edition d'une note
 */
void FenPrincipale::editNote(){
    int noteId = ui->idNoteLabel->text().toInt();
    Note* currentNote = app->getNote(noteId);
    app->getNoteListManager()->notify();

    if(QString::fromStdString(currentNote->getTitle()) != ui->titleNoteText->text()){
        currentNote->setTitle(ui->titleNoteText->text().toStdString());
    }

    Enumeration::typeNote typeNote = currentNote->getVersionManager().getTypeNote();

    if(typeNote == Enumeration::ARTICLE){
        Article* currentArticleVersion = static_cast<Article*>(currentNote->getCurrentVersion().getaNote());
        //si le titre a change, ou la description
        if(QString::fromStdString(currentArticleVersion->getText()) != ui->descriptionText->toPlainText() || QString::fromStdString(currentArticleVersion->getTitle()) != ui->titleVersion->text()){
            Article* newArticleVersion = static_cast<Article*>(nf.getNote(Enumeration::ARTICLE, ui->titleVersion->text().toStdString()));
            newArticleVersion->setText(ui->descriptionText->toPlainText().toStdString());
            currentNote->addVersion(newArticleVersion);
        }
    }
    else if(typeNote == Enumeration::TASK){
        Task* currentTaskVersion = static_cast<Task*>(currentNote->getCurrentVersion().getaNote());
        //si le titre a change, ou la description, ou la priorite, ou la deadline /!\ A ameliorer (condition a rallonge...)
        if(QString::fromStdString(currentTaskVersion->getAction()) != ui->descriptionText->toPlainText() || QString::fromStdString(currentTaskVersion->getTitle()) != ui->titleVersion->text() || currentTaskVersion->getPriority() != EnumerationLibrary::qstringToPriority(ui->priorityComboBox->currentText()) || currentTaskVersion->getDeadline() != (time_t)atoll(ui->dateTimeEditDeadline->date().toString().toStdString().c_str())){
            Task* newTaskVersion = static_cast<Task*>(nf.getNote(Enumeration::TASK, ui->titleVersion->text().toStdString()));
            newTaskVersion->setAction(ui->descriptionText->toPlainText().toStdString());

            newTaskVersion->setPriority(EnumerationLibrary::qstringToPriority(ui->priorityComboBox->currentText()));

            newTaskVersion->setStatus(EnumerationLibrary::qstringToTaskStatus(ui->taskStatusComboBox->currentText()));

            QString deadLineString = ui->dateTimeEditDeadline->date().toString();
            newTaskVersion->setDeadline((time_t)atoll(deadLineString.toStdString().c_str()));
            currentNote->addVersion(newTaskVersion);
        }
    }else if(typeNote == Enumeration::IMAGE || typeNote == Enumeration::AUDIO || typeNote == Enumeration::VIDEO){
        Media* currentMediaVersion = static_cast<Media*>(currentNote->getCurrentVersion().getaNote());

        // si le titre a change, ou la description, ou l'url
        if(QString::fromStdString(currentMediaVersion->getTitle()) != ui->titleVersion->text() || QString::fromStdString(currentMediaVersion->getDescription()) != ui->descriptionText->toPlainText() || QString::fromStdString(currentMediaVersion->getUrlMedia()) != ui->urlText->text()){
            Media* newVersion = static_cast<Media*>(nf.getNote(typeNote, ui->titleVersion->text().toStdString()));
            newVersion->setDescription(ui->descriptionText->toPlainText().toStdString());
            newVersion->setUrlMedia(ui->urlText->text().toStdString());
            currentNote->addVersion(newVersion);
        }
    }
    //MAJ de la date de modification
    updateButtonsNaviguationVersions(currentNote->getIterator()->current().getNoVersion(), currentNote->getNombreDeVersion());
    ui->dateTimeEdit->setText(QString::fromStdString(TimeLibrary::getString(currentNote->updateLastModification(), true)));
    QMessageBox::information(this,tr("Information"),tr("Note mise a jour"));
}

/**
 * @brief FenPrincipale::createNote Fonction gerant la creation d'une note. Gestion des differents types de notes et de leurs specificites. Ajout de celle-ci dans le manager associe.
 */
void FenPrincipale::createNote(){
    // si pas de titre de note renseignee
    if(ui->titleNoteText->text().toStdString() == ""){
        QMessageBox::information(this,tr("Information"),tr("Vous devez renseigner le titre de la note"));
        return;
    }

    Note newNote(Enumeration::ACTIVE, "");
    ui->typeNoteComboBox->setEnabled(true);

    QString typeNote = ui->typeNoteComboBox->currentText();
    if(typeNote == "Article"){
        AbstractNote* abstractArticle = nf.getNote(Enumeration::ARTICLE, ui->titleNoteText->text().toStdString());
        Article* newArticle = static_cast<Article*>(abstractArticle);
        newArticle->setTitle(ui->titleVersion->text().toStdString());
        newArticle->setText(ui->descriptionText->toPlainText().toStdString());
        newNote.addVersion(newArticle);
    } else if(typeNote == "Tache"){
        ui->taskStatusComboBox->hide();
        ui->taskStatusLabel->hide();
        AbstractNote* abstractTask = nf.getNote(Enumeration::TASK, ui->titleNoteText->text().toStdString());
        Task* newTask = static_cast<Task*>(abstractTask);
        newTask->setAction(ui->descriptionText->toPlainText().toStdString());
        newTask->setPriority(EnumerationLibrary::qstringToPriority(ui->priorityComboBox->currentText()));
        newTask->setTitle(ui->titleVersion->text().toStdString());
        QString deadLineString = ui->dateTimeEditDeadline->date().toString();

        time_t tim;
        time(&tim);

        newTask->setDeadline(tim); // todo récupérer la date choisie par l'utilisateur
        //newTask->setDeadline((time_t)atoll(deadLineString.toStdString().c_str()));
        newTask->setStatus(Enumeration::DOING);
        newNote.addVersion(newTask);
    } else if(typeNote == "Image" || typeNote == "Audio" || typeNote == "Video"){
        AbstractNote* abstractArticle = nf.getNote(EnumerationLibrary::qstringToTypeNote(typeNote), ui->titleNoteText->text().toStdString());
        Media* newMedia = static_cast<Media*>(abstractArticle);

        newMedia->setDescription(ui->descriptionText->toPlainText().toStdString());
        newMedia->setUrlMedia(ui->urlText->text().toStdString());
        newMedia->setTitle(ui->titleVersion->text().toStdString());
        newNote.addVersion(newMedia);
    } else {
        cout<<"Probleme not a type ?"<<endl;
    }

    newNote.setTitle(ui->titleNoteText->text().toStdString());
    app->getNoteListManager()->addNote(newNote);

    clearChampsText();
}

/**
 * @brief FenPrincipale::searchAndAddReferences
 * @param note
 */
void FenPrincipale::searchAndAddReferences(Note* note){
    QRegularExpression regexPattern("\\\\ref\\{[0-9]+\\}");

    // searchText: concatenation de tout le texte (titre, description, etc) ce qui doit être verifie
    QString searchText = note->getAllQString();

    if(searchText.contains(regexPattern)) {
        int indiceRelation = app->getRelationListManager()->getIdRelationStartedByIdNote(note->getId());

        // s'il y a deja une relation existance pour cette note, on la supprime
        if(indiceRelation != -1) {
            app->getRelationListManager()->getRelationList()->erase(app->getRelationListManager()->getRelationList()->begin()+indiceRelation);
        }
        app->getRelationListManager()->notify();

        Relation newReference;
        newReference.setIsReference(true);

        QRegularExpressionMatchIterator iteratorMatchs = regexPattern.globalMatch(searchText);

        int nbCoupleAdd = 0;
        int nbMatchs = 0;

        // permet de gerer les tentatives de referencement sur des id non existants
        // des qu'un couple correct est ajoute : on cree la reference,
        // si nbCoupleAdd != taille de la QStringList -> on avertit dans l'UI
        while (iteratorMatchs.hasNext()) {
            nbMatchs++;

            QRegularExpressionMatch match = iteratorMatchs.next();
            QString str = match.captured(0);
            QString subString = str.mid(5, str.size()-6);

            // recuperation de la note referencee
            int idNoteBReference = subString.toInt(); // bug ici : substring est vide => converti en 0 => creer une relation
            Note *noteB = app->getNoteListManager()->getNote(idNoteBReference);

            // on verifie que cette note existe
            if(noteB != NULL) {
                // on creer le nouveau couple
                Couple newCouple(note->getId(), noteB->getId());
                // et on l'ajoute a la liste de couples de notre nouvelle relation
                newReference.addCouple(newCouple);
                nbCoupleAdd++;
            }
        }

        if(nbCoupleAdd > 0){
            newReference.setTitre("Relation "+std::to_string(Relation::nbRelation));
            app->getRelationListManager()->addRelation(newReference);
            QMessageBox::information (this, "Confirmation", "Reference(s) ajoutee(s) !");
            if(nbCoupleAdd != nbMatchs){
                QMessageBox::information (this, "Avertissement", "Attention, certaine(s) tentative(s) de referencement ont echouee(s), ID(s) note(s) non existant(s)");
            }
        }
    }
}

/**
 * @brief FenPrincipale::on_createOrEditBut_clicked Fonction gerant la gestion d'une note et appelant les bonnes methodes : edition ou creation.
 */
void FenPrincipale::on_createOrEditBut_clicked(){
    //Si le texte est "Ajouter la note" : on est en mode creation :
    int idNote = -1;
    if(ui->createOrEditBut->text() == "Ajouter"){
        createNote();
        idNote = Note::nbNote-1;
        searchAndAddReferences(app->getNoteListManager()->getNote(idNote));
    }else if(ui->createOrEditBut->text() == "Modifier"){
        ui->createOrEditBut->text()== "Editer la note";
        ui->activeNoteList->setItemSelected(ui->activeNoteList->item(ui->activeNoteList->size().height()), true);
        editNote();
        idNote = ui->idNoteLabel->text().toInt();
        searchAndAddReferences(app->getNoteListManager()->getNote(idNote));
    } else {
        std::cout<<"Bouton sans comportement (label du bouton incorrect?)"<<std::endl;
        return;
    }
}

/**
 * @brief FenPrincipale::on_typeNoteComboBox_currentIndexChanged Charge la vue du centre avec les informations sur la note. Permet le passage a l'edition de celle-ci.
 * @param arg1
 */
void FenPrincipale::on_typeNoteComboBox_currentIndexChanged(const QString &)
{
    ui->taskDetailGroupBox->hide();
    ui->urlLabel->hide();
    ui->urlText->hide();

    if(ui->typeNoteComboBox->currentText()=="Article"){
        ui->descriptionLabel->text()="Corps de l'article";
        ui->titleVersionLabel->text()="Titre de l'article";
    }

    else if(ui->typeNoteComboBox->currentText()=="Tache"){
        ui->taskDetailGroupBox->setHidden(false);
        ui->descriptionLabel->text()="Description";
        ui->titleVersionLabel->text()="Titre de la tache";
    }
    else{
        ui->urlLabel->setHidden(false);
        ui->urlText->setHidden(false);
        ui->descriptionLabel->text()="Description du media";
        ui->titleVersionLabel->text()="Titre du media";
    }
}

/**
 * @brief FenPrincipale::getQStringId
 * @param itemData
 * @return QString correspondant a l'id de la note selectionnee
 */
QString FenPrincipale::getQStringId(QVariant itemData){
    return itemData.toString();
}

/**
 * @brief FenPrincipale::displayNumVersion Gere l'affichage des numeros de versions d'une note.
 * @param b
 */
void FenPrincipale::displayNumVersion(bool b){
    ui->buttonNextVersion->setVisible(b);
    ui->buttonPreviousVersion->setVisible(b);
    ui->nbVersionActual->setVisible(b);
    ui->nbVersionLabel->setVisible(b);
    ui->buttonRestaure->setVisible(b);
}

/**
 * @brief FenPrincipale::displayAbstractNote Gere l'affichage des notes de maniere generique, s'adaptant aux differents types de notes.
 * @param currentNote
 */
void FenPrincipale::displayAbstractNote(Note* currentNote){
    Enumeration::typeNote typeNote = currentNote->getTypeNote();
    AbstractNote* currentAbstractNote = currentNote->getIterator()->current().getaNote();
    if(typeNote == Enumeration::ARTICLE){
        Article* currentArticleVersion = static_cast<Article*>(currentAbstractNote);

        ui->titleNoteText->setText(QString::fromStdString(app->getNoteListManager()->getNote(currentNote->getId())->getTitle()));
        ui->titleVersion->setText(QString::fromStdString(currentArticleVersion->getTitle()));
        ui->descriptionText->setPlainText(QString::fromStdString(currentArticleVersion->getText()));
    } else if(typeNote == Enumeration::IMAGE || typeNote == Enumeration::AUDIO || typeNote == Enumeration::VIDEO){
        Media* currentMediaVersion = static_cast<Media*>(currentAbstractNote);

        ui->titleVersion->setText(QString::fromStdString(currentMediaVersion->getTitle()));
        ui->descriptionText->setPlainText(QString::fromStdString(currentMediaVersion->getDescription()));
        ui->urlText->setText(QString::fromStdString(currentMediaVersion->getUrlMedia()));
    } else if(typeNote == Enumeration::TASK){
        Task* currentTaskVersion = static_cast<Task*>(currentAbstractNote);

        ui->taskDetailGroupBox->setHidden(false);
        ui->taskDetailGroupBox->setEnabled(true);

        ui->titleNoteText->setText(QString::fromStdString(app->getNoteListManager()->getNote(currentNote->getId())->getTitle()));
        ui->titleVersion->setText(QString::fromStdString(currentTaskVersion->getTitle()));
        ui->descriptionText->setPlainText(QString::fromStdString(currentTaskVersion->getAction()));
        //ui->dateTimeEditDeadline->setDateTime();
        ui->taskStatusComboBox->setCurrentText(EnumerationLibrary::taskStatusToQstring(currentTaskVersion->getStatus()));
        ui->priorityComboBox->setCurrentText(EnumerationLibrary::priorityToQString(currentTaskVersion->getPriority()));
    } else {
        cout<<"Erreur ? typeNote non reconnu"<<endl;
    }
}

/**
 * @brief FenPrincipale::displayDescendingNotesAuxiliaire Fonction recursive. Recupère les fils de l'id mis en argument et les ajoute
 * dans l'item du QTreeWidget parent
 * @param parent
 * @param listeDescendants (tableau de vecteur contenant les descendants directs de chaque Note ; cle = id de la note)
 * @param id
 */
void FenPrincipale::displayDescendingNotesAuxiliaire(QTreeWidgetItem* parent, vector<int>* listeDescendants, int id){
    // Pour chacun des descendants de la note ayant l'id ID
    for(unsigned int i = 0 ; i < listeDescendants[id].size() ; i++){
        // ajout de la Note
        Note* note = app->getNote(listeDescendants[id].at(i));
        QTreeWidgetItem* item = new QTreeWidgetItem(parent);
        item->setText(0, QString::fromStdString(note->getTitle()));
        item->setData(0, Qt::UserRole, note->getId());
        // ajout des childrens de cette Note
        displayDescendingNotesAuxiliaire(item, listeDescendants, note->getId());
    }
}

/**
 * @brief FenPrincipale::displayDescendingNotes ajoute les Notes a la treeview contenant les descendants de la note consultee
 */
void FenPrincipale::displayDescendingNotes(){
    ui->notesDescendantes->clear();
    // recuperation de la note consultee
    Note* noteActual = app->getNote(ui->idNoteLabel->text().toInt());

    // ajout de la note actuelle dans l'arbre
    QTreeWidgetItem* parent = new QTreeWidgetItem(ui->notesDescendantes);
    parent->setText(0,QString::fromStdString(noteActual->getTitle()));
    parent->setData(0, Qt::UserRole, noteActual->getId());

    vector<int>* listeDescendants = app->getRelationListManager()->listeDescendantsParNote();
    // appel de la fonction recursive ajoutant les noeuds enfants
    displayDescendingNotesAuxiliaire(parent, listeDescendants, noteActual->getId());
}

/**
 * @brief FenPrincipale::displayAscendingNotesAuxiliaire Fonction recursive. Recupère les fils de l'id mis en argument et les ajoute
 * dans l'item du QTreeWidget parent
 * @param parent
 * @param listeDescendants (tableau de vecteur contenant les ascendants directs de chaque Note ; cle = id de la note)
 * @param id
 */
vector<QTreeWidgetItem*> FenPrincipale::displayAscendingNotesAuxiliaire(vector<int>* peresDirect, int id){
    vector<QTreeWidgetItem*> peresItems;
    // si n'est pas une racine, on recupère ses ancètres
    if(peresDirect[id].size() != 0){
        // Pour tous les ascendants directs
        vector<QTreeWidgetItem> ascendants;
        for(unsigned int i = 0 ; i < peresDirect[id].size() ; i++){
            Note *note = app->getNote(peresDirect[id].at(i));

            // recuperation des items "grandPeres"
            vector<QTreeWidgetItem*> grandPeresItem = displayAscendingNotesAuxiliaire(peresDirect, note->getId());
            // si est une racine, on l'ajoute en premier de la liste
            if(grandPeresItem.size() == 0){
                // ajout des items peres
                QTreeWidgetItem* pere = new QTreeWidgetItem(ui->notesAscendantes);
                pere->setText(0, QString::fromStdString(note->getTitle()));
                pere->setData(0, Qt::UserRole, note->getId());
                peresItems.push_back(pere);
            } else {
                // sinon on parcours les grands pères
                for(unsigned int j = 0 ; j < grandPeresItem.size() ; j++){
                    // ajout des items peres
                    QTreeWidgetItem* pere = new QTreeWidgetItem(grandPeresItem.at(j));
                    pere->setText(0, QString::fromStdString(note->getTitle()));
                    pere->setData(0, Qt::UserRole, note->getId());
                    peresItems.push_back(pere);
                }
            }
        }
    }
    return peresItems;
}

/**
 * @brief FenPrincipale::displayAscendingNotes ajoute les Notes a la treeview contenant les ascendants de la note consultee
 */
void FenPrincipale::displayAscendingNotes(){
    ui->notesAscendantes->clear();
    // recuperation de l'id de la note consultee
    int idNoteActual = ui->idNoteLabel->text().toInt();
    Note* noteActual = app->getNote(idNoteActual);

    vector<int>* listeAscendants = app->getRelationListManager()->listeAscendantsParNote();
    // appel de la fonction recursive ajoutant les noeuds parents

    vector<QTreeWidgetItem*> peresDirectsItems = displayAscendingNotesAuxiliaire(listeAscendants, idNoteActual);

    for(unsigned int i = 0 ; i < peresDirectsItems.size() ; i++){
        QTreeWidgetItem* itemActual = new QTreeWidgetItem(peresDirectsItems.at(i));
        itemActual->setText(0, QString::fromStdString(noteActual->getTitle()));
        itemActual->setData(0, Qt::UserRole, noteActual->getId());
    }
}

/**
 * @brief FenPrincipale::genericItemDoubleClickedListFirstView Gere de maniere generique le clic sur les ListWidgetItem dans la 1ere vue.
 * @param item
 */
void FenPrincipale::genericItemDoubleClickedListFirstView(QString noteIdString){
    ui->arborescenceNote->setVisible(true);
    ui->showArborescenceButton->setText(QString::fromStdString("--"));
    ui->showArborescenceButton->setVisible(true);
    // afficher milieu de l'ecran
    ui->noteResumGroup->setVisible(true);
    ui->DescriptionNoteArea->setVisible(true);

    displayDateDerniereModif(true);
    displayNumVersion(true);

    int noteId = noteIdString.toInt();
    ui->idNoteLabel->setText(noteIdString);

    ui->createOrEditBut->setText("Modifier");
    ui->createOrEditBut->setEnabled(true);
    ui->deleteNoteBut->setEnabled(true);

    Note* currentNote = app->getNote(noteId);
    int noVersion = currentNote->getIterator()->current().getNoVersion();

    updateButtonsNaviguationVersions(noVersion, currentNote->getNombreDeVersion());

    // On change le type affichee dans la combo box
    ui->typeNoteComboBox->setCurrentText(currentNote->getNameTypeNote());
    ui->typeNoteComboBox->setDisabled(true);

    ui->noteResumGroup->setTitle("Mode edition");
    ui->idNoteLabel->setHidden(false);
    ui->label_21->setHidden(false);

    ui->typeNoteComboBox->setEnabled(false);

    ui->dateTimeEdit->setText(QString::fromStdString(TimeLibrary::getString(currentNote->getLastModification(),true)));
    ui->titleNoteText->setText(QString::fromStdString(currentNote->getTitle()));
    ui->dateTimeEdit->setText(QString::fromStdString(TimeLibrary::getString(currentNote->getLastModification(),true)));

    displayAbstractNote(currentNote);
    displayDescendingNotes();
    displayAscendingNotes();

    ui->deleteNoteBut->setVisible(true);
    ui->buttonRestaure->setEnabled(false);
    enableModificationNote(true);
}

/**
 * @brief FenPrincipale::on_activeNoteList_itemDoubleClicked
 * @param item
 */
void FenPrincipale::on_activeNoteList_itemDoubleClicked(QListWidgetItem *item){
    QString noteIdString = getQStringId(item->data(Qt::UserRole));
    genericItemDoubleClickedListFirstView(noteIdString);
}

/**
 * @brief FenPrincipale::enableModificationNote permet l'activation/desactivation des elements permettant la suppression d'une note
 * @param b
 */
void FenPrincipale::enableModificationNote(bool b){
    ui->descriptionText->setEnabled(b);
    ui->titleVersion->setEnabled(b);
    ui->titleNoteText->setEnabled(b);
    ui->createOrEditBut->setEnabled(b);
    ui->deleteNoteBut->setEnabled(b);
    ui->priorityComboBox->setEnabled(b);
    ui->dateTimeEditDeadline->setEnabled(b);
    ui->deadlineLabel->setEnabled(b);
    ui->taskStatusLabel->setEnabled(b);
    ui->taskDetailGroupBox->setEnabled(b);
    ui->taskStatusComboBox->setEnabled(b);
}

/**
 * @brief FenPrincipale::on_archivedNoteList_itemDoubleClicked
 * @param item
 */
void FenPrincipale::on_archivedNoteList_itemDoubleClicked(QListWidgetItem *item){
    QString noteIdString = getQStringId(item->data(Qt::UserRole));
    genericItemDoubleClickedListFirstView(noteIdString);
    ui->buttonRestaure->setEnabled(true);

    enableModificationNote(false);
}

/**
 * @brief FenPrincipale::on_taskList_itemDoubleClicked
 * @param item
 * @param column
 */
void FenPrincipale::on_taskList_itemDoubleClicked(QTreeWidgetItem *item, int){
    //correction du +1 pour compenser le forcing (cf NoteListObserver.cpp)
    QString noteIdString = getQStringId(item->data(1,Qt::UserRole).toInt()-1);
    if(noteIdString != QString::fromStdString("-1")){
        genericItemDoubleClickedListFirstView(noteIdString);
    }
}

/**
 * @brief FenPrincipale::on_notesDescendantes_itemDoubleClicked
 * @param item
 * @param column
 */
void FenPrincipale::on_notesDescendantes_itemDoubleClicked(QTreeWidgetItem *item, int){
    QString noteIdString = getQStringId(item->data(0, Qt::UserRole));
    genericItemDoubleClickedListFirstView(noteIdString);
}

/**
 * @brief FenPrincipale::on_notesAscendantes_itemDoubleClicked
 * @param item
 * @param column
 */
void FenPrincipale::on_notesAscendantes_itemDoubleClicked(QTreeWidgetItem *item, int)
{
    QString noteIdString = getQStringId(item->data(0, Qt::UserRole));
    genericItemDoubleClickedListFirstView(noteIdString);
}


/**
 * @brief FenPrincipale::on_showArborescenceButton_clicked Permet de rendre retractable l'arborescence de la premiere vue
 */
void FenPrincipale::on_showArborescenceButton_clicked()
{
    if(ui->showArborescenceButton->text()==QString::fromStdString("--")){
        ui->arborescenceNote->hide();
        ui->showArborescenceButton->setText(QString::fromStdString("+"));
    }else{
        ui->arborescenceNote->setHidden(false);
        ui->showArborescenceButton->setText(QString::fromStdString("--"));
    }
}

/**
 * @brief FenPrincipale::getNoteFromLabel Recupere une note depuis l'id affiche dans l'UI.
 * @return un pointeur sur la note associee a l'id affiche
 */
Note* FenPrincipale::getNoteFromLabel(){
    QString noteIdString = ui->idNoteLabel->text();
    int noteId = noteIdString.toInt();
    return app->getNote(noteId);
}

/**
 * @brief FenPrincipale::on_buttonRestaure_clicked Gère la restauration d'une version de note : recuperation, actualisation de la version actuelle + affichage
 */
void FenPrincipale::on_buttonRestaure_clicked(){
    Note* n = getNoteFromLabel();
    if(n->getStatus() == Enumeration::ARCHIVED){
        // le bouton permet de restaurer la note
        app->getNoteListManager()->activeNote(n->getId());
        displayMainAndRightLayout(false);
        QMessageBox::information(this,tr("Information"),tr("La note a ete restaure"));
    } else{
        // le bouton permet de restaurer une version
        Version v = n->getCurrentVersion();
        n->addVersion(v.getaNote());
        n->getIterator()->goToEnd();
        displayAbstractNote(n);
        updateButtonsNaviguationVersions(n->getIterator()->current().getNoVersion(), n->getNombreDeVersion());
    }
}

/*********
 * DEUXIEME ONGLET : GESTION DES RELATIONS
 *
 *
 * *******/

/**
 * @brief FenPrincipale::clearChampsTextSecondView Fonction de nettoyage des champs textes (+liste couples) pour la seconde vue
 */
void FenPrincipale::clearChampsTextSecondView(){
    ui->titleRelationText->setText("");
    ui->TextDescriptionRelation->setPlainText("");
    ui->tableWidgetCouple->setRowCount(0);
    ui->tableWidgetCouple->clearContents();
    ui->labelNewCouple->setText("");
}

/**
 * @brief FenPrincipale::on_ButtonAddCouple_clicked Permet l'ajout d'un nouveau couple a une relation. Le label est optionnel.
 */
void FenPrincipale::on_ButtonAddCouple_clicked(){
    if(ui->ComboBoxNoteACouple->currentData(Qt::UserRole) == ui->ComboBoxNoteBCouple->currentData(Qt::UserRole)){
        QMessageBox::information(this,tr("Information"),tr("Vous devez renseigner deux notes differentes"));
        return;
    }
    int noteAId = ui->ComboBoxNoteACouple->currentData(Qt::UserRole).toInt();
    int noteBId = ui->ComboBoxNoteBCouple->currentData(Qt::UserRole).toInt();

    Note* noteA = app->getNoteListManager()->getNote(noteAId);
    Note* noteB = app->getNoteListManager()->getNote(noteBId);

    //on verifie maintenant que le couple ajoute n'est pas deja dans la liste des couples de la relation
    bool isFind = false;
    //si la relation est oriente : on peut ajouter un couple AB et un couple BA (considere comme differents)
    if(ui->radioButtonYesOrientation->isChecked()==true){
        for(int i=0; i<ui->tableWidgetCouple->rowCount(); i++){
            if(noteA->getId() == getQStringId(ui->tableWidgetCouple->item(i,1)->data(Qt::UserRole)).toInt() && noteB->getId() == getQStringId(ui->tableWidgetCouple->item(i,2)->data(Qt::UserRole)).toInt()){
                isFind = true;
                break;
            }
        }
    }else{
        //si la relation est non oriente, on considere que couple AB = couple BA donc restriction supplementaire
        for(int i=0; i<ui->tableWidgetCouple->rowCount(); i++){
            int idNoteGauche = getQStringId(ui->tableWidgetCouple->item(i,1)->data(Qt::UserRole)).toInt();
            int idNoteDroite = getQStringId(ui->tableWidgetCouple->item(i,2)->data(Qt::UserRole)).toInt();
            if((noteA->getId() == idNoteGauche || noteA->getId() == idNoteDroite) && (noteB->getId() == idNoteGauche || noteB->getId() == idNoteDroite)){
                isFind = true;
                break;
            }
        }
    }
    if(isFind){
        QMessageBox::information(this,tr("Information"),tr("Le nouveau couple est deja existant"));
        return;
    }else{
        int insertRow = ui->tableWidgetCouple->rowCount();
        ui->tableWidgetCouple->insertRow(insertRow);

        QTableWidgetItem* newNoteLabelItem = new QTableWidgetItem;
        QTableWidgetItem* newNoteNoteAItem = new QTableWidgetItem;
        QTableWidgetItem* newNoteNoteBItem = new QTableWidgetItem;

        newNoteLabelItem->setText(ui->labelNewCouple->text());
        newNoteNoteAItem->setText(QString::fromStdString(noteA->getTitle()));
        newNoteNoteAItem->setData(Qt::UserRole,noteA->getId());
        newNoteNoteBItem->setText(QString::fromStdString(noteB->getTitle()));
        newNoteNoteBItem->setData(Qt::UserRole,noteB->getId());

        //Empecher l'edition des notes A/notes B
        newNoteNoteAItem->setFlags(newNoteNoteAItem->flags() &  ~Qt::ItemIsEditable);
        newNoteNoteBItem->setFlags(newNoteNoteBItem->flags() & ~Qt::ItemIsEditable);

        ui->tableWidgetCouple->setItem(insertRow, 0, newNoteLabelItem);
        ui->tableWidgetCouple->setItem(insertRow, 1, newNoteNoteAItem);
        ui->tableWidgetCouple->setItem(insertRow, 2, newNoteNoteBItem);

        // ajout du bouton suppression
        QTableWidgetItem* deleteLabel = new QTableWidgetItem;
        deleteLabel->setText("Supprimer");
        deleteLabel->setBackgroundColor(Qt::red);
        deleteLabel->setTextAlignment(Qt::AlignCenter);
        ui->tableWidgetCouple->setItem(insertRow, 3, deleteLabel);
    }
}


/**
 * @brief FenPrincipale::on_createRelationButton_clicked
 * Evenement au clic sur le bouton creation relation
 *
 */
void FenPrincipale::on_createRelationButton_clicked()
{
        // afficher milieu de l'ecran
        ui->radioButtonYesOrientation->setChecked(true);
        ui->relationResumGroup->setVisible(true);
        ui->groupBoxCouple->setVisible(true);

        ui->relationResumGroup->setTitle("Mode creation");
        ui->ButtonAddOrEditRelation->setText("Creer la relation");
        clearChampsTextSecondView();
}

/**
 * @brief FenPrincipale:: Factorisation pour ajouter des couples a une relation (appelee par la creation et l'edition d'une relation)
 * @param r
 */

void FenPrincipale::addCouplesToRelation(Relation* r){
    for(int i = 0; i < ui->tableWidgetCouple->rowCount(); i++){
        string label = ui->tableWidgetCouple->item(i,0)->text().toStdString();
        int idNoteA = getQStringId(ui->tableWidgetCouple->item(i,1)->data(Qt::UserRole)).toInt();
        int idNoteB = getQStringId(ui->tableWidgetCouple->item(i,2)->data(Qt::UserRole)).toInt();

        Couple cpl(idNoteA, idNoteB, label);
        r->addCouple(cpl);
    }
}

/**
 * @brief FenPrincipale::createRelation Gere la creation de relation : titre, description, orientation et couples
 */
void FenPrincipale::createRelation(){
    // si pas de titre de relation renseigne
       if(ui->titleRelationText->text().toStdString() == ""){
           QMessageBox::information(this,tr("Information"),tr("Vous devez renseigner le titre de la relation"));
           return;
       }
       ui->groupBoxCouple->setVisible(true);

       Relation newRelation(ui->titleRelationText->text().toStdString(), ui->TextDescriptionRelation->toPlainText().toStdString());
       newRelation.setIsOriented(ui->radioButtonYesOrientation->isChecked());

       for(int i = 0; i < ui->tableWidgetCouple->rowCount(); i++){
           string label = ui->tableWidgetCouple->item(i,0)->text().toStdString();
           int idNoteA = getQStringId(ui->tableWidgetCouple->item(i,1)->data(Qt::UserRole)).toInt();
           int idNoteB = getQStringId(ui->tableWidgetCouple->item(i,2)->data(Qt::UserRole)).toInt();
           Couple cpl(idNoteA, idNoteB, label);
           newRelation.addCouple(cpl);
       }
       app->getRelationListManager()->addRelation(newRelation);
       QMessageBox::information(this,tr("Information"),tr("Relation ajoutee"));
       clearChampsTextSecondView();
}

void FenPrincipale::on_ButtonDeleteRelation_clicked(){
    app->getRelationListManager()->deleteRelation(ui->idRelation->text().toInt());
    QMessageBox::information(this,tr("Information"),tr("Relation supprimee"));
    ui->relationResumGroup->setVisible(false);
    ui->tableWidgetCouple->setVisible(false);
}

/**
 * @brief FenPrincipale::editRelation Gere l'edition d'une relation (changement de titre, de description, d'orientation et de la gestion des couples
 */
void FenPrincipale::editRelation(){
    Relation* currentRel = app->getRelationListManager()->getRelation(ui->idRelation->text().toInt());

    // si pas de titre de relation renseigne
    if(ui->titleRelationText->text().toStdString() == ""){
        QMessageBox::information(this,tr("Information"),tr("Vous devez renseigner le titre de la relation"));
        return;
    }
    ui->groupBoxCouple->setVisible(true);
    currentRel->cleanCoupleList();
    currentRel->setTitre(ui->titleRelationText->text().toStdString());
    currentRel->setDescription(ui->TextDescriptionRelation->toPlainText().toStdString());
    currentRel->setIsOriented(!ui->radioButtonNoOrientation->isChecked());
    addCouplesToRelation(currentRel);

    app->getRelationListManager()->notify();
    QMessageBox::information(this,tr("Information"),tr("Relation editee"));
    clearChampsTextSecondView();
}

/**
 * @brief FenPrincipale::on_ButtonAddOrEditRelation_clicked Gere le bouton de creation/edition d'une relation, et appelle les fonctions associees.
 */
void FenPrincipale::on_ButtonAddOrEditRelation_clicked()
{
    //Si le texte est "Creer la relation" : on est en mode creation :
    if(ui->ButtonAddOrEditRelation->text() == "Creer la relation"){
        createRelation();
    }else if(ui->ButtonAddOrEditRelation->text() == "Editer la relation"){
        editRelation();
    } else {
        std::cout<<"Bouton sans comportement (label du bouton incorrect?)"<<std::endl;
    }
}

void FenPrincipale::genericItemDoubleClickedListSecondView(Relation* rel){

    std::vector<Couple> vectorCouple = rel->getcoupleList();

    for(unsigned int i=0; i<vectorCouple.size(); i++){
        ui->tableWidgetCouple->insertRow(i);

        QTableWidgetItem* newNoteLabelItem = new QTableWidgetItem;
        QTableWidgetItem* newNoteNoteAItem = new QTableWidgetItem;
        QTableWidgetItem* newNoteNoteBItem = new QTableWidgetItem;

        newNoteLabelItem->setText(QString::fromStdString(vectorCouple[i].getLabel()));
        newNoteNoteAItem->setText(QString::fromStdString(app->getNote(vectorCouple[i].getNoteA())->getTitle()));
        newNoteNoteAItem->setData(Qt::UserRole,vectorCouple[i].getNoteA());
        newNoteNoteBItem->setText(QString::fromStdString(app->getNote(vectorCouple[i].getNoteB())->getTitle()));
        newNoteNoteBItem->setData(Qt::UserRole,vectorCouple[i].getNoteB());

        //Empecher l'edition des notes A/notes B
        newNoteNoteAItem->setFlags(newNoteNoteAItem->flags() &  ~Qt::ItemIsEditable);
        newNoteNoteBItem->setFlags(newNoteNoteBItem->flags() & ~Qt::ItemIsEditable);

        ui->tableWidgetCouple->setItem(i, 0, newNoteLabelItem);
        ui->tableWidgetCouple->setItem(i, 1, newNoteNoteAItem);
        ui->tableWidgetCouple->setItem(i, 2, newNoteNoteBItem);

        bool isNotReference = ui->relationTitleLabel->isVisible();
        if(isNotReference){
            QTableWidgetItem* deleteLabel = new QTableWidgetItem;
            deleteLabel->setText("Supprimer");
            deleteLabel->setBackgroundColor(Qt::red);
            deleteLabel->setTextAlignment(Qt::AlignCenter);
            ui->tableWidgetCouple->setItem(i, 3, deleteLabel);
        }
    }
}

/**
 * @brief FenPrincipale::on_relationSimpleList_itemDoubleClicked Charge la vue du centre avec les informations sur la relation. Permet le passage a l'edition.
 * @param item
 */
void FenPrincipale::on_relationSimpleList_itemDoubleClicked(QListWidgetItem *item){
    int relationId= getQStringId(item->data(Qt::UserRole)).toInt();
    Relation* rel = app->getRelationListManager()->getRelation(relationId);

    clearChampsTextSecondView();
    ui->relationResumGroup->setTitle(QString::fromStdString("Mode edition"));
    ui->ButtonAddOrEditRelation->setText(QString::fromStdString("Editer la relation"));
    ui->relationResumGroup->setVisible(true);
    ui->groupBoxCouple->setVisible(true);

    ui->titleRelationText->setText(QString::fromStdString(rel->getTitre()));
    ui->idRelation->setText(getQStringId(item->data(Qt::UserRole)));
    ui->TextDescriptionRelation->setPlainText(QString::fromStdString(rel->getDescription()));
    ui->radioButtonYesOrientation->setChecked(rel->getIsOriented());
    ui->radioButtonNoOrientation->setChecked(!rel->getIsOriented());

    genericItemDoubleClickedListSecondView(rel);
}

/**
 * @brief FenPrincipale::on_referenceList_itemDoubleClicked
 * @param item
 */
void FenPrincipale::on_referenceList_itemDoubleClicked(QListWidgetItem *item)
{
    ui->relationResumGroup->hide();
    ui->groupBoxCouple->setVisible(true);

    int relationId= getQStringId(item->data(Qt::UserRole)).toInt();
    Relation* rel = app->getRelationListManager()->getRelation(relationId);

    clearChampsTextSecondView();
    genericItemDoubleClickedListSecondView(rel);
}

/**
 * @brief FenPrincipale::on_tableWidgetCouple_itemClicked Au clic sur l'item "Suppression" : suppression du couple en question de la relation
 * @param item
 */
void FenPrincipale::on_tableWidgetCouple_itemClicked(QTableWidgetItem *item){
    if(item->column()==3){
        int reponse = QMessageBox::question(this, "Confirmation suppression", "Confirmer la suppression du couple ?", QMessageBox ::Yes | QMessageBox::No);
        if (reponse == QMessageBox::Yes){
            Relation* currentRel = app->getRelationListManager()->getRelation(ui->idRelation->text().toInt());
            int idNoteA = ui->tableWidgetCouple->item(item->row(), 1)->data(Qt::UserRole).toInt();
            int idNoteB = ui->tableWidgetCouple->item(item->row(), 2)->data(Qt::UserRole).toInt();
            currentRel->removeCoupleWithIds(idNoteA,idNoteB);
            ui->tableWidgetCouple->removeRow(item->row());
        }
    }
}

void FenPrincipale::on_infoButton_clicked()
{
    QString messageToShow = QString::fromStdString("Notes et references :\n"
                                                   "    - Pour ajouter une reference a une note, utiliser la syntaxe \\ref{idNoteAReferencer}\n"
                                                   "    - Tous les champs textes d'une note peuvent être utiliser le referencement\n"
                                                   "\n"
                                                   "Relations et couples :\n"
                                                   "    - Pour editer le label d'un couple, double cliquer sur la cellule label de la ligne en question dans la liste des couples\n"
                                                   "    - La suppression de tous les couples n'entraine pas la suppression de la relation\n"
                                                   "\n"
                                                   "Informations diverses :\n"
                                                   "    - Quand vous quittez l'application, l'etat de votre travail est sauvegarde et restitue a votre prochaine venue\n"
                                                   "    - Des raccourcis claviers sont a votre disposition (CTRL+s pour sauvegarder, CTRL+q pour quitter)\n"
                                                   "\n"
                                                   "Projet pluriNotes, tous droits reserves.");
    QMessageBox::information(this,tr("Informations diverses"),messageToShow);
}

/*********
 * TROISIEME ONGLET : GESTION CORBEILLE
 *
 *
 * *******/

/**
 * @brief FenPrincipale::on_inTrashNoteList_itemDoubleClicked gestion de la liste affichant les Notes dans la corbeille (double click)
 * @param item
 */
void FenPrincipale::on_inTrashNoteList_itemDoubleClicked(QListWidgetItem *item){
    ui->trashInformationNote->setVisible(true);

    int noteId = getQStringId(item->data(Qt::UserRole)).toInt();
    Note* currentNote = app->getNoteListManager()->getNote(noteId);
    ui->trashIdNote->setText(QString::number(currentNote->getId()));
    ui->trashTypeNote->setText(currentNote->getIterator()->current().getaNote()->getNameTypeNote());
    ui->trashTitleNote->setText(QString::fromStdString(currentNote->getTitle()));
    ui->trashTitleVersion->setText(QString::fromStdString(currentNote->getIterator()->current().getaNote()->getTitle()));
}

/**
 * @brief FenPrincipale::on_buttonViderCorbeille_clicked gestion du bouton pour vider la corbeille
 */
void FenPrincipale::on_buttonViderCorbeille_clicked(){
    vector<Note>* noteList = app->getNoteListManager()->getNoteList();

    for(unsigned int i = 0 ; i < noteList->size() ; i++){
        if((*noteList)[i].getStatus() == Enumeration::IN_TRASH){
            app->getNoteListManager()->deleteNote((*noteList)[i].getId());
        }
    }
    QMessageBox::information(this,tr("Information"),tr("La corbeille a ete videe"));
    ui->inTrashNoteList->clear();
    ui->trashInformationNote->setVisible(false);
}

/**
 * @brief FenPrincipale::on_buttonTrashDeleteNote_clicked gestion du bouton pour supprimer une node de la corbeille
 */
void FenPrincipale::on_buttonTrashDeleteNote_clicked(){
    int noteId = ui->trashIdNote->text().toInt();
    app->getNoteListManager()->deleteNote(noteId);
    ui->trashInformationNote->setVisible(false);
    QMessageBox::information(this,tr("Information"),tr("La note a ete supprime"));
}

void FenPrincipale::on_buttonTrashRestaureNote_clicked(){
    int noteId = ui->trashIdNote->text().toInt();
    app->getNoteListManager()->activeNote(noteId);
    QMessageBox::information(this,tr("Information"),tr("La note a ete restaure"));
}

/*********
 * QUATRIEME ONGLET : GESTION DES PARAMETRES
 *
 *
 * *******/

/**
 * @brief FenPrincipale::displayParameterData met a jour l'affichage des paramètres de l'application
 */
void FenPrincipale::displayParameterData(){
    Parameter *params = app->getParameters();

    if(params->getSaveAuto()){
        ui->parameterSaveAuto->setChecked(true);
    } else {
        ui->parameterNoSaveAuto->setChecked(true);
    }
    if(params->getDisplayTaskByPriority()){
        ui->parameterTaskDisplayingPriority->setChecked(true);
    } else{
        ui->parameterTaskDisplayingDate->setChecked(true);
    }
    if(params->getRemoveAuto()){
        ui->parameterVidageAuto->setChecked(true);
    } else {
        ui->parameterNonVidageAuto->setChecked(true);
    }
}

/**
 * @brief FenPrincipale::on_parameterButtonSave_clicked permet de prendre en compte les modifications des paramètres de l'application
 */
void FenPrincipale::on_parameterButtonSave_clicked(){
    Parameter *params = app->getParameters();
    params->setSaveAuto(ui->parameterSaveAuto->isChecked());
    params->setDisplayTaskByPriority(ui->parameterTaskDisplayingPriority->isChecked());
    params->setRemoveAuto(ui->parameterVidageAuto->isChecked());
    saveApplication();
}

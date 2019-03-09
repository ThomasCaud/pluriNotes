#ifndef FENPRINCIPALE_H
#define FENPRINCIPALE_H

#include <QMainWindow>
#include <QPushButton>
#include <QLayout>
#include <QString>
#include <QListWidgetItem>
#include <QDialog>
#include "NoteFactory.h"
#include "Application.h"
#include "ui_FenPrincipale.h"
#include "FormatDataStrategy.h"

namespace Ui {
class FenPrincipale;
}

/**
 * @brief The FenPrincipale class est la class centrale de l'application. Elle permet de gérer l'ensemble de la vue. Elle gère ainsi l'ensemble de signaux et régule l'expérience utilisateur.
 */
class FenPrincipale : public QMainWindow
{
    Q_OBJECT

public:
    ~FenPrincipale();
    void display();
    Ui::FenPrincipale* getWindow(){return ui;}
    Application* getApp(){return app;}
    void load(){loadData();}
    static FenPrincipale* getInstance();
    void simuleClickedActiveNoteList(QListWidgetItem *item){on_activeNoteList_itemDoubleClicked(item);}

private slots:
    void on_createNoteButton_clicked();

    void on_createOrEditBut_clicked();

    void on_typeNoteComboBox_currentIndexChanged(const QString &);

    void on_activeNoteList_itemDoubleClicked(QListWidgetItem*);

    void saveApplication();
    void loadData();
    void leftApplication();

    void on_buttonPreviousVersion_clicked();
    void on_buttonNextVersion_clicked();
    void on_buttonRestaure_clicked();
    void on_deleteNoteBut_clicked();

    //Relation view
    void on_ButtonAddOrEditRelation_clicked();
    void on_ButtonDeleteRelation_clicked();

    // bin view
    void on_buttonViderCorbeille_clicked();
    void on_inTrashNoteList_itemDoubleClicked(QListWidgetItem*);
    void on_buttonTrashDeleteNote_clicked();
    void on_buttonTrashRestaureNote_clicked();
    void on_createRelationButton_clicked();

    // parameters view
    void on_parameterButtonSave_clicked();
    void on_ButtonAddCouple_clicked();
    void on_relationSimpleList_itemDoubleClicked(QListWidgetItem*);
    void on_tableWidgetCouple_itemClicked(QTableWidgetItem*);
    void on_taskList_itemDoubleClicked(QTreeWidgetItem*, int);

    void on_archivedNoteList_itemDoubleClicked(QListWidgetItem*);
    void genericItemDoubleClickedListFirstView(QString);
    void genericItemDoubleClickedListSecondView(Relation*);

    void on_notesDescendantes_itemDoubleClicked(QTreeWidgetItem *item, int column);
    void on_showArborescenceButton_clicked();
    void on_notesAscendantes_itemDoubleClicked(QTreeWidgetItem *item, int column);
    void on_referenceList_itemDoubleClicked(QListWidgetItem *item);

    void on_infoButton_clicked();

private:
    explicit FenPrincipale(QWidget *parent = 0);
    static FenPrincipale* f_instance;
    Application* app;
    Ui::FenPrincipale *ui;
    NoteFactory nf;
    void clearLayout(QLayout *layout);
    QString getQStringId(QVariant itemData);
    void editNote();
    void createNote();
    void clearChampsText();
    void displayDateDerniereModif(bool);
    void displayNumVersion(bool b);
    void displayMainAndRightLayout(bool b);
    void displayAbstractNote(Note*);
    void updateButtonsNaviguationVersions(int, int);
    Note* getNoteFromLabel();
    void addCouplesToRelation(Relation*);

    void displayDescendingNotes();
    void displayDescendingNotesAuxiliaire(QTreeWidgetItem*, vector<int>*, int);
    void displayAscendingNotes();
    vector<QTreeWidgetItem*> displayAscendingNotesAuxiliaire(vector<int>*, int);

    FormatDataStrategy* database;

    //functions for second view
    void createRelation();
    void editRelation();
    void clearChampsTextSecondView();
    void displayParameterData();

    void searchAndAddReferences(Note* note);
    void enableModificationNote(bool);
};

#endif // FENPRINCIPALE_H

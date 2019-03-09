#-------------------------------------------------
#
# Project created by QtCreator 2017-05-18T15:10:38
#
#-------------------------------------------------

QT       += core gui
QT       += xml

CONFIG   += c++11
QMAKE_CXXFLAGS += -std=c++11

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = pluriNotes
TEMPLATE = app

SOURCES += *.cpp

HEADERS  += *.h

FORMS    += \
    FenPrincipale.ui

RC_FILE = myapp.rc

OTHER_FILES += \
    modifDiagClass.txt

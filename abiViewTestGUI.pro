#-------------------------------------------------
#
# Project created by QtCreator 2013-07-07T15:36:29
#
#-------------------------------------------------

QT       += core gui

TARGET = abiViewTestGUI
TEMPLATE = app


SOURCES += main.cpp\
           mainwindow.cpp \
           graphwidget.cpp

HEADERS  += mainwindow.h \
            graphwidget.h

FORMS    += mainwindow.ui

INCLUDEPATH += includes/emboss/

unix:!macx:!symbian: LIBS += -L$$PWD/libs/ -lajax
unix:!macx:!symbian: LIBS += -L$$PWD/libs/ -lepcre
unix:!macx:!symbian: LIBS += -L$$PWD/libs/ -leexpat
unix:!macx:!symbian: LIBS += -L$$PWD/libs/ -lezlib

INCLUDEPATH += $$PWD/libs
DEPENDPATH += $$PWD/libs

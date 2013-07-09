#-------------------------------------------------
#
# Project created by QtCreator 2013-07-07T15:36:29
#
#-------------------------------------------------

QT       += core gui

TARGET = abiViewTestGUI
TEMPLATE = app

QMAKE_CXXFLAGS += -std=c++0x

SOURCES += src/main.cpp\
           src/mainwindow.cpp \
           src/graphwidget.cpp \
           src/wilscrollarea.cpp \


HEADERS  += includes/mainwindow.h \
            includes/graphwidget.h \
            includes/wilscrollarea.h \


FORMS    += mainwindow.ui

INCLUDEPATH += \
        includes/ \
        includes/emboss/ \

unix:!macx:!symbian: LIBS += -L$$PWD/libs/ -lajax
unix:!macx:!symbian: LIBS += -L$$PWD/libs/ -lepcre
unix:!macx:!symbian: LIBS += -L$$PWD/libs/ -leexpat
unix:!macx:!symbian: LIBS += -L$$PWD/libs/ -lezlib

INCLUDEPATH += $$PWD/libs
DEPENDPATH += $$PWD/libs

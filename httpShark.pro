#-------------------------------------------------
#
# Project created by QtCreator 2010-08-28T15:56:52
#
#-------------------------------------------------

QT       += core gui webkit network xmlpatterns

TARGET = httpShark
DESTDIR = bin
TEMPLATE = app

# Anti-trash ;)
OBJECTS_DIR = temp
MOC_DIR = temp
UI_DIR = temp
RCC_DIR = temp

SOURCES += src/cpp/main.cpp\
        src/cpp/mainwindow.cpp

HEADERS  += src/headers/mainwindow.h

FORMS    += src/ui/mainwindow.ui

RESOURCES +=

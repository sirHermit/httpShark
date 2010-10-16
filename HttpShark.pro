#-------------------------------------------------
#
# Project created by QtCreator 2010-03-06T17:58:20
# And edited by Volodymyr Fedyk
#
#-------------------------------------------------

QT       += network webkit xml

TARGET = HttpShark
DESTDIR = bin
TEMPLATE = app

# Anti-trash ;)
OBJECTS_DIR = temp
MOC_DIR = temp
UI_DIR = temp
RCC_DIR = temp

#Sources
SOURCES += src/cpp/main.cpp \
        src/cpp/mainwindow.cpp

HEADERS  += src/headers/mainwindow.h

FORMS    += src/ui/mainwindow.ui
TRANSLATIONS += translations/httpshark_uk-UA.ts translations/httpshark_en_US.ts
unix {
 target.path =  $$/usr/bin
 icon.files =  images/httpshark.png
 icon.path =  /usr/share/pixmaps
 desktop.files =  netfleet.desktop
 desktop.path =  /usr/share/applications
 INSTALLS +=  target  icon  desktop
}

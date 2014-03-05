#-------------------------------------------------
#
# Project created by QtCreator 2011-06-11T17:44:42
#
#-------------------------------------------------

QT       += core gui webkit

TARGET = onshell
TEMPLATE = app

SOURCES += main.cpp\
        on.cpp\
        wallpaper.cpp\
        inotify/inotify-cxx.cpp

HEADERS  += on.h\
        wallpaper.h\
        inotify/inotify-cxx.h

CONFIG += link_pkgconfig
PKGCONFIG += gtk+-3.0 gconf-2.0 dconf 
#libnotify
OBJECTS_DIR = build
MOC_DIR = build
UI_DIR = build
RCC_DIR = build

target.path = /usr/bin
INSTALLS += target

shell.path = /usr/share/on/shell
shell.files = shell/*
INSTALLS += shell

QMAKE_CXXFLAGS 	+= -fpermissive -Wunused-parameter -Wno-format-security

CXXFLAGS+=-fpermissive -Wunused-parameter -Wno-format-security

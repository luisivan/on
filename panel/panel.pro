#-------------------------------------------------
#
# Project created by QtCreator 2011-07-23T13:37:30
#
#-------------------------------------------------

QT       += core gui dbus

TARGET = onpanel
TEMPLATE = app

SOURCES += main.cpp\
        onpanel.cpp\
        common/applet.cpp\
        common/cairoutils.cpp\
        common/gconnector.cpp\
        common/panelstyle.cpp\
        indicator/indicator.c\
        indicator/indicatorapplet.cpp\
        legacytray/fdoselectionmanager.cpp\
        legacytray/fdotask.cpp\
        legacytray/x11embedcontainer.cpp\
        legacytray/x11embeddelegate.cpp\
        legacytray/x11embedpainter.cpp\
        legacytray/legacytrayapplet.cpp

HEADERS  += onpanel.h\
        common/applet.h\
        common/cairoutils.h\
        common/gconnector.h\
        common/panelstyle.h\
        common/gscopedpointer.h\
        indicator/indicator-config.h\
        indicator/indicator.h\
        indicator/indicatorapplet.h\
        legacytray/fdoselectionmanager.h\
        legacytray/fdotask.h\
        legacytray/x11embedcontainer.h\
        legacytray/x11embeddelegate.h\
        legacytray/x11embedpainter.h\
        legacytray/legacytrayapplet.h

CONFIG += link_pkgconfig

PKGCONFIG += gtk+-3.0 indicator3-0.4 libqtbamf libwnck-3.0

OBJECTS_DIR = build
MOC_DIR = build
UI_DIR = build
RCC_DIR = build

target.path = /usr/bin
INSTALLS += target

QMAKE_CXXFLAGS 	+= -fpermissive -Wno-c++0x-compat -std=c++0x -Wno-unused-parameter

CXXFLAGS += -fpermissive -Wno-c++0x-compat -std=c++0x -Wno-unused-parameter

QMAKE_CFLAGS += -Wno-ignored-qualifiers -DGTK_ENABLE_BROKEN=1 -Wno-unused-parameter -Wno-unused-variable -Wno-implicit-function-declaration

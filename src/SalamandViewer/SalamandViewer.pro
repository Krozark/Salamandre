#-------------------------------------------------
#
# Project created by QtCreator 2014-04-03T21:06:07
#
#-------------------------------------------------

QT       += core gui opengl

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = SalamandViewer
TEMPLATE = app

include("./gui/gui.pri")
include("./shared/shared.pri")

SOURCES += main.cpp

QMAKE_CXXFLAGS += -std=c++11

RESOURCES += \
    Salamander.qrc

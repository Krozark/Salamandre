#-------------------------------------------------
#
# Project created by QtCreator 2014-04-08T15:45:13
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = Viewer
TEMPLATE = app

include("./forms/forms.pri")
include("./objects/objects.pri")

QMAKE_CXXFLAGS += -std=c++11
LIBS += -lcryptopp

SOURCES += main.cpp

HEADERS  +=

RESOURCES += \
    Viewer.qrc

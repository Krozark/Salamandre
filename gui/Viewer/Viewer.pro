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
include("./record/record.pri")

INCLUDEPATH += ../../daemon/include/

QMAKE_CXXFLAGS_DEBUG += -pg
QMAKE_CXXFLAGS += -std=c++11 -O3

LIBS += -lcryptopp -lz -lsocket

SOURCES += main.cpp

HEADERS  +=

RESOURCES += \
    Viewer.qrc

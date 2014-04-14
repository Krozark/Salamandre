#-------------------------------------------------
#
# Project created by QtCreator 2014-04-07T22:33:56
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = Salamandre
TEMPLATE = app

TEMPLATE = subdirs
SUBDIRS = Simulator \
    Viewer

app1.file = Simulator.pro
app2.file = Viewer.pro

QMAKE_CXXFLAGS += -std=c++11

SOURCES +=
HEADERS +=

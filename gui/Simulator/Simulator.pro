#-------------------------------------------------
#
# Project created by QtCreator 2014-04-08T15:46:00
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = Simulator
TEMPLATE = app

include("./forms/forms.pri")

QMAKE_CXXFLAGS += -std=c++11

SOURCES += main.cpp

HEADERS +=

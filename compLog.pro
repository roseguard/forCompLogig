#-------------------------------------------------
#
# Project created by QtCreator 2016-10-29T20:08:01
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = compLog
TEMPLATE = app


SOURCES += main.cpp\
        widget.cpp \
    solver.cpp

HEADERS  += widget.h \
    solver.h

CONFIG += mobility
MOBILITY = 


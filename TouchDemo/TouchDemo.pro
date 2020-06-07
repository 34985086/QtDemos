#-------------------------------------------------
#
# Project created by QtCreator 2017-09-29T18:56:49
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = TouchDemo
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    TouchWidget.cpp \
    MoveWidget.cpp

HEADERS  += mainwindow.h \
    TouchWidget.h \
    MoveWidget.h

FORMS    += mainwindow.ui \
    TouchWidget.ui \
    MoveWidget.ui

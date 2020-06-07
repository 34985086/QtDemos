#-------------------------------------------------
#
# Project created by QtCreator 2017-05-03T21:09:12
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = RightMenu
TEMPLATE = app


SOURCES += main.cpp\
        widget.cpp \
    ContextMenu.cpp \
    PushButton.cpp

HEADERS  += widget.h \
    ContextMenu.h \
    PushButton.h

FORMS    += widget.ui

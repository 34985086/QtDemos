#-------------------------------------------------
#
# Project created by QtCreator 2018-06-01T16:00:57
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = DeviceMonitor
TEMPLATE = app
CONFIG += console
LIBS += -lUser32 -lWtsapi32 -lwevtapi
SOURCES += main.cpp\
        mainwindow.cpp \
    DeviceMonitor.cpp

HEADERS  += mainwindow.h \
    DeviceMonitor.h

FORMS    += mainwindow.ui

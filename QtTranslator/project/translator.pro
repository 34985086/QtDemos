#-------------------------------------------------
#
# Project created by QtCreator 2015-12-04T22:28:09
#
#-------------------------------------------------

QT       += core gui xml axcontainer

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = translator
TEMPLATE = app

SOURCES += main.cpp\
    TsEngine.cpp \
    MainWindow.cpp \
    TrThread.cpp \
    TrWidget.cpp \
    ExcelEngine.cpp \
    FindDialog.cpp \
    TrWizard.cpp \
    LgCache.cpp \
    FileWorker.cpp \
    SaveExcelThread.cpp \
    ExcelWriteWorker.cpp

HEADERS  += \
    TsEngine.h \
    Config.h \
    MainWindow.h \
    TrThread.h \
    TrWidget.h \
    ExcelEngine.h \
    FindDialog.h \
    TrWizard.h \
    LgCache.h \
    FileWorker.h \
    SaveExcelThread.h \
    ExcelWriteWorker.h

RESOURCES += \
    res/res.qrc

FORMS +=

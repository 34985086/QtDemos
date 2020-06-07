#include "ContextMenu.h"

#include <QDebug>
#include <QEvent>

ContextMenu::ContextMenu(QWidget *parent) : QMenu(parent)
{
    setObjectName("ContextMenu");

    setStyleSheet("QMenu {"
                  " background-color: rgb(74, 78, 82);"
                  "}\n"
                  "QMenu::item {"
                  "padding-left: 70px;"
                  " background-color: transparent;"
                  " width: 130px;"
                  " height: 60px;"
                  " border-radius: 3px;"
                  " font-family: SegoeUI;"
                  " font-size: 18px;"
                  " color: #ffffff;"
                  "}\n"
                  "QMenu::item:selected  {"
                  " background-color: #3c3f43;"
                  "}\n"
                  "QMenu::icon {"
                  " top: 0px;"
                  " right: 0px;"
                  " bottom: 0px;"
                  " left: 0px;"
                  "}\n"
                  "QMenu::indicator {"
                  " width: 40px;"
                  " height: 60px;"
                  "}"
                  );

    addAction(tr("Edit"));
    addAction(tr("Copy"));
    addAction(tr("Paste"));

    setFixedSize(200, 300);

//    installEventFilter(this);
}

bool ContextMenu::eventFilter(QObject *watched, QEvent *event)
{
    qDebug() << watched->objectName() << ":" << event->type();
    return false;
}

void ContextMenu::focusInEvent(QFocusEvent *event)
{
    qDebug() << "focusInEvent" << event;
}

void ContextMenu::focusOutEvent(QFocusEvent *event)
{
    qDebug() << "focusOutEvent" << event;;
}

void ContextMenu::closeEvent(QCloseEvent *event)
{
    qDebug() << "closeEvent" << event;;
}

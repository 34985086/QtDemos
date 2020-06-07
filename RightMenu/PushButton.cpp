#include "PushButton.h"
#include <QDebug>
#include <QEvent>

PushButton::PushButton(QWidget *parent) : QPushButton(parent)
{
//    installEventFilter(this);
//    grabMouse();
}

bool PushButton::eventFilter(QObject *watched, QEvent *event)
{
    qDebug() << watched->objectName() << ":" << event->type();
    return false;
}

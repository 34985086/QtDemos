#include "ShadowMask.h"
#include <QDebug>
#include <QTimer>
#include <QMouseEvent>
#include <QApplication>

ShadowMask::ShadowMask(QWidget *parent)
    : QWidget(parent)
{
    setObjectName("ShadowMask");
    setAttribute(Qt::WA_TransparentForMouseEvents, true);
    qApp->installEventFilter(this);
}

void ShadowMask::showEvent(QShowEvent *e)
{
    QWidget::showEvent(e);
    raise();
}

bool ShadowMask::eventFilter(QObject *watched, QEvent *event)
{
    Q_UNUSED(watched);
    switch(event->type()){
    case QEvent::MouseMove:
    {
        QMouseEvent *me = static_cast<QMouseEvent *>(event);
        QPoint position = mapFromGlobal(me->globalPos());
        bool outside = !childrenRegion().contains(position);
        setAttribute(Qt::WA_TransparentForMouseEvents, outside);
        break;
    }
    }
    return false;
}


#include "TouchWidget.h"
#include "ui_TouchWidget.h"

#include <QDebug>
#include <QTouchEvent>
#include <QPainter>


TouchWidget::TouchWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::TouchWidget)
{
    ui->setupUi(this);
    for(auto child : children()){
        MoveWidget *mw = qobject_cast<MoveWidget *>(child);
        if(mw){
            mWidgetList.append(mw);
            mw->installEventFilter(this);
        }
    }

    installEventFilter(this);
//    setAttribute(Qt::WA_AcceptTouchEvents, true);
//    setAttribute(Qt::WA_WState_AcceptedTouchBeginEvent, true);
}

TouchWidget::~TouchWidget()
{
    delete ui;
}

void TouchWidget::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event);
    QPainter painter(this);

    QPen pen;
    pen.setColor(Qt::red);
    pen.setWidth(2);

    painter.setPen(pen);
    for(auto point : m_tpList){
        painter.drawEllipse(point.pos(), 20, 20);
    }

    if(m_mousePoint.x() >= 0 && m_mousePoint.y() >= 0){
        painter.drawEllipse(m_mousePoint, 20, 20);
    }
}

bool TouchWidget::eventFilter(QObject *watched, QEvent *event)
{
    switch (event->type()) {
    case QEvent::TouchBegin:
    case QEvent::TouchUpdate:
    {
        qDebug() << "QEvent::TouchUpdate";
        QTouchEvent *te = static_cast<QTouchEvent *>(event);
        m_tpList = te->touchPoints();
        if(m_tpList.size() > touchPointCount){
            touchPointCount = m_tpList.size();

            for(auto touch : m_tpList){
                moveBegin(touch.pos().toPoint());
            }
        }else{
            touchPointCount = m_tpList.size();
        }

        for(auto touch : m_tpList){
            moveUpdate(touch.pos().toPoint());
        }
        break;
    }
    case QEvent::TouchEnd:
        qDebug() << "QEvent::TouchEnd";
        m_tpList.clear();
        touchPointCount = m_tpList.size();
        moveEnd();
        break;
#if 1
    case QEvent::MouseButtonPress:
    {
        QMouseEvent *me = static_cast<QMouseEvent *>(event);
        m_mousePoint = me->pos();
        moveBegin(m_mousePoint);
        break;
    }
    case QEvent::MouseMove:
    {
        QMouseEvent *me = static_cast<QMouseEvent *>(event);
        m_mousePoint = me->pos();
        moveUpdate(m_mousePoint);
        break;
    }
    case QEvent::MouseButtonRelease:
        m_mousePoint = QPoint(-1, -1);
        moveEnd();
        break;
#endif
    default:
        break;
    }

    update();

    return false;
}

void TouchWidget::moveBegin(const QPoint &point)
{
    qDebug() << "moveBegin:" << point;
    for(auto widget : mWidgetList){
        if(widget->geometry().contains(point)){
            widget->moveBegin(point);
        }
    }
}

void TouchWidget::moveUpdate(const QPoint &point)
{
    for(auto widget : mWidgetList){
        if(widget->geometry().contains(point)){
            widget->moveUpdate(point);
        }
    }
}

void TouchWidget::moveEnd()
{
    for(auto widget : mWidgetList){
        widget->moveEnd();
    }
}

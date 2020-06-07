#include "DropShadowWidget.h"

#include <QPainter>
#include <QtMath>
#include <QMouseEvent>
#include <QDebug>

#define SHADOW_WIDTH 10

struct FramelessWindowPrivate {
    FramelessWindowPrivate(QWidget *contentWidget) : contentWidget(contentWidget) {}
    QWidget *contentWidget;
    QPoint mousePressedPosition; // 鼠标按下时的坐标
    QPoint windowPositionAsDrag; // 鼠标按小时窗口左上角的坐标
};

DropShadowWidget::DropShadowWidget(QWidget *parent)
    : QWidget(parent)
    , mIsValidArea(false)
{
    setWindowOpacity(0.7);
    setWindowFlags(Qt::FramelessWindowHint);    // 去掉边框
    setAttribute(Qt::WA_TranslucentBackground); // 背景透明
    setAttribute(Qt::WA_TransparentForMouseEvents);
    setMouseTracking(true);

    d = new FramelessWindowPrivate(this);

    mBorderPath.addRoundRect(SHADOW_WIDTH, SHADOW_WIDTH, width() - SHADOW_WIDTH*2, height() - SHADOW_WIDTH*2, 10);
}

DropShadowWidget::~DropShadowWidget()
{

}

void DropShadowWidget::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event);
    QPainterPath path;
    path.setFillRule(Qt::WindingFill);
    path.addRoundRect(SHADOW_WIDTH, SHADOW_WIDTH, width() - SHADOW_WIDTH*2, height() - SHADOW_WIDTH*2, 10);
//    path.addRect(SHADOW_WIDTH, SHADOW_WIDTH, width() - SHADOW_WIDTH*2, height() - SHADOW_WIDTH*2);
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing, true);
    painter.fillPath(path, QBrush(Qt::white));

    QColor color(255, 0, 0, 50);
    for(int i = 0; i < SHADOW_WIDTH; i++)
    {
        QPainterPath path;
        path.setFillRule(Qt::WindingFill);
//        path.addRect(SHADOW_WIDTH - i, SHADOW_WIDTH - i, width()-(SHADOW_WIDTH-i)*2, height()-(SHADOW_WIDTH-i)*2);
        path.addRoundRect(SHADOW_WIDTH - i, SHADOW_WIDTH - i, width()-(SHADOW_WIDTH-i)*2, height()-(SHADOW_WIDTH-i)*2, 10);
        color.setAlpha(150 - qSqrt(i)*50);
        painter.setPen(color);
        painter.drawPath(path);
    }
}

void DropShadowWidget::mousePressEvent(QMouseEvent *e)
{
    if(mBorderPath.contains(e->localPos())){
        qDebug() << "contains" << e->localPos();
        mIsValidArea = true;
        d->mousePressedPosition = e->globalPos();
        d->windowPositionAsDrag = pos();
        setCursor(Qt::ClosedHandCursor);
    }else{
        mIsValidArea = false;
        setCursor(Qt::ArrowCursor);
    }
}

void DropShadowWidget::mouseReleaseEvent(QMouseEvent *e)
{
    Q_UNUSED(e)
    // 鼠标放开始设置鼠标按下的位置为 null，表示鼠标没有被按下
    d->mousePressedPosition = QPoint();
}

void DropShadowWidget::mouseMoveEvent(QMouseEvent *e)
{
    qDebug() << "mouseMoveEvent, " << e;
    if (mIsValidArea && !d->mousePressedPosition.isNull()) {
        // 鼠标按下并且移动时，移动窗口, 相对于鼠标按下时的位置计算，是为了防止误差累积
        QPoint delta = e->globalPos() - d->mousePressedPosition;
        move(d->windowPositionAsDrag + delta);
    }
}

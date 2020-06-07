#include "NumberPicker.h"

#include <QPainter>

NumberPicker::NumberPicker(QWidget *parent)
    : QWidget(parent)
{

}

void NumberPicker::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event);
    QPainter painter(this);

    painter.fillRect(rect(), QColor("#80ff0000"));
#if 0
    QPen pen;
    pen.setColor(Qt::black);
    pen.setWidth(1);
    painter.setPen(pen);
    painter.drawEllipse(width()/2, height()/2, width()/2, width()/2);
#endif
    QFont font;
    font.setPointSize(16);
    font.setBold(false);
    painter.setFont(font);
    painter.drawText(QRect(0, 0, width(), height()/3), Qt::AlignCenter, m_prev);
    painter.drawText(QRect(0, 2*height()/3, width(), height()/3), Qt::AlignCenter, m_next);

    font.setPointSize(18);
    font.setBold(true);
    painter.setFont(font);
    painter.drawText(QRect(0, height()/3, width(), height()/3), Qt::AlignCenter, m_cur);
}


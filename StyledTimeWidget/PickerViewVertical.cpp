#include "PickerViewVertical.h"
#include <QDebug>

PickerViewVertical::PickerViewVertical(QWidget *parent)
    : QWidget(parent)
    , m_minimum(0)
    , m_maximum(100)
    , m_current(50)
    , m_isDragging(0)
    , m_deviation(0)
    , m_numSize(6)
{
    m_homingAni  = new QPropertyAnimation(this, "deviation");
    m_homingAni->setDuration(1300);
    m_homingAni->setEasingCurve(QEasingCurve::OutQuad);
}

void PickerViewVertical::setRange(int min,int max)
{
    m_minimum = min;
    m_maximum = max;

    if(m_current < min)
        m_current = min;
    if(m_current > max)
        m_current = max;

    //计算字符尺寸
    m_numSize = 3;
    int temp = m_maximum;
    while(temp > 0)
    {
        temp /= 10;
        m_numSize++;
    }

    repaint();
}

int PickerViewVertical::readValue()
{
    return m_current;
}

void PickerViewVertical::mousePressEvent(QMouseEvent *e)
{
    m_homingAni->stop();
    m_isDragging = true;
    m_mouseSrcPos = e->pos().y();
}

void PickerViewVertical::mouseMoveEvent(QMouseEvent *e)
{
    if(m_isDragging)
    {
        //数值到边界时，阻止继续往对应方向移动
        if((m_current == m_minimum && e->pos().y() >= m_mouseSrcPos)||
          (m_current == m_maximum && e->pos().y() <= m_mouseSrcPos))
            return;

        m_deviation = e->pos().y() - m_mouseSrcPos;

        //若移动速度过快时进行限制
        if(m_deviation > (height()-1)/4)
            m_deviation = (height()-1)/4;
        else if(m_deviation < -(height()-1)/4)
            m_deviation = -(height()-1)/4;

        emit deviationChange((float)m_deviation/((height()-1)/4));

        repaint();
    }
}

void PickerViewVertical::mouseReleaseEvent(QMouseEvent *)
{
    if(m_isDragging)
    {
        m_isDragging = 0;
        homing();
    }
}

void PickerViewVertical::paintEvent(QPaintEvent *)
{
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing, true);

    int w = width()-1;
    int h = height()-1;

    if(m_deviation >= h/4 && m_current > m_minimum)
    {
        m_mouseSrcPos += h/4;
        m_deviation -= h/4;
        m_current -= 1;
    }
    if(m_deviation <= -h/4 && m_current < m_maximum)
    {
        m_mouseSrcPos -= h/4;
        m_deviation += h/4;
        m_current += 1;
    }

    //中间数字
    paintNum(painter, m_current, m_deviation);

    //两侧数字1
    if(m_current != m_minimum)
        paintNum(painter, m_current-1, m_deviation - h/4);
    if(m_current != m_maximum)
        paintNum(painter, m_current+1, m_deviation + h/4);

    //两侧数字2,超出则不显示
    if(m_deviation >= 0 && m_current - 2 >= m_minimum)
        paintNum(painter, m_current - 2, m_deviation - h/2);
    if(m_deviation <= 0 && m_current + 2 <= m_maximum)
        paintNum(painter, m_current + 2, m_deviation + h/2);

    //边框
    painter.setPen(QPen(QColor(0,0,0,120), 2));
    painter.drawLine(0, h/8*3, w, h/8*3);
    painter.drawLine(0, h/8*5, w, h/8*5);
}

void PickerViewVertical::paintNum(QPainter &painter, int num, int deviation)
{
    int w = width()-1;
    int h = height()-1;

    int size = (h - qAbs(deviation))/m_numSize;
    int transparency = 255-510*qAbs(deviation)/h;
    int H = h/2 - 3*qAbs(deviation)/4;
    int y = h/2 + deviation - H/2;

    QFont font;
    font.setPixelSize(size);
    painter.setFont(font);
    painter.setPen(QColor(0, 0, 0, transparency));
    painter.drawText(QRectF(0, y, w, H), Qt::AlignCenter, QString::number(num));
}

void PickerViewVertical::homing()
{
    //将数字矫正到中心
    if(m_deviation > height()/8) {
        m_homingAni->setStartValue(m_deviation - height()/4);
        m_homingAni->setEndValue(0);
        m_current--;
    }
    else if(m_deviation < -height()/8) {
        m_homingAni->setStartValue(m_deviation + height()/4);
        m_homingAni->setEndValue(0);
        m_current++;
    }
    else {
        m_homingAni->setStartValue(m_deviation);
        m_homingAni->setEndValue(0);
    }
    emit valueChanged(m_current);
    m_homingAni->start();
}

int PickerViewVertical::readDeviation()
{
    return m_deviation;
}

void PickerViewVertical::setDeviation(int n)
{
    m_deviation = n;
    repaint();
}


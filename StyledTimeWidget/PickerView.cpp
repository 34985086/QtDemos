#include "PickerView.h"
#include <QDebug>

PickerView::PickerView(QWidget *parent)
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

void PickerView::setRange(int min,int max)
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

int PickerView::readValue()
{
    return m_current;
}

void PickerView::mousePressEvent(QMouseEvent *e)
{
    m_homingAni->stop();
    m_isDragging = true;
    m_mouseSrcPos = e->pos().x();
}

void PickerView::mouseMoveEvent(QMouseEvent *e)
{
    if(m_isDragging)
    {
        //数值到边界时，阻止继续往对应方向移动
        if((m_current == m_minimum && e->pos().x() >= m_mouseSrcPos)||
          (m_current == m_maximum && e->pos().x() <= m_mouseSrcPos))
            return;

        m_deviation = e->pos().x() - m_mouseSrcPos;

        //若移动速度过快时进行限制
        if(m_deviation > (width()-1)/4)
            m_deviation = (width()-1)/4;
        else if(m_deviation < -(width()-1)/4)
            m_deviation = -(width()-1)/4;

        emit deviationChange((float)m_deviation/((width()-1)/4));

        repaint();
    }
}

void PickerView::mouseReleaseEvent(QMouseEvent *)
{
    if(m_isDragging)
    {
        m_isDragging = 0;
        homing();
    }
}

void PickerView::paintEvent(QPaintEvent *)
{
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing, true);

    int w = width()-1;
    int h = height()-1;

    if(m_deviation >= w/4 && m_current > m_minimum)
    {
        m_mouseSrcPos += w/4;
        m_deviation -= w/4;
        m_current -= 1;
    }
    if(m_deviation <= -w/4 && m_current < m_maximum)
    {
        m_mouseSrcPos -= w/4;
        m_deviation += w/4;
        m_current += 1;
    }

    //中间数字
    paintNum(painter, m_current, m_deviation);

    //两侧数字1
    if(m_current != m_minimum)
        paintNum(painter, m_current-1, m_deviation - w/4);
    if(m_current != m_maximum)
        paintNum(painter, m_current+1, m_deviation + w/4);

    //两侧数字2,超出则不显示
    if(m_deviation >= 0 && m_current - 2 >= m_minimum)
        paintNum(painter, m_current - 2, m_deviation - w/2);
    if(m_deviation <= 0 && m_current + 2 <= m_maximum)
        paintNum(painter, m_current + 2, m_deviation + w/2);

    //边框
    painter.setPen(QPen(QColor(0,0,0,120), 2));
    painter.drawLine(w/8*3, 0, w/8*3, h);
    painter.drawLine(w/8*5, 0, w/8*5, h);
}

void PickerView::paintNum(QPainter &painter, int num, int deviation)
{
    int w = width()-1;
    int h = height()-1;

    qDebug() << "m_numSize:" << m_numSize;
    int size = (w - qAbs(deviation))/m_numSize;
    int transparency = 255-510*qAbs(deviation)/w;
    int width = w/2 - 3*qAbs(deviation)/4;
    int x = w/2 + deviation - width/2;

    QFont font;
    font.setPixelSize(size);
    painter.setFont(font);
    painter.setPen(QColor(0, 0, 0, transparency));
    painter.drawText(QRectF(x, 0, width, h), Qt::AlignCenter, QString::number(num));
}

void PickerView::homing()
{
    //将数字矫正到中心
    if(m_deviation > width()/8) {
        m_homingAni->setStartValue(m_deviation - width()/4);
        m_homingAni->setEndValue(0);
        m_current--;
    }
    else if(m_deviation < -width()/8) {
        m_homingAni->setStartValue(m_deviation + width()/4);
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

int PickerView::readDeviation()
{
    return m_deviation;
}

void PickerView::setDeviation(int n)
{
    m_deviation = n;
    repaint();
}

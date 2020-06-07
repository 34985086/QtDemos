#ifndef PICKERVIEWVERTICAL_H
#define PICKERVIEWVERTICAL_H

#include <QWidget>
#include <QMouseEvent>
#include <QPainter>
#include <QPropertyAnimation>

class PickerViewVertical : public QWidget
{
    Q_OBJECT
    Q_PROPERTY(int deviation READ readDeviation WRITE setDeviation)
public:
    explicit PickerViewVertical(QWidget *parent = 0);
    void setRange(int min,int max);
    int readValue();
protected:
    void mousePressEvent(QMouseEvent *);
    void mouseMoveEvent(QMouseEvent *);
    void mouseReleaseEvent(QMouseEvent *);
    void paintEvent(QPaintEvent *);
signals:
    void valueChanged(int value);
    void deviationChange(float deviation);
private:
    void paintNum(QPainter &painter, int num, int deviation);
    void homing();
    int readDeviation();
    void setDeviation(int n);
private:
    int m_minimum;
    int m_maximum;
    int m_current;
    bool m_isDragging;
    int m_deviation;
    int m_mouseSrcPos;
    int m_numSize;//计算所得的数字字符尺寸，以最长的情况为准
    QPropertyAnimation *m_homingAni;
};

#endif // PICKERVIEWVERTICAL_H

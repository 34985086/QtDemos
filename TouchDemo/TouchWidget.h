#ifndef TOUCHWIDGET_H
#define TOUCHWIDGET_H

#include <QWidget>
#include <QPaintEvent>
#include <QEvent>
#include "MoveWidget.h"

namespace Ui {
class TouchWidget;
}

class TouchWidget : public QWidget
{
    Q_OBJECT

public:
    explicit TouchWidget(QWidget *parent = 0);
    ~TouchWidget();
protected:
    void paintEvent(QPaintEvent *event);
    bool eventFilter(QObject *watched, QEvent *event);
    void moveBegin(const QPoint &point);
    void moveUpdate(const QPoint &point);
    void moveEnd();
private:
    Ui::TouchWidget *ui;
    QPoint m_mousePoint = QPoint(-1, -1);
    QList<QTouchEvent::TouchPoint>m_tpList;
    int touchPointCount = 0;
    QList<MoveWidget *>mWidgetList;
};

#endif // TOUCHWIDGET_H

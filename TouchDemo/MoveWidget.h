#ifndef MOVEWIDGET_H
#define MOVEWIDGET_H

#include <QWidget>
#include <QEvent>
#include <QTouchEvent>

namespace Ui {
class MoveWidget;
}

class MoveWidget : public QWidget
{
    Q_OBJECT

public:
    explicit MoveWidget(QWidget *parent = 0);
    ~MoveWidget();
    void moveBegin(const QPoint &point);
    void moveUpdate(const QPoint &point);
    void moveEnd();
protected:
private:
    Ui::MoveWidget *ui;
    QPoint m_prevPoint;
    bool m_isPressed = false;
};

#endif // MOVEWIDGET_H

#ifndef DROPSHADOWWIDGET_H
#define DROPSHADOWWIDGET_H

#include <QWidget>

struct FramelessWindowPrivate;
class DropShadowWidget : public QWidget
{
    Q_OBJECT

public:
    DropShadowWidget(QWidget *parent = 0);
    ~DropShadowWidget();
protected:
    void paintEvent(QPaintEvent *event) override;
    void mousePressEvent(QMouseEvent *e) override;
    void mouseReleaseEvent(QMouseEvent *e) override;
    void mouseMoveEvent(QMouseEvent *e) override;
private:
    FramelessWindowPrivate *d;
    QPainterPath mBorderPath;
    bool mIsValidArea;
};

#endif // DROPSHADOWWIDGET_H

#ifndef SHADOWWINDOW_H
#define SHADOWWINDOW_H

#include <QWidget>
#include <QGraphicsBlurEffect>
#include <QResizeEvent>

class ShadowWindow : public QWidget
{
    Q_OBJECT
public:
    explicit ShadowWindow(QWidget *parent = nullptr);
    void setShadowStyle(const QColor &color, int radius = 10, int blur = 20);
protected:
    void resizeEvent(QResizeEvent *e);
private:
    void updateShadowStyle();
    QWidget *m_shadow = Q_NULLPTR;
    QGraphicsBlurEffect *m_effect = Q_NULLPTR;
};

#endif // SHADOWWINDOW_H

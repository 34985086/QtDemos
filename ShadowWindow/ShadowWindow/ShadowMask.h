#ifndef SHADOWMASK_H
#define SHADOWMASK_H

#include <QWidget>
#include <QShowEvent>

class ShadowMask : public QWidget
{
    Q_OBJECT
public:
    explicit ShadowMask(QWidget *parent = nullptr);
protected:
    void showEvent(QShowEvent *e);
    bool eventFilter(QObject *watched, QEvent *event);
};

#endif // SHADOWMASK_H

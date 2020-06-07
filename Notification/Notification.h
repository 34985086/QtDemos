#ifndef NOTIFICATION_H
#define NOTIFICATION_H

#include <QWidget>
#include <QParallelAnimationGroup>
#include <QShowEvent>

namespace Ui {
class Notification;
}

class Notification : public QWidget
{
    Q_OBJECT

public:
    explicit Notification(QWidget *parent = 0);
    ~Notification();
    void setText(const QString &text);
protected:
    void showEvent(QShowEvent *event);
private:
    void setupAnimation();
    Ui::Notification *ui;
    QParallelAnimationGroup mAnimationGrp;
};

#endif // NOTIFICATION_H

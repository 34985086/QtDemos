#ifndef PUSHBUTTON_H
#define PUSHBUTTON_H

#include <QPushButton>

class PushButton : public QPushButton
{
    Q_OBJECT
public:
    explicit PushButton(QWidget *parent = 0);

signals:

public slots:
protected:
    bool eventFilter(QObject *watched, QEvent *event);
};

#endif // PUSHBUTTON_H

#ifndef ROUNDBUTTON_H
#define ROUNDBUTTON_H

#include <QWidget>

namespace Ui {
class RoundButton;
}

class RoundButton : public QWidget
{
    Q_OBJECT

public:
    explicit RoundButton(QWidget *parent = 0);
    ~RoundButton();

private:
    Ui::RoundButton *ui;
};

#endif // ROUNDBUTTON_H

#include "RoundButton.h"
#include "ui_RoundButton.h"
#include <QGraphicsDropShadowEffect>

RoundButton::RoundButton(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::RoundButton)
{
    ui->setupUi(this);

    QGraphicsDropShadowEffect *effect = new QGraphicsDropShadowEffect(this);
    effect->setColor(Qt::red);
    effect->setBlurRadius(20);
//    effect->
    ui->pushButton->setGraphicsEffect(effect);
}

RoundButton::~RoundButton()
{
    delete ui;
}

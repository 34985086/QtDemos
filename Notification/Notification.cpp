#include "Notification.h"
#include "ui_Notification.h"

#include <QGraphicsOpacityEffect>
#include <QPropertyAnimation>
#include <QDebug>

Notification::Notification(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Notification)
{
    ui->setupUi(this);
    setupAnimation();
}

Notification::~Notification()
{
    delete ui;
}

void Notification::setText(const QString &text)
{
    ui->label->setText(text);

    QFontMetrics fm(ui->label->font());
    int w = fm.width(text);
    resize(w + 120, height());

    int x = parentWidget()->width()/2 - width()/2;
    int y = parentWidget()->height() - height() - 20;
    move(x, y);

    show();
}

void Notification::showEvent(QShowEvent *event)
{
    QWidget::showEvent(event);
    mAnimationGrp.start();
}

void Notification::setupAnimation()
{
    QGraphicsOpacityEffect* effect = new QGraphicsOpacityEffect(this);
    setGraphicsEffect(effect);

    QPropertyAnimation *animate = new QPropertyAnimation(effect, "opacity");
    animate->setEasingCurve(QEasingCurve::InQuad);
    animate->setDuration(2000);
    animate->setStartValue(1.0);
    animate->setEndValue(0.0);

    mAnimationGrp.addAnimation(animate);

    connect(&mAnimationGrp, &QParallelAnimationGroup::finished, [this](){
        hide();
    });
}

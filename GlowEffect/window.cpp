#include "window.h"
#include <QLabel>
#include <QHBoxLayout>
#include "gloweffect.h"
#include <QToolTip>
#include "QGraphicsGlowEffect.h"


Window::Window(QWidget *parent)
    : QWidget(parent)
{
    lbl = new QLabel("Let's have a try!", this);
    QHBoxLayout * layout = new QHBoxLayout(this);
    layout->addWidget(lbl);
    setLayout(layout);

    setFont(QToolTip::font());

    GlowEffect *effect = new GlowEffect(this);
    effect->setRadius(15);
    effect->setGlowColor(Qt::blue);
    lbl->setGraphicsEffect(effect);

//    QGraphicsGlowEffect * glow = new QGraphicsGlowEffect();
//    glow->setStrength(4);
//    glow->setBlurRadius(10);
//    lbl->setGraphicsEffect(glow);
}

Window::~Window()
{
}

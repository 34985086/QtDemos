#include "ShadowWindow.h"
#include <QDebug>

ShadowWindow::ShadowWindow(QWidget *parent)
    : QWidget(parent)
    , m_shadow(new QWidget(parent))
    , m_effect(new QGraphicsBlurEffect(this))
{
    setAttribute(Qt::WA_StyledBackground, true);
    m_shadow->stackUnder(this);

    setShadowStyle(Qt::black, 0, 15);
}

void ShadowWindow::setShadowStyle(const QColor &color, int radius, int blur)
{
    QStringList style;
    style << QString("background-color:%1;").arg(color.name())
          << QString("border-radius:%1px;").arg(radius);
    m_shadow->setStyleSheet(style.join(""));

    m_effect->setBlurRadius(blur);
    m_shadow->setGraphicsEffect(m_effect);
}

void ShadowWindow::resizeEvent(QResizeEvent *e)
{
    QWidget::resizeEvent(e);
    m_shadow->resize(size());
    m_shadow->move(this->x(), this->y());
}

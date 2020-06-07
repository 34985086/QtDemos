#include "MoveWidget.h"
#include "ui_MoveWidget.h"
#include <QDebug>

MoveWidget::MoveWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::MoveWidget)
{
    ui->setupUi(this);
    setAttribute(Qt::WA_StyledBackground, true);
    setAttribute(Qt::WA_TransparentForMouseEvents, true);
}

MoveWidget::~MoveWidget()
{
    delete ui;
}

void MoveWidget::moveBegin(const QPoint &point)
{
    m_prevPoint = mapFromParent(point);
    m_isPressed = true;
}

void MoveWidget::moveUpdate(const QPoint &point)
{
    if(m_isPressed){
        move(point - m_prevPoint);
        m_prevPoint = mapFromParent(point);
    }
}

void MoveWidget::moveEnd()
{
    m_isPressed = false;
}



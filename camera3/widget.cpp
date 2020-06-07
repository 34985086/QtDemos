#include "widget.h"
#include "ui_widget.h"
#include "CameraFilter.h"
#include "CustomFilter.h"

#include <QQuickWidget>
#include <QQuickItem>
#include <QThread>

#include "QZXing.h"

Widget::Widget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Widget)
{
    ui->setupUi(this);
}

Widget::~Widget()
{
    delete ui;
}

void Widget::resizeEvent(QResizeEvent *event)
{
    QWidget::resizeEvent(event);
    ui->camera->resize(size());
    ui->camera->move(0, 0);
}

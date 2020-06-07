#include "widget.h"
#include "ui_widget.h"

Widget::Widget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Widget)
{
    ui->setupUi(this);
    ui->quick->setSource(QUrl("qrc:///main.qml"));
    ui->quick->setResizeMode(QQuickWidget::SizeRootObjectToView);
}

Widget::~Widget()
{
    delete ui;
}

void Widget::resizeEvent(QResizeEvent *event)
{
    QWidget::resizeEvent(event);
    ui->quick->setGeometry(0, 0, width(), height());
    ui->label->setGeometry(0, 0, width(), ui->label->height());

//    ui->btn->setGeometry();
    ui->btn->move(width() - ui->btn->width() - 20, height() - ui->btn->height() - 20);
}

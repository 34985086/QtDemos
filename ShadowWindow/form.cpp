#include "form.h"
#include "ui_form.h"
#include <QDebug>

Form::Form(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Form)
{
    ui->setupUi(this);
//    ui->window->setStyleSheet("QWidget#window{background-color:blue; border-radius:20px;}");
}

Form::~Form()
{
    delete ui;
}

void Form::resizeEvent(QResizeEvent *event)
{
    ui->mask->resize(event->size());
    ui->mask->move(0, 0);
}

#include "form.h"
#include "ui_form.h"
#include <QDebug>

Form::Form(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Form)
{
    ui->setupUi(this);
    ui->btn->setText("<u>click here to show toast</u>");
}

Form::~Form()
{
    delete ui;
}

void Form::on_btn_clicked()
{
    qDebug() << "on_btn_clicked";
    ui->widget->setText("<u>This is a toast showing and hiding an animation.</u>");
}

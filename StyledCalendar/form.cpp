#include "form.h"
#include "ui_form.h"

Form::Form(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Form)
{
    ui->setupUi(this);
}

Form::~Form()
{
    delete ui;
}

void Form::resizeEvent(QResizeEvent *event)
{
    Q_UNUSED(event);
    ui->widget->setFixedSize(width(), (int)(width()/1.242));
}

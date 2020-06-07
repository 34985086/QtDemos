#include "TimeSpinner.h"
#include "ui_TimeSpinner.h"
#include <QDebug>

TimeSpinner::TimeSpinner(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::TimeSpinner)
{
    ui->setupUi(this);
    ui->hourSpinner->setTitle("hh");
    ui->hourSpinner->setRange(0, 23);
    ui->hourSpinner->setValue(10);
    ui->minSpinner->setTitle("mm");
    ui->minSpinner->setRange(0, 59);
    ui->minSpinner->setValue(30);
}

TimeSpinner::~TimeSpinner()
{
    delete ui;
}

void TimeSpinner::setCurrentTime(int hour, int min)
{
    ui->hourSpinner->setValue(hour);
    ui->minSpinner->setValue(min);
}

void TimeSpinner::on_hourSpinner_valueChanged(const QString &value)
{
    qDebug() << "on_hourSpinner_valueChanged," << value;
    m_hour = value;
    emit timeChanged(m_hour + ":" + m_minute);
}

void TimeSpinner::on_minSpinner_valueChanged(const QString &value)
{
    qDebug() << "on_minSpinner_valueChanged," << value;
    m_minute = value;
    emit timeChanged(m_hour + ":" + m_minute);
}

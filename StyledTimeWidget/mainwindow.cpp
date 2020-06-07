#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QDebug>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->timeSpinner->setCurrentTime(23, 32);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_timeSpinner_timeChanged(const QString &time)
{
    qDebug() << "on_timeSpinner_timeChanged, " << time;
    ui->time->setText(time);
}

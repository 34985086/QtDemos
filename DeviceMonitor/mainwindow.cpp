#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "DeviceMonitor.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , m_deviceMonitor(QSharedPointer<DeviceMonitor>::create())
{
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}

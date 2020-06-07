#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->lineEdit->setStyleSheet("QLineEdit{ background:rgba(0,0,0,0%); border:1px; font:10pt}"
                                "QLineEdit:hover{ border-image:url(:/btn_background.png); }"
                                "QLineEdit:!hover{background:rgba(0,0,0,0%);}"
                                "QLineEdit:focus {background:red;border-image:none; border:1px groove lightgray; border-radius:8px}");
}

MainWindow::~MainWindow()
{
    delete ui;
}

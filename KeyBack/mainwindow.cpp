#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::keyReleaseEvent(QKeyEvent *event)
{
    static int i = 0;
    switch(event->key()){
    case Qt::Key_Back:
    case Qt::Key_Backspace:
        ui->label->setText(QString().setNum(++i));
        if(i == 20){
            close();
        }
        break;
    }
}

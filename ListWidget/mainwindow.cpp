#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QDebug>
#include <QLabel>
#include "StyledItem.h"
#include <QGraphicsDropShadowEffect>

class SpacerItem : public QSpacerItem
{
public:
    explicit SpacerItem(int w, int h,
                        QSizePolicy::Policy hData = QSizePolicy::Minimum,
                        QSizePolicy::Policy vData = QSizePolicy::Minimum)
        : QSpacerItem(w, h, hData, vData){

    }
    QSize sizeHint(){
        return QSize(20, 40);
    }

    Qt::Orientations expandingDirections(){
        return  Qt::Vertical;
    }
};

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->list->addItem(new QLabel("11111111"), 10);
    ui->list->addItem(new StyledItem("22222222"), 60);
    ui->list->addItem(new QLabel("333333"), 25);

//    QGraphicsDropShadowEffect *effect = new QGraphicsDropShadowEffect(this);
//    effect->setBlurRadius(20);
//    effect->setColor(Qt::red);

    QGraphicsBlurEffect *effect = new QGraphicsBlurEffect(this);
    effect->setBlurRadius(20);
    ui->widget->setGraphicsEffect(effect);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_btn1_clicked()
{
    qDebug() << "on_btn1_clicked";
//    ui->verticalLayout->widget();
    ui->widget->setGraphicsEffect(Q_NULLPTR);
}

void MainWindow::on_btn2_clicked()
{
    qDebug() << "on_btn2_clicked";
    QGraphicsBlurEffect *effect = new QGraphicsBlurEffect(this);
    effect->setBlurRadius(20);
    ui->widget->setGraphicsEffect(effect);
}

#include "widget.h"
#include "ui_widget.h"
#include "ContextMenu.h"
#include <QDebug>
#include <QMessageBox>

Widget::Widget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Widget)
{
    ui->setupUi(this);
    mMessage = new QMessageBox(this);
    mMessage->setModal(false);
    mMessage->setText("The document has been modified.");
    m_pContextMenu = new ContextMenu;//(this);
//    m_pContextMenu->setAttribute(Q);

    //Qt::CustomContextMenu can trigger signal customContextMenuRequested
    setContextMenuPolicy(Qt::CustomContextMenu);
    connect(this, SIGNAL(customContextMenuRequested(const QPoint)), this,
        SLOT(contextMenuRequested(const QPoint&)));

//    installEventFilter(this);

//    grabMouse();

    qApp->installEventFilter(this);
}

Widget::~Widget()
{
    delete ui;
}

void Widget::contextMenuRequested(const QPoint& point)
{
    m_pContextMenu->popup(mapToGlobal(point));
}

void Widget::on_button_1_clicked()
{
    qDebug() << __FUNCTION__;
    mMessage->show();
//    mMessage->move();

}

void Widget::on_button_2_clicked()
{
    qDebug() << __FUNCTION__;
    m_pContextMenu->show();
}


void Widget::resizeEvent(QResizeEvent *event)
{
    Q_UNUSED(event);
}

bool Widget::eventFilter(QObject *watched, QEvent *event)
{
    qDebug() << watched->objectName() << ":" << event->type();
    return false;
}

#include "QuickContainer.h"
#include "ui_QuickContainer.h"

QuickContainer::QuickContainer(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::QuickContainer)
{
    ui->setupUi(this);
}

QuickContainer::~QuickContainer()
{
    delete ui;
}

void QuickContainer::setQml(const QString &qml)
{
    ui->quickWidget = new QQuickWidget(this);
    ui->quickWidget->setResizeMode(QQuickWidget::SizeRootObjectToView);
    ui->quickWidget->setSource(QUrl(qml));
    ui->quickWidget->setGeometry(0, 0, width(), height());
    ui->quickWidget->show();
}

QQuickItem *QuickContainer::root()
{
    return ui->quickWidget->rootObject();
}

void QuickContainer::resizeEvent(QResizeEvent *event)
{
    QWidget::resizeEvent(event);
    ui->quickWidget->resize(size());
    ui->quickWidget->move(0, 0);
}

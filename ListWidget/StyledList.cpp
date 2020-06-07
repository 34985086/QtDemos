#include "StyledList.h"
#include "ui_StyledList.h"

#include <QScroller>
#include <QListWidgetItem>

StyledList::StyledList(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::StyledList)
{
    ui->setupUi(this);
    ui->list->setVerticalScrollMode(QAbstractItemView::ScrollPerPixel);
    ui->list->setHorizontalScrollMode(QAbstractItemView::ScrollPerPixel);
    ui->list->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    ui->list->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    ui->list->setResizeMode(QListWidget::Adjust);
    ui->list->setSelectionMode(QAbstractItemView::SingleSelection);
    ui->list->setFocusPolicy(Qt::NoFocus);
    ui->list->setAcceptDrops(true);
    QScroller::grabGesture(ui->list, QScroller::TouchGesture);
}

StyledList::~StyledList()
{
    delete ui;
}

void StyledList::addItem(QWidget *w, int h)
{
    Q_ASSERT(w);
    QListWidgetItem *item = new QListWidgetItem(ui->list);
    item->setSizeHint(QSize(100, h));
    ui->list->setItemWidget(item, w);
}

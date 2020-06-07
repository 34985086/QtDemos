#include "Widget.h"
#include "TitleBar.h"
#include <QVBoxLayout>

Widget::Widget(QWidget *parent) : QWidget(parent)
{
    setWindowFlags(Qt::FramelessWindowHint | windowFlags());

    TitleBar *pTitleBar = new TitleBar(this);
    installEventFilter(pTitleBar);

    resize(400, 300);
    setWindowTitle("Custom Window");
//    setWindowIcon(QIcon(":/Images/logo"));

    QPalette pal(palette());
    pal.setColor(QPalette::Background, QColor(250, 250, 250));
    setAutoFillBackground(true);
    setPalette(pal);

    QVBoxLayout *pLayout = new QVBoxLayout();
    pLayout->addWidget(pTitleBar);
    pLayout->addStretch();
    pLayout->setSpacing(0);
    pLayout->setContentsMargins(0, 0, 0, 0);
    setLayout(pLayout);
}

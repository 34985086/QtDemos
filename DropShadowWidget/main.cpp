#include "DropShadowWidget.h"
#include <QApplication>
#include <QDebug>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

//    qDebug() << QString::fromLocal8Bit("一去丶二三里");
//    qDebug() << QString::fromLocal8Bit("青春不老，奋斗不止！");
//    qDebug() << QString::fromLocal8Bit("纯正开源之美，有趣、好玩、靠谱。。。");

    DropShadowWidget w;
    w.show();

    return a.exec();
}

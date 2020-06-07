#include "widget.h"
#include "CameraFilter.h"
#include "CustomFilter.h"
#include "qDebug2Logcat.h"

#include <QtQml>
#include <QApplication>

int main(int argc, char *argv[])
{
    installLogcatMessageHandler("Tag");
    QApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
    QApplication a(argc, argv);

    qmlRegisterType<CameraFilter>("CameraFilter", 1, 0, "CameraFilter");
    qmlRegisterType<CustomFilter>("MyVideoFilterLib", 1, 0, "MyVideoFilter");

    Widget w;
    w.show();

    return a.exec();
}

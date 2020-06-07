#include "StyledCalendar.h"
#include <QApplication>
#include "qDebug2Logcat.h"
#include "form.h"

int main(int argc, char *argv[])
{
    installLogcatMessageHandler("APP");
    QApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
    QApplication a(argc, argv);
    Form w;
    w.show();

    return a.exec();
}

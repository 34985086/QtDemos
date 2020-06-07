#include "dialog.h"
#include "qDebug2Logcat.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    installLogcatMessageHandler("SOAP");
    QApplication a(argc, argv);
    Dialog w;
    w.show();

    return a.exec();
}

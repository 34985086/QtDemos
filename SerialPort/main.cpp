#include "SerialPort.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    SerialPort w;
//    a.setAttribute(Qt::AA_EnableHighDpiScaling);
    w.show();
    return a.exec();
}

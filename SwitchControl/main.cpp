#include "SwitchControl.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    SwitchControl w;
    w.show();

    return a.exec();
}

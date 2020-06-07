#include "RoundButton.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    RoundButton w;
    w.show();

    return a.exec();
}

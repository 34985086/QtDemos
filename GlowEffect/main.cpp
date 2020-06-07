#include <QApplication>
#include "window.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    Window w;
    w.resize(500, 600);
    w.show();
    
    return a.exec();
}

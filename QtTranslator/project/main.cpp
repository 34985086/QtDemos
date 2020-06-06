
#include <QApplication>
#include "MainWindow.h"
#include "MdiAreaMainWin.h"
#include "FindDialog.h"

#include "TrWizard.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    //TrWizard w;
    //MdiAreaMainWin w;
    //FindDialog w;
    w.show();

    return a.exec();
}

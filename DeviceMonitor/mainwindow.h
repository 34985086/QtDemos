#ifndef MAINWINDOW_H
#define MAINWINDOW_H
#include "DeviceMonitor.h"
#include <QMainWindow>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private:
    Ui::MainWindow *ui;
    QSharedPointer<DeviceMonitor>m_deviceMonitor;
};

#endif // MAINWINDOW_H

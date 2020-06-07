#ifndef SERIALPORT_H
#define SERIALPORT_H

#include <QWidget>
#include <QSerialPort>

QT_BEGIN_NAMESPACE
namespace Ui { class SerialPort; }
QT_END_NAMESPACE

class SerialPort : public QWidget
{
    Q_OBJECT

public:
    SerialPort(QWidget *parent = nullptr);
    ~SerialPort();

private slots:
    void on_btnPost_clicked();

    void on_btnClear_clicked();

    void on_btnOpen_clicked();

    void on_btnBCC_clicked();

    void ReadData();

    void HandleError(QSerialPort::SerialPortError error);

private:
    Ui::SerialPort *ui;
    QSerialPort *serial;

    void ActivateBaudRate(const QString &arg1);

    void ActivateParity(int arg1);

    void ActivateDataBits(const QString &arg1);

    void ActivateStopBits(const QString &arg1);

    void ActivateFlowControl(int arg1);

    void InitPorts();

    void InitParameters();

    void CloseSerialPort();

    QString CalcBCC(QString text);
};
#endif // SERIALPORT_H

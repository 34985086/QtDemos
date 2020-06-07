#include <QSerialPortInfo>
#include <QMessageBox>
#include <QDebug>
#include "SerialPort.h"
#include "ui_SerialPort.h"

SerialPort::SerialPort(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::SerialPort)
{
    ui->setupUi(this);
    serial = new QSerialPort();
    connect(serial, &QSerialPort::readyRead, this, &SerialPort::ReadData);
    connect(serial, &QSerialPort::errorOccurred, this, &SerialPort::HandleError);

    InitPorts();

    InitParameters();

    qDebug() << "BCC:" << CalcBCC("G1");
}

SerialPort::~SerialPort()
{
    delete ui;
}

void SerialPort::ReadData()
{
    QByteArray buf;
    buf = serial->readAll();
    QString s = buf;
    qDebug() << s << endl;
    ui->teRecv->append(s);
}

void SerialPort::HandleError(QSerialPort::SerialPortError error)
{
    if (error == QSerialPort::ResourceError) {
        QMessageBox::critical(this, tr("Critical Error"), serial->errorString());
        CloseSerialPort();
    }
}

void SerialPort::CloseSerialPort()
{
    if (serial->isOpen())
        serial->close();
}

void SerialPort::on_btnPost_clicked()
{
    qDebug() << "on_btnPost_clicked";
    QString q = ui->tePost->toPlainText();
    QByteArray f = q.toLatin1();
    char *temp = f.data();
    serial->write(temp);
}

void SerialPort::on_btnClear_clicked()
{
    qDebug() << "on_btnClear_clicked";
    ui->teRecv->clear();
}

void SerialPort::on_btnOpen_clicked()
{
    qDebug() << "on_btnOpen_clicked";
    QSerialPortInfo *port_info = new QSerialPortInfo(ui->comBox->currentText());
    serial->setPort(*port_info);

    if(!serial->open(QIODevice::ReadWrite))
    {
        QMessageBox::warning(this,"Warning","Open serial port failed.");
    }

    ActivateBaudRate(ui->brBox->currentText());
    ActivateParity(ui->vbBox->currentIndex());
    ActivateDataBits(ui->dbBox->currentText());
    ActivateStopBits(ui->sbBox->currentText());
    ActivateFlowControl(ui->fcBox->currentIndex());
}

void SerialPort::on_btnBCC_clicked()
{
    qDebug() << "on_btnBCC_clicked";
    QString txt = ui->tePost->toPlainText();
    qDebug() << txt << ", BCC:" << CalcBCC(txt);
}

void SerialPort::ActivateBaudRate(const QString &arg1)
{
    qDebug() << "ActivateBaudRate, arg1:" << arg1;
    switch(arg1.toInt())
    {
    case 1200:
        serial->setBaudRate(QSerialPort::Baud1200);
        break;
    case 2400:
        serial->setBaudRate(QSerialPort::Baud2400);
        break;
    case 9600:
        serial->setBaudRate(QSerialPort::Baud9600);
        break;
    case 38400:
        serial->setBaudRate(QSerialPort::Baud38400);
        break;
    case 115200:
        serial->setBaudRate(QSerialPort::Baud115200);
        break;
    }
}

void SerialPort::ActivateParity(int bit)
{
    qDebug() << "ActivateParity, bit:" << bit;
    switch(bit)
    {
    case 0:
        serial->setParity(QSerialPort::OddParity);
        break;
    case 1:
        serial->setParity(QSerialPort::EvenParity);
        break;
    case 2:
        serial->setParity(QSerialPort::NoParity);
        break;
    }
}

void SerialPort::ActivateDataBits(const QString &arg1)
{
    qDebug() << "ActivateDataBits, arg1:" << arg1.toInt();
    switch(arg1.toInt())
    {
    case 5:
        serial->setDataBits(QSerialPort::Data5);
        break;
    case 6:
        serial->setDataBits(QSerialPort::Data6);
        break;
    case 7:
        serial->setDataBits(QSerialPort::Data7);
        break;
    case 8:
        serial->setDataBits(QSerialPort::Data8);
        break;
    }
}

void SerialPort::ActivateStopBits(const QString &arg1)
{
    qDebug() << "ActivateStopBits, arg1:" << arg1.toInt();
    switch(arg1.toInt())
    {
    case 1:
        serial->setStopBits(QSerialPort::OneStop);
        break;
    case 2:
        serial->setStopBits(QSerialPort::TwoStop);
        break;
    }
}

void SerialPort::ActivateFlowControl(int arg1)
{
    qDebug() << "ActivateFlowControl, arg1:" << arg1;
    switch(arg1)
    {
    case 0:
        serial->setFlowControl(QSerialPort::HardwareControl);
        break;
    case 1:
        serial->setFlowControl(QSerialPort::SoftwareControl);
        break;
    case 2:
        serial->setFlowControl(QSerialPort::NoFlowControl);
        break;
    }
}

void SerialPort:: InitPorts()
{
    QList<QSerialPortInfo> port = QSerialPortInfo::availablePorts();
    if(port.isEmpty())
    {
        ui->comBox->addItem("None");
    }
    else
    {
        foreach(const QSerialPortInfo &port_info, port)
        {
            ui->comBox->addItem(port_info.portName());
        }
    }
}

void SerialPort::InitParameters()
{
    ui->brBox->addItem(QStringLiteral("9600"), QSerialPort::Baud9600);
    ui->brBox->addItem(QStringLiteral("19200"), QSerialPort::Baud19200);
    ui->brBox->addItem(QStringLiteral("38400"), QSerialPort::Baud38400);
    ui->brBox->addItem(QStringLiteral("115200"), QSerialPort::Baud115200);

    ui->dbBox->addItem(QStringLiteral("5"), QSerialPort::Data5);
    ui->dbBox->addItem(QStringLiteral("6"), QSerialPort::Data6);
    ui->dbBox->addItem(QStringLiteral("7"), QSerialPort::Data7);
    ui->dbBox->addItem(QStringLiteral("8"), QSerialPort::Data8);
    ui->dbBox->setCurrentIndex(3);

    ui->vbBox->addItem(tr("None"), QSerialPort::NoParity);
    ui->vbBox->addItem(tr("Even"), QSerialPort::EvenParity);
    ui->vbBox->addItem(tr("Odd"), QSerialPort::OddParity);
    ui->vbBox->addItem(tr("Mark"), QSerialPort::MarkParity);
    ui->vbBox->addItem(tr("Space"), QSerialPort::SpaceParity);

    ui->sbBox->addItem(QStringLiteral("1"), QSerialPort::OneStop);
#ifdef Q_OS_WIN
    ui->sbBox->addItem(tr("1.5"), QSerialPort::OneAndHalfStop);
#endif
    ui->sbBox->addItem(QStringLiteral("2"), QSerialPort::TwoStop);

    ui->fcBox->addItem(tr("None"), QSerialPort::NoFlowControl);
    ui->fcBox->addItem(tr("RTS/CTS"), QSerialPort::HardwareControl);
    ui->fcBox->addItem(tr("XON/XOFF"), QSerialPort::SoftwareControl);
}

QString SerialPort::CalcBCC(QString text)
{
    char r = 0;
    foreach(QChar c, text)
    {
        r = r ^ c.toLatin1();
    }
    return QString::number(r, 10);
}

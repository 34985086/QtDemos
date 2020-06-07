#include "dialog.h"
#include "ui_dialog.h"
#include "qtsoap.h"

#include <QDebug>

Dialog::Dialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Dialog)
{
    qRegisterMetaType<QtSoapMessage>("QtSoapMessage");
    ui->setupUi(this);

//    m_http = new QtSoapHttpTransport(this);

//    connect(m_http, SIGNAL(responseReady(QtSoapMessage)), this, SLOT(onResponseReady(QtSoapMessage)));

    m_worker = new SoapWorker(this);
}

Dialog::~Dialog()
{
    delete ui;
    m_worker->terminate();
    m_worker->wait();
}

void Dialog::on_submit_clicked()
{
    qDebug() << "on_submit_clicked";
    ui->send->clear();
    ui->receive->clear();

#if 0
    QtSoapMessage message;
    message.setMethod("Login", "http://localhost/");
    message.addMethodArgument("userno", "", "sa");
    message.addMethodArgument("password", "", "123");

    m_http->setAction("http://localhost/Login");
    m_http->setHost("180.153.158.111", false, 88);
    m_http->submitRequest(message, "/assets.asmx");
#endif

#if 0
    QtSoapMessage message;
    message.setMethod("GetAsset", "http://localhost/");
    message.addMethodArgument("barcode", "", ui->number->text());

    m_http->setAction("http://localhost/GetAsset");
    m_http->setHost("180.153.158.111", false, 88);
    m_http->submitRequest(message, "/assets.asmx");
#endif

#if 0
    QtSoapMessage message;
    message.setMethod("ValidateEmailAddress", "http://WebXml.com.cn/");
    message.addMethodArgument("theEmail", "", ui->number->text());

    m_http->setAction("http://WebXml.com.cn/ValidateEmailAddress");
    m_http->setHost("www.webxml.com.cn");
    m_http->submitRequest(message, "/WebServices/ValidateEmailWebService.asmx");
#endif

#if 0 //查询qq
    QtSoapMessage message;
    message.setMethod("qqCheckOnline", "http://WebXml.com.cn/");
    message.addMethodArgument("qqCode", "", ui->number->text());

    m_http->setAction("http://WebXml.com.cn/qqCheckOnline");
    m_http->setHost("www.webxml.com.cn");
    m_http->submitRequest(message, "/webservices/qqOnlineWebService.asmx");
#endif

#if 0
    QtSoapMessage message;
    message.setMethod("GetMobileInfo", "http://gpsso.com/");
    message.addMethodArgument("PhoneNumber", "", ui->number->text());

    m_http->setAction("http://gpsso.com/GetMobileInfo");
    m_http->setHost("www.gpsso.com");
    m_http->submitRequest(message, "/WebService/Phone/Phone.asmx");
#endif

//    QString strXML = message.toXmlString();
//    ui->send->setText(strXML);
//    qDebug() << "strXML:" << strXML;


    QtSoapMessage message;
    message.setMethod("Login", "http://localhost/");
    message.addMethodArgument("userno", "", "sa");
    message.addMethodArgument("password", "", "123");

    m_worker->setAction("http://localhost/Login");
    m_worker->setHost("180.153.158.111", 88);
    m_worker->setRequest(message, "/assets.asmx");
    m_worker->start();
}

void Dialog::onResponseReady(const QtSoapMessage &response)
{
    qDebug() << "onResponseReady";

    QString strXML = response.toXmlString();
    qDebug() << "\nonResponseReady: strXML: " << strXML;

    ui->receive->setText(strXML);
#if 0
    QDomDocument doc;
    doc.setContent(strXML);

    QDomNodeList nodeList = doc.elementsByTagName("qqCheckOnlineResult");
    if (!nodeList.isEmpty())
    {
        QDomNode node = nodeList.at(0);
        QString strResult = node.toElement().text();
        ui->state->setText(strResult);
        qDebug() << "strResult:" << strResult;
    }
#endif
}

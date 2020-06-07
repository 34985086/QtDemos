#include "SoapWorker.h"

#include <QDebug>

#if 1

SoapWorker::SoapWorker(QObject *parent) : QThread(parent)
{
}

void SoapWorker::setHost(const QString &host, int port, bool useSecureHTTP)
{
    m_host = host;
    m_port = port;
    m_secure = useSecureHTTP;
}

void SoapWorker::setAction(const QString &action)
{
    m_action = action;
}

void SoapWorker::setRequest(QtSoapMessage &request, const QString &path)
{
    m_soapMessage = request;
    m_path = path;
}

void SoapWorker::onResponseReady(const QtSoapMessage &response)
{
    QString strXML = response.toXmlString();
    qDebug() << "\nonResponseReady: strXML: " << strXML;
}

void SoapWorker::run()
{
    qDebug() << "SoapWorker::run:" << QThread::currentThreadId();
    if(m_soap.isNull()){
        m_soap = QSharedPointer<QtSoapHttpTransport>::create();
        connect(m_soap.data(), SIGNAL(responseReady(QtSoapMessage)),
                this, SLOT(onResponseReady(QtSoapMessage)));
    }

    m_soap->setAction(m_action);
    m_soap->setHost(m_host, m_secure, m_port);
    m_soap->submitRequest(m_soapMessage, m_path);
    qDebug() << "m_soapMessage:" << m_soapMessage.toXmlString();

    exec();
}
#else

SoapWorker::SoapWorker(QObject *parent) : QObject(parent)
{
//    m_thread.start();
//    moveToThread(&m_thread);
    m_soap = new QtSoapHttpTransport(this);
    connect(m_soap, SIGNAL(responseReady(QtSoapMessage)), this, SLOT(onResponseReady(QtSoapMessage)));
    connect(this, SIGNAL(requestStart()), this, SLOT(onRequestStart()), Qt::QueuedConnection);
}

void SoapWorker::setHost(const QString &host, int port, bool useSecureHTTP)
{
    m_soap->setHost(host, useSecureHTTP, port);
}

void SoapWorker::setAction(const QString &action)
{
    m_soap->setAction(action);
}

void SoapWorker::setRequest(QtSoapMessage &request, const QString &path)
{
    m_soapMessage = request;
    m_path = path;
}

void SoapWorker::start()
{
    qDebug() << "start:" << QThread::currentThreadId();
    emit requestStart();
}

void SoapWorker::onRequestStart()
{
    qDebug() << "onRequestStart:" << QThread::currentThreadId();
    m_soap->submitRequest(m_soapMessage, m_path);
    qDebug() << "m_soapMessage:" << m_soapMessage.toXmlString();
}

void SoapWorker::onResponseReady(const QtSoapMessage &response)
{
    qDebug() << "onResponseReady:" << QThread::currentThreadId();
    qDebug() << "onResponseReady:" << response.toXmlString();
    emit responseComing(response);
}
#endif

#ifndef SOAPWORKER_H
#define SOAPWORKER_H

#include "qtsoap.h"

#if 1
#include <QThread>

class SoapWorker : public QThread
{
    Q_OBJECT
public:
    explicit SoapWorker(QObject *parent = nullptr);
    void setHost(const QString &host, int port = 0, bool useSecureHTTP = false);
    void setAction(const QString &action);
    void setRequest(QtSoapMessage &request, const QString &path);
signals:

public slots:
    void onResponseReady(const QtSoapMessage &response);
protected:
    void run();
private:
    QSharedPointer<QtSoapHttpTransport>m_soap;
    QtSoapMessage m_soapMessage;
    QString m_path;
    QString m_host;
    int m_port = 88;
    bool m_secure = false;
    QString m_action;
};
#else

#include <QObject>

class SoapWorker : public QObject
{
    Q_OBJECT
public:
    explicit SoapWorker(QObject *parent = nullptr);
    void setHost(const QString &host, int port = 0, bool useSecureHTTP = false);
    void setAction(const QString &action);
    void setRequest(QtSoapMessage &request, const QString &path);
    void start();
signals:
    void requestStart();
    void responseComing(const QtSoapMessage &response);
public slots:
    void onRequestStart();
    void onResponseReady(const QtSoapMessage &response);
private:
    QtSoapHttpTransport *m_soap;
    QtSoapMessage m_soapMessage;
    QString m_path;
//    QThread m_thread;
};
#endif

#endif // SOAPWORKER_H

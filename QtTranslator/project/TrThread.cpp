#include <QDebug>
#include "TrThread.h"


TrThread::TrThread(QObject *parent):
    QThread(parent),
    m_worker(NULL)
{
}

void TrThread::setWorker(TsEngine *worker)
{
    m_worker = worker;
}

void TrThread::run()
{
    if(m_worker)
    {
        m_worker->translate();
        qDebug() << "Translate, finished!";
    }
}

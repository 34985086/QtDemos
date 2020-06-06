#include "SaveExcelThread.h"
#include "MainWindow.h"

SaveExcelThread::SaveExcelThread(QObject *parent) :
    QThread(parent),
    m_worker(NULL)
{
}

void SaveExcelThread::run()
{
    if(m_worker)
    {
        m_worker->saveAsExcel(m_file);
    }
}

void SaveExcelThread::setWorker(MainWindow *worker, const QString &fileName)
{
    m_worker = worker;
    m_file   = fileName;

    start();
}

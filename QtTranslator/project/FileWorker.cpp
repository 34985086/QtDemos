#include <QThread>
#include "FileWorker.h"
#include "ExcelEngine.h"

FileWorker::FileWorker(QList<LgCache> *cache, QObject *parent) :
    QObject(parent),
    m_pCache(cache)
{
}

void FileWorker::slotReadHeader(QString file)
{
    qDebug() << "worker::slotReadHeader, " << QThread::currentThreadId();
    m_fileName = file;
    m_excel = new ExcelEngine;
    m_excel->open(m_fileName);

    for(int col = 1; col <= m_excel->columnCount(); col++)
    {
        emit sigReadHeader(m_excel->cellData(1, col).toString(), col);
    }

    emit sigReadHeaderDone();
}

void FileWorker::slotCacheTexts()
{
    qDebug() << "worker::slotCacheTexts, " << QThread::currentThreadId();

    QString str;
    int nrEngText = m_excel->rowCount() - 1;
    int langCount = m_pCache->size();

    emit sigProgressRange(0, nrEngText * langCount + 10);

    for(int lang = 0; lang < langCount; lang++)
    {
        for(int row = 1; row <= nrEngText; row++)
        {
            str = m_excel->cellData(row + 1, (*m_pCache)[lang].columnId()).toString();/*skip the header*/
            //qDebug() << str;
            if(str.isNull() || str.isEmpty())
            {
                (*m_pCache)[lang].append(QString(""));
            }else{
                (*m_pCache)[lang].append(str);
            }
            emit sigProgressValue(row + lang * nrEngText);
        }
    }
    m_excel->close();
    emit sigProgressValue(nrEngText * langCount + 10);
    emit sigCacheTextsDone();
}

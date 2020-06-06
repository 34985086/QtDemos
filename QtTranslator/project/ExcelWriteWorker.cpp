#include <QDebug>
#include "ExcelWriteWorker.h"
#include "ExcelEngine.h"

ExcelWriteWorker::ExcelWriteWorker(QObject *parent) :
    QObject(parent)
{
}

void ExcelWriteWorker::slotStartToSave(QString fileName)
{
    qDebug() << "ExcelWriteWorker::slotStartToSave, " << fileName;
    m_excel = new ExcelEngine;
    m_excel->open(fileName);
}

void ExcelWriteWorker::slotSaveContentsToExcel(int row, int col, QString txt)
{
    //qDebug("(%d, %d):%s \n", row, col, txt.toUtf8().data());
    m_excel->setCellData(row, col, txt);
    m_excel->setAutoWrap(row, col, true);
    if(row == 1)
    {
        m_excel->setBold(1, col, true);
        m_excel->setFontSize(1, col, 12);
        m_excel->setWidth(1, col, 30);
        m_excel->setHeight(1, col, 18);
    }
    emit sigProgressIncrease();
}

void ExcelWriteWorker::slotStopToSave()
{
    qDebug() << "ExcelWriteWorker::slotStopToSave.";
    m_excel->save();
    m_excel->close();
    emit sigFinished();
}

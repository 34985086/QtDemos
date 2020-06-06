#ifndef TKEXCELWRITEWORKER_H
#define TKEXCELWRITEWORKER_H

#include <QObject>

class ExcelEngine;
class ExcelWriteWorker : public QObject
{
    Q_OBJECT
public:
    explicit ExcelWriteWorker(QObject *parent = 0);

signals:
    void sigFinished();
    void sigProgressIncrease();
public slots:
    void slotStartToSave(QString fileName);
    void slotSaveContentsToExcel(int row, int col, QString txt);
    void slotStopToSave();
private:
    ExcelEngine *m_excel;
};

#endif // TKEXCELWRITEWORKER_H

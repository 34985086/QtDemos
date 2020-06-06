#ifndef TKRDXLSHORIHEADER_H
#define TKRDXLSHORIHEADER_H

#include <QObject>
#include "ExcelEngine.h"
#include "LgCache.h"

class FileWorker : public QObject
{
    Q_OBJECT
public:
    explicit FileWorker(QList<LgCache>*cache, QObject *parent = 0);

signals:
    void sigReadHeader(QString name, int col);
    void sigReadHeaderDone();

    void sigProgressRange(int, int);
    void sigProgressValue(int);
    void sigCacheTextsDone();
public slots:
    void slotReadHeader(QString file);
    void slotCacheTexts();
private:
    QString m_fileName;
    QList<LgCache>*m_pCache;
    ExcelEngine *m_excel;
};

#endif // TKRDXLSHORIHEADER_H

#ifndef tkTrThread_H
#define tkTrThread_H

#include <QThread>
#include "TsEngine.h"

class TrThread : public QThread
{
    Q_OBJECT
public:
    explicit TrThread(QObject *parent = 0);
    void     setWorker(TsEngine *worker);
    void     run();
signals:

public slots:

private:
    TsEngine *m_worker;
};

#endif // tkTrThread_H

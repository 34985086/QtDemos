#ifndef TKSAVEEXCELTHREAD_H
#define TKSAVEEXCELTHREAD_H

#include <QThread>

class MainWindow;
class SaveExcelThread : public QThread
{
    Q_OBJECT
public:
    explicit SaveExcelThread(QObject *parent = 0);
    void run();
    void setWorker(MainWindow *worker, const QString &fileName);
signals:

public slots:
private:
    MainWindow *m_worker;
    QString       m_file;
};

#endif // TKSAVEEXCELTHREAD_H

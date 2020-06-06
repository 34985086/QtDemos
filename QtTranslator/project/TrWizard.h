#ifndef TKTRWIZARD_H
#define TKTRWIZARD_H

#include <QWizard>
#include <QThread>
#include "LgCache.h"

typedef struct{
    QString name;
    int     col;
}HeaderCache;

class QLineEdit;
class QTableWidget;
class QProgressBar;
class FileWorker;
class TrWizard : public QWizard
{
    Q_OBJECT
public:
    explicit TrWizard(QList<LgCache>*cache, QWidget *parent = 0);
    ~TrWizard();

    void setLanguageFiles(const QStringList &files);

    QWizardPage *createIntroductionPage();
    QWizardPage *createSelectXlsFilePage();
    QWizardPage *createLoadFilePage();
signals:
    void sigStartToReadHeader(QString file);
    void sigStartToCacheTexts();

    void sigLoadFileFinished();
public slots:
    void slotPageIdChanged(int id);
    void slotSelectXlsFile();

    void slotReadHeader(QString name, int col);
    void slotReadHeaderDone();

    void slotCacheTextsDone();

    void slotProgressRange(int min, int max);
    void slotProgressValue(int v);
private:
    void startToReadHeader(const QString &file);
    void startToCacheTexts();
    QLineEdit         *m_file;
    QTableWidget      *m_table;
    QProgressBar      *m_progress;
    QList<LgCache>  *m_langCache;
    QList<HeaderCache> m_headerCache;
    /*a thread and a worker*/
    QThread            m_workerThread;
    FileWorker *m_worker;
};

#endif // TKTRWIZARD_H

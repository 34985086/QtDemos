#ifndef tkTsEngine_H
#define tkTsEngine_H
#include <QObject>
#include <QTableWidget>
#include <QDomDocument>
#include "ExcelEngine.h"

class QLabel;
class QStatusBar;

#define COLOR_FINISHED      Qt::white
#define COLOR_UNTRANSLATED  Qt::red
#define COLOR_OBSOLETE      Qt::gray

class TsEngine : public QObject
{
    Q_OBJECT
public:
    explicit TsEngine(QTableWidget *table, QObject *parent = 0);
    virtual ~TsEngine();

    void setCache(QList<LgCache> *cache);
    bool openFile(const QString &name);
    bool readFile();

    void setLanguage(const QString &target, const QString &source);
    void setStatusBar(QLabel *location, QLabel *classname, QLabel *nrRows);

    void readContextElement(QDomElement e);
    void readNameElement(QDomElement e);
    void readMessageElement(QDomElement e);
    void readLocationElemnet(QDomElement e);
    void readSourceElement(QDomElement e);
    void readTranslationElement(QDomElement e);
    bool translate();
    void showUntranslated(bool isUnfinished);
    bool untranslated();
    void showRowCount();
    bool saveFile();
    bool saveAs(const QString &fileName);
    void saveTextsAsExcel(ExcelEngine &excel, int col, bool isNeedSrc);
public slots:
    void slotItemChanged(QTableWidgetItem *item);
    void slotItemSelectionChanged();
signals:
    void sigItemChanged(QTableWidgetItem *item);
    void sigHasSomethingChanged();
private:
    QString findString(const QString &src);
    bool    isTranslatedStrIdx(int idx);
    QString       m_tsFile;
    QString       m_xlsFile;

    QDomDocument  m_doc;
    QTableWidget *m_table;
    QVector<QDomElement>m_listOfTr;
    QVector<QDomElement>m_listOfSrc;
    int           m_idx_src;
    int           m_idx_tr;
    int           m_count_row;

    QList<LgCache> *m_cache;
    LgCache    *m_dstCache;
    LgCache    *m_srcCache;

    QVector<int>  m_bUsedList;

    bool          m_isUnfinished;
    /*status bar*/
    QLabel       *m_lacation;
    QLabel       *m_class;
    QLabel       *m_nrRows;
};

#endif // tkTsEngine_H

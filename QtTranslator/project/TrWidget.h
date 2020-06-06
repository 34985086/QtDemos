#ifndef tkTrWidget_H
#define tkTrWidget_H

#include <QWidget>
#include <QProgressBar>
#include "TrThread.h"


class QTableWidget;
class TsEngine;
class TrWidget : public QWidget
{
    Q_OBJECT
public:
    explicit TrWidget(QWidget *parent = 0);
        ~TrWidget();
    void resizeEvent(QResizeEvent *evt);
    void closeEvent(QCloseEvent *evt);
    bool openFile(const QString &filename);
    void setCache(QList<LgCache>*cache);
    void setStatusBar(QLabel *location, QLabel *classname, QLabel *nrRows);
    bool isTranslated();
    bool isSaved();
    int  rowCount();
    void showRowCount();
    bool untranslated();
signals:
    void sigWndClosed();
    void sigHasSomethingChanged();

    void sigSaveCellDataAsExcel(int row, int col, QString data);
public slots:
    void slotSaveFile(bool isSave);
    void slotSaveAs(const QString &fileName);
    void saveAsExcel(int col, bool isNeedSrc);
    void saveTextsAsExcel(ExcelEngine &excel, int col, bool isNeedSrc);
    void slotTranslate();
    void slotUntranslated(bool isUnfinished);
    void slotHasSomethingChanged();
    void slotFindNext(const QString &str, Qt::CaseSensitivity cs);
    void slotFindPrev(const QString &str, Qt::CaseSensitivity cs);
    void slotCut();
    void slotCopy();
    void slotPaste();
    void slotDel();
private:
    QString copySelectedRange(QTableWidgetSelectionRange range);
    QTableWidgetSelectionRange selectedRange() const;
    QString       m_tsFile;
    QTableWidget *m_table;
    TsEngine   *m_parser;
    bool          m_isTranslated;
    bool          m_isSaved;
    bool          m_isForceQuit;
};

#endif // tkTrWidget_H

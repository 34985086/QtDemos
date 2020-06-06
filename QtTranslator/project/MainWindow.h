#ifndef TKMAINWINDOW_H
#define TKMAINWINDOW_H

#include <QMainWindow>
#include <QTableWidget>
#include "TrThread.h"
#include "TsEngine.h"
#include "LgCache.h"

class QLabel;
class QMdiArea;
class QMdiSubWindow;
class FindDialog;
class TrWidget;
class TrWizard;
class ExcelWriteWorker;

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
        ~MainWindow();
    void closeEvent(QCloseEvent *evt);
    void resizeEvent(QResizeEvent *evt);
signals:
    void sigStartToSaveAs(QString fileName);
    void sigStopToSaveAs();
public slots:
    void slotOpenFile();
    void slotSaveFile();
    void slotSaveAllFile();
    void slotSaveAs();
    void slotTranslate();
    void slotShowUntranslated(bool isUnfinished);
    void slotFindString();
    void slotSelectionChanged();
    void slotCopy();
    void slotCut();
    void slotPaste();
    void slotDel();
    void slotAbout();
    void slotHasSomethingChanged();
    void slotSubWindowActivated(QMdiSubWindow *sub);
    void slotWizardClosed(int result);

    void startProgress(int min, int max);
    void stopProgress();
    void increaseProgress();
    void saveAsExcel(const QString &file);
private:
    bool hasSubWindow();
    int  countOfSubWindow();
    void createActions();
    void createMenus();
    void createToolBars();
    void createStatusBar();
    bool hasFileUntranslated();
    bool hasFileUnsaved();
    void bindFindDialog();
    void saveAsTs(const QString &file);
    int  nrItemsSaveAsExcel();
    TrWidget *activeSubWindow();
private:
    QMdiArea         *m_mdiArea;
    QStringList       m_tsFiles;
    QToolBar         *m_fileToolBar;

    QAction          *m_openAction;
    QAction          *m_saveAction;
    QAction          *m_saveAllAction;
    QAction          *m_saveAsAction;
    QAction          *m_closeAction;
    QAction          *m_trAction;
    QAction          *m_showUnfinished;
    QAction          *m_findAction;
    QAction          *m_copyAction;
    QAction          *m_pasteAction;
    QAction          *m_cutAction;
    QAction          *m_delAction;
    QAction          *m_aboutAction;

    /*status bar*/
    QLabel           *m_lacation;
    QLabel           *m_class;
    QLabel           *m_nrRows;
    /*Menu*/
    QMenu            *m_fileMenu;
    QMenu            *m_editMenu;
    QMenu            *m_toolMenu;
    QMenu            *m_helpMenu;

    /**/
    FindDialog     *m_findDlg;
    TrWizard       *m_wizard;

    QList<LgCache>  m_langCache;

    QProgressBar     *m_progress;
    int               m_progress_value;

    ExcelWriteWorker *m_writeWorker;
    QThread             m_writeThread;
};

#endif // TKMAINWINDOW_H

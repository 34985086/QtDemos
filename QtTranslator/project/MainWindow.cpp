#include <QFile>
#include <QLabel>
#include <QDebug>
#include <QAction>
#include <QMdiArea>
#include <QToolBar>
#include <QStatusBar>
#include <QHeaderView>
#include <QFileDialog>
#include <QMdiSubWindow>
#include <QMessageBox>
#include <QMenuBar>
#include <QResizeEvent>

#include "Config.h"
#include "TrWidget.h"
#include "MainWindow.h"
#include "FindDialog.h"
#include "TrWizard.h"
#include "ExcelWriteWorker.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent),
      m_mdiArea(new QMdiArea),
      m_findDlg(NULL),
      m_wizard(NULL),
      m_progress(NULL)
{
    setWindowTitle("Translator for Qt Application");
    setMinimumSize(500, 700);

    createActions();
    createMenus();
    createToolBars();
    createStatusBar();

    setCentralWidget(m_mdiArea);

    statusBar()->showMessage(tr("Welcome to use translator!"), 2000);
    connect(m_mdiArea, SIGNAL(subWindowActivated(QMdiSubWindow *)),
            this, SLOT(slotSubWindowActivated(QMdiSubWindow *)));

    m_writeWorker = new ExcelWriteWorker;
    m_writeWorker->moveToThread(&m_writeThread);

    connect(&m_writeThread, SIGNAL(finished()), m_writeWorker, SLOT(deleteLater()));
    connect(this, SIGNAL(sigStartToSaveAs(QString)), m_writeWorker, SLOT(slotStartToSave(QString)));
    connect(this, SIGNAL(sigStopToSaveAs()), m_writeWorker, SLOT(slotStopToSave()));
    connect(m_writeWorker, SIGNAL(sigProgressIncrease()), this, SLOT(increaseProgress()));
    connect(m_writeWorker, SIGNAL(sigFinished()), this, SLOT(stopProgress()));
}

MainWindow::~MainWindow()
{
    m_writeThread.terminate();
    m_writeThread.wait();
}

void MainWindow::closeEvent(QCloseEvent *evt)
{
    Q_UNUSED(evt);
    if(!hasFileUnsaved())
    {
        m_mdiArea->closeAllSubWindows();
        return;
    }

    if(QMessageBox::Yes == QMessageBox::warning(this,
                                               "File changed",
                                               "<p>Some files have been changed."
                                               "<p>Do you want to save them?",
                                               QMessageBox::Yes, QMessageBox::No))
    {
        qDebug() << "Save all files.";
        slotSaveAllFile();
    }else{
        TrWidget *widget;
        QMdiSubWindow *subwnd;

        for(int i = 0; i < m_mdiArea->subWindowList().size(); i++)
        {
            subwnd = m_mdiArea->subWindowList().at(i);
            widget = qobject_cast<TrWidget *>(subwnd->widget());
            widget->slotSaveFile(false);
        }
    }
    m_mdiArea->closeAllSubWindows();
}

void MainWindow::resizeEvent(QResizeEvent *evt)
{
    if(m_progress)
    {
        m_progress->resize(evt->size().width(), 30);
    }
}

void MainWindow::slotOpenFile()
{
    qDebug() << "slotOpenFile!";
    QString tsFilter("language(*.ts)");
    m_tsFiles = QFileDialog::getOpenFileNames(this,
                                              tr("open ts file"),
                                              "D:\\vmshare\\workplace\\Prism\\ui\\src\\resources\\languages",
                                              "language(*.ts)",
                                              &tsFilter,
                                              QFileDialog::ReadOnly);
    if(m_tsFiles.isEmpty()) return;

    for(int i = 0; i < m_tsFiles.size(); i++)
    {
        qDebug() << m_tsFiles.at(i);
        TrWidget *widget = new TrWidget;
        widget->setCache(&m_langCache);
        widget->setStatusBar(m_lacation, m_class, m_nrRows);
        connect(widget, SIGNAL(sigHasSomethingChanged()),
                this, SLOT(slotHasSomethingChanged()));

        if(!widget->openFile(m_tsFiles.at(i)))
        {
            qDebug() << "open "<< m_tsFiles.at(i) << " failed.";
        }
        QMdiSubWindow *subwnd = new QMdiSubWindow;
        subwnd->setWidget(widget);
        subwnd->setAttribute(Qt::WA_DeleteOnClose);
        m_mdiArea->addSubWindow(subwnd);
        subwnd->show();
    }

    if(!m_wizard){
        m_wizard = new TrWizard(&m_langCache, this);
        connect(m_wizard, SIGNAL(finished(int)), this, SLOT(slotWizardClosed(int)));

        m_wizard->setLanguageFiles(m_tsFiles);
        m_wizard->show();
        m_wizard->raise();
        m_wizard->activateWindow();
    }
}

void MainWindow::slotSaveFile()
{
    qDebug() << "slotSaveFile!";
    QMdiSubWindow *subwnd = m_mdiArea->activeSubWindow();
    TrWidget *widget = qobject_cast<TrWidget *>(subwnd->widget());
    widget->slotSaveFile(true);
    m_saveAction->setEnabled(!widget->isSaved());
    m_saveAllAction->setEnabled(hasFileUnsaved());
}

void MainWindow::slotSaveAllFile()
{
    TrWidget *widget;
    QMdiSubWindow *subwnd;

    for(int i = 0; i < m_mdiArea->subWindowList().size(); i++)
    {
        subwnd = m_mdiArea->subWindowList().at(i);
        widget = qobject_cast<TrWidget *>(subwnd->widget());
        widget->slotSaveFile(true);
    }
    m_saveAction->setEnabled(false);
    m_saveAllAction->setEnabled(false);
}

void MainWindow::slotSaveAs()
{
    QString fileName;
    fileName = QFileDialog::getSaveFileName(this,
                                            tr("Save a file"),
                                            "D:\\vmshare\\workplace",
                                            "excel(*.xlsx);;"
                                            "excel(*.xls);;"
                                            "language(*.ts)");

    qDebug() << "slotSaveAs:" << fileName;

    if(fileName.isNull() || fileName.isEmpty()) return;

    if(QFileInfo(fileName).suffix() == QString("ts"))
    {
        qDebug() << "will be stored into ts file.";
        saveAsTs(fileName);
    }else{
        startProgress(0, nrItemsSaveAsExcel());
        saveAsExcel(fileName);
        //m_saveExcelThread = new SaveExcelThread;
        //m_saveExcelThread->setWorker(this, fileName);
    }
}

void MainWindow::slotTranslate()
{
    TrWidget *widget;
    QMdiSubWindow *subwnd;

    m_trAction->setEnabled(false);

    qDebug() << "number:"<< m_mdiArea->subWindowList().size();
    for(int i = 0; i < m_mdiArea->subWindowList().size(); i++)
    {
        subwnd = m_mdiArea->subWindowList().at(i);
        widget = qobject_cast<TrWidget *>(subwnd->widget());
        widget->slotTranslate();
    }
}

void MainWindow::slotShowUntranslated(bool isUnfinished)
{
    qDebug() << "Untranslated:" << isUnfinished;
    if(isUnfinished){
        m_showUnfinished->setText(tr("&Display all(Ctrl+D)"));
    }else{
        m_showUnfinished->setText(tr("&Display unfinished(Ctrl+D)"));
    }
    if(hasSubWindow()){
        QMdiSubWindow *subwnd = m_mdiArea->activeSubWindow();
        TrWidget *widget = qobject_cast<TrWidget *>(subwnd->widget());
        widget->slotUntranslated(isUnfinished);
    }
}

void MainWindow::slotFindString()
{
    if(!m_findDlg){
        m_findDlg = new FindDialog(this);
    }
    m_findDlg->show();
    m_findDlg->raise();
    m_findDlg->activateWindow();

    bindFindDialog();
}

void MainWindow::slotSelectionChanged()
{
    qDebug() << "slotSelectionChanged";
}

void MainWindow::slotCopy()
{
    TrWidget *widget = activeSubWindow();
    if(widget){
        widget->slotCopy();
        m_pasteAction->setEnabled(true);
    }
}

void MainWindow::slotCut()
{
    TrWidget *widget = activeSubWindow();
    if(widget){
        widget->slotCut();
        m_pasteAction->setEnabled(true);
    }
}

void MainWindow::slotPaste()
{
    TrWidget *widget = activeSubWindow();
    if(widget){
        widget->slotPaste();
    }
}

void MainWindow::slotDel()
{
    TrWidget *widget = activeSubWindow();
    if(widget){
        widget->slotDel();
    }
}

void MainWindow::slotAbout()
{
    QMessageBox::about(this, tr("About LangTool"),
                       tr("<h2>LangTool V1.0</h2"
                          "<p>Copyright &copy; cruiser_ysw"
                          "<p>LangTool is designed for translators, which same as "
                          "Qt's linguist but it is more efficient for translators "
                          "and software designers."));
}

void MainWindow::slotHasSomethingChanged()
{
    m_saveAction->setEnabled(true);
    m_saveAllAction->setEnabled(true);
}

void MainWindow::slotSubWindowActivated(QMdiSubWindow *sub)
{
    Q_UNUSED(sub);

    qDebug() << "slotSubWindowActivated, sub window count="
             << m_mdiArea->subWindowList().size();

    bindFindDialog();

    if(hasSubWindow())
    {
        if(m_wizard && m_wizard->isVisible()) return;

        TrWidget *widget = qobject_cast<TrWidget *>(sub->widget());
        m_saveAction->setEnabled(!widget->isSaved());
        m_saveAllAction->setEnabled(hasFileUnsaved());
        m_saveAsAction->setEnabled(true);

        m_trAction->setEnabled(!widget->isTranslated());
        m_showUnfinished->setEnabled(true);
        m_showUnfinished->setChecked(widget->untranslated());
        m_findAction->setEnabled(true);

        m_cutAction->setEnabled(true);
        m_copyAction->setEnabled(true);
        m_delAction->setEnabled(true);
        widget->showRowCount();
    }else{
        m_saveAction->setEnabled(false);
        m_saveAllAction->setEnabled(false);
        m_saveAsAction->setEnabled(false);
        m_trAction->setEnabled(false);
        m_showUnfinished->setChecked(false);
        m_showUnfinished->setEnabled(false);
        m_findAction->setEnabled(false);
        m_cutAction->setEnabled(false);
        m_copyAction->setEnabled(false);
        m_pasteAction->setEnabled(false);
        m_delAction->setEnabled(false);
        m_nrRows->setText("");
    }
}

void MainWindow::slotWizardClosed(int result)
{
    qDebug() << "slotWizardClosed, result:" << result;
    if(result)
    {
        m_trAction->setEnabled(true);
    }

    m_wizard->close();

    delete m_wizard;
    m_wizard = NULL;
}

void MainWindow::startProgress(int min, int max)
{
    qDebug("slotSetProgressRange(%d, %d).\n", min, max);
    //overrideWindowFlags(windowFlags() &~ Qt::WindowCloseButtonHint);

    m_progress = new QProgressBar(this);
    m_progress->lower();
    m_progress->setVisible(true);
    m_progress->setGeometry(0, 20, width(), 30);
    m_progress->setTextVisible(false);;
    m_progress->setRange(min, max);
    m_progress_value = 0;
}

void MainWindow::stopProgress()
{
    if(m_progress)
    {
        m_progress->setVisible(false);
        delete m_progress;
        m_progress = NULL;
    }
}

void MainWindow::increaseProgress()
{
    m_progress_value++;
    //qDebug("slotProgressIncrease(%d).\n", m_progress_value);
    if(m_progress)
    {
        m_progress->setValue(m_progress_value);
    }
}

bool MainWindow::hasSubWindow()
{
    return (bool)m_mdiArea->subWindowList().size();
}

int MainWindow::countOfSubWindow()
{
    return m_mdiArea->subWindowList().size();
}

void MainWindow::createActions()
{
    m_openAction = new QAction(tr("&Open"), this);
    m_openAction->setIcon(QIcon("://icons/open.png"));
    m_openAction->setShortcut(QKeySequence::Open);
    connect(m_openAction, SIGNAL(triggered()), this, SLOT(slotOpenFile()));

    m_saveAction = new QAction(tr("&Save"), this);
    m_saveAction->setIcon(QIcon("://icons/save.png"));
    m_saveAction->setShortcut(QKeySequence::Save);
    m_saveAction->setEnabled(false);
    connect(m_saveAction, SIGNAL(triggered()), this, SLOT(slotSaveFile()));

    m_saveAllAction = new QAction(tr("&Save All"), this);
    m_saveAllAction->setIcon(QIcon("://icons/saveall.png"));
    m_saveAllAction->setShortcut(QKeySequence("Ctrl+Shift+S"));
    m_saveAllAction->setEnabled(false);
    connect(m_saveAllAction, SIGNAL(triggered()), this, SLOT(slotSaveAllFile()));

    m_saveAsAction = new QAction(tr("&Save As..."), this);
    //m_saveAsAction->setIcon(QIcon("://icons/saveall.png"));
    //m_saveAsAction->setShortcut(QKeySequence("Ctrl+Shift+S"));
    m_saveAsAction->setEnabled(false);
    connect(m_saveAsAction, SIGNAL(triggered()), this, SLOT(slotSaveAs()));

    m_closeAction = new QAction(tr("&Close"), this);
    m_closeAction->setShortcut(QKeySequence("Ctrl+Q"));
    connect(m_closeAction, SIGNAL(triggered()), this, SLOT(close()));

    m_trAction = new QAction(tr("&Translate"), this);
    m_trAction->setIcon(QIcon("://icons/translation.png"));
    m_trAction->setShortcut(QKeySequence("F5"));
    m_trAction->setEnabled(false);
    connect(m_trAction, SIGNAL(triggered()), this, SLOT(slotTranslate()));

    m_showUnfinished = new QAction(tr("&Display unfinished"), this);
    m_showUnfinished->setIcon(QIcon("://icons/all.png"));
    m_showUnfinished->setShortcut(QKeySequence("Ctrl+D"));
    m_showUnfinished->setEnabled(false);
    m_showUnfinished->setCheckable(true);
    connect(m_showUnfinished, SIGNAL(toggled(bool)), this, SLOT(slotShowUntranslated(bool)));

    m_findAction = new QAction(tr("&Find"), this);
    m_findAction->setIcon(QIcon("://icons/find.png"));
    m_findAction->setShortcut(QKeySequence("Ctrl+F"));
    m_findAction->setEnabled(false);
    connect(m_findAction, SIGNAL(triggered()), this, SLOT(slotFindString()));

    m_copyAction = new QAction(tr("&Copy"), this);
    m_copyAction->setIcon(QIcon("://icons/copy.png"));
    m_copyAction->setShortcut(QKeySequence("Ctrl+C"));
    m_copyAction->setEnabled(false);
    connect(m_copyAction, SIGNAL(triggered()), this, SLOT(slotCopy()));

    m_pasteAction = new QAction(tr("&Paste"), this);
    m_pasteAction->setIcon(QIcon("://icons/paste.png"));
    m_pasteAction->setShortcut(QKeySequence("Ctrl+V"));
    m_pasteAction->setEnabled(false);
    connect(m_pasteAction, SIGNAL(triggered()), this, SLOT(slotPaste()));

    m_cutAction = new QAction(tr("&Cut"), this);
    m_cutAction->setIcon(QIcon("://icons/cut.png"));
    m_cutAction->setShortcut(QKeySequence("Ctrl+X"));
    m_cutAction->setEnabled(false);
    connect(m_cutAction, SIGNAL(triggered()), this, SLOT(slotCut()));

    m_delAction = new QAction(tr("&Delete"), this);
    m_delAction->setShortcut(QKeySequence("Del"));
    m_delAction->setIcon(QIcon("://icons/delete.png"));
    m_delAction->setEnabled(false);
    connect(m_delAction, SIGNAL(triggered()), this, SLOT(slotDel()));

    m_aboutAction = new QAction(tr("&About"), this);
    m_aboutAction->setIcon(QIcon("://icons/about.png"));
    m_aboutAction->setEnabled(true);
    connect(m_aboutAction, SIGNAL(triggered()), this, SLOT(slotAbout()));
}

void MainWindow::createMenus()
{
    m_fileMenu = menuBar()->addMenu(tr("&File"));
    m_fileMenu->addAction(m_openAction);
    m_fileMenu->addSeparator();
    m_fileMenu->addAction(m_saveAction);
    m_fileMenu->addAction(m_saveAllAction);
    m_fileMenu->addSeparator();
    m_fileMenu->addAction(m_saveAsAction);
    m_fileMenu->addSeparator();
    m_fileMenu->addAction(m_closeAction);

    m_editMenu = menuBar()->addMenu(tr("&Edit"));
    m_editMenu->addAction(m_cutAction);
    m_editMenu->addAction(m_copyAction);
    m_editMenu->addAction(m_pasteAction);
    m_editMenu->addSeparator();
    m_editMenu->addAction(m_delAction);

    m_toolMenu = menuBar()->addMenu(tr("&Tools"));
    m_toolMenu->addAction(m_trAction);
    m_toolMenu->addAction(m_findAction);
    m_toolMenu->addAction(m_showUnfinished);

    m_helpMenu = menuBar()->addMenu(tr("&Help"));
    m_helpMenu->addAction(m_aboutAction);
}

void MainWindow::createToolBars()
{
    m_fileToolBar = addToolBar(tr("&File"));
    m_fileToolBar->addAction(m_openAction);
    m_fileToolBar->addAction(m_saveAction);
    m_fileToolBar->addAction(m_saveAllAction);
    m_fileToolBar->addSeparator();
    m_fileToolBar->addAction(m_trAction);
    m_fileToolBar->addSeparator();
    m_fileToolBar->addAction(m_showUnfinished);
    m_fileToolBar->addAction(m_findAction);
    m_fileToolBar->addSeparator();
    m_fileToolBar->addAction(m_cutAction);
    m_fileToolBar->addAction(m_copyAction);
    m_fileToolBar->addAction(m_pasteAction);
    m_fileToolBar->addAction(m_delAction);
    m_fileToolBar->addSeparator();
    m_fileToolBar->addAction(m_aboutAction);
    m_fileToolBar->setFixedHeight(HEIGHT_OF_TOOLBAR);
}

void MainWindow::createStatusBar()
{
    m_lacation = new QLabel;
    m_lacation->setFixedHeight(20);
    m_lacation->setIndent(4);
    m_lacation->setFrameStyle(QFrame::WinPanel | QFrame::Sunken);

    m_class = new QLabel;
    m_class->setFixedHeight(20);
    m_class->setMinimumWidth(150);
    m_class->setFrameStyle(QFrame::WinPanel | QFrame::Sunken);

    m_nrRows = new QLabel;
    m_nrRows->setFixedHeight(20);
    m_nrRows->setMinimumWidth(80);
    m_nrRows->setFrameStyle(QFrame::WinPanel | QFrame::Sunken);

    statusBar()->addWidget(m_lacation, 1);
    statusBar()->addPermanentWidget(m_class);
    statusBar()->addPermanentWidget(m_nrRows);
}

bool MainWindow::hasFileUntranslated()
{
    TrWidget *trwiget;
    QMdiSubWindow *subwnd;

    for(int i = 0; i < m_mdiArea->subWindowList().size(); i++)
    {
        subwnd = m_mdiArea->subWindowList().at(i);
        trwiget  = qobject_cast<TrWidget *>(subwnd->widget());
        if(!trwiget->isTranslated())
        {
            return true;
        }
    }
    return false;
}

bool MainWindow::hasFileUnsaved()
{
    TrWidget *trwiget;
    QMdiSubWindow *subwnd;

    for(int i = 0; i < m_mdiArea->subWindowList().size(); i++)
    {
        subwnd = m_mdiArea->subWindowList().at(i);
        trwiget  = qobject_cast<TrWidget *>(subwnd->widget());
        if(!trwiget->isSaved())
        {
            return true;
        }
    }
    return false;
}

void MainWindow::bindFindDialog()
{
    if(m_findDlg && hasSubWindow())
    {
        m_findDlg->disconnect();
        QMdiSubWindow *subwnd = m_mdiArea->activeSubWindow();
        TrWidget *widget = qobject_cast<TrWidget *>(subwnd->widget());
        connect(m_findDlg, SIGNAL(findNext(QString,Qt::CaseSensitivity)),
                widget, SLOT(slotFindNext(QString,Qt::CaseSensitivity)));
        connect(m_findDlg, SIGNAL(findPrev(QString,Qt::CaseSensitivity)),
                widget, SLOT(slotFindPrev(QString,Qt::CaseSensitivity)));
    }
}

void MainWindow::saveAsExcel(const QString &file)
{
    m_writeThread.start();
    emit sigStartToSaveAs(file);

    QMdiSubWindow *subwnd = NULL;
    TrWidget    *widget = NULL;
    for(int i = 0; i < m_mdiArea->subWindowList().size(); i++)
    {
        subwnd = m_mdiArea->subWindowList().at(i);
        widget = qobject_cast<TrWidget *>(subwnd->widget());
        connect(widget, SIGNAL(sigSaveCellDataAsExcel(int,int,QString)),
                m_writeWorker, SLOT(slotSaveContentsToExcel(int,int,QString)));
        if(i == 0){
            widget->saveAsExcel(1, true);
            widget->saveAsExcel(2, false);
        }else{
            widget->saveAsExcel(i+2, false);
        }

        disconnect(widget, SIGNAL(sigSaveCellDataAsExcel(int,int,QString)),
                   m_writeWorker, SLOT(slotSaveContentsToExcel(int,int,QString)));
    }

    emit sigStopToSaveAs();
}

void MainWindow::saveAsTs(const QString &file)
{
    //qDebug() << "MainWindow::slotSaveAs:" << file;
    QMdiSubWindow *subwnd = m_mdiArea->currentSubWindow();
    TrWidget *widget = qobject_cast<TrWidget *>(subwnd->widget());
    widget->slotSaveAs(file);
    //m_saveAction->setEnabled(!widget->isSaved());
    //m_saveAllAction->setEnabled(hasFileUnsaved());
}

int MainWindow::nrItemsSaveAsExcel()
{
    int nrItems = 0;
    QMdiSubWindow *subwnd = NULL;
    TrWidget    *widget = NULL;
    for(int i = 0; i < m_mdiArea->subWindowList().size(); i++)
    {
        subwnd = m_mdiArea->subWindowList().at(i);
        widget = qobject_cast<TrWidget *>(subwnd->widget());
        if(i == 0)
        {
            nrItems += widget->rowCount() + 1;// add column header
            nrItems += widget->rowCount() + 1;// add column header
        }else{
            nrItems += widget->rowCount() + 1;// add column header
        }
    }

    return nrItems;
}

TrWidget *MainWindow::activeSubWindow()
{
    TrWidget *widget = NULL;
    if(hasSubWindow()){
        widget = qobject_cast<TrWidget *>(m_mdiArea->activeSubWindow()->widget());
    }
    return widget;
}

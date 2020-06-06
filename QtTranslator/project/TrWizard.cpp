#include <QLabel>
#include <QVBoxLayout>
#include <QLineEdit>
#include <QPushButton>
#include <QFileDialog>
#include <QTableWidget>
#include <QHeaderView>
#include <QProgressBar>
#include <QDebug>
#include <QThread>
#include "ExcelEngine.h"
#include "TrWizard.h"
#include "FileWorker.h"

TrWizard::TrWizard(QList<LgCache> *cache, QWidget *parent) :
    QWizard(parent),
    m_table(NULL),
    m_progress(NULL),
    m_langCache(cache)
{
    setWindowTitle("Import xls file");
    addPage(createIntroductionPage());
    addPage(createSelectXlsFilePage());
    addPage(createLoadFilePage());

    connect(this, SIGNAL(currentIdChanged(int)), this, SLOT(slotPageIdChanged(int)));

    m_worker = new FileWorker(m_langCache);
    m_worker->moveToThread(&m_workerThread);
    connect(&m_workerThread, SIGNAL(finished()),
            m_worker, SLOT(deleteLater()));
    /*to read header*/
    connect(this, SIGNAL(sigStartToReadHeader(QString)),
            m_worker, SLOT(slotReadHeader(QString)));
    connect(m_worker, SIGNAL(sigReadHeader(QString, int)),
            this, SLOT(slotReadHeader(QString, int)));
    connect(m_worker, SIGNAL(sigReadHeaderDone()),
            this, SLOT(slotReadHeaderDone()));
    /*to cache texts*/
    connect(this, SIGNAL(sigStartToCacheTexts()),
            m_worker, SLOT(slotCacheTexts()));
    connect(m_worker, SIGNAL(sigCacheTextsDone()),
            this, SLOT(slotCacheTextsDone()));

    /*connect progress bar*/
    connect(m_worker, SIGNAL(sigProgressRange(int,int)),
            this, SLOT(slotProgressRange(int,int)));
    connect(m_worker, SIGNAL(sigProgressValue(int)),
            this, SLOT(slotProgressValue(int)));
}

TrWizard::~TrWizard()
{
    m_workerThread.terminate();
    m_workerThread.wait();
}

void TrWizard::setLanguageFiles(const QStringList &files)
{
    m_table->setRowCount(files.size() + 1);
    QTableWidgetItem *item = new QTableWidgetItem("English");
    item->setFlags(item->flags() & (~Qt::ItemIsEditable));
    m_table->setItem(0, 0, item);
    for(int i = 0; i < files.size(); i++)
    {
        QString file = files.at(i);
        item = new QTableWidgetItem(QFileInfo(file).baseName());
        item->setFlags(item->flags() & (~Qt::ItemIsEditable));
        m_table->setItem(i + 1, 0, item);
    }
}

QWizardPage *TrWizard::createIntroductionPage()
{
    QWizardPage *page = new QWizardPage;
    page->setTitle("Prepare translated texts");
    QLabel *label = new QLabel(tr("Please put all translated texts into a excel file"
                               " and arrage them as following table, one language"
                               " one column."));
    label->setWordWrap(true);

    QLabel *pic = new QLabel;
    pic->setPixmap(QPixmap("://icons/introduction.png"));

    QVBoxLayout *layout = new QVBoxLayout;
    layout->addWidget(label);
    layout->addWidget(pic);
    page->setLayout(layout);
    return page;
}

QWizardPage *TrWizard::createSelectXlsFilePage()
{
    QWizardPage *page = new QWizardPage;
    page->setTitle("Choose a xls file");

    QLabel   *label = new QLabel(tr("File:"));
    m_file = new QLineEdit();
    label->setBuddy(m_file);

    QPushButton *button = new QPushButton(tr("Browse..."));
    connect(button, SIGNAL(clicked()), this, SLOT(slotSelectXlsFile()));

    QHBoxLayout *hLayout = new QHBoxLayout;
    hLayout->addWidget(label);
    hLayout->addWidget(m_file);
    hLayout->addWidget(button);

    QFrame *line = new QFrame;
    line->setObjectName(QStringLiteral("line"));
    line->setFrameShape(QFrame::HLine);
    line->setFrameShadow(QFrame::Sunken);

    QLabel *info = new QLabel(tr("Please assign a colomn number (in digit) to every "
                                 "language in the follow table."));
    info->setWordWrap(true);
    QFont font;
    font.setPixelSize(12);
    font.setBold(true);
    info->setFont(font);

    m_table = new QTableWidget;
    m_table->setColumnCount(2);
    m_table->setRowCount(2);
    m_table->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    QStringList header;
    header << tr("Language") << tr("Column Number");
    m_table->setHorizontalHeaderLabels(header);

    m_table->horizontalHeaderItem(0)->setFont(font);
    m_table->horizontalHeaderItem(1)->setFont(font);
    //m_table->horizontalHeader()->setVisible(false);
    //m_table->verticalHeader()->setVisible(false);

    QVBoxLayout *vLayout = new QVBoxLayout;
    vLayout->addLayout(hLayout);
    vLayout->addSpacing(2);
    vLayout->addWidget(line);
    vLayout->addWidget(info);
    vLayout->addSpacing(2);
    vLayout->addWidget(m_table);
    page->setLayout(vLayout);

    return page;
}

QWizardPage *TrWizard::createLoadFilePage()
{
    QWizardPage *page = new QWizardPage;
    page->setTitle("Load xls file");
    m_progress = new QProgressBar;
    m_progress->setTextVisible(false);
    m_progress->setValue(0);

    QVBoxLayout *vLayout = new QVBoxLayout;
    vLayout->addWidget(m_progress);
    page->setLayout(vLayout);
    return page;
}

void TrWizard::slotPageIdChanged(int id)
{
    qDebug() << "slotPageIdChanged:" << id;
    switch(id)
    {
    case 0:
        break;
    case 1:
        button(QWizard::NextButton)->setEnabled(false);
        break;
    case 2:
        button(QWizard::FinishButton)->setEnabled(false);
        for(int i = 0; i < m_table->rowCount(); i++)
        {
            LgCache cache;
            cache.setName(m_table->item(i, 0)->text());
            cache.setColumnId(m_table->item(i, 1)->text().toInt());
            m_langCache->append(cache);
        }

        qDebug() << m_langCache->size() << " language to read!";
        startToCacheTexts();
        break;
    default:
        for(int lang = 0; lang < m_langCache->size(); lang++)
        {
            LgCache cache = m_langCache->at(lang);
            qDebug() << "Language:" << cache.name();
            for(int i = 0; i < cache.size(); i++)
            {
                qDebug()<< cache.at(i);
            }
        }
        break;
    }
}

void TrWizard::slotSelectXlsFile()
{
    /*Open xls file*/
    QString xlsFilter("excel(*.xlsx; *.xls)");
    QString xlsFile = QFileDialog::getOpenFileName(this,
                                                   tr("open xls file"),
                                                   "D:\\language",
                                                   "excel(*.xlsx; *.xls)",
                                                   &xlsFilter,
                                                   QFileDialog::ReadOnly);
    if(xlsFile.isNull() || xlsFile.isEmpty())
    {
        return;
    }
    m_file->setText(xlsFile);

    startToReadHeader(xlsFile);
}

void TrWizard::slotReadHeader(QString name, int col)
{
    //qDebug() << name << ":" << col;
    HeaderCache cache;
    cache.name = name;
    cache.col  = col;
    m_headerCache.append(cache);
}

void TrWizard::slotReadHeaderDone()
{
    qDebug() << "slotReadHeaderDone";
    QString langName;
    QTableWidgetItem *item = NULL;
    for(int i = 0; i < m_table->rowCount(); i++)
    {
        langName = m_table->item(i, 0)->text();
        for(int j = 0; j < m_headerCache.size(); j++)
        {
            if(langName == m_headerCache.at(j).name){
                item = new QTableWidgetItem(QString::number(m_headerCache.at(j).col));
                m_table->setItem(i, 1, item);
                break;
            }
        }
    }

    button(QWizard::NextButton)->setEnabled(true);
}

void TrWizard::slotCacheTextsDone()
{
    qDebug("slotCacheTextsDone\n");
    emit sigLoadFileFinished();
    button(QWizard::FinishButton)->setEnabled(true);
}

void TrWizard::slotProgressRange(int min, int max)
{
    //qDebug("slotProgressRange:%d, %d \n", min, max);
    m_progress->setRange(min, max);
}

void TrWizard::slotProgressValue(int v)
{
    //qDebug("slotProgressValue:%d\n", v);
    m_progress->setValue(v);
}

void TrWizard::startToReadHeader(const QString &file)
{
    qDebug() << "startToReadHeader:" << QThread::currentThreadId();

    m_workerThread.start();

    emit sigStartToReadHeader(file);
}

void TrWizard::startToCacheTexts()
{
    qDebug() << "startToCacheTexts:" << QThread::currentThreadId();
    emit sigStartToCacheTexts();
}


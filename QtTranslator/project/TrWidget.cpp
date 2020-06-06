#include <QApplication>
#include <QClipboard>
#include <QFileDialog>
#include <QTableWidget>
#include <QResizeEvent>
#include <QHBoxLayout>
#include <QHeaderView>
#include <QMessageBox>
#include "TsEngine.h"
#include "TrWidget.h"

TrWidget::TrWidget(QWidget *parent) :
    QWidget(parent),
    m_table(new QTableWidget(this)),
    m_parser(new TsEngine(m_table)),
    m_isTranslated(false),
    m_isSaved(true),
    m_isForceQuit(false)
{
    setMinimumSize(200, 300);
    m_parser->setLanguage("Target Language", "English");

    connect(m_parser, SIGNAL(sigHasSomethingChanged()),
            this, SLOT(slotHasSomethingChanged()));

    QFont font("DejaVu Sans");
    font.setPixelSize(15);
    font.setBold(true);
    m_table->horizontalHeaderItem(0)->setFont(font);
    m_table->horizontalHeaderItem(1)->setFont(font);
    m_table->setSelectionMode(QAbstractItemView::ContiguousSelection);
}

TrWidget::~TrWidget()
{
}

void TrWidget::resizeEvent(QResizeEvent *evt)
{
    //qDebug() << evt->size();
    m_table->resize(evt->size());
}

void TrWidget::closeEvent(QCloseEvent *evt)
{
    Q_UNUSED(evt);
    qDebug() << "TrWidget::closeEvent";
    if(m_isForceQuit || m_isSaved) return;
    int ret;
    ret = QMessageBox::warning(this, "File changed", "<p>This file has been changed."
                         "<p>Do you want to save it?", QMessageBox::Yes, QMessageBox::No);

    if(ret == QMessageBox::Yes)
    {
        qDebug() << "QMessageBox::Yes";
        slotSaveFile(true);
    }else{
        qDebug() << "QMessageBox::No";
    }
}

bool TrWidget::openFile(const QString &filename)
{
    if(filename.isNull() || filename.isEmpty())
    {
        return false;
    }
    m_tsFile = filename;
    setWindowTitle(filename);
    setObjectName(QFileInfo(filename).baseName());
    if(!m_parser->openFile(filename))
    {
        qCritical() << "ERROR:m_parser->openFile:" << filename;
        return false;
    }

    m_parser->setLanguage(QFileInfo(filename).baseName(), "English");
    return m_parser->readFile();
}

void TrWidget::setCache(QList<LgCache> *cache)
{
    m_parser->setCache(cache);
}

void TrWidget::setStatusBar(QLabel *location, QLabel *classname, QLabel *nrRows)
{
    m_parser->setStatusBar(location, classname, nrRows);
}

bool TrWidget::isTranslated()
{
    return m_isTranslated;
}

bool TrWidget::isSaved()
{
    return m_isSaved;
}

int TrWidget::rowCount()
{
    return m_table->rowCount();
}

void TrWidget::showRowCount()
{
    m_parser->showRowCount();
}

bool TrWidget::untranslated()
{
    return m_parser->untranslated();
}

void TrWidget::slotSaveFile(bool isSave)
{
    m_isForceQuit = !isSave;
    if(m_isForceQuit) return;
    if(m_parser->saveFile())
    {
        m_isSaved = true;
    }else{
        m_isSaved = false;
    }
}

void TrWidget::slotSaveAs(const QString &fileName)
{
    //qDebug() << "TrWidget::slotSaveAs:" << fileName;
    if(m_parser->saveAs(fileName))
    {
        m_isSaved = true;
    }else{
        m_isSaved = false;
    }
}

void TrWidget::saveAsExcel(int col, bool isNeedSrc)
{
    QString data;
    if(isNeedSrc){
        emit sigSaveCellDataAsExcel(1, col, "English");
    }else{
        emit sigSaveCellDataAsExcel(1, col, QFileInfo(m_tsFile).baseName());
    }

    for(int row = 0; row < m_table->rowCount(); row++)
    {
        if(isNeedSrc){
            data = m_table->item(row, 0)->text();//src language;
        }else{
            data = m_table->item(row, 1)->text();//target language;
        }
        emit sigSaveCellDataAsExcel((row + 2), col, data);
    }
}

void TrWidget::saveTextsAsExcel(ExcelEngine &excel, int col, bool isNeedSrc)
{
    m_parser->saveTextsAsExcel(excel, col, isNeedSrc);
}

void TrWidget::slotTranslate()
{
    qDebug() << "Start to translate" << objectName() << "!";
    if(m_isTranslated) return;
    m_isTranslated = true;
    m_isSaved = false;
    m_parser->translate();
}

void TrWidget::slotUntranslated(bool isUnfinished)
{
    qDebug() << "TrWidget::slotUntranslated";
    m_parser->showUntranslated(isUnfinished);
}

void TrWidget::slotHasSomethingChanged()
{
    m_isSaved = false;
    emit sigHasSomethingChanged();
}

void TrWidget::slotFindNext(const QString &str, Qt::CaseSensitivity cs)
{
    qDebug() << "slotFindNext";
    int row = m_table->currentRow() + 1;
    int col = m_table->currentColumn();

    while(col < m_table->columnCount())
    {
        while(row < m_table->rowCount())
        {
            if(m_table->item(row, col)->text().contains(str, cs)){
                m_table->clearSelection();
                m_table->setCurrentCell(row, col);
                activateWindow();
                return;
            }
            row++;
        }
        row = 0;
        col++;
    }

    QApplication::beep();
}

void TrWidget::slotFindPrev(const QString &str, Qt::CaseSensitivity cs)
{
    qDebug() << "slotFindPrev";
    int row = m_table->currentRow() - 1;
    int col = m_table->currentColumn();
    while(col >= 0)
    {
        while(row >= 0)
        {
            if(m_table->item(row, col)->text().contains(str, cs)){
                m_table->clearSelection();
                m_table->setCurrentCell(row, col);
                activateWindow();
                return;
            }
            row--;
        }
        row = m_table->rowCount() - 1;
        col--;
    }

    QApplication::beep();
}

void TrWidget::slotCut()
{
    slotCopy();
    slotDel();
}

void TrWidget::slotCopy()
{
#if 0
    QList<QTableWidgetSelectionRange> ranges = m_table->selectedRanges();
    QString str;
    qDebug() << "slotCopy:" << ranges.size();
    for(int i = 0; i < ranges.size(); i++)
    {
        if(i > 0) str += "\n";
        str += copySelectedRange(ranges.at(i));
    }

    qDebug() << str;
    QApplication::clipboard()->setText(str);
#endif
    QString str;
    QTableWidgetItem *item;
    QTableWidgetSelectionRange range = selectedRange();
    qDebug() << "rowCount:" << range.rowCount();
    qDebug() << "columnCount:" << range.columnCount();
    for(int row = 0; row < range.rowCount(); row++)
    {
        if(row > 0){
            str += "\n";
        }
        for(int col = 0; col < range.columnCount(); col++)
        {
            if(col > 0) str += "\t";
            item = m_table->item(range.topRow() + row, range.leftColumn() + col);
            if(item->text().contains('\n')){
                str += QString("\"%1\"").arg(item->text());
            }else{
                str += item->text();
            }
        }
    }
    qDebug() << str;
    QApplication::clipboard()->setText(str);
}

void TrWidget::slotPaste()
{
    QTableWidgetSelectionRange range = selectedRange();
    QString str = QApplication::clipboard()->text();
    qDebug() << str;
    QStringList rows = str.split('\n');
    int nRows = rows.count();
    int nCols = rows.first().count('\t') + 1;

    for(int i = 0; i < nRows; i++)
    {
        QStringList columns = rows.at(i).split('\t');
        for(int j = 0; j < nCols; j++)
        {
            int row = range.topRow() + i;
            int col = range.leftColumn() + j;
            m_table->item(row, col)->setText(columns.at(j));
        }
    }
}

void TrWidget::slotDel()
{
    QList<QTableWidgetItem *>items = m_table->selectedItems();
    if(!items.isEmpty()){
        for(int i = 0; i < items.size(); i++)
        {
            items.at(i)->setText("");
        }
    }
}

QString TrWidget::copySelectedRange(QTableWidgetSelectionRange range)
{
    QTableWidgetItem *item;
    QString str;
    qDebug() << "rowCount:" << range.rowCount();
    qDebug() << "columnCount:" << range.columnCount();
    for(int row = 0; row < range.rowCount(); row++)
    {
        if(row > 0){
            str += "\n";
        }
        for(int col = 0; col < range.columnCount(); col++)
        {
            if(col > 0) str += "\t";
            item = m_table->item(range.topRow() + row, range.leftColumn() + col);
            str += item->text();
        }
    }
    return str;
}

QTableWidgetSelectionRange TrWidget::selectedRange() const
{
    QList<QTableWidgetSelectionRange>ranges = m_table->selectedRanges();
    if(ranges.isEmpty())
        return QTableWidgetSelectionRange();
    return ranges.first();
}


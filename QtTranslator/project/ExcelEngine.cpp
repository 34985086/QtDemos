#include "ExcelEngine.h"
#include "qt_windows.h"

ExcelEngine::ExcelEngine(QObject *parent):
    QObject(parent),
    m_pExcel(NULL),
    m_pWorkbooks(NULL),
    m_pWorkbook(NULL),
    m_pWorksheet(NULL),
    m_sXlsFile(""),
    m_nCurSheet(0),
    m_bVisible(false),
    m_nRowCount(0),
    m_nColCount(0),
    m_nStartRow(0),
    m_nStartCol(0),
    m_bOpened(false),
    m_bValid(false),
    m_bNewFile(false),
    m_bSaved(false)
{
    HRESULT r = OleInitialize(0);
    if (r != S_OK && r != S_FALSE){
        qDebug("Qt: Could not initialize OLE (error %x)", (unsigned int)r);
    }
}

ExcelEngine::~ExcelEngine()
{
    if ( m_bOpened ){
        close();
    }
    OleUninitialize();
}

bool ExcelEngine::open(int nSheet, bool visible)
{
    if ( m_bOpened ){
        close();
    }

    m_nCurSheet = nSheet;
    m_bVisible  = visible;

    if( NULL == m_pExcel ){
        m_pExcel = new QAxObject("Excel.Application");
        if ( m_pExcel ){
            m_bValid = true;
        }else{
            m_bValid = false;
            m_bOpened  = false;
            return m_bOpened;
        }
        m_pExcel->dynamicCall("SetVisible(bool)", m_bVisible);
    }

    if ( !m_bValid ){
        m_bOpened  = false;
        return m_bOpened;
    }

    if ( m_sXlsFile.isEmpty() ){
        m_bOpened  = false;
        return m_bOpened;
    }

    QFile f(m_sXlsFile);
    if (!f.exists()){
        m_bNewFile = true;
    }else{
        m_bNewFile = false;
    }

    if (!m_bNewFile){
        m_pWorkbooks = m_pExcel->querySubObject("WorkBooks");
        m_pWorkbook  = m_pWorkbooks->querySubObject("Open(QString, QVariant)",
                                                    m_sXlsFile, QVariant(0));
    }else{
        m_pWorkbooks = m_pExcel->querySubObject("WorkBooks");
        m_pWorkbooks->dynamicCall("Add");
        m_pWorkbook  = m_pExcel->querySubObject("ActiveWorkBook");
    }
    m_pWorksheet = m_pWorkbook->querySubObject("WorkSheets(int)", m_nCurSheet);

    QAxObject *usedrange = m_pWorksheet->querySubObject("UsedRange");
    QAxObject *rows      = usedrange->querySubObject("Rows");
    QAxObject *columns   = usedrange->querySubObject("Columns");

    m_nStartRow          = usedrange->property("Row").toInt();
    m_nRowCount          = rows->property("Count").toInt();



    m_nStartCol          = usedrange->property("Column").toInt();
    m_nColCount          = columns->property("Count").toInt();

    m_bOpened  = true;
    return m_bOpened;
}


bool ExcelEngine::open(QString xlsFile, int nSheet, bool visible)
{
    m_sXlsFile = xlsFile;
    m_nCurSheet = nSheet;
    m_bVisible = visible;

    return open(m_nCurSheet, m_bVisible);
}
#include <QDir>
void ExcelEngine::save()
{
    if (m_pWorkbook){
        if (m_bSaved){
            return ;
        }

        if (!m_bNewFile){
            m_pWorkbook->dynamicCall("Save()");
        }else{
#if 0
            m_pWorkbook->dynamicCall("SaveAs (const QString&,int,const QString&,const QString&,bool,bool)",
                m_sXlsFile,56,QString(""),QString(""),false,false);
#endif
            m_pWorkbook->dynamicCall("SaveAs(const QString&)",QDir::toNativeSeparators(m_sXlsFile));

        }
        m_bSaved = true;
    }
}


void ExcelEngine::close()
{
    if ( m_pExcel && m_pWorkbook ){
        m_pWorkbook->dynamicCall("Close(bool)", true);
        m_pExcel->dynamicCall("Quit()");

        delete m_pExcel;
        m_pExcel = NULL;

        m_bOpened  = false;
        m_bValid   = false;
        m_bNewFile = false;
        m_bSaved   = true;
    }
}

bool ExcelEngine::setCellData(int row, int column, QVariant data)
{
    bool op = false;

    QAxObject *cell = m_pWorksheet->querySubObject("Cells(int,int)",row,column);
    if ( cell ){
        QString strData = data.toString();
        cell->dynamicCall("SetValue(const QVariant&)",strData);
        op = true;
    }else{
        op = false;
    }
    emit sigProgressIncrease();
    return op;
}

QVariant ExcelEngine::cellData(int row, int column)
{
    QVariant data;

    QAxObject *cell = m_pWorksheet->querySubObject("Cells(int,int)",row,column);
    if ( cell ){
        data = cell->dynamicCall("Value2()");
    }

    return data;
}

bool ExcelEngine::setColor(int row, int column, const QColor &color)
{
    QAxObject *cell = m_pWorksheet->querySubObject("Cells(int,int)",row,column);
    if(cell){
        QAxObject *font = cell->querySubObject("Font");
        font->setProperty("Color", color);
        return true;
    }
    return false;
}

bool ExcelEngine::setBold(int row, int column, bool isBold)
{
    QAxObject *cell = m_pWorksheet->querySubObject("Cells(int,int)",row,column);
    if(cell){
        QAxObject *font = cell->querySubObject("Font");
        font->setProperty("Bold", isBold);
        return true;
    }
    return false;
}

bool ExcelEngine::setFontSize(int row, int column, int size)
{
    QAxObject *cell = m_pWorksheet->querySubObject("Cells(int,int)",row,column);
    if(cell){
        QAxObject *font = cell->querySubObject("Font");
        font->setProperty("Size", size);
        return true;
    }

    return false;
}

bool ExcelEngine::setWidth(int row, int column, int width)
{
    QAxObject *cell = m_pWorksheet->querySubObject("Cells(int,int)",row,column);
    if(cell){
        cell->setProperty("ColumnWidth", width);
        return true;
    }
    return false;
}

bool ExcelEngine::setHeight(int row, int column, int height)
{
    QAxObject *cell = m_pWorksheet->querySubObject("Cells(int,int)",row,column);
    if(cell){
        cell->setProperty("RowHeight", height);
        return true;
    }
    return false;
}

bool ExcelEngine::setAutoWrap(int row, int column, bool enable)
{
    QAxObject *cell = m_pWorksheet->querySubObject("Cells(int,int)",row,column);
    if(cell){
        cell->setProperty("WrapText", enable);
        return true;
    }
    return false;
}

bool ExcelEngine::setBackgroundColor(int row, int column, const QColor &color)
{
    QAxObject *cell = m_pWorksheet->querySubObject("Cells(int,int)",row,column);
    if(cell){
        QAxObject* interior = cell->querySubObject("Interior");
        interior->setProperty("Color", color);
        return true;
    }
    return false;
}

int ExcelEngine::rowCount()const
{
    return m_nRowCount;
}

int ExcelEngine::columnCount()const
{
    return m_nColCount;
}

bool ExcelEngine::isOpen()
{
    return m_bOpened;
}

bool ExcelEngine::isValid()
{
    return m_bValid;
}

void ExcelEngine::clear()
{
    m_sXlsFile  = "";
    m_nRowCount = 0;
    m_nColCount = 0;
    m_nStartRow = 0;
    m_nStartCol = 0;
}


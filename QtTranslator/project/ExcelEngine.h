#ifndef TKEXCELENGINE_H
#define TKEXCELENGINE_H

#include <QObject>
#include <QFile>
#include <QString>
#include <QStringList>
#include <QVariant>
#include <QAxBase>
#include <QAxObject>

#include <QTableWidget>
#include <QTableView>
#include <QTableWidgetItem>
#include <QDebug>
#include <QProgressBar>
#include "LgCache.h"

typedef struct{
    QString src;
    QString dst;
}LangList;

class ExcelEngine : public QObject
{
    Q_OBJECT
public:
    explicit    ExcelEngine(QObject *parent=0);
    virtual     ~ExcelEngine();

public:
    bool        open(int nSheet = 1, bool visible = false);
    bool        open(QString xlsFile, int nSheet = 1, bool visible = false);
    void        save();
    void        close();

    bool        setCellData(int row, int column, QVariant data);
    QVariant    cellData(int row, int column);
    bool        setColor(int row, int column, const QColor &color);
    bool        setBold(int row, int column, bool isBold);
    bool        setFontSize(int row, int column, int size);
    bool        setWidth(int row, int column, int width);
    bool        setHeight(int row, int column, int height);
    bool        setAutoWrap(int row, int column, bool enable);
    bool        setBackgroundColor(int row, int column, const QColor &color);
    int         rowCount()      const;
    int         columnCount()   const;

    bool        isOpen();
    bool        isValid();

signals:
    void        sigProgressIncrease();
protected:
    void        clear();
private:
    QAxObject  *m_pExcel;
    QAxObject  *m_pWorkbooks;
    QAxObject  *m_pWorkbook;
    QAxObject  *m_pWorksheet;

    QString     m_sXlsFile;
    int         m_nCurSheet;
    bool        m_bVisible;
    int         m_nRowCount;
    int         m_nColCount;
    int         m_nStartRow;
    int         m_nStartCol;

    bool        m_bOpened;
    bool        m_bValid;
    bool        m_bNewFile;
    bool        m_bSaved;
};


#endif // TKEXCELENGINE_H

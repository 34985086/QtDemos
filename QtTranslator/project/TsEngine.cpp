#include <QFile>
#include <QDebug>
#include <QLabel>
#include <QStatusBar>
#include <QHeaderView>
#include <QFileInfo>
#include "TsEngine.h"

TsEngine::TsEngine(QTableWidget *table, QObject *parent):
    QObject(parent),
    m_count_row(0),
    m_dstCache(NULL),
    m_srcCache(NULL),
    m_isUnfinished(false)
{
    m_table = table;
    m_table->setColumnCount(2);

    m_idx_src = 0;
    m_idx_tr  = 0;

    connect(this, SIGNAL(sigItemChanged(QTableWidgetItem*)),
            this, SLOT(slotItemChanged(QTableWidgetItem*)));
}

TsEngine::~TsEngine()
{
}

void TsEngine::setCache(QList<LgCache> *cache)
{
    m_cache = cache;
}

bool TsEngine::openFile(const QString &name)
{
    m_tsFile = name;
    QFile file(name);
    if(!file.open(QFile::ReadOnly | QFile::Text))
    {
        qDebug() << QObject::tr("error::openFile(%s)\n") << name;
        return false;
    }

    if(!m_doc.setContent(&file))
    {
        qDebug() << QObject::tr("error::openFile->doc.setContent \n") << name;
        file.close();
        return false;
    }
    file.close();
    return true;
}

bool TsEngine::readFile()
{
    QDomElement root = m_doc.documentElement();

    if(root.isNull() || root.tagName() != "TS") return false;

    qDebug() << root.tagName()
             << root.attributeNode("version").value()
             << root.attributeNode("language").value()
             << root.attributeNode("sourcelanguage").value();
    QDomNodeList context = root.childNodes();
    int nrRows = 0;
    for(int i = 0; i < context.count(); i++)
    {
        nrRows += (context.at(i).childNodes().count() - 1);
    }
    m_count_row = nrRows;
    m_table->setRowCount(nrRows);

    for(int i = 0; i < context.count(); i++)
    {
        readContextElement(context.at(i).toElement());
    }

    connect(m_table, SIGNAL(itemChanged(QTableWidgetItem*)),
            this, SLOT(slotItemChanged(QTableWidgetItem*)));

    connect(m_table, SIGNAL(itemSelectionChanged()),
            this, SLOT(slotItemSelectionChanged()));

    showRowCount();

    return true;
}

void TsEngine::setLanguage(const QString &target, const QString &source)
{
    QStringList header;
    header << source<< target;
    m_table->setHorizontalHeaderLabels(header);
    m_table->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    //m_table->verticalHeader()->setSectionResizeMode(QHeaderView::Stretch);
}

void TsEngine::setStatusBar(QLabel *location, QLabel *classname, QLabel *nrRows)
{
    Q_ASSERT(location);
    Q_ASSERT(classname);
    Q_ASSERT(nrRows);
    m_lacation = location;
    m_class    = classname;
    m_nrRows   = nrRows;
}

void TsEngine::readContextElement(QDomElement e)
{
    if(e.isNull() || e.tagName() != "context") return;

    QDomNodeList list = e.childNodes();
    for(int i = 0; i < list.count(); i++){
        QDomNode node = list.at(i);
        if(node.nodeName() == "name"){
            readNameElement(node.toElement());
        }else if(node.nodeName() == "message"){
            readMessageElement(node.toElement());
        }
    }
}

void TsEngine::readNameElement(QDomElement e)
{
    Q_UNUSED(e);
    //qDebug() << e.firstChild().nodeValue();
}

void TsEngine::readMessageElement(QDomElement e)
{
    QDomNodeList list = e.childNodes();

    for(int i = 0; i < list.count(); i++){
        QDomNode node = list.at(i);
        if(node.nodeName() == "location"){
            //readLocationElemnet(node.toElement());
        }else if(node.nodeName() == "source"){
            readSourceElement(node.toElement());
        }else if(node.nodeName() == "translation"){
            readTranslationElement(node.toElement());
        }
    }
}

void TsEngine::readLocationElemnet(QDomElement e)
{
    //qDebug() << e.attribute("filename");
    //qDebug() << e.attribute("line");

    QString str = e.attribute("filename") + ":" + e.attribute("line");
    m_lacation->setText(str + "; " + m_lacation->text());
}

void TsEngine::readSourceElement(QDomElement e)
{
    m_listOfSrc.append(e);
    QTableWidgetItem *item = new QTableWidgetItem(e.firstChild().nodeValue());
    QFont font("DejaVu Sans");
    font.setPixelSize(16);
    item->setFont(font);
    //item->setFlags(Qt::ItemIsEnabled);
    //item->setFlags(item->flags() & (~Qt::ItemIsEditable));
    item->setWhatsThis(item->text());
    m_table->setItem(m_idx_src++, 0, item);

    if(e.nextSiblingElement("translation").attribute("type") == "unfinished"){
        item->setBackgroundColor(COLOR_UNTRANSLATED);
    }else if(e.nextSiblingElement("translation").attribute("type") == "obsolete"){
        item->setTextColor(COLOR_OBSOLETE);
    }
}

void TsEngine::readTranslationElement(QDomElement e)
{
    m_listOfTr.append(e);
    QString nodeValue = e.firstChild().nodeValue();
    QTableWidgetItem *item = new QTableWidgetItem(nodeValue);
    QFont font("DejaVu Sans");
    font.setPixelSize(16);
    item->setFont(font);
    m_table->setItem(m_idx_tr++, 1, item);

    //qDebug() << e.firstChild().nodeValue();
    if(e.attribute("type") == "unfinished"){
        if(nodeValue.isEmpty() || nodeValue.isNull())
        {
            //item->setBackgroundColor(Qt::red);
        }else{
            e.removeAttribute("type");
        }
    }else if(e.attribute("type") == "obsolete"){
        item->setTextColor(COLOR_OBSOLETE);
    }
}

bool TsEngine::translate()
{
    m_srcCache = (LgCache *)&(m_cache->at(0));
    m_dstCache = NULL;
    //qDebug() << "m_cache->size():" <<m_cache->size();
    for(int i = 0; i < m_cache->size(); i++)
    {
        m_dstCache = (LgCache *)&(m_cache->at(i));
        //qDebug() << "translate:" << m_dstCache->name();
        //qDebug() << "QFileInfo(m_tsFile).baseName():" <<QFileInfo(m_tsFile).baseName();
        if(m_dstCache->name() == QFileInfo(m_tsFile).baseName())
        {
            qDebug() << "Find target cache!";
            break;
        }
    }


    QString source;
    for(int i = 0; i < m_idx_src; i++)
    {
        source = m_table->item(i, 0)->text();
        m_table->item(i, 1)->setText(findString(source));
        emit sigItemChanged(m_table->item(i, 1));
    }

    return true;
}

void TsEngine::showUntranslated(bool isUnfinished)
{
    m_isUnfinished = isUnfinished;

    int nrOfUnfinished = 0;
    for(int i = 0; i < m_idx_src; i++)
    {
        if(m_table->item(i, 0)->backgroundColor() == COLOR_UNTRANSLATED){
            nrOfUnfinished++;
            continue;
        }
        m_table->setRowHidden(i, isUnfinished);
    }
    if(isUnfinished){
        m_count_row = nrOfUnfinished;
    }else{
        m_count_row = m_idx_src;
    }

    showRowCount();
}

bool TsEngine::untranslated()
{
    return m_isUnfinished;
}

void TsEngine::showRowCount()
{
    if(m_count_row > 0)
    {
        m_nrRows->setText(tr("Count: ") + QString::number(m_count_row));
    }else{
        m_nrRows->setText("");
    }
}

bool TsEngine::saveFile()
{
    QFile filexml(m_tsFile);
    if(!filexml.open( QFile::WriteOnly | QFile::Truncate))
    {
        qWarning("error::TsEngine::saveFile. \n");
        return false;
    }
    QTextStream ts(&filexml);
    ts.reset();
    ts.setCodec("utf-8");
    m_doc.save(ts, 4, QDomNode::EncodingFromDocument);
    filexml.close();
    return true;
}

bool TsEngine::saveAs(const QString &fileName)
{
    //qDebug() << "TsEngine::saveAs:" << fileName;
    QFile filexml(fileName);
    if(!filexml.open( QFile::WriteOnly | QFile::Truncate))
    {
        qWarning("error::TsEngine::saveAs. \n");
        return false;
    }
    QTextStream ts(&filexml);
    ts.reset();
    ts.setCodec("utf-8");
    m_doc.save(ts, 4, QDomNode::EncodingFromDocument);
    filexml.close();
    return true;
}

void TsEngine::saveTextsAsExcel(ExcelEngine &excel, int col, bool isNeedSrc)
{
    QString data;
    if(isNeedSrc){
        excel.setCellData(1, col, "English");
    }else{
        excel.setCellData(1, col, QFileInfo(m_tsFile).baseName());
    }
    excel.setBold(1, col, true);
    excel.setFontSize(1, col, 12);
    excel.setBackgroundColor(1, col, Qt::red);
    excel.setWidth(1, col, 30);
    for(int row = 0; row < m_table->rowCount(); row++)
    {
        if(isNeedSrc){
            data = m_table->item(row, 0)->text();//src language;
            excel.setCellData((row + 2), col, data);
        }else{
            data = m_table->item(row, 1)->text();//target language;
            excel.setCellData((row + 2), col, data);
        }
        excel.setAutoWrap(row + 2, col, true);
    }
}

void TsEngine::slotItemChanged(QTableWidgetItem *item)
{
#if 0
    qDebug() << "(" << item->row()
             << "," << item->column()
             << "):" << item->text();
#endif
    QString newText = item->text();


    if(m_table->column(item) == 0)
    {
        //source language item has something changed.
        QDomElement e = m_listOfSrc.at(m_table->row(item));
        QString src = e.firstChild().nodeValue();
        //don't change
        m_table->item(m_table->row(item), 0)->setText(src);
        return;
    }
/*change item according translation's string*/
    QDomElement e = m_listOfTr.at(m_table->row(item));
    bool txtInValid = newText.isEmpty() || newText.isNull();
    if(e.attribute("type") == "unfinished")
    {
        if(!txtInValid)
        {
            //add new string
            e.appendChild(m_doc.createTextNode(newText));
            e.removeAttribute("type");
            m_table->item(m_table->row(item), 0)->setBackgroundColor(COLOR_FINISHED);
        }
    }else{
        if(txtInValid)
        {
            m_table->item(m_table->row(item), 0)->setBackgroundColor(COLOR_UNTRANSLATED);

            e.removeChild(e.firstChild());
            e.setAttribute("type", "unfinished");
        }else{
            //change the string
            QDomNode oldnode = e.firstChild();
            e.firstChild().setNodeValue(newText);
            QDomNode newnode = e.firstChild();
            e.replaceChild(newnode, oldnode);
        }
    }
    m_table->scrollToItem(item);
    emit sigHasSomethingChanged();
}

void TsEngine::slotItemSelectionChanged()
{
    m_lacation->setText("");
    QDomElement e = m_listOfTr.at(m_table->currentRow());

    QDomNodeList msgNodeList = e.parentNode().childNodes();
    for(int i = 0; i < msgNodeList.count(); i++)
    {
        QDomNode node = msgNodeList.at(i);
        if(node.nodeName() == "location"){
            readLocationElemnet(node.toElement());
        }
    }

    QDomElement className = e.parentNode().parentNode().firstChild().toElement();
    m_class->setText(className.firstChild().nodeValue());
    //qDebug() << "class:" << className.firstChild().nodeValue();
}

QString TsEngine::findString(const QString &src)
{
    if(m_srcCache == NULL || m_dstCache == NULL){
        return QString("");
    }

    int i;
    for(i = 0; i < m_srcCache->size(); i++)
    {
        if(isTranslatedStrIdx(i))
            continue;

        if(m_srcCache->at(i) == src){
            return m_dstCache->at(i);
        }
    }
    m_bUsedList.append(i);
    return QString("");
}

bool TsEngine::isTranslatedStrIdx(int idx)
{
    for(int i = 0; i < m_bUsedList.size(); i++)
    {
        if(idx == m_bUsedList.at(i)){
            return true;
        }
    }
    return false;
}

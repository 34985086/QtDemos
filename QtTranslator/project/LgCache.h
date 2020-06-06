#ifndef TKLANGBUFFER_H
#define TKLANGBUFFER_H

#include <QString>
#include <QStringList>

class LgCache
{
public:
    explicit LgCache();
    void setName(const QString &name);
    const QString &name() const;
    void setColumnId(int id);
    int columnId();
    void append(const QString &text);
    const QString &at(int i) const;
    int size();
private:
    QString m_name;
    int m_column_id;/*idx in excel file*/
    QStringList m_list;
};

#endif // TKLANGBUFFER_H

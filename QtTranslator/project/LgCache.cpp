#include "LgCache.h"

LgCache::LgCache()
{
}

void LgCache::setName(const QString &name)
{
    m_name = name;
}

const QString &LgCache::name() const
{
    return (QString &)m_name;
}

void LgCache::setColumnId(int id)
{
    m_column_id = id;
}

int LgCache::columnId()
{
    return m_column_id;
}

void LgCache::append(const QString &text)
{
    m_list.append(text);
}

const QString &LgCache::at(int i) const
{
    return (QString &)m_list.at(i);
}

int LgCache::size()
{
    return m_list.size();
}

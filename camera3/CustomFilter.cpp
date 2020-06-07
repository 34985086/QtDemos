#include "CustomFilter.h"
#include "CustomFilterRunnable.h"

CustomFilter::CustomFilter(QObject* parent) :
    QAbstractVideoFilter(parent),
    m_Orientation(0)
{
}

QVideoFilterRunnable* CustomFilter::createFilterRunnable()
{
    return new CustomFilterRunnable(this);
}

#ifndef CUSTOMFILTERRUNNABLE_H
#define CUSTOMFILTERRUNNABLE_H

#include <QVideoFilterRunnable>
#include <QVideoFrame>
#include <QSharedPointer>
#include "QZXing.h"

class CustomFilter;

class CustomFilterRunnable : public QVideoFilterRunnable
{
public:
    CustomFilterRunnable(CustomFilter* parent = Q_NULLPTR);

    QVideoFrame run(QVideoFrame *input, const QVideoSurfaceFormat &surfaceFormat, RunFlags flags) Q_DECL_OVERRIDE;
    QVideoFrame run(QVideoFrame *input, const QVideoSurfaceFormat &surfaceFormat, RunFlags flags, QImage image);

protected:
    CustomFilter* m_Filter = Q_NULLPTR;
    int m_Orientation;
    int m_Flip;
    QSharedPointer<QZXing> m_zxing;
};

#endif

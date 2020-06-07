#ifndef CAMERAFILTER_H
#define CAMERAFILTER_H

#include <QAbstractVideoFilter>
#include <QVideoFilterRunnable>

class CameraFilter : public QAbstractVideoFilter
{
    Q_OBJECT
    Q_PROPERTY(int orientation MEMBER m_Orientation)
public:
    explicit CameraFilter();
    ~CameraFilter();
    QVideoFilterRunnable *createFilterRunnable();
protected:
    int m_Orientation;
signals:
    void cameraResolution(const QSize &resolution);
    void cameraImage(QImage image);
public slots:
};

class CameraFilterRunnable : public QVideoFilterRunnable
{
public:
    explicit CameraFilterRunnable(CameraFilter* parent = nullptr);

    QVideoFrame run(QVideoFrame *input, const QVideoSurfaceFormat &fmt, RunFlags flags) Q_DECL_OVERRIDE;
    QVideoFrame run(QVideoFrame *input, const QVideoSurfaceFormat &fmt, RunFlags flags, QImage image);

protected:
    CameraFilter* m_filter;
    int m_orientation;
    int m_flip;
};

#endif // CAMERAFILTER_H

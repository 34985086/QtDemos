#include "CameraFilter.h"

#include <QDebug>
#include <QVideoFrame>
#include <QThread>

#include <QDebug>
#include <QOpenGLContext>
#include <QOpenGLFunctions>
#include <QQmlContext>
#include <QDateTime>

/**
 * @brief CameraFilter::CameraFilter
 */
CameraFilter::CameraFilter()
{

}

CameraFilter::~CameraFilter()
{

}

QVideoFilterRunnable *CameraFilter::createFilterRunnable()
{
    return new CameraFilterRunnable(this);
}


/**
 * @brief CameraFilterRunnable::CameraFilterRunnable
 * @param filter
 */
extern QImage qt_imageFromVideoFrame(const QVideoFrame& f);

CameraFilterRunnable::CameraFilterRunnable(CameraFilter* parent) :
    m_filter(parent),
    m_orientation(0),
    m_flip(0)
{
}

QVideoFrame CameraFilterRunnable::run(QVideoFrame *input, const QVideoSurfaceFormat &fmt, RunFlags flags)
{
    if (!input->isValid())
    {
        qWarning("Invalid input format");
        return *input;
    }

    static qint64 lastDraw = 0;

    if (QDateTime::currentDateTime().currentMSecsSinceEpoch() < lastDraw + 3000)
    {
    }
    else if (QDateTime::currentDateTime().currentMSecsSinceEpoch() < lastDraw + 6000)
    {
        return *input;
    }
    else
    {
        lastDraw = QDateTime::currentDateTime().currentMSecsSinceEpoch();
    }

    m_orientation = m_filter ? m_filter->property("orientation").toInt() : 0;

#ifdef Q_OS_ANDROID
    m_flip = true;
#else
    m_flip = fmt.scanLineDirection() == QVideoSurfaceFormat::BottomToTop;
#endif

    if (input->handleType() == QAbstractVideoBuffer::NoHandle)
    {
        QImage img = qt_imageFromVideoFrame(*input);

        qDebug() << Q_FUNC_INFO << "NoHandle";
        if (img.format() == QImage::Format_ARGB32)
        {
            return run(input, fmt, flags, img);
        }

        return run(input, fmt, flags, img.convertToFormat(QImage::Format_ARGB32));
    }

    if (input->handleType() == QAbstractVideoBuffer::GLTextureHandle)
    {
        qDebug() << Q_FUNC_INFO << "OpenGL";
        QImage img(input->width(), input->height(), QImage::Format_ARGB32);
        GLuint textureId = input->handle().toInt();
        QOpenGLContext* ctx = QOpenGLContext::currentContext();
        QOpenGLFunctions* f = ctx->functions();
        GLuint fbo;
        f->glGenFramebuffers(1, &fbo);
        GLuint prevFbo;
        f->glGetIntegerv(GL_FRAMEBUFFER_BINDING, (GLint *) &prevFbo);
        f->glBindFramebuffer(GL_FRAMEBUFFER, fbo);
        f->glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, textureId, 0);
        f->glReadPixels(0, 0, input->width(), input->height(), GL_RGBA, GL_UNSIGNED_BYTE, img.bits());
        f->glBindFramebuffer(GL_FRAMEBUFFER, prevFbo);
        m_flip = false;
        auto pixel = img.bits();
        for (int y = 0; y < img.height(); y++)
        {
            for (int x = 0; x < img.width(); x++)
            {
                auto T = pixel[0];
                pixel[0] = pixel[2];
                pixel[2] = T;
                pixel += 4;
            }
        }

        return run(input, fmt, flags, img);
    }

    qDebug() << Q_FUNC_INFO << "Unsupported handle type " << input->handleType();
    return *input;
}

QVideoFrame CameraFilterRunnable::run(QVideoFrame* input, const QVideoSurfaceFormat &fmt, RunFlags flags, QImage image)
{
    Q_UNUSED(fmt)
    Q_UNUSED(flags)

    auto bits = !image.isNull() ? image.bits() : input->bits();
    int bytesPerLine = !image.isNull() ? image.bytesPerLine() : input->bytesPerLine();
    auto bytesPerPixel = bytesPerLine / input->width();
    //QVideoFrame::PixelFormat pixelFormat = !image.isNull() ? QVideoFrame::pixelFormatFromImageFormat(image.format()) : input->pixelFormat();

    // qDebug() << Q_FUNC_INFO << "Orientation: " << m_Orientation << "Flip: " << m_Flip;

    for (int y = 0; y < input->height() && y < 32; y++)
    {
        unsigned char* line = bits + y * bytesPerLine;
        auto leftPixel = line;
        auto rightPixel = line + bytesPerLine - bytesPerPixel;
        for (int x = 0; x < input->width() && x < 32; x++)
        {
            leftPixel[0] = 0;
            leftPixel[1] = 255;
            leftPixel[2] = 0;
            leftPixel += bytesPerPixel;
            rightPixel[0] = 0;
            rightPixel[1] = 0;
            rightPixel[2] = 255;
            rightPixel -= bytesPerPixel;
        }
    }

    if(image.isNull()){
        return *input;
    }else{
        emit m_filter->cameraResolution(image.size());
        QImage img = image.scaled(QSize(400, 300), Qt::KeepAspectRatio);
        emit m_filter->cameraImage(img);
        return QVideoFrame(image);
    }

//    return !image.isNull() ? QVideoFrame(image) : *input;
}

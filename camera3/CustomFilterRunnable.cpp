#include "CustomFilterRunnable.h"

#include <QDebug>
#include <QOpenGLContext>
#include <QOpenGLFunctions>
#include <QQmlContext>
#include <QDateTime>
#include "CustomFilter.h"
#include "QZXing.h"

extern QImage qt_imageFromVideoFrame(const QVideoFrame& f);

CustomFilterRunnable::CustomFilterRunnable(CustomFilter* parent) :
    m_Filter(parent),
    m_Orientation(0),
    m_Flip(0)
{
}

QVideoFrame CustomFilterRunnable::run(QVideoFrame *input, const QVideoSurfaceFormat &surfaceFormat, RunFlags flags)
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

    m_Orientation = m_Filter ? m_Filter->property("orientation").toInt() : 0;

#ifdef Q_OS_ANDROID
    m_Flip = true;
#else
    m_Flip = surfaceFormat.scanLineDirection() == QVideoSurfaceFormat::BottomToTop;
#endif

    if (input->handleType() == QAbstractVideoBuffer::NoHandle)
    {
        QImage img = qt_imageFromVideoFrame(*input);

        qDebug() << Q_FUNC_INFO << "NoHandle";
        if (img.format() == QImage::Format_ARGB32)
        {
            return run(input, surfaceFormat, flags, img);
        }

        return run(input, surfaceFormat, flags, img.convertToFormat(QImage::Format_ARGB32));
    }

    if (input->handleType() == QAbstractVideoBuffer::GLTextureHandle)
    {
//        qDebug() << Q_FUNC_INFO << "OpenGL";
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
        m_Flip = false;
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
        return run(input, surfaceFormat, flags, img);
    }

    qDebug() << Q_FUNC_INFO << "Unsupported handle type " << input->handleType();
    return *input;
}

QVideoFrame CustomFilterRunnable::run(QVideoFrame* input, const QVideoSurfaceFormat &surfaceFormat, RunFlags flags, QImage image)
{
    Q_UNUSED(surfaceFormat)
    Q_UNUSED(flags)

    if(image.isNull()){
        return *input;
    }else{
//        qDebug() << "image size=" << image.size();
        if(m_zxing.isNull()){
            m_zxing = QSharedPointer<QZXing>::create();
        }
        QImage rotatedImg;
        if(image.width() > image.height()){
            QMatrix  matrix;
            matrix.rotate(90);
            rotatedImg = image.transformed(matrix);
        }else{
            rotatedImg = image;
        }

        float w = 0.65 * rotatedImg.width();
        float h = w;

        QRectF rc(rotatedImg.width()/2 - w/2, rotatedImg.height()/6 - 20, w, h);
        QImage cropImage = rotatedImg.copy(rc.toRect());

//        qDebug() << "crop=" << rc;

        QString qrcode = m_zxing->decodeImage(cropImage.scaled(QSize(1000, 1000), Qt::KeepAspectRatio, Qt::SmoothTransformation));
        if(!qrcode.isEmpty()){
            qDebug() << "!!!!!!!!!!!!! qrcode:" << qrcode;
            emit m_Filter->qrCodeReadout(qrcode);
        }

        emit m_Filter->cameraImage(cropImage.scaled(QSize(150, 150)));
        return QVideoFrame(image);
    }
}

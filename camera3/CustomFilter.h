#ifndef CUSTOMFILTER_H
#define CUSTOMFILTER_H

#include <QAbstractVideoFilter>

class CustomFilter : public QAbstractVideoFilter
{
    Q_OBJECT

    Q_PROPERTY(int orientation MEMBER m_Orientation)

public:
    CustomFilter(QObject* parent = Q_NULLPTR);

    QVideoFilterRunnable* createFilterRunnable() Q_DECL_OVERRIDE;
signals:
    void qrCodeReadout(const QString &code);
    void cameraImage(const QImage &image);
protected:
    int m_Orientation;
};

#endif

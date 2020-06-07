#ifndef CameraWidget_H
#define CameraWidget_H

#include <QWidget>
#include <QVideoFrame>
#include <QQuickWidget>

#include "QZXing.h"
namespace Ui {
class CameraWidget;
}

class CameraWidget : public QWidget
{
    Q_OBJECT

public:
    explicit CameraWidget(QWidget *parent = 0);
    ~CameraWidget();
    void setDebugOn(bool on);
signals:
    void qrCodeReadout(const QString &code);
    void inputQrCodeManual();
protected:
    void resizeEvent(QResizeEvent *event);
private slots:
    void onInputQrCode();
    void on_btnDebug_clicked();
private:
    Ui::CameraWidget *ui;
//    QZXing m_zxing;
    int m_debug_counter = 0;
    bool m_debugOn = false;
};

#endif // CameraWidget_H

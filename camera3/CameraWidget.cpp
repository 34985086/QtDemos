#include "CameraWidget.h"
#include "ui_CameraWidget.h"
#include "CameraFilter.h"
#include "CustomFilter.h"

#include <QQuickWidget>
#include <QQuickItem>
#include <QThread>

#include "QZXing.h"

CameraWidget::CameraWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::CameraWidget)
{
    ui->setupUi(this);

    qmlRegisterType<CustomFilter>("CustomFilter", 1, 0, "CustomFilter");

    ui->quickWidget = new QQuickWidget(this);
    ui->quickWidget->setResizeMode(QQuickWidget::SizeRootObjectToView);
    ui->quickWidget->setSource(QUrl("qrc:///camera/camera.qml"));
    ui->quickWidget->setGeometry(0, 0, width(), height());
    ui->quickWidget->show();

    QQuickItem *root = ui->quickWidget->rootObject();
    connect(root, SIGNAL(inputQrCode()), this, SLOT(onInputQrCode()));

    CustomFilter* filter = root->findChild<CustomFilter*>("cameraFilterObject");
    if(filter){
        connect(filter, &CustomFilter::qrCodeReadout, [this](const QString &code){
            ui->result->setText(code);
            emit qrCodeReadout(code);
        });
        connect(filter, &CustomFilter::cameraImage, [this](const QImage &image){
            ui->preview->setPixmap(QPixmap::fromImage(image).scaledToWidth(ui->preview->width()));

//            QString qrcode = m_zxing.decodeImage(image);
//            if(!qrcode.isEmpty()){
//                emit qtDetected(qrcode);
//                qDebug() << "!!!!!!!!!!!!! qrcode:" << qrcode;
//                ui->result->setText(qrcode);
//            }
        });
    }else{
        qDebug() << "Not find filter.";
    }

    setDebugOn(false);
}

CameraWidget::~CameraWidget()
{
    delete ui;
}

void CameraWidget::setDebugOn(bool on)
{
    qDebug() << "CameraWidget::setDebugOn, " << on;
    if(on){
        ui->preview->show();
        ui->preview->raise();
        ui->result->show();
        ui->result->raise();
    }else{
        ui->preview->hide();
        ui->result->hide();
    }
}

void CameraWidget::resizeEvent(QResizeEvent *event)
{
    QWidget::resizeEvent(event);
    ui->quickWidget->resize(size());
    ui->quickWidget->move(0, 0);

    ui->preview->move(0, height() - ui->preview->height());
    ui->result->move(0, height() - ui->preview->height() - ui->result->height());
    ui->btnDebug->move(0, 0);
    ui->btnDebug->raise();
}

void CameraWidget::onInputQrCode()
{
    qDebug() << "QQQQQQQQQQ, onInputQrCode";
    emit inputQrCodeManual();
}

void CameraWidget::on_btnDebug_clicked()
{
    qDebug() << "QQQQQQQQQQ, on_btnDebug_clicked";
    if(++m_debug_counter >= 5){
        m_debug_counter = 0;
        m_debugOn = !m_debugOn;
        setDebugOn(m_debugOn);
    }
}

#ifndef DIALOG_H
#define DIALOG_H

#include <QDialog>
#include "qtsoap.h"
#include "SoapWorker.h"

namespace Ui {
class Dialog;
}

class Dialog : public QDialog
{
    Q_OBJECT

public:
    explicit Dialog(QWidget *parent = 0);
    ~Dialog();
public slots:
    void on_submit_clicked();
    void onResponseReady(const QtSoapMessage &response);
private:
    Ui::Dialog *ui;

    QtSoapHttpTransport *m_http;
    SoapWorker *m_worker;
};

#endif // DIALOG_H

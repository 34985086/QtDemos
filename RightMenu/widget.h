#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include <QMenu>
#include <QResizeEvent>

namespace Ui {
class Widget;
}
class QMessageBox;
class Widget : public QWidget
{
    Q_OBJECT

public:
    explicit Widget(QWidget *parent = 0);
    ~Widget();

public slots:
    void contextMenuRequested(const QPoint &point);
    void on_button_1_clicked();
    void on_button_2_clicked();
protected:
    void resizeEvent(QResizeEvent *event);
    bool eventFilter(QObject *watched, QEvent *event);
private:
    Ui::Widget *ui;
    QMenu*   m_pContextMenu;
    QMessageBox *mMessage;
};

#endif // WIDGET_H

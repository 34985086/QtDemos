#ifndef QUICKCONTAINER_H
#define QUICKCONTAINER_H

#include <QWidget>
#include <QQuickItem>

namespace Ui {
class QuickContainer;
}

class QuickContainer : public QWidget
{
    Q_OBJECT

public:
    explicit QuickContainer(QWidget *parent = 0);
    ~QuickContainer();
    void setQml(const QString &qml);

    QQuickItem *root();

protected:
    void resizeEvent(QResizeEvent *event);
private:
    Ui::QuickContainer *ui;
};

#endif // QUICKCONTAINER_H

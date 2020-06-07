#ifndef CONTEXTMENU_H
#define CONTEXTMENU_H

#include <QMenu>

class ContextMenu : public QMenu
{
    Q_OBJECT
public:
    explicit ContextMenu(QWidget *parent = 0);

signals:

public slots:
protected:
    bool eventFilter(QObject *watched, QEvent *event);

    void focusInEvent(QFocusEvent *event);
    void focusOutEvent(QFocusEvent *event);
    void closeEvent(QCloseEvent *event);
};

#endif // CONTEXTMENU_H

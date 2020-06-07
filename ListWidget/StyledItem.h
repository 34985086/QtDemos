#ifndef STYLEDITEM_H
#define STYLEDITEM_H

#include <QLabel>
class StyledItem : public QLabel
{
    Q_OBJECT
public:
    explicit StyledItem(QWidget *parent=Q_NULLPTR, Qt::WindowFlags f=Qt::WindowFlags());
    explicit StyledItem(const QString &text, QWidget *parent=Q_NULLPTR, Qt::WindowFlags f=Qt::WindowFlags());
};

#endif // STYLEDITEM_H

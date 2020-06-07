#include "StyledItem.h"

StyledItem::StyledItem(QWidget *parent, Qt::WindowFlags f)
    : QLabel(parent, f)
{
    setStyleSheet("background-color:white;"
                  "border:1px solid red;"
                  "border-radius:0px;");
}

StyledItem::StyledItem(const QString &text, QWidget *parent, Qt::WindowFlags f)
    : QLabel(text, parent, f)
{
    setStyleSheet("background-color:white;"
                  "border:1px solid red;"
                  "border-radius:0px;");
}

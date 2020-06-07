#ifndef SPINNER_H
#define SPINNER_H

#include "QuickContainer.h"

class Spinner : public QuickContainer
{
    Q_OBJECT

public:
    explicit Spinner(QWidget *parent = 0);
    ~Spinner();
    void setTitle(const QString &text);
    void setTitleColor(const QColor &color);
    void setTitleSize(int size);
    void setRange(int min, int max);
    void setValue(int value);
    void setNumSize(int size);
    void setNumColor(const QColor &color);
    void setBackgroundColor(const QColor &color);
signals:
    void valueChanged(const QString &value);
private slots:
    void onValueChanged(const QString &value);
private:

};

#endif // SPINNER_H

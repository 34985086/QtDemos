#ifndef NUMBERPICKER_H
#define NUMBERPICKER_H

#include <QWidget>
#include <QEvent>
#include <QPaintEvent>

class NumberPicker : public QWidget
{
    Q_OBJECT
public:
    explicit NumberPicker(QWidget *parent = nullptr);
    void addNumber(const QString &number);
signals:
    void picked(const QString &num);
protected:
    void paintEvent(QPaintEvent *event);
private:
    QStringList m_numberList;
    QString m_prev = "01";
    QString m_cur = "02";
    QString m_next = "03";
};

#endif // NUMBERPICKER_H

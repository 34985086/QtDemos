#ifndef TIMESPINNER_H
#define TIMESPINNER_H

#include <QWidget>

namespace Ui {
class TimeSpinner;
}

class TimeSpinner : public QWidget
{
    Q_OBJECT

public:
    explicit TimeSpinner(QWidget *parent = 0);
    ~TimeSpinner();
    void setCurrentTime(int hour, int min);
signals:
    void timeChanged(const QString &time);
private slots:
    void on_hourSpinner_valueChanged(const QString &value);
    void on_minSpinner_valueChanged(const QString &value);
private:
    Ui::TimeSpinner *ui;
    QString m_hour;
    QString m_minute;
};

#endif // TIMESPINNER_H

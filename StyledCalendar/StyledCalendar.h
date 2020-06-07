#ifndef STYLEDCALENDAR_H
#define STYLEDCALENDAR_H

#include <QWidget>

namespace Ui {
class StyledCalendar;
}

class StyledCalendar : public QWidget
{
    Q_OBJECT

public:
    explicit StyledCalendar(QWidget *parent = 0);
    ~StyledCalendar();
    void setWeekendColor(const QColor &color);
signals:
    void clicked(const QDate &date);
protected:
    void resizeEvent(QResizeEvent *event);
    void showEvent(QShowEvent *event);
    void hideEvent(QHideEvent *event);
    bool eventFilter(QObject *watched, QEvent *event);
private slots:
    void on_calendar_clicked(const QDate &date);
    void on_calendar_currentPageChanged(int year, int month);
    void on_btnPrev_clicked();
    void on_btnNext_clicked();
    void on_btnToday_clicked();
private:
    void reLayout();
    void showPrevMonth();
    void showNextMonth();
    Ui::StyledCalendar *ui;
    bool m_isPressed = false;
    bool m_changed = false;
};

#endif // STYLEDCALENDAR_H

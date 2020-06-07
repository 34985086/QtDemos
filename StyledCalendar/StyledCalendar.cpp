#include "StyledCalendar.h"
#include "ui_StyledCalendar.h"
#include <QDebug>
#include <QTextCharFormat>
#include <QEvent>
#include <QMouseEvent>

StyledCalendar::StyledCalendar(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::StyledCalendar)
{
    ui->setupUi(this);

    ui->calendar->setCurrentPage(2017, 12);

    setWeekendColor(Qt::white);

    reLayout();
}

StyledCalendar::~StyledCalendar()
{
    delete ui;
}

void StyledCalendar::setWeekendColor(const QColor &color)
{
    QTextCharFormat format;
    format.setForeground(color);
    QFont font;
    font.setPixelSize(16);
    font.setBold(false);
    format.setFont(font);
    ui->calendar->setWeekdayTextFormat(Qt::Saturday, format);
    ui->calendar->setWeekdayTextFormat(Qt::Sunday, format);
}

void StyledCalendar::resizeEvent(QResizeEvent *event)
{
    Q_UNUSED(event);
    reLayout();
}

void StyledCalendar::showEvent(QShowEvent *event)
{
    qApp->installEventFilter(this);
}

void StyledCalendar::hideEvent(QHideEvent *event)
{
    qApp->removeEventFilter(this);
}

bool StyledCalendar::eventFilter(QObject *watched, QEvent *event)
{
    Q_UNUSED(watched);
    switch(event->type()){
    case QEvent::TouchBegin:
    case QEvent::TouchUpdate:
    {

        QTouchEvent *te = static_cast<QTouchEvent *>(event);
        if(te->touchPoints().size()){
            QTouchEvent::TouchPoint tp = te->touchPoints().at(0);
            QPointF offset = tp.lastScreenPos() - tp.startScreenPos();
            if(qAbs(offset.x()) > 10){
                m_isPressed = true;
            }
            if(offset.x() > 100){
                showPrevMonth();
            }
            if(offset.x() < -100){
                showNextMonth();
            }
//            qDebug() << "TouchUpdate:" << offset << ", velocity:" << tp.velocity();
        }
        break;
    }
    case QEvent::TouchEnd:
        m_isPressed = false;
        m_changed = false;
//        qDebug() << event;
        break;

    case QEvent::MouseButtonRelease:
        if(m_isPressed){
            return true;
        }
        qDebug() << "QEvent::MouseButtonRelease";
        break;
    }
    return false;
}

void StyledCalendar::on_calendar_clicked(const QDate &date)
{
    emit clicked(date);
    qDebug() << "on_calendar_clicked," << date;
}

void StyledCalendar::on_calendar_currentPageChanged(int year, int month)
{
    qDebug() << "on_calendar_currentPageChanged, year=" << year << ", month=" << month;
    ui->naviBar->setText(QString().setNum(year) + " year " + QString().setNum(month) + " month");
}

void StyledCalendar::on_btnPrev_clicked()
{
    m_changed = false;
    showPrevMonth();
}

void StyledCalendar::on_btnNext_clicked()
{
    m_changed = false;
    showNextMonth();
}

void StyledCalendar::on_btnToday_clicked()
{
//    ui->calendar->showToday();
    ui->calendar->setSelectedDate(QDate::currentDate());
    qDebug() << "on_btnToday_clicked";
}

void StyledCalendar::reLayout()
{
    ui->content->resize(width(), height());
    ui->btnToday->setFixedSize(ui->naviBar->height()*2, ui->naviBar->height());
    int x = ui->naviBar->x() + ui->naviBar->width() - ui->btnToday->width();
    ui->btnToday->move(x, ui->naviBar->y());
    ui->btnToday->raise();
}

void StyledCalendar::showPrevMonth()
{
    if(!m_changed){
        ui->calendar->showPreviousMonth();
        m_changed = true;
    }
}

void StyledCalendar::showNextMonth()
{
    if(!m_changed){
        ui->calendar->showNextMonth();
        m_changed = true;
    }
}

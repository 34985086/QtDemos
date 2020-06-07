#include "Spinner.h"
#include "ui_Spinner.h"
#include <QDebug>

Spinner::Spinner(QWidget *parent) :
    QuickContainer(parent)
{
    setQml("qrc:/spinner/Spinner.qml");
    if(root()){
        connect(root(), SIGNAL(changed(QString)), this, SLOT(onValueChanged(QString)));
    }

    setBackgroundColor(Qt::black);
    setTitleColor(Qt::black);
    setTitleSize(20);
    setNumColor(Qt::white);
    setNumSize(20);
}

Spinner::~Spinner()
{
}

void Spinner::setTitle(const QString &text)
{
    if(root()){
        root()->setProperty("label", text);
    }
}

void Spinner::setTitleColor(const QColor &color)
{
    if(root()){
        root()->setProperty("labelColor", color);
    }
}

void Spinner::setTitleSize(int size)
{
    if(root()){
        root()->setProperty("labelSize", size);
    }
}

void Spinner::setRange(int min, int max)
{
    if(root()){
        root()->setProperty("minium", min);
        root()->setProperty("maximum", max);
    }
}

void Spinner::setValue(int value)
{
    if(root()){
        root()->setProperty("value", value);
    }
}

void Spinner::setNumSize(int size)
{
    if(root()){
        root()->setProperty("numSize", size);
    }
}

void Spinner::setNumColor(const QColor &color)
{
    if(root()){
        root()->setProperty("numColor", color);
    }
}

void Spinner::setBackgroundColor(const QColor &color)
{
    if(root()){
        root()->setProperty("backgroundColor", color);
    }
}

void Spinner::onValueChanged(const QString &value)
{
//    qDebug() << "onValueChanged, value:" << value;
    emit valueChanged(value);
}

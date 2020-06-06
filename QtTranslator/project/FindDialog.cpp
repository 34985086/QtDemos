#include <QLabel>
#include <QLineEdit>
#include <QCheckBox>
#include <QPushButton>
#include <QHBoxLayout>
#include "FindDialog.h"

FindDialog::FindDialog(QWidget *parent) :
    QDialog(parent),
    m_label(new QLabel(tr("Find &what:"))),
    m_lineEdit(new QLineEdit),
    m_caseCheckBox(new QCheckBox(tr("&Match case"))),
    m_backwardCheckBox(new QCheckBox(tr("Search &backward"))),
    m_findButton(new QPushButton(tr("&Find"))),
    m_cancelButton(new QPushButton(tr("&Cancel")))
{
    m_label->setBuddy(m_lineEdit);
    m_findButton->setDefault(true);
    m_findButton->setEnabled(false);

    connect(m_lineEdit, SIGNAL(textChanged(QString)), this, SLOT(enableFindButton(QString)));
    connect(m_findButton, SIGNAL(clicked()), this, SLOT(findClicked()));
    connect(m_cancelButton, SIGNAL(clicked()), this, SLOT(close()));

    QHBoxLayout *hTopLeftLayout = new QHBoxLayout;
    hTopLeftLayout->addWidget(m_label);
    hTopLeftLayout->addWidget(m_lineEdit);

    QVBoxLayout *hLeftLayout = new QVBoxLayout;
    hLeftLayout->addLayout(hTopLeftLayout);
    hLeftLayout->addWidget(m_caseCheckBox);
    hLeftLayout->addWidget(m_backwardCheckBox);

    QVBoxLayout *hRightLayout = new QVBoxLayout;
    hRightLayout->addWidget(m_findButton);
    hRightLayout->addWidget(m_cancelButton);
    hRightLayout->addStretch();

    QHBoxLayout *hMainLayout = new QHBoxLayout;
    hMainLayout->addLayout(hLeftLayout);
    hMainLayout->addLayout(hRightLayout);

    setLayout(hMainLayout);

    setWindowTitle(tr("Find"));
    setFixedSize(sizeHint());
}

void FindDialog::findClicked()
{
    QString text = m_lineEdit->text();
    Qt::CaseSensitivity cs = m_caseCheckBox->isChecked() ?
                Qt::CaseSensitive : Qt::CaseInsensitive;

    if(m_backwardCheckBox->isChecked()){
        emit findPrev(text, cs);
    }else{
        emit findNext(text, cs);
    }
}

void FindDialog::enableFindButton(const QString &txt)
{
    m_findButton->setEnabled(!txt.isEmpty());
}

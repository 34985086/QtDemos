#ifndef TKFINDDIALOG_H
#define TKFINDDIALOG_H

#include <QDialog>

class QLabel;
class QLineEdit;
class QCheckBox;
class QPushButton;

class FindDialog : public QDialog
{
    Q_OBJECT
public:
    explicit FindDialog(QWidget *parent = 0);

signals:
    void findNext(const QString &str, Qt::CaseSensitivity cs);
    void findPrev(const QString &str, Qt::CaseSensitivity cs);

private slots:
    void findClicked();
    void enableFindButton(const QString &txt);
private:
    QLabel      *m_label;
    QLineEdit   *m_lineEdit;
    QCheckBox   *m_caseCheckBox;
    QCheckBox   *m_backwardCheckBox;
    QPushButton *m_findButton;
    QPushButton *m_cancelButton;
};

#endif // TKFINDDIALOG_H

#ifndef STYLEDLIST_H
#define STYLEDLIST_H

#include <QWidget>

namespace Ui {
class StyledList;
}

class StyledList : public QWidget
{
    Q_OBJECT

public:
    explicit StyledList(QWidget *parent = 0);
    ~StyledList();

    void addItem(QWidget *w, int h);

private:
    Ui::StyledList *ui;
};

#endif // STYLEDLIST_H

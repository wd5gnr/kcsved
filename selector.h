#ifndef SELECTOR_H
#define SELECTOR_H

#include <QDialog>
#include "csvmodel.h"


namespace Ui {
    class selector;
}

class selector : public QDialog
{
    Q_OBJECT

public:
    explicit selector(QWidget *parent = 0);
    ~selector();
    int pick;

public slots:
    int exec(csvmodel &model, int item, int current);

private slots:

private:
    Ui::selector *ui;
};

#endif // SELECTOR_H

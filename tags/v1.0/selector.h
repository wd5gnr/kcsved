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
    unsigned pick;

public slots:
    // open up given model, which column, the current selection, and the low # (0 or 1 depending on header editing)
    // returns the selection # in pick
    int exec(csvmodel &model, int item, int current, unsigned low);

private slots:

private:
    Ui::selector *ui;
};

#endif // SELECTOR_H

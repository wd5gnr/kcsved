#ifndef OPTDIALOG_H
#define OPTDIALOG_H

#include <QDialog>

namespace Ui {
    class optdialog;
}

class optdialog : public QDialog
{
    Q_OBJECT

public:
    explicit optdialog(QWidget *parent = 0);
    ~optdialog();

public slots:
    void accept(void);

private:
    Ui::optdialog *ui;
};

#endif // OPTDIALOG_H

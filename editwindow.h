#ifndef EDITWINDOW_H
#define EDITWINDOW_H

#include <QDialog>

namespace Ui {
    class editWindow;
}

class editWindow : public QDialog
{
    Q_OBJECT

public:
    explicit editWindow(QWidget *parent = 0,QString textin="");
    ~editWindow();
    QString text;
    bool changed;

private slots:

    void on_saveBtn_clicked();

    void on_htmlmode_clicked();

private:
    Ui::editWindow *ui;
};

#endif // EDITWINDOW_H

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
    editWindow(QWidget *parent=0, QString textin="", QString title="");
    ~editWindow();
    QString text;
    bool changed;

private slots:

    void on_saveBtn_clicked();


    void on_browseView_clicked();

private:
    Ui::editWindow *ui;
};

#endif // EDITWINDOW_H

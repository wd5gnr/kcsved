#ifndef KCsvEDMAIN_H
#define KCsvEDMAIN_H

#include <QMainWindow>
#include <QList>
#include <QSignalMapper>
#include "editrow.h"
#include "csvmodel.h"

namespace Ui {
    class KCsvEdMain;
}

class KCsvEdMain : public QMainWindow
{
    Q_OBJECT

public:
    explicit KCsvEdMain(QWidget *parent = 0);
    ~KCsvEdMain();

private slots:
    void edit_item(int item);

    void on_action_New_triggered();

    void on_action_Open_triggered();

    void on_action_Next_triggered();

    void on_action_Previous_triggered();

    void on_action_First_triggered();

    void on_action_Last_triggered();

    void setDirty(bool dirty=true);

    void on_actionE_xit_triggered();

    void on_action_Revert_triggered();

    void on_action_Paste_triggered();

    void on_actionCu_t_triggered();

    void on_action_Copy_triggered();

    void focuschange(bool gotfocus);

    void btnfocus(bool gotfocus);

    void select(int item);

    void on_action_Save_As_triggered();

    void on_action_Save_triggered();

    void on_actionEdit_Header_toggled();

    void on_action_About_triggered();

    void on_action_Insert_Row_triggered();

    void on_action_Delete_Row_triggered();

    void on_action_Add_New_Row_triggered();

    void on_action_Options_triggered();

    void on_actionAdd_New_Field_triggered();

private:
    Ui::KCsvEdMain *ui;
    QList<EditRow *> rows;
    QSignalMapper *mapper;
    QSignalMapper *vmapper;
    csvmodel *model;
    unsigned current_row;
    void viewupdate(void);
    void commit(void);
    int lowindex;
    void addeditrow(int);
    void destroyUI();

protected:
    void closeEvent(QCloseEvent *);

};

#endif // KCsvEDMAIN_H

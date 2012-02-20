#ifndef KCsvEDMAIN_H
#define KCsvEDMAIN_H

#include <QMainWindow>
#include <QList>
#include <QSignalMapper>
#include "editrow.h"
#include "csvmodel.h"
#include "recentfilemgr.h"

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

    void open_file(QString);  // open file by name


    void on_actionData_base_Save_triggered();

    void on_action_Database_Open_triggered();

private:
    Ui::KCsvEdMain *ui;
    QList<EditRow *> rows;   // UI rows
    QSignalMapper *mapper;   // mappers used to aggregate signals to one slot
    QSignalMapper *vmapper;
    csvmodel model;         // the document
    unsigned current_row;     // our spot in the document
    void viewupdate(void);   // refresh display
    void commit(void);      // scrape the screen into the document
    unsigned lowindex;         // 0 if the header is editable data, 1 if not
    void addeditrow(int);    // add another edit row
    void destroyUI();     // kill all the UI
    void recentMenu(QString filename);  // set up recent file menu

    RecentFileMgr recent;

protected:
    void closeEvent(QCloseEvent *);

};

#endif // KCsvEDMAIN_H

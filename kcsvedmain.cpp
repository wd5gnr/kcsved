/*

kcsved - A simple CSV editor
Copyright (C) 2012 by Al Williams (al.williams@awce.com)

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.


*/

#include "kcsvedmain.h"
#include "ui_kcsvedmain.h"
#include <QString>
#include <QFileDialog>
#include <QMessageBox>
#include <QCloseEvent>
#include <QSettings>
#include <QVariant>
#include "csvmodel.h"
#include "editrow.h"
#include "editwindow.h"
#include "qcsvlineedit.h"
#include "qcsvpushbutton.h"
#include "selector.h"
#include "optdialog.h"


// Create main window
KCsvEdMain::KCsvEdMain(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::KCsvEdMain)
{
    QSettings settings;
    mapper=new QSignalMapper(this); // maps the dynamic button signals to one slot
    vmapper=new QSignalMapper(this);  // same mapping for the select buttons
    ui->setupUi(this);      // standard call
    ui->editFrame->layout()->setAlignment(Qt::AlignTop|Qt::AlignJustify);
    lowindex=settings.value("default/lowindex",1).toInt();
    // hook up the aggreators
    connect(mapper,SIGNAL(mapped(int)),this,SLOT(edit_item(int)));
    connect(vmapper,SIGNAL(mapped(int)),this,SLOT(select(int)));
    on_action_New_triggered();  // simulate New action
}

KCsvEdMain::~KCsvEdMain()
{
    delete ui;
}

// user request to edit text in the big editor
void KCsvEdMain::edit_item(int item)
{
    QString text=rows[item]->edit()->text();  // pull current text not model text
    QString title=rows[item]->label()->text();
    editWindow *editor=new editWindow(this,text,title);
    editor->exec();
    if (editor->result()!=QDialog::Rejected && editor->changed)
    {
        rows[item]->edit()->setText(editor->text);
        ui->action_Save->setDisabled(false);
        // do not commit or update because you could still revert back to model
    }

}

// wipe out dynamic part of UI (useful if you are going to replace it with another
void KCsvEdMain::destroyUI()
{
    qDeleteAll(rows);
    rows.clear();
}


// create new document

void KCsvEdMain::on_action_New_triggered()
{
    destroyUI();
    model.newdoc(true);
    // need to clear the UI
    destroyUI();
    // note: destroying mapped objects breaks the signal mapping
// set up initial menu state
    ui->action_First->setDisabled(true);
    ui->action_Previous->setDisabled(true);
    ui->action_Next->setDisabled(true);
    ui->action_Last->setDisabled(true);
    ui->action_Save->setDisabled(true);
    ui->action_Save_As->setDisabled(true);
    current_row=1;
    ui->action_Save_As->setDisabled(false);
    addeditrow(0);
    viewupdate();
}

// add an edit row with the indicated command ID
void KCsvEdMain::addeditrow(int i)
{
    EditRow *edit=new EditRow();
    edit->AddRow(ui->editFrame);
    connect(edit->eButton(),SIGNAL(clicked()),mapper,SLOT(map()));
    connect(edit->vButton(),SIGNAL(clicked()),vmapper,SLOT(map()));
    connect(edit->edit(),SIGNAL(textEdited(QString)),this,SLOT(setDirty()));
    connect(edit->edit(),SIGNAL(focussed(bool)),this,SLOT(focuschange(bool)));
    connect(edit->vButton(),SIGNAL(focussed(bool)),this,SLOT(btnfocus(bool)));
    connect(edit->eButton(),SIGNAL(focussed(bool)),this,SLOT(btnfocus(bool)));


    mapper->setMapping(edit->eButton(),i);
    vmapper->setMapping(edit->vButton(),i);
    rows<<edit;
}

// open a file
void KCsvEdMain::on_action_Open_triggered()
{
    QString filename;
    destroyUI();
    model.newdoc(false);
    rows.empty();  // do I need to kill all the edit rows? probably TODO
    filename=QFileDialog::getOpenFileName(this,tr("Select CSV File"),"",tr("CSV Files (*.csv);;All Files (*.*)"));
    if (filename.isEmpty()) return;  // cancelled
    if (!model.readdoc(filename))
    {
        QMessageBox::critical(this,tr("Error"),filename+tr(": Can't open file"));
        return;
    }
    // create the edit rows and wire them up plus populate them
    for (int i=0;i<model.columnCount();i++)
    {
        addeditrow(i);
    }

    // set current row to 1 even if edit headers is checked
    current_row=1;
    ui->action_Save_As->setDisabled(false);
    setWindowTitle("KCsvEd - " + filename);
    viewupdate();

}

// this populates the view from the model any time that is necessary, also manages tool bar enables
void KCsvEdMain::viewupdate(void)
{
    for (int i=0;i<model.columnCount();i++)
    {
        if (model.getRow(0).count()>i)  // this handles the case where a row is longer than the header
        {
            rows[i]->label()->setText(model.getCol(0,i));
            rows[i]->label()->setToolTip(model.getCol(0,i));
            rows[i]->label()->setCursorPosition(0);
        }
        if (model.getRow(current_row).count()>i) // this handles the case where a row is longer than the header
            rows[i]->edit()->setText(model.getCol(current_row,i));
        else
            rows[i]->edit()->setText("");
    }
    ui->action_First->setDisabled(current_row==lowindex);
    ui->action_Previous->setDisabled(current_row==lowindex);
    ui->action_Next->setDisabled(current_row==model.count()-1);
    ui->action_Last->setDisabled(current_row==model.count()-1);
    ui->statusBar->showMessage(QString(tr("Record: ")) + QString::number(current_row));

}


// when a button gets the focus we assume we should disable the cut/copy/paste
void KCsvEdMain::btnfocus(bool gotfocus)
{
    if (gotfocus)
    {
        ui->actionCu_t->setEnabled(false);
        ui->action_Copy->setEnabled(false);
        ui->action_Paste->setEnabled(false);
    }
}

// when a line edit gets the focus we assume we should turn on cut/copy/paste
void KCsvEdMain::focuschange(bool gotfocus)
{
    // The problem with this is that when you pop down the edit menu
    // you lose focus and thus cut/copy/paste on the menu bar is always disabled

    // So what we can do is assume only buttons and text have real focus here
    // if the editor gets focus enable, if a button gets focus, disable
    if (gotfocus)
    {
        ui->actionCu_t->setEnabled(true);
        ui->action_Copy->setEnabled(true);
        ui->action_Paste->setEnabled(true);
    }
}

// scrape the data off the screen back to the model
void KCsvEdMain::commit(void)
{
    if (model.isEmpty()) return; // nothing to do
    for (int i=0;i<model.getRow(current_row).count();i++)
    {
        if (model.getCol(current_row,i)!=rows[i]->edit()->text())
        {
            ui->action_Save->setEnabled(true);
            model.getCol(current_row,i)=rows[i]->edit()->text();
        }

    }
}

// Next record
void KCsvEdMain::on_action_Next_triggered()
{
    commit();
    if (current_row!=model.count()) current_row++;
    viewupdate();
}

// previous record
void KCsvEdMain::on_action_Previous_triggered()
{
    commit();
    if (current_row!=lowindex) current_row--;
    viewupdate();
}

// go to first
void KCsvEdMain::on_action_First_triggered()
{
    commit();
    current_row=lowindex;
    viewupdate();
}

// go to last
void KCsvEdMain::on_action_Last_triggered()
{
    commit();
    current_row=model.count()-1;
    viewupdate();
}


// set dirty (default==true) -- note we use the Save action's enabled state as a dirty flag
void KCsvEdMain::setDirty(bool dirty)
{
    ui->action_Save->setDisabled(!dirty);
}


// prompt for close
void KCsvEdMain::closeEvent(QCloseEvent *e)
{
    if (ui->action_Save->isEnabled())
    {
        if (QMessageBox::question(this,tr("Confirm"),tr("Document not saved. Really Close?"),QMessageBox::Close|QMessageBox::Abort)
                ==QMessageBox::Abort) e->ignore(); else e->accept();
    }
    else e->accept();

}

void KCsvEdMain::on_actionE_xit_triggered()
{
    close();
}


// revert current record to how it was when we entered it
void KCsvEdMain::on_action_Revert_triggered()
{
    viewupdate();  // discard current changes
    ui->action_Save->setDisabled(true);
}

// paste (forward to edit control)
void KCsvEdMain::on_action_Paste_triggered()
{
    // this seems miserably inelegant
    QLineEdit *current=dynamic_cast<QLineEdit *>(focusWidget());
    if (current) current->paste();
}

// cut (forward to edit control)
void KCsvEdMain::on_actionCu_t_triggered()
{
    QLineEdit *current=dynamic_cast<QLineEdit *>(focusWidget());
    if (current) current->cut();
}

// copy (forward to edit control)
void KCsvEdMain::on_action_Copy_triggered()
{
    QLineEdit *current=dynamic_cast<QLineEdit *>(focusWidget());
    if (current) current->copy();
}

// handle the V button so you can select a record by any field
void KCsvEdMain::select(int item)
{
    selector *dlg=new selector(this);
    dlg->exec(model,item, current_row-1);
    if (dlg->result()!=QDialog::Rejected)
    {
        commit();
        current_row=dlg->pick+1;
        viewupdate();
    }
}

// Save AS
void KCsvEdMain::on_action_Save_As_triggered()
{
    commit();
    QString fn=QFileDialog::getSaveFileName(this,tr("Save As..."),"",tr("CSV Files (*.csv);;All Files (*.*"));
    if (fn.isNull()) return; // cancel
    model.savedoc(fn);  // TODO error check
    setDirty(false);
    setWindowTitle("KCsvEd - " + fn);
}


// Regular save (TODO error checking)
void KCsvEdMain::on_action_Save_triggered()
{
    commit();
    if (model.openfile().isEmpty()) on_action_Save_As_triggered(); else  model.savedoc();
    setDirty(false);
}

// Handle edit headers toggle
void KCsvEdMain::on_actionEdit_Header_toggled()
{
    bool state=ui->actionEdit_Header->isChecked();
    lowindex=state?0:1;
    commit();
    if (lowindex==0)
    {
        current_row=0;  // move to header
        viewupdate();
    }
    else
    {
        if (current_row==0)
        {
            // jump to first record
            current_row=1;
            viewupdate();
        }

        // note if you are on another record and turn it off, that's fine. No need to redraw.
    }
}

// Simple about box
void KCsvEdMain::on_action_About_triggered()
{
    QMessageBox::about(this,tr("About kcsved"),tr("kcsved V1.0 - A simple CSV editor by <A href=http://www.awce.com>http://www.awce.com</a>. <A href=http://www.gnu.org/copyleft/gpl.html>License: GNU General Public License Version 3</A>"));
}


// Insert a row
void KCsvEdMain::on_action_Insert_Row_triggered()
{
    commit();
    model.insertrow(current_row);
    setDirty();
    viewupdate();
}

// Delete a row
void KCsvEdMain::on_action_Delete_Row_triggered()
{
    if (current_row==0) return;  // won't delete header
    model.deleterow(current_row);
    current_row=1;  // bad if we delete too many fix me
    setDirty();
    viewupdate();
}

// Add a new row
void KCsvEdMain::on_action_Add_New_Row_triggered()
{
    commit();
    model.insertrow(model.count());
    current_row=model.count()-1;
    setDirty();
    viewupdate();
}


// show options
void KCsvEdMain::on_action_Options_triggered()
{
    optdialog dlg(this);
    dlg.exec();  // all options are handled through QSettings
}

// add new field
void KCsvEdMain::on_actionAdd_New_Field_triggered()
{
    commit();
    addeditrow(rows.count());  // need a new editrow
    model.addcol(tr("New Field"));  // arbitrary name
    setDirty();
    viewupdate();
}

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
#include "editwindow.h"
#include "ui_editwindow.h"

// Create edit window
editWindow::editWindow(QWidget *parent,QString textin, QString title) :
    QDialog(parent),
    ui(new Ui::editWindow)
{
    text=textin;
    ui->setupUi(this);
    ui->columnName->setText(title);
    ui->textEdit->setPlainText(text);
    changed=false;
}

editWindow::~editWindow()
{
    delete ui;
}


// if you save, we need to grab the plain text and decide if it changed
void editWindow::on_saveBtn_clicked()
{
    QString newtext;
    newtext=ui->textEdit->toPlainText();
    if (text!=newtext)
    {
        changed=true;
        text=newtext;
    }
    accept();
}

// html preview. Since QT dinks with the HTML, this is read only
// and we never save the HTML, just generate it on demand to check
// our markup
// Note that complex markup doesn't look right... probably should use QWebView
void editWindow::on_htmlmode_clicked()
{
    if (ui->htmlmode->isChecked())   // go from plaint to HTML
    {
        text=ui->textEdit->toPlainText();
        ui->textEdit->setHtml(text);
        ui->textEdit->setReadOnly(true);  // preview only
    }
    else
    {
        ui->textEdit->setPlainText(text);  // go from HTML to plain
        ui->textEdit->setReadOnly(false);
    }
}

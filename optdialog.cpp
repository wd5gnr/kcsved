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
#include "optdialog.h"
#include "ui_optdialog.h"
#include <QSettings>

optdialog::optdialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::optdialog)
{
    QSettings settings;
    int q;
    ui->setupUi(this);
    // Initialize widgets from QSettings
    ui->headeron->setChecked(settings.value("default/lowindex",true).toBool());
    q=settings.value("characters/inquotestd",1).toInt();
    ui->charInQuote->setText(q?QString('"'):settings.value("characters/inquote").toString());
    ui->charInSep->setText(settings.value("characters/incomma",",").toString());
    q=settings.value("characters/outquotestd",1).toInt();
    ui->charOutQuote->setText(q?QString('"'):settings.value("characters/outquote").toString());
    ui->charOutSep->setText(settings.value("characters/outcomma",',').toString());
    ui->headerRow->setChecked(settings.value("default/firstheader",true).toBool());
    ui->quoteSave->setChecked(settings.value("default/writequote",true).toBool());
}

optdialog::~optdialog()
{
    delete ui;
}

// If the user click OK we need to save the new values in QSettings
void optdialog::accept()
{
    QSettings settings;
    char c;
    settings.setValue("default/lowindex",ui->headeron->isChecked()?1:0);
    settings.setValue("default/firstheader",ui->headerRow->isChecked()?1:0);
    settings.setValue("default/writequote",ui->quoteSave->isChecked());
    settings.setValue("characters/inquotestd",ui->charInQuote->text()=="\""?1:0);
    if (ui->charInQuote->text()!="\"")
       settings.setValue("characters/inquote",ui->charInQuote->text());
    settings.setValue("characters/incomma",ui->charInSep->text());
    settings.setValue("characters/outquotestd",ui->charOutQuote->text()=="\""?1:0);
    if (ui->charOutQuote->text()!="\"")
       settings.setValue("characters/outquote",ui->charOutQuote->text());
    settings.setValue("characters/outcomma",ui->charOutSep->text());
    settings.sync();
    QDialog::accept();
}

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

// Simple list box to select a record by field

#include "selector.h"
#include "ui_selector.h"
#include "csvmodel.h"

selector::selector(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::selector)
{
    ui->setupUi(this);
}

selector::~selector()
{
    delete ui;
}

int selector::exec(csvmodel &model,int item, int current, unsigned low)
{
    unsigned i;
    int rv;
    ui->columnName->setText(model.getCol(0,item));
    for (i=low;i<model.count();i++)
        ui->listWidget->addItem(model.getCol(i,item));
    ui->listWidget->setCurrentRow(current-low);
    rv=QDialog::exec();
    pick=ui->listWidget->currentRow();
    return rv;
}


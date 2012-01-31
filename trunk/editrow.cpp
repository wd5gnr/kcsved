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

#include "editrow.h"
#include <QStyle>
#include <QApplication>


// Construct a graphic box for a given column in the file including controls
// NOTE: Label used to be a label but is now a read only line edit
EditRow::EditRow(void)
{
    QStyle *style;
    style=QApplication::style();
   box=new QHBoxLayout();
   _label=new QLineEdit("");
   _label->setReadOnly(true);
   _label->setStyleSheet("QLineEdit { color: gray; border-radius: 8px; }");
   _label->setMinimumWidth(64);
   box->addWidget(_label);
 //  _label->setFixedWidth(80);
   _vButton=new QCSVPushButton(style->standardIcon(QStyle::SP_ArrowDown),"");
   _vButton->setFixedWidth(25);
   _vButton->setToolTip("Select a new record by this field");
   box->addWidget(_vButton);
   box->addWidget(_edit=new QCSVLineEdit());
   _eButton=new QCSVPushButton(QIcon(":/icons/edit-4.png"),"");
   _eButton->setFixedWidth(25);
   _eButton->setToolTip("Edit this field in a larger editor");
   box->addWidget(_eButton);
   box->setStretch(0,1);
   box->setStretch(2,3);

}

// clean up everything
EditRow::~EditRow()
{
    delete _label;
    delete _vButton;
    delete _eButton;
    delete _edit;
    delete box;
}


// Add row to the parent's layout (note: the row is in its own layout
void EditRow::AddRow(QWidget *parent)
{
    QBoxLayout *super=(QBoxLayout *)parent->layout();
    super->addLayout(box);
}

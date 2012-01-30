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

// Custom push button to send signal on focus events

#include "qcsvpushbutton.h"

QCSVPushButton::QCSVPushButton(QWidget *parent) :
    QPushButton(parent)
{
}

QCSVPushButton::QCSVPushButton(const QIcon &icon, const QString &title, QWidget *parent) : QPushButton(icon,title,parent)
{

}

void QCSVPushButton::focusInEvent(QFocusEvent *e)
{
    QPushButton::focusInEvent(e);
    emit(focussed(true));
}

void QCSVPushButton::focusOutEvent(QFocusEvent *e)
{
    QPushButton::focusOutEvent(e);
    emit(focussed(false));
}

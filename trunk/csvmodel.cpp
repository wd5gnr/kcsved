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
#include "csvmodel.h"
#include <QStringList>
#include <csv_parser/csv_parser.hpp>
#include <QTemporaryFile>
#include <QFile>
#include <QTextStream>
#include <QSettings>
#include <QVariant>



csvmodel::csvmodel()
{
    openfile.clear();  // initalize a few things
    colct=0;
}


// Create a new document
// if argument if false, we have an empty document suitable for an open
// if it is true, we start out with a bare-bones document

void csvmodel::newdoc(bool emptydoc)
{
    // we need to delete all the old strings
    // and all the old rows
    qDeleteAll(rows);
    rows.clear();
    openfile.clear();
    colct=0;
    if (emptydoc)
    {
        QStringList *hdr=new QStringList();   // Header + record with one field
        QStringList *rec=new QStringList();
        hdr->append("Field 1");
        rec->append("");
        rows.append(hdr);
        rows.append(rec);
        colct=1;
    }
}

// Read a document from the given file name

bool csvmodel::readdoc(QString fnqs)
{
    csv_parser file_parser;
    QSettings settings;
    bool headerin=false;
    char q,c,nl;   // quote, comma, newline
    int qstd;
    // get the values out of the settings
    qstd=settings.value("characters/inquotestd",1).toInt();
    q=qstd?'"':settings.value("characters/inquote",'"').toString()[0].toAscii();
    c=settings.value("characters/incomma",",").toString()[0].toAscii();
    nl=settings.value("characters/innewline","\n").toString()[0].toAscii();
    const char*fn=fnqs.toLatin1();
    // clear out anything already there
    newdoc();
    // get going
    if (!file_parser.init(fn)) return false;
    file_parser.set_enclosed_char(q,ENCLOSURE_OPTIONAL);
    file_parser.set_field_term_char(c);
    file_parser.set_line_term_char(nl);
    // loop through all the rows
    while (file_parser.has_more_rows())
    {
        unsigned int i;
        QString *str;
        csv_row row=file_parser.get_row();  // get row
        QStringList *rowlist=new QStringList;  // put it in a string list
        if (!headerin)  // do we need the header?
        {
            headerin=true;  // never again on this load
            if (!settings.value("default/firstheader",true).toBool())
            {
                for (i=0;i<row.size();i++)  // build a fake header
                {
                    QString fld;
                    fld="Field " + QString::number(i+1);
                    rowlist->append(fld);
                }
                rows<<rowlist;   // prime to make this line, line #1 instead of 0
                rowlist=new QStringList;
            }
        }
        // we need to remember the biggest line in the document
        if (colct<row.size()) colct=row.size();
        // transfer it all over
        for (i=0;i<row.size();i++)
        {
            str = new QString(row[i].c_str());
            rowlist->append(*str);
        }
        rows<<rowlist;
    }
    openfile=fnqs;
    return true;

}

/* save a document using its known name */
bool csvmodel::savedoc(void)
{
    return savedoc(openfile);
}



/* Save a document with the given name */
bool csvmodel::savedoc(QString fnqs)
{
    int i,j;
    // The idea is we make a temp file, save to it,
    // delete any old backup, copy the original file
    // to the backup and then rename the save file
    QTemporaryFile savefile;
    QSettings settings;
    char q,c,nl;  // quote, comma, newline
    int qstd;
// pick up characters from settings
    qstd=settings.value("characters/outquotestd",1).toInt();
    q=qstd?'"':settings.value("characters/outquote","\"").toString()[0].toAscii();
    c=settings.value("characters/outcomma",",").toString()[0].toAscii();
    nl=settings.value("characters/outnewline","\n").toString()[0].toAscii();
    // don't delete the temp file automatically
    savefile.setAutoRemove(false);
// Write it out
    savefile.open();
    QTextStream out(&savefile);
    QString fld;
    QStringList *rowlist;
    int start=0;
    if (!settings.value("default/firstheader",true).toBool()) start=1;
    for (i=start;i<rows.count();i++)
    {
        rowlist=rows[i];
        for (j=0;j<rowlist->count();j++)
        {
            out<<q;
            fld=(*rowlist)[j];
            out<<fld;
            out<<q;
            if (j<rowlist->count()-1) out<<c;
        }
        out<<nl;
    }
    savefile.close();
    // this fails if fnqs already exists, so we need to rename it first and/or unlink
    QFile old(fnqs);
    QFile oldbak(fnqs+".bak");
    oldbak.remove();
    old.rename(fnqs+".bak");
    savefile.rename(fnqs);
    openfile=fnqs;
    return true;
}


// delete a row out of the fiile
void csvmodel::deleterow(int index)
{
    rows.removeAt(index);
}


// insert a row at the indicated position
void csvmodel::insertrow(int index)
{
    QStringList *newlist=new QStringList(*rows[0]);  // make copy of header
    rows.insert(index,newlist);
}

// add a column
void csvmodel::addcol(QString name)
{
    colct++;
    if (rows.isEmpty()) // if nothing there
    {
        // really a new document
        QStringList *doc=new QStringList;
        doc->append(name);
        rows.append(doc);
    }
    else  // just add one
    {
        rows[0]->append(name);  // to header
        for (int i=1;i<rows.count();i++)  // add one to all other rows
        {
            rows[i]->append("");
        }

    }
}

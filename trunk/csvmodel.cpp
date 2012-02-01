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

// Third party parser was messing up so rewrote it (twice, since the first time got eaten by the dog)

csvmodel::csvmodel()
{
    _openfile.clear();  // initalize a few things
    colct=0;
}


// Create a new document
// if argument if false, we have an empty document suitable for an open
// if it is true, we start out with a bare-bones document
void csvmodel::newdoc(bool emptydoc)
{
    // we need to delete all the old strings
    // and all the old rows
    rows.clear();
    _openfile.clear();  // no file name
    colct=0;            // no columns
    if (emptydoc)
    {
        QStringList hdr;   // Header + record with one field
        QStringList rec;
        hdr.append("Field 1");   // arbitrary name
        rec.append("");
        rows.append(hdr);
        rows.append(rec);
        colct=1;
    }
}

// Read a document from the given file name

bool csvmodel::readdoc(QString fnqs)
{
    enum parsestate { NOFIELD, QFIELD, FIELD, QFEND, DONE };  // parse states: no field, quoted, ordinary, quote ending, and done
    parsestate state;
    bool headerin=false;  // have we loaded the header yet?
    QFile infile(fnqs);
    if (!infile.open(QIODevice::ReadOnly)) return false;  // can't open file
    QTextStream in(&infile);
    QSettings settings;
    int i;
    QChar quote, sep;    // quote and comma characters
    int qstd;     // Settings has trouble with a " character so if qstd==1 then we use " otherwise we use the characters/inquote setting
    // get the values out of the settings
    qstd=settings.value("characters/inquotestd",1).toInt();
    quote=qstd?QChar('"'):settings.value("characters/inquote",'"').toString()[0];
    sep=settings.value("characters/incomma",",").toString()[0];
    newdoc();   // clear document

    while (!in.atEnd())   // while still lines
    {
        QString line, field;   // these are on the stack because qt does implicit copy
        QStringList curRow;
        QChar c;
        line=in.readLine();   // get a line
        line+=QString("\n");  // add end marker
// set initial conditions
        state=NOFIELD;
        i=0;
        while (state!=DONE && c!=QChar('\n'))  // should not need the \n test but safe
        {
            c=line[i++];    // get current character
            switch (state)
            {
            case NOFIELD:   // not inside a field (yet)
                if (c.isSpace()) continue;  // skip leading space in field
                if (c==quote)
                {
                    state=QFIELD;
                    continue;
                }
                if (c==sep)   // empty field
                {
                    curRow.append(field);
                    // field is empty so no need to clear it
                    continue;
                }
                field.append(c);   // found a normal field start
                state=FIELD;
                continue;
                break;
            case QFIELD:    // quoted field
                if (c==quote)
                {
                    state=QFEND;   // look for end or double quote
                    continue;
                }
                if (c==QChar('\n'))
                {
                    // technically an error but we will take it
                    curRow.append(field);
                    state=DONE;
                    continue;
                }
                field.append(c);
                continue;
                break;
            case FIELD:   // normal field
                if (c==sep)   // end?
                {
                    state=NOFIELD;
                    curRow.append(field);
                    field.clear();
                    continue;
                }
                if (c==QChar('\n'))
                {
                    state=DONE;
                    curRow.append(field);
                    continue;
                }
                field.append(c);   // keep going
                continue;
                break;
            case QFEND:   // quoted field looks like it ended, but could be a double quote escape
                if (c==sep)  // nope, its ended
                {
                    state=NOFIELD;
                    curRow.append(field);
                    field.clear();
                    continue;
                }
                if (c==quote)   // quoted quote
                {
                    state=QFIELD;
                    field.append("\"");
                    continue;
                }
                if (c==QChar('\n'))   // ended at the end of a line
                {
                    state=DONE;
                    curRow.append(field);
                    continue;
                }
                break;
            case DONE:
                // never get here but shut up compiler!
                break;
            }

        }
        if (!headerin)  // do we need the header?
        {
            headerin=true;  // never again on this load
            if (!settings.value("default/firstheader",true).toBool())
            {
                QStringList hdr;
                for (i=0;i<curRow.size();i++)  // build a fake header
                {
                    QString fld;
                    fld="Field " + QString::number(i+1);
                    hdr.append(fld);
                }
                rows<<hdr;   // prime to make this line, line #1 instead of 0
            }
        }
        rows<<curRow;  // add to document
        // we need to remember the biggest line in the document
        if (colct<curRow.size()) colct=curRow.size();
        // go do another line
    }
    infile.close();  // close the file, should not error on close from read
    _openfile=fnqs;  // remember file name
    return true;
}




/* save a document using its known name */
bool csvmodel::savedoc(void)
{
    if (_openfile.isEmpty()||_openfile.isNull()) return false; // bad!
    return savedoc(_openfile);
}



/* Save a document with the given name */ // need to improve error checking TODO
bool csvmodel::savedoc(QString fnqs)
{
    int i,j;
    // The idea is we make a temp file, save to it,
    // delete any old backup, copy the original file
    // to the backup and then rename the save file
    QTemporaryFile savefile;
    QSettings settings;
    QChar quote, sep;
    int qstd;
    bool writequote;
    // pick up characters from settings
    qstd=settings.value("characters/outquotestd",1).toInt();
    quote=qstd?QChar('"'):settings.value("characters/outquote","\"").toString()[0];
    sep=settings.value("characters/outcomma",",").toString()[0];
    // don't delete the temp file automatically
    savefile.setAutoRemove(false);
// Write it out
    if (!savefile.open()) return false;
    QTextStream out(&savefile);
    QString fld;
    QStringList rowlist;
    int start=0;
    // if we have fake headers don't write them out
    if (!settings.value("default/firstheader",true).toBool()) start=1;
    for (i=start;i<rows.count();i++)
    {
        rowlist=rows[i];
        for (j=0;j<rowlist.count();j++)
        {
            // we will write quotes if you asked us to or if the field contains a seperator (,)
            writequote=settings.value("default/writequote",true).toBool();
            fld=rowlist[j];
            if (fld.indexOf(sep)>=0) writequote=true;  // always write if sep appears in data
            if (writequote) out<<quote;
// if quoting, we double any quotes in the field
            if (writequote) fld.replace(quote,QString(quote)+QString(quote));
            out<<fld;
            if (writequote) out<<quote;
            if (j<rowlist.count()-1) out<<sep;
        }
        out<<'\n';
    }
    savefile.close();  // oddly enought Qt doesn't indicate any error on close!
    // this fails if fnqs already exists, so we need to rename it first and/or unlink
    QFile old(fnqs);
    QFile oldbak(fnqs+".bak");
    oldbak.remove();
    old.rename(fnqs+".bak");
    savefile.rename(fnqs);
    _openfile=fnqs;   // remember this name
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
    rows.insert(index,rows[0]);   // copies the header
}

// add a column
void csvmodel::addcol(QString name)
{
    colct++;
    if (rows.isEmpty()) // if nothing there
    {
        // really a new document
        QStringList doc;
        doc.append(name);
        rows.append(doc);
    }
    else  // just add one
    {
        rows[0].append(name);  // to header
        for (int i=1;i<rows.count();i++)  // add one to all other rows
        {
            rows[i].append("");
        }

    }
}

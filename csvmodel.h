#ifndef CSVMODEL_H
#define CSVMODEL_H

#include <QList>
#include <QStringList>

class csvmodel
{
public:
    csvmodel();
    QStringList &getRow(int index) { return rows[index]; }
    QString &getCol(int row, int col) { return rows[row][col]; }
    bool isEmpty(void) { return rows.isEmpty(); }
    int count(void) { return rows.count(); }
    void newdoc(bool emptydoc=false);  // create a new document for opening (false) or blank (true)
    bool readdoc(QString fnqs);       // read the file, true on success
    bool savedoc(QString fnqs);       // save a file, true on success
    bool savedoc(void);               // save using the openfile name
    QString &openfile(void) { return _openfile; }
    void deleterow(int index);        // delete a row
    void insertrow(int index);        // insert a row at position
    void addcol(QString name);        // add a new column of name
    int columnCount(void) { return colct; }
protected:
    QList<QStringList> rows;        // array of string lists, each list is a row of the file
    QString _openfile;                 // currently open file
    int colct;                        // max number of columns
};

#endif // CSVMODEL_H

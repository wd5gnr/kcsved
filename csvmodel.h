#ifndef CSVMODEL_H
#define CSVMODEL_H

#include <QList>
#include <QStringList>

class csvmodel
{
public:
    csvmodel();
    QList<QStringList *> rows;
    void newdoc(bool emptydoc=false);
    bool readdoc(QString fnqs);
    bool savedoc(QString fnqs);
    bool savedoc(void);
    QString openfile;
    void deleterow(int index);
    void insertrow(int index);
    void addcol(QString name);
    int columnCount(void) { return colct; }
private:
    int colct;
};

#endif // CSVMODEL_H

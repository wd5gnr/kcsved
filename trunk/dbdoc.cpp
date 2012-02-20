#include "dbdoc.h"
#include <QMessageBox>
#include <QSqlError>
#include <QVariant>

#include <QDebug>
dbdoc::dbdoc()
{
}


bool dbdoc::save(QString dbname, csvmodel *model)
{
    QSqlDatabase db=QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName(dbname);
    if (!db.open()) {
        QMessageBox::critical(NULL,"Error","Can't Open Database" + db.lastError().databaseText());
        return false;
    }
    QSqlQuery query(db);
    bool b;
    QString q0;
    QString q1;
    QString q;
    QString col;
    b=query.exec("Create table if not exists csvcolumns (name text)");
    b=query.exec("delete from csvcolumns");
    q1=q0="(";
    bool first=true;
    foreach(col,model->getRow(0)) {
        q="insert into csvcolumns (name) values (" + QString("\"") + col + "\")";

        if (!first) {
            q0+=",";
            q1+=",";
        }
        else {
            first=false;
        }
        col.replace(" ","_");
        q0+=col+" text";
        q1+=col;
        b=query.exec(q);
    }
    q0+=")";
    q1+=")";
    q="create table if not exists csvdata " + q0;
    b=query.exec(q);
    b=query.exec("delete from csvdata");
    for (int i=1;i<model->count();i++)
    {
        q="insert into csvdata " + q1 + " VALUES (";
        for (int j=0;j<model->getRow(i).count();j++) {
            QString v=model->getCol(i,j);
            v.replace("\"","\"\"");
            q+="\""+v+"\"";
            if (j+1!=model->getRow(i).count()) q+=",";
        }
        q+=")";
        b=query.exec(q);
    }
    b=db.commit();
    db.close();
    return true;
}

bool dbdoc::load(QString dbname, csvmodel *model)
{
    QSqlDatabase db=QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName(dbname);
    if (!db.open()) {
        QMessageBox::critical(NULL,"Error","Can't Open Database");
        return false;
    }
    model->newdoc(true);
    QSqlQuery query(db);
    bool b;
    // query columns
    b=query.exec("select name from csvcolumns");
    model->insertrow(0);
    while (query.next())
    {
        qDebug()<<query.value(0).toString();
        model->getRow(0)<<query.value(0).toString();
        model->getRow(1)<<query.value(0).toString();
        model->colct++;
    }
    // put them in row[0]
    // get all rows and put each one in a row
    db.close();
    QMessageBox::critical(NULL,"Error","Unimplemented");
    return true;
}

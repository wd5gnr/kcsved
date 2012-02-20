#ifndef DBDOC_H
#define DBDOC_H

#include <qsqlquery.h>
#include "csvmodel.h"

class dbdoc
{
public:
    dbdoc();
    static bool save(QString dbname, csvmodel *model);
    static bool load(QString dbname, csvmodel *model);
};

#endif // DBDOC_H

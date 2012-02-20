#ifndef RECENTFILEMGR_H
#define RECENTFILEMGR_H

#include <QString>
#include <QMenu>
#include <QStringList>

#include <QObject>

class RecentFileMgr : public QObject
{
    Q_OBJECT
public:
    // Add a file to your recent list
    // list is the recent list, file is the new file to add
    // menu is the "recent" menu, max is the number to show
    // longname is true if you want the whole path in the menu
    // You don't have to do anything with the list except
    // read it from storage (e.g., QSetting)
    // and then save it again after calling this
    void Add(QStringList *list, QString file, QMenu *menu, int max=5, bool longname=false);


signals:
    void openRecent(QString);   // hook this up to get open file actions

public slots:

protected slots:
    void recent_trigger(void);  // the menus hook up here


};

#endif // RECENTFILEMGR_H

#include <QFileInfo>
#include "recentfilemgr.h"

void RecentFileMgr::Add(QStringList *list, QString file, QMenu *menu, int max, QWidget *parent)
{
    int i=1;
    if (list->isEmpty()&&file.isEmpty()) return;
    menu->clear();
    QFileInfo ifile(file);
    if (!files.isEmpty() && ifile.exists()) list->insert(0,ifile.canonicalFilePath());
    list->removeDuplicates();
    while (list->count()>max) list->removeFirst();
    foreach(QString f,*list)
    {
        QFileInfo tfile(f);
        if (!tfile.exists())
        {
            list->removeOne(f);
            continue;
        }
        QString ftitle;
        QString prefix="&" + QString::number(i++);
        if (longfname) ftitle=tfile.canonicalFilePath(); else ftitle=tfile.fileName();
        QAction *act=menu->addAction(i<=10?prefix+". "+ftitle:ftitle,this,SLOT(recent_trigger()));
        act->setData(f);
        act->setToolTip(tfile.canonicalFilePath());
    }
}

RecentFileMgr::RecentFileMgr(int maxfiles,bool longnames,QObject *parent) :
    QObject(parent)
{
    longfname=longnames;
    max=maxfiles;
    // could set up default here
    db=NULL;
}

void RecentFileMgr::SetKeys(QString vendor, QString app)
{
    if (db) delete db;
    db=new QSettings(vendor,app,this);
}

void RecentFileMgr::SetKeys(QSettings &set)
{
    db=new QSettings(set.organizationName(),set.applicationName(),this);
}

void RecentFileMgr::SetMax(int n)
{
    max=n;
}

void RecentFileMgr::menupdate(void)
{
    int i=1;
    mymenu->clear();
    files=db->value("files/recent").toStringList();
    foreach(QString f,files)
    {
        QFileInfo tfile(f);
        if (!tfile.exists()) continue;
        QString ftitle;
        QString prefix="&" + QString::number(i++);
        if (longfname) ftitle=f; else ftitle=tfile.fileName();
        QAction *act=mymenu->addAction(i<=10?prefix+". "+ftitle:ftitle,this,SLOT(recent_trigger()));
        act->setData(f);
    }

}

void RecentFileMgr::Attach(QWidget *parent, QMenu *menu)
{
    setParent(parent);
    mymenu=menu;
    menupdate();
}

void RecentFileMgr::Add(QString file)
{
    QFileInfo tfile(file);
    files<<tfile.canonicalFilePath();
    files.removeDuplicates();
    db->setValue("files/recent",files);
    menupdate();
}

void RecentFileMgr::recent_trigger()
{
    QAction *menu=dynamic_cast<QAction *>(sender());
    emit(openRecent(menu->data().toString()));
}

// todo: need a delete item
// could scan if file not exist, remove automatically
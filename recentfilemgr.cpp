#include <QFileInfo>
#include "recentfilemgr.h"

// Add a file to your recent list
// list is the recent list, file is the new file to add
// menu is the "recent" menu, max is the number to show
// longname is true if you want the whole path in the menu
// You don't have to do anything with the list except
// read it from storage (e.g., QSetting)
// and then save it again after calling this

void RecentFileMgr::Add(QStringList *list, QString file, QMenu *menu, int max, bool longname)
{
    int i=1;
    int j;
    menu->clear();  // nuke menu
    // ignore if everything is empty
    if (list->isEmpty()&&file.isEmpty()) return;
    QFileInfo ifile(file);   // find out about file
    // if the file exists, add it to the list
    if (ifile.exists())
        list->insert(0,ifile.canonicalFilePath());
    list->removeDuplicates();  // this appears to keep the lowest # entries
    QStringListIterator listi(*list);
    while (listi.hasNext()) {
        QString fn=listi.next();
        QFileInfo tfile(fn);
        if (!tfile.exists()) list->removeOne(fn);
    }
    // trim list to size
    while (list->count()>max) list->removeLast();
    for (j=0; j<list->count(); j++) {
        QFileInfo tfile(list->at(j));
        if (!tfile.exists())
            continue;
        QString ftitle;
        QString prefix="&" + QString::number(i++);
        if (longname)
            ftitle=tfile.canonicalFilePath();
        else
            ftitle=tfile.fileName();
        QAction *act=menu->addAction(i<=10?prefix+". "+ftitle:ftitle,this,SLOT(recent_trigger()));
        act->setData(list->at(j));
        act->setToolTip(tfile.canonicalFilePath());
    }
}



void RecentFileMgr::recent_trigger()
{
    QAction *menu=dynamic_cast<QAction *>(sender());
    emit(openRecent(menu->data().toString()));
}


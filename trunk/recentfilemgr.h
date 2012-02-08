#ifndef RECENTFILEMGR_H
#define RECENTFILEMGR_H

#include <QString>
#include <QSettings>
#include <QMenu>

#include <QObject>

class RecentFileMgr : public QObject
{
    Q_OBJECT
public:
    explicit RecentFileMgr(int maxfiles=5,bool longfiles=true,QObject *parent = 0);
    void SetKeys(QString vendor, QString app);
    void SetKeys(QSettings &set);
    void SetMax(int n);
    void Attach(QWidget * parent, QMenu *menu);
    void Add(QString file);
private:
    QSettings *db;
    QMenu *mymenu;
    QStringList files;
    int max;
    bool longfname;
    void menupdate(void);

signals:
    void openRecent(QString);

public slots:

protected slots:
    void recent_trigger(void);


};

#endif // RECENTFILEMGR_H

#ifndef BROWSEVIEW_H
#define BROWSEVIEW_H

#include <QDialog>

namespace Ui {
    class BrowseView;
}

class BrowseView : public QDialog
{
    Q_OBJECT

public:
    explicit BrowseView(QWidget *parent = 0, QString html = "");
    ~BrowseView();

private:
    Ui::BrowseView *ui;
};

#endif // BROWSEVIEW_H

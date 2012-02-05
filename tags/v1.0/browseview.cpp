#include "browseview.h"
#include "ui_browseview.h"
#include <QtWebKit/QWebView>

BrowseView::BrowseView(QWidget *parent, QString html) :
    QDialog(parent),
    ui(new Ui::BrowseView)
{
    ui->setupUi(this);
    ui->webView->setHtml(html);
}

BrowseView::~BrowseView()
{
    delete ui;
}

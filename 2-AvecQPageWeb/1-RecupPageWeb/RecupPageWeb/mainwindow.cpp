#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QWebPage>
#include <QWebFrame>
#include <QDebug>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    QWebPage* webPage = new QWebPage(this);
    QWebFrame* webFrame = webPage->currentFrame();
    connect(webPage, SIGNAL(loadFinished(bool)), SLOT(webPage_loadFinished(bool)));
    webFrame->load(QUrl("http://www.mountyhall.com/ftp/"));
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::webPage_loadFinished(bool ok)
{
    qDebug() << ok;

    QWebPage* webPage = (QWebPage*) sender();
    QWebFrame* webFrame = webPage->mainFrame();

    qDebug() << webFrame->toHtml();
}

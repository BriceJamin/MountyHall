#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QDebug>
#include <QString>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    _networkDownloader = new NetworkDownloader();
    connect(_networkDownloader, SIGNAL(sig_bytesReceived(qint64)),
             this, SLOT(bytesReceived(qint64)));
    connect(_networkDownloader, SIGNAL(sig_bytesTotal(qint64)),
             this, SLOT(bytesTotal(qint64)));
    connect(_networkDownloader, SIGNAL(sig_error(const QString&)),
             this, SLOT(error(const QString&)));
    connect(_networkDownloader, SIGNAL(sig_finished(const QFile&)),
             this, SLOT(finished(const QFile&)));
}

MainWindow::~MainWindow()
{
    delete ui;
    delete _networkDownloader;
}

void MainWindow::changeEvent(QEvent *e)
{
    QMainWindow::changeEvent(e);
    switch (e->type()) {
    case QEvent::LanguageChange:
        ui->retranslateUi(this);
        break;
    default:
        break;
    }
}

void MainWindow::on_pushButton_clicked()
{
    qDebug() << Q_FUNC_INFO;

    ui->progressBar->setValue(0);
    ui->progressBar->setMaximum(0);
    ui->progressBar->setFormat("%p% (%v/?o)");
    ui->pushButton->setEnabled(false);
    ui->statusBar->showMessage("Téléchargement...");

    QString urlString;
    QString fileName;
    urlString = ui->lineEdit->text();
    fileName = ui->lineEdit_2->text();

    _networkDownloader->get(urlString, fileName);
}

void MainWindow::bytesReceived(qint64 bytesReceived)
{
    qDebug() << Q_FUNC_INFO << bytesReceived;

    ui->progressBar->setValue(bytesReceived);
}

void MainWindow::bytesTotal(qint64 bytesTotal)
{
    qDebug() << Q_FUNC_INFO << bytesTotal;

    if(bytesTotal <= 0)
    {
        bytesTotal = 0;
        ui->progressBar->setFormat("%p% (%vo/?o)");
    }
    else
        ui->progressBar->setFormat("%p% (%vo/%mo)");

    ui->progressBar->setMaximum(bytesTotal);
}

void MainWindow::error(const QString& errorString)
{
    qDebug() << Q_FUNC_INFO << errorString;

    ui->statusBar->showMessage("Error: " + errorString);
}

void MainWindow::finished(const QFile& downloaded)
{
    QFile file(downloaded.fileName());
    qDebug() << Q_FUNC_INFO << file.fileName();

    ui->statusBar->showMessage("Téléchargé.");
    ui->pushButton->setEnabled(true);
    ui->progressBar->setFormat("%p% (%mo)");

    file.open(QIODevice::ReadOnly);
    QString data = file.readAll();
    file.close();

    qDebug() << Q_FUNC_INFO << "data :" << endl << data;
}

#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QUrl>
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QDebug>
#include <QString>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    _networkManager = new QNetworkAccessManager(this);
    connect(_networkManager, SIGNAL(finished(QNetworkReply*)),
             this, SLOT(replyFinished(QNetworkReply*)));
}

MainWindow::~MainWindow()
{
    delete ui;
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

    ui->pushButton->setEnabled(false);
    ui->progressBar->setEnabled(true);
    ui->progressBar->setFormat("%p%");
    ui->progressBar->setValue(0);
    ui->progressBar->setMaximum(0);
    ui->statusBar->showMessage("Téléchargement...");

    QString urlString = ui->lineEdit->text();
    QUrl url(urlString);
    QNetworkRequest networkRequest(url);
    QNetworkReply* networkReply = _networkManager->get(networkRequest);
    connect(networkReply, SIGNAL(downloadProgress(qint64,qint64)),
            this, SLOT(downloadProgress(qint64,qint64)));
}

void MainWindow::replyFinished(QNetworkReply* networkReply)
{
    qDebug() << Q_FUNC_INFO;

    QNetworkReply::NetworkError networkError = networkReply->error();

    if(networkError == QNetworkReply::NoError)
    {
        ui->statusBar->showMessage("Téléchargé.");

        QString data(networkReply->readAll());
        //qDebug() << Q_FUNC_INFO << "dataReceived :" << endl << data;
    }
    else
    {
        ui->progressBar->setMaximum(100);
        ui->progressBar->setValue(0);
        QString formatString("%p%");
        ui->progressBar->setFormat(formatString);

        QString errorString = "Erreur: ";
        errorString += networkReply->errorString();
        ui->statusBar->showMessage(errorString);
    }

    ui->pushButton->setEnabled(true);
    networkReply->deleteLater();
}

void MainWindow::downloadProgress(qint64 bytesReceived, qint64 bytesTotal)
{
    qDebug() << Q_FUNC_INFO << bytesReceived << bytesTotal;

    QString formatString("%p% (%vo/%mo)");

    int value = bytesReceived;
    int max = bytesTotal;

    if(value == max)
    {
        qDebug() << Q_FUNC_INFO << "max < 0";

        formatString = "%p% (%mo)";
    }

    if(max < 0)
    {
        qDebug() << Q_FUNC_INFO << "max < 0";

        max = 0;
        formatString = "%p% (%vo/?)";
    }

    ui->progressBar->setMaximum(max);
    ui->progressBar->setValue(value);
    qDebug() << "formatString" << formatString;
    ui->progressBar->setFormat(formatString);
}

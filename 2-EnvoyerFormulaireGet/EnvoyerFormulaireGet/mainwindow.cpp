#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QDebug>
#include <QString>
#include <QMap>

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
    connect(_networkDownloader, SIGNAL(sig_finished(QString)),
             this, SLOT(finished(QString)));
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
    ui->progressBar->setMaximum(100);
    ui->progressBar->setFormat("%p% (%v o/?)");
    ui->pushButton->setEnabled(false);
    ui->statusBar->showMessage("Téléchargement...");

    QString urlString, fileName;
    QString getName, getValue;
    QMap <QString, QString> getNameValueMap;

    urlString = ui->lineEdit->text();
    fileName = ui->lineEdit_2->text();
    getName = ui->lineEdit_3->text();
    getValue = ui->lineEdit_4->text();

    if(!getName.isEmpty())
        getNameValueMap.insert(getName, getValue);

    _networkDownloader->get(urlString, fileName, getNameValueMap);
}

void MainWindow::bytesReceived(qint64 bytesReceived)
{
    qDebug() << Q_FUNC_INFO << bytesReceived;

/*    qDebug() << Q_FUNC_INFO
             << "value:" << ui->progressBar->value()
             << "max:"   << ui->progressBar->maximum();
*/
    if(bytesReceived > ui->progressBar->maximum())
    {
        //qDebug() << Q_FUNC_INFO << "bytesReceived > ui->progressBar->maximum()";
        QString format;
        format += QString::number(bytesReceived);
        format += " o / ?";

        ui->progressBar->setFormat(format);
        //bool bienPrisEnCompte = (format == ui->progressBar->text());
        //qDebug() << Q_FUNC_INFO << ui->progressBar->text();
    }
    else
        ui->progressBar->setValue(bytesReceived);

    //qDebug() << Q_FUNC_INFO << "newValue:" << ui->progressBar->value();
}

void MainWindow::bytesTotal(qint64 bytesTotal)
{
    qDebug() << Q_FUNC_INFO << bytesTotal;

    if(bytesTotal <= 0)
        ui->progressBar->setFormat("(%v o/?)");
    else
    {
        ui->progressBar->setFormat("%p% (%v o/%m o)");
        ui->progressBar->setMaximum(bytesTotal);
    }
}

void MainWindow::error(const QString& errorString)
{
    qDebug() << Q_FUNC_INFO << errorString;

    ui->statusBar->showMessage("Error: " + errorString);
    ui->pushButton->setEnabled(true);
    ui->progressBar->setValue(0);
    ui->progressBar->setMaximum(100);
    ui->progressBar->setFormat("%p%");
}

void MainWindow::finished(QString downloaded)
{
    qDebug() << Q_FUNC_INFO << downloaded;

    ui->statusBar->showMessage("Téléchargé.");
    ui->pushButton->setEnabled(true);
    ui->progressBar->setFormat("%p% (%m o)");

/*
    QFile file(downloaded);
    bool opened = file.open(QIODevice::ReadOnly);

    if(!opened)
    {
        emit error(file.errorString());
        return;
    }

    QString data = file.readAll();
    file.close();

    qDebug() << Q_FUNC_INFO << "data :" << endl << data;
*/
}

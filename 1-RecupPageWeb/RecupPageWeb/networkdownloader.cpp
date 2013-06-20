#include "networkdownloader.h"

#include <QUrl>
#include <QDebug>
#include <QNetworkRequest>
#include <QFile>

NetworkDownloader::NetworkDownloader(QObject* parent)
    : QObject(parent), _bytesReceived(-1), _bytesTotal(-1)
{
    _networkAccessManager = new QNetworkAccessManager(this);
    connect(_networkAccessManager, SIGNAL(finished(QNetworkReply*)),
             this, SLOT(finished(QNetworkReply*)));
}

bool NetworkDownloader::get(const QString& urlString,
                            const QString& fileName)
{
    qDebug() << Q_FUNC_INFO;

    QUrl url(urlString);

    if(!url.isValid())
    {
        qDebug() << Q_FUNC_INFO << "!url.isValid()";
        return false;
    }

    QFile file(fileName);
    bool valid = file.open(QIODevice::ReadWrite);
    file.close();

    if(!valid)
    {
        qDebug() << Q_FUNC_INFO << "file !valid";
        return false;
    }

    _fileName = fileName;
    QNetworkRequest networkRequest(url);
    QNetworkReply* networkReply;
    networkReply = _networkAccessManager->get(networkRequest);
    connect(networkReply, SIGNAL(downloadProgress(qint64,qint64)),
            this, SLOT(downloadProgress(qint64,qint64)));

    return true;
}

void NetworkDownloader::setBytesReceived(qint64 received)
{
    if(received != _bytesReceived)
    {
        qDebug() << Q_FUNC_INFO;

        _bytesReceived = received;
        emit sig_bytesReceived(_bytesReceived);
    }
}

void NetworkDownloader::setBytesTotal(qint64 total)
{
    if(total != _bytesTotal)
    {
        qDebug() << Q_FUNC_INFO;

        _bytesTotal = total;
        emit sig_bytesTotal(_bytesTotal);
    }
}

void NetworkDownloader::downloadProgress(qint64 received, qint64 total)
{
    qDebug() << Q_FUNC_INFO;

    setBytesTotal(total);
    setBytesReceived(received);
}

void NetworkDownloader::finished(QNetworkReply* networkReply)
{
    qDebug() << Q_FUNC_INFO;

    QNetworkReply::NetworkError networkError;
    networkError = networkReply->error();

    if(networkError == QNetworkReply::NoError)
    {
        _bytesReceived = _bytesTotal;
        emit sig_bytesReceived(_bytesReceived);

        QFile file(_fileName);
        file.open(QIODevice::WriteOnly);
        file.write(networkReply->readAll());
        file.close();
        networkReply->deleteLater();
        emit sig_finished(file);
    }
    else
    {
        QString errorString = networkReply->errorString();
        networkReply->deleteLater();
        emit sig_error(errorString);
    }
}

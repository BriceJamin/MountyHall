#include "networkdownloader.h"
#include "url.h"

#include <QUrl>
#include <QDebug>
#include <QNetworkRequest>
#include <QFile>

NetworkDownloader::NetworkDownloader(QObject* parent)
    : QObject(parent),
      _bytesReceived(-2),
      _bytesTotal(-2)
{
    qDebug() << Q_FUNC_INFO;

    _networkAccessManager = new QNetworkAccessManager(this);
    connect(_networkAccessManager, SIGNAL(finished(QNetworkReply*)),
             this, SLOT(finished(QNetworkReply*)));
}

bool NetworkDownloader::get(const QString& urlString,
                            const QString& fileName)
{
    qDebug() << Q_FUNC_INFO;

    _bytesReceived = -2;
    _bytesTotal = -2;

    QUrl url = Url::http(urlString);

    if(!url.isValid())
    {
        qDebug() << Q_FUNC_INFO << "!url.isValid()";
        emit sig_error(url.errorString());
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

// Ne pas mettre de get dans l'url, les passer via le QMap
bool NetworkDownloader::get(const QString& urlString,
                            const QString& fileName,
                            const QMap<QString, QString>& nameValueMap)
{
    qDebug() << Q_FUNC_INFO << urlString << fileName << nameValueMap;

    QMapIterator<QString, QString> mapIterator(nameValueMap);
    QString finalUrlString(urlString);

    finalUrlString += "?";

    while (mapIterator.hasNext())
    {
        mapIterator.next();

        QString name = mapIterator.key();
        QString value = mapIterator.value();

        if(!name.isEmpty())
        {
           finalUrlString += name;
           finalUrlString += "=";
           finalUrlString += value;
           finalUrlString += "&";
        }
    }

    finalUrlString.chop(1);

    qDebug() << "finalUrlString:" << finalUrlString;

    return get(finalUrlString, fileName);
}

void NetworkDownloader::setBytesReceived(qint64 received)
{
    qDebug() << Q_FUNC_INFO;

    _bytesReceived = received;
    emit sig_bytesReceived(_bytesReceived);
}

void NetworkDownloader::setBytesTotal(qint64 total)
{
    qDebug() << Q_FUNC_INFO;

    _bytesTotal = total;
    emit sig_bytesTotal(_bytesTotal);
}

void NetworkDownloader::downloadProgress(qint64 received, qint64 total)
{
    qDebug() << Q_FUNC_INFO << received << total;

    setBytesTotal(total);
    setBytesReceived(received);
}

void NetworkDownloader::finished(QNetworkReply* networkReply)
{
    qDebug() << Q_FUNC_INFO;
    networkReply->deleteLater(); // Parait dangereux mais ne semble pas poser de soucis.

    QNetworkReply::NetworkError networkError;
    networkError = networkReply->error();

    if(networkError != QNetworkReply::NoError)
    {
        qDebug() << Q_FUNC_INFO << "!= NoError";

        QString errorString = networkReply->errorString();
        emit sig_error(errorString);

        return;
    }

    QFile file(_fileName);
    file.remove();

    bool opened = file.open(QIODevice::WriteOnly);
    if(!opened)
    {
        qDebug() << Q_FUNC_INFO << "!opened";

        emit sig_error(file.errorString());
        return;
    }

    if(_bytesReceived > 0)
    {
        bool writen = file.write(networkReply->readAll());
        if(!writen)
        {
            qDebug() << Q_FUNC_INFO << "!writen";

            emit sig_error(file.errorString());
        }
    }

    file.close();
    emit sig_finished(file.fileName());
}

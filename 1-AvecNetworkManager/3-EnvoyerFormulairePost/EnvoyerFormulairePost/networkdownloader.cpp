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
    qDebug() << Q_FUNC_INFO << urlString << fileName;
    QString emptyString;
    return send(urlString, fileName, emptyString);
}

// Ne pas mettre de get dans l'url, les passer via le QMap
bool NetworkDownloader::send(const QString& urlString,
                            const QString& fileName)
{
    qDebug() << Q_FUNC_INFO << urlString << fileName;

    QMapIterator<QString,QString> getMapIterator(_getMap);
    QString finalUrlString(urlString);

    finalUrlString += "?";

    while (getMapIterator.hasNext())
    {
        getMapIterator.next();

        QString name = getMapIterator.key();
        QString value = getMapIterator.value();

        if(!name.isEmpty())
        {
           finalUrlString += name;
           finalUrlString += "=";
           finalUrlString += value;
           finalUrlString += "&";
        }
    }

    finalUrlString.chop(1);

    qDebug() << Q_FUNC_INFO << "GET:" << finalUrlString;

    QMapIterator<QString,QString> postMapIterator(_postMap);
    QString postString;

    while (postMapIterator.hasNext())
    {
        postMapIterator.next();

        QString name = postMapIterator.key();
        QString value = postMapIterator.value();

        if(!name.isEmpty())
        {
            postString += name;
            postString += "=";
            postString += value;
            postString += "&";
        }
    }

    postString.chop(1);

    qDebug() << Q_FUNC_INFO << "Post:" << postString;

    _getMap.clear();
    _postMap.clear();

    return send(finalUrlString, fileName, postString);
}

void NetworkDownloader::addGet(QString name, QString value)
{
    if(!name.isEmpty())
        _getMap.insert(name, value);
}

void NetworkDownloader::addPost(QString name, QString value)
{
    if(!name.isEmpty())
        _postMap.insert(name, value);
}

void NetworkDownloader::setBytesReceived(qint64 received)
{
    //qDebug() << Q_FUNC_INFO << received;

    _bytesReceived = received;
    emit sig_bytesReceived(_bytesReceived);
}

void NetworkDownloader::setBytesTotal(qint64 total)
{
    //qDebug() << Q_FUNC_INFO << total;

    _bytesTotal = total;
    emit sig_bytesTotal(_bytesTotal);
}

bool NetworkDownloader::send(QString urlString, QString fileName, QString postString)
{
    _fileName = fileName;
    _bytesTotal = _bytesReceived = -2;

    QUrl url = Url::http(urlString);

    if(!url.isValid())
    {
        qDebug() << Q_FUNC_INFO << "!url.isValid()";
        emit sig_error(url.errorString());
        return false;
    }

    QNetworkRequest networkRequest(url);
    networkRequest.setRawHeader("User-Agent", "MH_Navigo");
    //networkRequest.setRawHeader("Referer", "Grande Tanière");

    networkRequest.setRawHeader("Content-Type", "application/x-www-form-urlencoded");

    QNetworkReply* networkReply;
    QByteArray postByteArray = postString.toLatin1();

    qDebug() << Q_FUNC_INFO << "FINAL:"
             << url << postByteArray;

    networkReply = _networkAccessManager->post(networkRequest, postByteArray);
    connect(networkReply, SIGNAL(downloadProgress(qint64,qint64)),
            this, SLOT(downloadProgress(qint64,qint64)));

    return true;
}

void NetworkDownloader::downloadProgress(qint64 received, qint64 total)
{
    //qDebug() << Q_FUNC_INFO << received << total;

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

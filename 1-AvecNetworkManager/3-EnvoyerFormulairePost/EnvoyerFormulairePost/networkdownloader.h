#ifndef NETWORKDOWNLOADER_H
#define NETWORKDOWNLOADER_H

#include <QObject>
#include <QFile>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QMap>

class NetworkDownloader : public QObject
{
    Q_OBJECT
public:
    NetworkDownloader(QObject* =0);
    bool get(const QString&, const QString&);
    bool send(const QString&, const QString&);
    void addGet(QString, QString);
    void addPost(QString, QString);

signals:
    void sig_bytesReceived(qint64);
    void sig_bytesTotal(qint64);
    void sig_error(QString);
    void sig_finished(QString);

private:
    void setBytesReceived(qint64);
    void setBytesTotal(qint64);
    bool send(QString, QString, QString);

private slots:
    void downloadProgress(qint64, qint64);
    void finished(QNetworkReply*);

private:
    QNetworkAccessManager* _networkAccessManager;
    QString _fileName;
    qint64 _bytesReceived;
    qint64 _bytesTotal;
    QMap<QString,QString> _getMap;
    QMap<QString,QString> _postMap;
};

#endif // NETWORKDOWNLOADER_H

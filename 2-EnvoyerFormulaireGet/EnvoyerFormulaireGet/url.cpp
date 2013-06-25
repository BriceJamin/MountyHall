#include "url.h"
#include <QRegExp>
#include <QUrl>

QUrl Url::http(const QString& urlString)
{
    return protocol("http", urlString);
}

QUrl Url::https(const QString& urlString)
{
    return protocol("https", urlString);
}

QUrl Url::protocol(const QString& protocolString, const QString& urlString)
{
    QString urlFinalString(urlString);
    QString pattern("^" + protocolString + "://");
    QRegExp regex(pattern);

    if(!urlFinalString.contains(regex))
        urlFinalString.prepend(protocolString + "://");

    return QUrl(urlFinalString, QUrl::StrictMode);
}

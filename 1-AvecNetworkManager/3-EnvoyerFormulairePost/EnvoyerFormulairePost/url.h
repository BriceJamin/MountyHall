#ifndef URL_H
#define URL_H

#include <QUrl>

class Url : public QUrl
{
public:
    static QUrl http(const QString&);
    static QUrl https(const QString&);
    static QUrl protocol(const QString&, const QString&);

private:
    Url();
    Url(const Url&);
    ~Url();
};

#endif // URL_H

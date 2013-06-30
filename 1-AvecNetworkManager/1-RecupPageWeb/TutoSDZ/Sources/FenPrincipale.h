#ifndef HEADER_FENPRINCIPALE
#define HEADER_FENPRINCIPALE

#include <QtGui>
#include <QtNetwork>
#include <QNetworkReply>
#include <QNetworkRequest>
#include <QNetworkAccessManager>

class FenPrincipale : public QWidget
{
    Q_OBJECT


    public:
        FenPrincipale();

    private slots:
        void telechargement();
        void messageErreur(QNetworkReply::NetworkError);
        void enregistrer();
        void progressionDownload(qint64 bytesReceived, qint64 bytesTotal);

    private:
    QVBoxLayout *verticalLayout;
    QHBoxLayout *horizontalLayout;
    QLabel *label;
    QLineEdit *urlEdit;
    QPushButton *demarrerTelechargement;
    QProgressBar *progression;

};

#endif

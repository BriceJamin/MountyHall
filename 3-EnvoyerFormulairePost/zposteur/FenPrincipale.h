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
        void envoi();
        void messageErreur(QNetworkReply::NetworkError);
        void fin();
        void progressionEnvoi(qint64 bytesReceived, qint64 bytesTotal);

    private:
    QVBoxLayout *verticalLayout;
    QFormLayout *formLayout;
    QLabel *label;
    QLineEdit *pseudoEdit;
    QLabel *label_2;
    QLineEdit *textEdit;
    QPushButton *boutonEnvoyer;
    QProgressBar *progression;

};

#endif

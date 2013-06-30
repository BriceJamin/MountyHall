#include "FenPrincipale.h"

FenPrincipale::FenPrincipale()
{
    //On donne une taille par défaut à la fenêtre
    resize(228, 112);
    setWindowTitle("zDownloader");

    //On définit les layouts
    verticalLayout = new QVBoxLayout(this);
    horizontalLayout = new QHBoxLayout();

    label = new QLabel(this);
    label->setText("URL :");
    horizontalLayout->addWidget(label);

    //Le lineEdit qui contient l'URL entrée par le visiteur
    urlEdit = new QLineEdit(this);
    horizontalLayout->addWidget(urlEdit);

    /*On ajoute à notre Layout horizontal (notre fenêtre)
    le layout vertical qui contient le lineEdit et le label "Url"*/
    verticalLayout->addLayout(horizontalLayout);

    demarrerTelechargement = new QPushButton(this);
    demarrerTelechargement->setText("Télécharger");
    verticalLayout->addWidget(demarrerTelechargement);

    progression = new QProgressBar(this);
    progression->setValue(0);
    verticalLayout->addWidget(progression);

    connect(demarrerTelechargement, SIGNAL(clicked()), this, SLOT(telechargement()));
}

void FenPrincipale::telechargement()
{
    demarrerTelechargement->setEnabled(false);
    const QUrl url = QUrl(urlEdit->text()); //On récupère l'URL entrée par l'utilisateur.
    const QNetworkRequest requete(url); //On crée notre requête
    QNetworkAccessManager *m = new QNetworkAccessManager;
    QNetworkReply *r = m->get(requete);
    connect(r, SIGNAL(error(QNetworkReply::NetworkError)), this, SLOT(messageErreur(QNetworkReply::NetworkError)));
    connect(r, SIGNAL(finished()), this, SLOT(enregistrer()));
    connect(r, SIGNAL(downloadProgress(qint64, qint64)), this, SLOT(progressionDownload(qint64, qint64) ));
}

void FenPrincipale::enregistrer()
{
    QNetworkReply *r = qobject_cast<QNetworkReply*>(sender()); //On récupère la réponse du serveur
    QFile f("fichier.txt"); //On ouvre le fichier
    f.open(QIODevice::WriteOnly);
    f.write(r->readAll()); ////On lit la réponse du serveur que l'on met dans un fichier
    f.close(); //On ferme le fichier
    r->deleteLater(); //IMPORTANT : on emploie la fonction deleteLater() pour supprimer la réponse du serveur.
    //Si vous ne le faites pas, vous risquez des fuites de mémoire ou autre.

    QMessageBox::information(this, "Fin de téléchargement", "Téléchargement terminé !"); //On indique que tout s'est bien passé
    close(); //On ferme le programme
}

void FenPrincipale::messageErreur(QNetworkReply::NetworkError)
{
    QNetworkReply *r = qobject_cast<QNetworkReply*>(sender());
    QMessageBox::critical(this, "Erreur", "Erreur lors du chargement. Vérifiez votre connexion internet ou réessayez plus tard <br /><br /> Code de l'erreur : <br /><em>" + r->errorString() + "</em>");
    close();
}

void FenPrincipale::progressionDownload(qint64 bytesReceived, qint64 bytesTotal)
{
    if (bytesTotal != -1)
    {
        progression->setRange(0, bytesTotal);
        progression->setValue(bytesReceived);
    }
}

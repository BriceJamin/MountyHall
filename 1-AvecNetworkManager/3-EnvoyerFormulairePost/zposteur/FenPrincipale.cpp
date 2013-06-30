#include "FenPrincipale.h"

FenPrincipale::FenPrincipale()
{
    resize(212, 123);
    setWindowTitle("zPosteur");

    verticalLayout = new QVBoxLayout(this);
    formLayout = new QFormLayout();

    label = new QLabel(this);
    label->setText("Pseudo :");
    formLayout->setWidget(0, QFormLayout::LabelRole, label);

    pseudoEdit = new QLineEdit(this);
    pseudoEdit->setMaxLength(20);
    formLayout->setWidget(0, QFormLayout::FieldRole, pseudoEdit);

    label_2 = new QLabel(this);
    label_2->setText("Message :");
    formLayout->setWidget(1, QFormLayout::LabelRole, label_2);

    textEdit = new QLineEdit(this);
    textEdit->setMaxLength(200);
    formLayout->setWidget(1, QFormLayout::FieldRole, textEdit);
    verticalLayout->addLayout(formLayout);

    boutonEnvoyer = new QPushButton(this);
    boutonEnvoyer->setText("Envoyer !");
    verticalLayout->addWidget(boutonEnvoyer);

    progression = new QProgressBar(this);
    progression->setValue(0);
    verticalLayout->addWidget(progression);

    connect(boutonEnvoyer, SIGNAL(clicked()), this, SLOT(envoi()));
}

void FenPrincipale::envoi()
{
    boutonEnvoyer->setEnabled(false);
    const QUrl url = QUrl("http://serialtueur.free.fr/sdz/post.php?password=42"); //On récupère l'URL entrée par l'utilisateur.
    const QNetworkRequest requete(url); //On crée notre requête
    QString donnees = "text=" + textEdit->text() + "&pseudo=" + pseudoEdit->text();

    QNetworkAccessManager *m = new QNetworkAccessManager;
    QNetworkReply *r = m->post(requete, donnees.toLatin1());
    connect(r, SIGNAL(error(QNetworkReply::NetworkError)), this, SLOT(messageErreur(QNetworkReply::NetworkError)));
    connect(r, SIGNAL(finished()), this, SLOT(fin()));
    connect(r, SIGNAL(downloadProgress(qint64, qint64)), this, SLOT(progressionEnvoi(qint64, qint64) ));
}

void FenPrincipale::fin()
{
	QMessageBox::information(this, "Fin de post", "L'envoi de données par POST a été effectué avec succès !");
	close();
}

void FenPrincipale::messageErreur(QNetworkReply::NetworkError)
{
    QNetworkReply *r = qobject_cast<QNetworkReply*>(sender());
    QMessageBox::critical(this, "Erreur", "Erreur lors du chargement. Vérifiez votre connexion internet ou réessayez plus tard <br /><br /> Code de l'erreur : <br /><em>" + r->errorString() + "</em>");
    close();
}

void FenPrincipale::progressionEnvoi(qint64 bytesReceived, qint64 bytesTotal)
{
    if (bytesTotal != -1)
    {
        progression->setRange(0, bytesTotal);
        progression->setValue(bytesReceived);
    }
}

class FenPrincipale
!!!134530.cpp!!!	FenPrincipale()

    //On donne une taille par d�faut � la fen�tre
    resize(228, 112);
    setWindowTitle("zDownloader");

    //On d�finit les layouts
    verticalLayout = new QVBoxLayout(this);
    horizontalLayout = new QHBoxLayout();

    label = new QLabel(this);
    label->setText("URL :");
    horizontalLayout->addWidget(label);

    //Le lineEdit qui contient l'URL entr�e par le visiteur
    urlEdit = new QLineEdit(this);
    horizontalLayout->addWidget(urlEdit);

    /*On ajoute � notre Layout horizontal (notre fen�tre)
    le layout vertical qui contient le lineEdit et le label "Url"*/
    verticalLayout->addLayout(horizontalLayout);

    demarrerTelechargement = new QPushButton(this);
    demarrerTelechargement->setText("T�l�charger");
    verticalLayout->addWidget(demarrerTelechargement);

    progression = new QProgressBar(this);
    progression->setValue(0);
    verticalLayout->addWidget(progression);

    connect(demarrerTelechargement, SIGNAL(clicked()), this, SLOT(telechargement()));
!!!134658.cpp!!!	telechargement() : void

    demarrerTelechargement->setEnabled(false);
    const QUrl url = QUrl(urlEdit->text()); //On r�cup�re l'URL entr�e par l'utilisateur.
    const QNetworkRequest requete(url); //On cr�e notre requ�te
    QNetworkAccessManager *m = new QNetworkAccessManager;
    QNetworkReply *r = m->get(requete);
    connect(r, SIGNAL(error(QNetworkReply::NetworkError)), this, SLOT(messageErreur(QNetworkReply::NetworkError)));
    connect(r, SIGNAL(finished()), this, SLOT(enregistrer()));
    connect(r, SIGNAL(downloadProgress(qint64, qint64)), this, SLOT(progressionDownload(qint64, qint64) ));
!!!134914.cpp!!!	enregistrer() : void

    QNetworkReply *r = qobject_cast<QNetworkReply*>(sender()); //On r�cup�re la r�ponse du serveur
    QFile f("fichier.txt"); //On ouvre le fichier
    f.open(QIODevice::WriteOnly);
    f.write(r->readAll()); ////On lit la r�ponse du serveur que l'on met dans un fichier
    f.close(); //On ferme le fichier
    r->deleteLater(); //IMPORTANT : on emploie la fonction deleteLater() pour supprimer la r�ponse du serveur.
    //Si vous ne le faites pas, vous risquez des fuites de m�moire ou autre.

	QMessageBox::information(this, "Fin de t�l�chargement", "T�l�chargement termin� !"); //On indique que tout s'est bien pass�
	close(); //On ferme le programme
!!!134786.cpp!!!	messageErreur(in  : QNetworkReply::NetworkError) : void

    QNetworkReply *r = qobject_cast<QNetworkReply*>(sender());
    QMessageBox::critical(this, "Erreur", "Erreur lors du chargement. V�rifiez votre connexion internet ou r�essayez plus tard <br /><br /> Code de l'erreur : <br /><em>" + r->errorString() + "</em>");
    close();
!!!135042.cpp!!!	progressionDownload(in bytesReceived : qint64, in bytesTotal : qint64) : void

    if (bytesTotal != -1)
    {
        progression->setRange(0, bytesTotal);
        progression->setValue(bytesReceived);
    }

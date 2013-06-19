#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QNetworkAccessManager>

namespace Ui {
    class MainWindow;
}

class MainWindow : public QMainWindow {
    Q_OBJECT
public:
    MainWindow(QWidget *parent = 0);
    ~MainWindow();

protected:
    void changeEvent(QEvent *e);

private:
    Ui::MainWindow *ui;
    QNetworkAccessManager* _networkManager;

private slots:
    void on_pushButton_clicked();
    void replyFinished(QNetworkReply*);
    void downloadProgress(qint64,qint64);
};

#endif // MAINWINDOW_H

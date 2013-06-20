#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "networkdownloader.h"

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
    NetworkDownloader* _networkDownloader;

private slots:
    void on_pushButton_clicked();
    void bytesReceived(qint64);
    void bytesTotal(qint64);
    void error(const QString&);
    void finished(const QFile&);
};

#endif // MAINWINDOW_H

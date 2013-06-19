#include <QtGui/QApplication>
#include "mainwindow.h"

#include <QDebug>
#include <QTextCodec>

int main(int argc, char *argv[])
{
    qDebug();

    QTextCodec::setCodecForCStrings(QTextCodec::codecForName("UTF-8"));
    QTextCodec::setCodecForTr(QTextCodec::codecForName("UTF-8"));

    QApplication a(argc, argv);
    MainWindow w;
    w.show();
    return a.exec();
}

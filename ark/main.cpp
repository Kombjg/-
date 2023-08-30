#include "mainwindow.h"
#include <QMediaPlayer>
#include <QMediaPlaylist>
#include <QUrl>
#include <QApplication>
///////////////////////////////////////////////
///
///
///     Time
///
///
///
int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;

    w.show();
    return a.exec();
}


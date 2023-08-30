#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QUrl>
#include "game.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    QIcon icon(":/head.jpg");
    setWindowIcon(icon);
    setWindowTitle("方舟的骑士");

    //  start.mp3
    playlist.addMedia(QUrl("qrc:/start.mp3"));
    mediaPlayer.setPlaylist(&playlist);
    mediaPlayer.play();
}

MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::on_start_clicked()
{

    game *GAME = new game;
    GAME->show();
    this->hide();
    playlist.removeMedia(0);
    playlist.addMedia(QUrl("qrc:/gascanofvictory.wav"));
    // set the list to QMediaPlayer
    mediaPlayer.setPlaylist(&playlist);
    playlist.setPlaybackMode(QMediaPlaylist::Loop); // set the loop model
    // start to paly
    mediaPlayer.play();


}

void MainWindow::on_pushButton_clicked()
{
    QCoreApplication::exit();
}

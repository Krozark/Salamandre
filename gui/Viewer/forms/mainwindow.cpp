#include "mainwindow.hpp"
#include "ui_mainwindow.h"

#include <QDir>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    this->init();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::init()
{
    QString userFolderPath = "./save/"+this->currentUserId;
    QDir userFolder = QDir(userFolderPath);

    if(!userFolder.exists()){

    }
}

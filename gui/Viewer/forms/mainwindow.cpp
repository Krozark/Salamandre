#include <forms/mainwindow.hpp>
#include <ui_mainwindow.h>


#include <QDir>
#include <QMessageBox>
#include <QDebug>

MainWindow::MainWindow(salamandre::Doctor *doctor, salamandre::Patient *patient, QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    this->doctor = doctor;
    this->patient = patient;

    this->setWindowTitle(this->windowTitle() + " - Patient n°"+this->patient->getId());

    this->init();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::init()
{

}

void MainWindow::startDownloadClientData(int clientNumber)
{
    Q_UNUSED(clientNumber);
}

void MainWindow::createNewClientData()
{
    QDir dir(this->patient->getDirPath());
    if(dir.exists()){
        QMessageBox::information(this, "Patient existant", "Ce patient existe déjà, il sera chargé.");
    }

    dir.mkdir(dir.path());
}

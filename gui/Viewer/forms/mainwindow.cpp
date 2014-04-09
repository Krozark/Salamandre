#include <forms/mainwindow.hpp>
#include <ui_mainwindow.h>
#include <forms/choosedialog.hpp>

#include <QDir>
#include <QDebug>

MainWindow::MainWindow(QString idMedecin, QString passMedecin, QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    this->passMedecin = passMedecin;
    this->idMedecin = idMedecin;

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
        chooseDialog *chDialog = new chooseDialog(this);
        chDialog->setWindowFlags(Qt::Dialog | Qt::FramelessWindowHint | Qt::CustomizeWindowHint | Qt::WindowTitleHint | Qt::WindowMinMaxButtonsHint);
        int res = chDialog->exec();

        if(res  == QDialog::Rejected){
            this->close();
        }

        chooseDialog::Choice choice = chDialog->getChoice();
        int clientNumber = chDialog->getClientNumber();

        switch(choice){
        case chooseDialog::Choice::GET_ALL_CLIENTS_DATA:
            this->startDownloadClientData();
            break;
        case chooseDialog::Choice::GET_ONE_CLIENT_DATA:
            this->startDownloadClientData(clientNumber);
            break;
        case chooseDialog::Choice::NEW_CLIENT_DATA:
            break;
        default:
            break;
        }

        delete chDialog;
    }
}

void MainWindow::startDownloadClientData(int clientNumber)
{
    Q_UNUSED(clientNumber);
}

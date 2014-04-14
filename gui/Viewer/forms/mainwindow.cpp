#include <forms/mainwindow.hpp>
#include <ui_mainwindow.h>
#include <forms/choosedialog.hpp>

#include <QDir>
#include <QMessageBox>
#include <QInputDialog>
#include <QDebug>

MainWindow::MainWindow(salamandre::Doctor *doctor, salamandre::Patient *patient, QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    this->doctor = doctor;
    this->patient = patient;

    this->init();

    //salamandre::RegistryRecord *r = new salamandre::RegistryRecord();
    //r->setAdress("1 rue du test");
    //qDebug() << QString::fromStdString(r->serialize());

    //std::string pass = doctor->getPass().toStdString();
    //std::string encrypt = r->strEncrypt(pass, "coucou");
    //std::string decrypt = r->strDecrypt(pass, encrypt);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::init()
{
    this->setWindowTitle(QString("Salamandre") + " - Patient n°"+this->patient->getId());

    this->ui->lineEdit_patientIdNumber->setText(this->patient->getId());
    this->ui->lineEdit_confidentialPatientNumber->setText(this->patient->getId());
    this->ui->lineEdit_medicalPatientNumber->setText(this->patient->getId());
    this->ui->lineEdit_numericalPatientNumber->setText(this->patient->getId());

    this->loadRecords();
}

void MainWindow::startDownloadClientData(int clientNumber)
{
    Q_UNUSED(clientNumber);
}

void MainWindow::loadRecords()
{
    this->loadFEC();
    this->loadFCT();
    this->loadFMT();
    this->loadFMN();
}

void MainWindow::loadFEC()
{

}

void MainWindow::loadFCT()
{

}

void MainWindow::loadFMT()
{

}

void MainWindow::loadFMN()
{

}

bool MainWindow::checkNeedSave()
{
    return true;
}

void MainWindow::on_actionNouveau_patient_triggered()
{
    if(this->checkNeedSave())
    {
        QString id = QInputDialog::getText(this, QString("Salamandre"), QString("Entrer le numéro du nouveau patient"), QLineEdit::Normal, QString(), nullptr, Qt::Dialog, Qt::ImhDigitsOnly);

        if(!id.isEmpty()){
            salamandre::Patient *newPatient = new salamandre::Patient();
            newPatient->setId(id);
            newPatient->setDirPath(doctor->getDirPath()+"/"+id);
            QDir dirPatient = QDir(newPatient->getDirPath());
            if(!dirPatient.exists()){
                dirPatient.mkdir(dirPatient.path());

                delete this->patient;
                this->patient =  newPatient;
                this->init();
            }
            else{
                delete newPatient;
                QMessageBox::warning(nullptr, "Salamandre", "Ce patient existe déjà dans votre répertoire.");
            }
        }
    }
}

void MainWindow::on_actionChanger_de_patient_triggered()
{
    if(this->checkNeedSave())
    {
        chooseDialog *chDialog = new chooseDialog(this->doctor, nullptr);
        int res = chDialog->exec();

        if(res  == QDialog::Accepted){
            delete this->patient;
            this->patient = chDialog->getPatient();
            this->init();
        }

        delete chDialog;
    }
}

void MainWindow::on_actionEnregistrer_triggered()
{
    //this->patient->getConfidentialRecord()->save();
    //this->patient->getDigitalRecord()->save();
    //this->patient->getMedicalRecord()->save();
    this->patient->getRegistryRecord()->save(this->doctor->getPass().toStdString());
}

void MainWindow::on_actionQuitter_triggered()
{
    if(this->checkNeedSave())
    {
        this->close();
    }
}


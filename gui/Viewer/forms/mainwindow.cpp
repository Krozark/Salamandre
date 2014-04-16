#include <forms/mainwindow.hpp>
#include <ui_mainwindow.h>
#include <forms/choosedialog.hpp>

#include <QDir>
#include <QMessageBox>
#include <QInputDialog>
#include <QDebug>
#include <QDate>

MainWindow::MainWindow(salamandre::Doctor *doctor, salamandre::Patient *patient, QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    this->doctor = doctor;
    this->patient = patient;
    this->listViewDigitalFiles = new ListView(nullptr);
    this->threadUpload = new threadUploadFile(this->patient->getDirPath(), nullptr);

    this->connect(this->listViewDigitalFiles, SIGNAL(dropFile(QStringList)), SLOT(startUploadDigitalFile(QStringList)));

    this->init();
}

MainWindow::~MainWindow()
{
    delete this->threadUpload;
    delete this->listViewDigitalFiles;
    delete ui;
}

void MainWindow::init()
{
    this->setWindowTitle(QString("Salamandre") + " - Patient n°"+this->patient->getId());

    this->ui->verticalLayout_listView->addWidget(this->listViewDigitalFiles);

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
    salamandre::RegistryRecord *record = this->patient->getRegistryRecord();

    if(QFile::exists(QString::fromStdString(record->getFilePath())))
    {
        record->load(this->doctor->getPass().toStdString());

        this->ui->lineEdit_patientLastName->setText(QString::fromStdString(record->getLastName()));
        this->ui->lineEdit_patientFirstName->setText(QString::fromStdString(record->getFirstName()));
        this->ui->lineEdit_patientAdress->setText(QString::fromStdString(record->getAdress()));
        this->ui->dateEdit_patientBirthDate->setDate(QDate::fromString(QString::fromStdString(record->getBirthDate()), "yyyy-MM-dd"));
        if(QString::fromStdString(record->getSex()) == "M")
            this->ui->radioButton_patientSexMale->setChecked(true);
        else
            this->ui->radioButton_patientSexFemale->setChecked(true);
    }
    else{
        record->setVersionNumber(0); // will be automatically increment at save.
    }
}

void MainWindow::loadFCT()
{
    salamandre::ConfidentialRecord *record = this->patient->getConfidentialRecord();

    if(QFile::exists(QString::fromStdString(record->getFilePath())))
    {
        record->load(this->doctor->getPass().toStdString());

        this->ui->plainTextEdit_confidentialTextPatient->setPlainText(QString::fromStdString(record->getContent()));
    }
    else{
        record->setVersionNumber(0); // will be automatically increment at save.
    }
}

void MainWindow::loadFMT()
{
    salamandre::MedicalRecord *record = this->patient->getMedicalRecord();

    if(QFile::exists(QString::fromStdString(record->getFilePath())))
    {
        record->load(this->doctor->getPass().toStdString());

        this->ui->plainTextEdit_medicalTextPatient->setPlainText(QString::fromStdString(record->getContent()));
    }
    else{
        record->setVersionNumber(0); // will be automatically increment at save.
    }
}

void MainWindow::loadFMN()
{

}

void MainWindow::saveRecords()
{
    this->saveFEC();
    this->saveFCT();
    this->saveFMT();
    this->saveFMN();
}

void MainWindow::saveFEC()
{
    salamandre::RegistryRecord *record = this->patient->getRegistryRecord();
    record->setAdress(this->ui->lineEdit_patientAdress->text().toStdString());
    record->setBirthDate(this->ui->dateEdit_patientBirthDate->date().toString("yyyy-MM-dd").toStdString());
    record->setFirstName(this->ui->lineEdit_patientFirstName->text().toStdString());
    record->setLastName(this->ui->lineEdit_patientLastName->text().toStdString());
    if(this->ui->radioButton_patientSexMale->isChecked())
        record->setSex("M");
    else
        record->setSex("F");

    record->save(this->doctor->getPass().toStdString());
}

void MainWindow::saveFCT()
{
    salamandre::ConfidentialRecord *record = this->patient->getConfidentialRecord();
    record->setContent(this->ui->plainTextEdit_confidentialTextPatient->toPlainText().toStdString());

    record->save(this->doctor->getPass().toStdString());
}

void MainWindow::saveFMT()
{
    salamandre::MedicalRecord *record = this->patient->getMedicalRecord();
    record->setContent(this->ui->plainTextEdit_medicalTextPatient->toPlainText().toStdString());

    record->save(this->doctor->getPass().toStdString());
}

void MainWindow::saveFMN()
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

    this->saveRecords();
}

void MainWindow::on_actionQuitter_triggered()
{
    if(this->checkNeedSave())
    {
        this->close();
    }
}

void MainWindow::startUploadDigitalFile(QStringList listFile)
{
    this->threadUpload->addFileToUpload(listFile);
}

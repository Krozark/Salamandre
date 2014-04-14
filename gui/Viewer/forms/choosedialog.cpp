#include <forms/choosedialog.hpp>
#include <ui_choosedialog.h>

#include <QDir>
#include <QMessageBox>
#include <QDebug>

chooseDialog::chooseDialog(salamandre::Doctor *doctor, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::chooseDialog)
{
    ui->setupUi(this);

    this->doctor = doctor;
    this->patient = new salamandre::Patient();

    if(this->doctor->getType() == salamandre::Doctor::TypeDoctor::DOCTOR_ALREADY_EXIST){
        this->model = new QStandardItemModel();
        this->filterModel = new QSortFilterProxyModel();

        QDir dir(this->doctor->getDirPath());
        if(!dir.exists()){
            QMessageBox::critical(this, "Erreur critique", "Une erreur critique s'est produite.");
            this->reject();
        }

        QFileInfoList listInfo = dir.entryInfoList(QDir::AllDirs | QDir::NoDotAndDotDot, QDir::Time);
        int nbSubDir = listInfo.size();

        QStringList fileFilter = QStringList() << QString::fromStdString(salamandre::ConfidentialRecord::fileName)
                                               << QString::fromStdString(salamandre::MedicalRecord::fileName)
                                               << QString::fromStdString(salamandre::DigitalRecord::fileName)
                                               << QString::fromStdString(salamandre::RegistryRecord::fileName);

        for(int i = 0; i < nbSubDir; ++i){
            QFileInfo dInfo = listInfo.at(i);
            QDir dirClient(dInfo.absoluteFilePath());

            QFileInfoList listFileInfo = dirClient.entryInfoList(fileFilter, QDir::Files | QDir::NoDotAndDotDot, QDir::Time);
            int nbSubFile = listFileInfo.size();

            QStandardItem *item = new QStandardItem(dInfo.fileName());
            item->setData(dInfo.fileName());

            if(nbSubFile != 4){
                item->setIcon(QPixmap(":/icons/controls/update.png"));

                if(nbSubFile == 3)
                    item->setToolTip("Une fiche est manquante, elle sera récupérée.");
                else
                    item->setToolTip("Certaines fiches sont manquantes, elles seront récupérées.");
            }

            this->model->appendRow(item);
        }

        this->filterModel->setSourceModel(this->model);
        this->ui->listView_availablePatient->setModel(this->filterModel);
        this->ui->label_firstConnection->setVisible(false);
        this->ui->stackedWidget->setCurrentIndex(this->doctor->getType());
    }
    else if(this->doctor->getType() == salamandre::Doctor::TypeDoctor::DOCTOR_ALREADY_EXIST_BUT_NOTHING){
        this->ui->label_firstConnection->setVisible(true);
        this->ui->stackedWidget->setCurrentIndex(salamandre::Doctor::TypeDoctor::DOCTOR_ALREADY_EXIST);
    }
    else{
        this->ui->stackedWidget->setCurrentIndex(this->doctor->getType());
    }
}

chooseDialog::~chooseDialog()
{
    delete ui;
}

void chooseDialog::accept()
{
    switch(this->ui->stackedWidget->currentIndex()){
    case salamandre::Doctor::TypeDoctor::NEW_DOCTOR:
        this->patient->setId(this->ui->lineEdit_newMedecinAndClientData->text());
        this->patient->setType(salamandre::Patient::TypePatient::NEW_PATIENT);
        break;
    case salamandre::Doctor::TypeDoctor::DOCTOR_ALREADY_EXIST:
        if(this->ui->radioButton_availablePatient->isChecked()){
            QItemSelectionModel *index = this->ui->listView_availablePatient->selectionModel();
            this->patient->setId(this->model->item(index->currentIndex().row())->data().toString());
            this->patient->setType(salamandre::Patient::TypePatient::PATIENT_ALREADY_EXIST);
        }
        else if(this->ui->radioButton_newClientData->isChecked()){
            this->patient->setId(this->ui->lineEdit_newClientData->text());
            this->patient->setType(salamandre::Patient::TypePatient::NEW_PATIENT);
        }
        else if(this->ui->radioButton_getDataClient->isChecked()){
            this->patient->setId(this->ui->lineEdit_getDataClient->text());
            this->patient->setType(salamandre::Patient::TypePatient::PATIENT_ALREADY_EXIST);
        }
        break;
    default:
        QMessageBox::critical(this, "Erreur critique", "Une erreur critique s'est produite.");
        this->reject();
        break;
    }

    this->patient->setDirPath(this->doctor->getDirPath()+"/"+this->patient->getId());
    QDir dirPatient(this->patient->getDirPath());

    if(!dirPatient.exists()){
        dirPatient.mkdir(dirPatient.path());
    }

    if(this->patient->getType() == salamandre::Patient::TypePatient::NEW_PATIENT){
        this->createNewRecords();
    }
    else{
        this->loadAllRecords();
    }

    QDialog::accept();
}

void chooseDialog::loadAllRecords()
{
    QStringList fileFilter = QStringList() << QString::fromStdString(salamandre::ConfidentialRecord::fileName)
                                           << QString::fromStdString(salamandre::MedicalRecord::fileName)
                                           << QString::fromStdString(salamandre::DigitalRecord::fileName)
                                           << QString::fromStdString(salamandre::RegistryRecord::fileName);

    QDir dirPatient(this->patient->getDirPath());
    QFileInfoList listFileInfo = dirPatient.entryInfoList(fileFilter, QDir::Files | QDir::NoDotAndDotDot);

    int nbFile = listFileInfo.size();

    std::string key = this->doctor->getPass().toStdString();

    if(nbFile == 4){
        this->patient->setConfidentialRecord(new salamandre::ConfidentialRecord(this->patient->getDirPath().toStdString()+"/"+salamandre::ConfidentialRecord::fileName, key));
        this->patient->setDigitalRecord(new salamandre::DigitalRecord(this->patient->getDirPath().toStdString()+"/"+salamandre::DigitalRecord::fileName, key));
        this->patient->setMedicalRecord(new salamandre::MedicalRecord(this->patient->getDirPath().toStdString()+"/"+salamandre::MedicalRecord::fileName, key));
        this->patient->setRegistryRecord(new salamandre::RegistryRecord(this->patient->getDirPath().toStdString()+"/"+salamandre::RegistryRecord::fileName, key));
    }
    else{
        this->patient->setConfidentialRecord(new salamandre::ConfidentialRecord(this->patient->getDirPath().toStdString()+"/"+salamandre::ConfidentialRecord::fileName, key));
        this->patient->setDigitalRecord(new salamandre::DigitalRecord(this->patient->getDirPath().toStdString()+"/"+salamandre::DigitalRecord::fileName, key));
        this->patient->setMedicalRecord(new salamandre::MedicalRecord(this->patient->getDirPath().toStdString()+"/"+salamandre::MedicalRecord::fileName, key));
        this->patient->setRegistryRecord(new salamandre::RegistryRecord(this->patient->getDirPath().toStdString()+"/"+salamandre::RegistryRecord::fileName, key));

        for(int i = 0; i < nbFile; ++i){

        }
    }

}

void chooseDialog::createNewRecords()
{
    std::string key = this->doctor->getPass().toStdString();

    this->patient->setConfidentialRecord(new salamandre::ConfidentialRecord(this->patient->getDirPath().toStdString()+"/"+salamandre::ConfidentialRecord::fileName, key));
    this->patient->setDigitalRecord(new salamandre::DigitalRecord(this->patient->getDirPath().toStdString()+"/"+salamandre::DigitalRecord::fileName, key));
    this->patient->setMedicalRecord(new salamandre::MedicalRecord(this->patient->getDirPath().toStdString()+"/"+salamandre::MedicalRecord::fileName, key));
    this->patient->setRegistryRecord(new salamandre::RegistryRecord(this->patient->getDirPath().toStdString()+"/"+salamandre::RegistryRecord::fileName, key));
}

salamandre::Patient* chooseDialog::getPatient()
{
    return this->patient;
}

void chooseDialog::on_radioButton_newClientData_clicked()
{
    this->ui->listView_availablePatient->setEnabled(false);
    this->ui->lineEdit_newClientData->setEnabled(true);
    this->ui->lineEdit_getDataClient->setEnabled(false);

    this->ui->buttonBox->setEnabled(!this->ui->lineEdit_newClientData->text().isEmpty());
}

void chooseDialog::on_radioButton_getDataClient_clicked()
{
    this->ui->listView_availablePatient->setEnabled(false);
    this->ui->lineEdit_getDataClient->setEnabled(true);
    this->ui->lineEdit_newClientData->setEnabled(false);

    this->ui->buttonBox->setEnabled(!this->ui->lineEdit_getDataClient->text().isEmpty());
}

void chooseDialog::on_radioButton_availablePatient_clicked()
{
    this->ui->listView_availablePatient->setEnabled(true);
    this->ui->lineEdit_getDataClient->setEnabled(false);
    this->ui->lineEdit_newClientData->setEnabled(false);

    this->ui->buttonBox->setEnabled(this->ui->listView_availablePatient->currentIndex().isValid());
}

void chooseDialog::on_lineEdit_newMedecinAndClientData_textChanged(const QString &arg1)
{
    this->ui->buttonBox->setEnabled(!arg1.isEmpty());
}

void chooseDialog::on_listView_availablePatient_clicked(const QModelIndex &index)
{
    if(this->ui->radioButton_availablePatient->isChecked())
        this->ui->buttonBox->setEnabled(index.isValid());
    else
        this->ui->buttonBox->setEnabled(false);
}

void chooseDialog::on_lineEdit_newClientData_textChanged(const QString &arg1)
{
    if(this->ui->radioButton_newClientData->isChecked())
        this->ui->buttonBox->setEnabled(!arg1.isEmpty());
    else
        this->ui->buttonBox->setEnabled(false);
}

void chooseDialog::on_lineEdit_getDataClient_textChanged(const QString &arg1)
{
    if(this->ui->radioButton_getDataClient->isChecked())
        this->ui->buttonBox->setEnabled(!arg1.isEmpty());
    else
        this->ui->buttonBox->setEnabled(false);
}

void chooseDialog::on_lineEdit_researchPatient_textChanged(const QString &arg1)
{
    this->filterModel->setFilterRegExp(arg1);
}

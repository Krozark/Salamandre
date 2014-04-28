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
    this->patient = nullptr;

    this->model = new QStandardItemModel();
    this->filterModel = new QSortFilterProxyModel();

    if(this->doctor->getType() == salamandre::Doctor::TypeDoctor::DOCTOR_ALREADY_EXIST){
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
    delete this->model;
    delete this->filterModel;
    delete ui;
}

void chooseDialog::reject()
{
    delete this->doctor;
    delete this->patient;
    QDialog::reject();
}

void chooseDialog::accept()
{
    QString idPatient = "";
    salamandre::Patient::TypePatient typePatient = salamandre::Patient::TypePatient::NEW_PATIENT;

    switch(this->ui->stackedWidget->currentIndex()){
    case salamandre::Doctor::TypeDoctor::NEW_DOCTOR:
        idPatient = this->ui->lineEdit_newMedecinAndClientData->text();
        typePatient = salamandre::Patient::TypePatient::NEW_PATIENT;
        break;
    case salamandre::Doctor::TypeDoctor::DOCTOR_ALREADY_EXIST:
        if(this->ui->radioButton_availablePatient->isChecked()){
            QItemSelectionModel *index = this->ui->listView_availablePatient->selectionModel();
            idPatient = this->model->item(index->currentIndex().row())->data().toString();
            typePatient = salamandre::Patient::TypePatient::PATIENT_ALREADY_EXIST;
        }
        else if(this->ui->radioButton_newClientData->isChecked()){
            idPatient = this->ui->lineEdit_newClientData->text();
            typePatient = salamandre::Patient::TypePatient::NEW_PATIENT;

        }
        else if(this->ui->radioButton_getDataClient->isChecked()){
            idPatient = this->ui->lineEdit_getDataClient->text();
            typePatient = salamandre::Patient::TypePatient::PATIENT_ALREADY_EXIST;
        }
        break;
    default:
        QMessageBox::critical(this, "Erreur critique", "Une erreur critique s'est produite.");
        this->reject();
        break;
    }

    QString dirPathPatient = this->doctor->getDirPath()+"/"+idPatient;
    QDir dirPatient(dirPathPatient);

    if(!dirPatient.exists()){
        dirPatient.mkdir(dirPatient.path());
    }

    this->patient = new salamandre::Patient(dirPathPatient);
    this->patient->setId(idPatient);
    this->patient->setType(typePatient);

    QDialog::accept();
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

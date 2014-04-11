#include <forms/choosedialog.hpp>
#include <ui_choosedialog.h>

#include <objects/FicheConfidentielle.hpp>
#include <objects/FicheDonneesMedicales.hpp>
#include <objects/FicheDonneesNumeriques.hpp>
#include <objects/FicheEtatCivil.hpp>

#include <QDir>
#include <QMessageBox>
#include <QDebug>

chooseDialog::chooseDialog(Doctor *doctor, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::chooseDialog)
{
    ui->setupUi(this);

    this->doctor = doctor;
    this->patient = new Patient();

    if(this->doctor->getType() == Doctor::TypeDoctor::NEW_DOCTOR){
        this->ui->widget_clientNumber->setVisible(false);
        this->ui->lineEdit_clientNumber->setValidator(new QIntValidator());
    }
    else if(this->doctor->getType() == Doctor::TypeDoctor::DOCTOR_ALREADY_EXIST){
        this->model = new QStandardItemModel();

        QDir dir(this->doctor->getDirPath());
        if(!dir.exists()){
            QMessageBox::critical(this, "Erreur critique", "Une erreur critique s'est produite.");
            this->reject();
        }

        QFileInfoList listInfo = dir.entryInfoList(QDir::AllDirs | QDir::NoDotAndDotDot, QDir::Time);
        int nbSubDir = listInfo.size();

        QStringList fileFilter = QStringList() << QString::fromStdString(salamandre::FicheConfidentielle::fileName)
                                               << QString::fromStdString(salamandre::FicheDonneesMedicales::fileName)
                                               << QString::fromStdString(salamandre::FicheDonneesNumeriques::fileName)
                                               << QString::fromStdString(salamandre::FicheEtatCivil::fileName);

        for(int i = 0; i < nbSubDir; ++i){
            QFileInfo dInfo = listInfo.at(i);
            QDir dirClient(dInfo.absoluteFilePath());

            QFileInfoList listFileInfo = dirClient.entryInfoList(fileFilter, QDir::Files | QDir::NoDotAndDotDot, QDir::Time);
            int nbSubFile = listFileInfo.size();

            QStandardItem *item = new QStandardItem(dInfo.fileName());
            item->setData(dInfo.fileName());

            if(nbSubFile != 4){
                item->setIcon(QPixmap(":/icons/controls/update.png"));
                item->setToolTip("Une fiche est manquante, elle sera récupérée.");
            }

            this->model->appendRow(item);
        }

        this->ui->listView_availablePatient->setModel(this->model);
    }
    else{
    }

    this->ui->stackedWidget->setCurrentIndex(this->doctor->getType());

}

chooseDialog::~chooseDialog()
{
    delete this->ui->lineEdit_clientNumber->validator();
    delete ui;
}

void chooseDialog::close()
{
    this->patient->setId(this->getPatientNumber());
    QDialog::close();
}

Patient* chooseDialog::getPatient()
{
    return this->patient;
}

chooseDialog::Choice chooseDialog::getChoice()
{
    if(this->ui->radioButton_getAllClientsFiles->isChecked()){
        return Choice::GET_ALL_CLIENTS_DATA;
    }
    else if(this->ui->radioButton_getClientFiles->isChecked()){
        return Choice::GET_ONE_CLIENT_DATA;
    }
    else if(this->ui->radioButton_createClientFiles->isChecked()){
        return Choice::NEW_CLIENT_DATA;
    }

    return Choice::NEW_CLIENT_DATA;
}

QString chooseDialog::getPatientNumber()
{
    return this->ui->lineEdit_clientNumber->text();
}

void chooseDialog::on_radioButton_getAllClientsFiles_clicked()
{
    this->ui->widget_clientNumber->setVisible(false);
    this->ui->buttonBox->setEnabled(true);
}

void chooseDialog::on_radioButton_getClientFiles_clicked()
{
    this->ui->widget_clientNumber->setVisible(true);
    this->ui->buttonBox->setEnabled(!this->ui->lineEdit_clientNumber->text().isEmpty());
}

void chooseDialog::on_radioButton_createClientFiles_clicked()
{
    this->ui->widget_clientNumber->setVisible(true);
    this->ui->buttonBox->setEnabled(!this->ui->lineEdit_clientNumber->text().isEmpty());
}

void chooseDialog::on_lineEdit_clientNumber_textChanged(const QString &arg1)
{
    this->ui->buttonBox->setEnabled(!arg1.isEmpty());
}

void chooseDialog::on_radioButton_newClientData_clicked()
{
    this->ui->listView_availablePatient->setEnabled(false);
    this->ui->lineEdit_newClientData->setEnabled(true);
    this->ui->lineEdit_getDataClient->setEnabled(false);
}

void chooseDialog::on_radioButton_getDataClient_clicked()
{
    this->ui->listView_availablePatient->setEnabled(false);
    this->ui->lineEdit_getDataClient->setEnabled(true);
    this->ui->lineEdit_newClientData->setEnabled(false);
}

void chooseDialog::on_radioButton_availablePatient_clicked()
{
    this->ui->listView_availablePatient->setEnabled(true);
    this->ui->lineEdit_getDataClient->setEnabled(false);
    this->ui->lineEdit_newClientData->setEnabled(false);
}

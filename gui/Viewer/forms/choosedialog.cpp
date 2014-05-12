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
    this->validator = new QIntValidator(nullptr);
    this->updfileDialog = new updateFileDialog(this);

    this->filterModel->setSourceModel(this->model);
    this->ui->listView_availablePatient->setModel(this->filterModel);

    this->ui->lineEdit_getDataClient->setValidator(this->validator);
    this->ui->lineEdit_newClientData->setValidator(this->validator);
    this->ui->lineEdit_newMedecinAndClientData->setValidator(this->validator);
    this->ui->lineEdit_researchPatient->setValidator(this->validator);

    this->connect(this->updfileDialog, SIGNAL(accepted()), this, SLOT(updatePatientAvailable()));

    this->contextMenu = new QMenu();
    this->actionMaj = new QAction(QPixmap(":/icons/controls/update.png"), "Mettre à jour les fiches de ce patient", nullptr);
    this->connect(this->actionMaj, SIGNAL(triggered()), this, SLOT(updatePatientRecord()));

    this->contextMenu->addAction(this->actionMaj);

    this->setContextMenuPolicy(Qt::CustomContextMenu);
    this->connect(this, SIGNAL(customContextMenuRequested(QPoint)), this, SLOT(showContextMenu(const QPoint)));

    if(this->doctor->getType() == salamandre::Doctor::TypeDoctor::DOCTOR_ALREADY_EXIST){
        this->updatePatientAvailable();
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

void chooseDialog::updatePatientAvailable()
{
    this->model->clear();

    QDir dir(this->doctor->getDirPath());
    if(!dir.exists()){
        QMessageBox::critical(this, "Erreur critique", "Une erreur critique s'est produite.\nLe dossier du médecin à été supprimé durant l'exécution de l'application.\nMerci de redémarrer celle-ci.");
        this->reject();
    }

    QFileInfoList listInfo = dir.entryInfoList(QDir::AllDirs | QDir::NoDotAndDotDot, QDir::Time);
    int nbSubDir = listInfo.size();

    std::string FCTName = salamandre::ConfidentialRecord::getFileName();
    std::string FECName = salamandre::RegistryRecord::getFileName();
    std::string FMNName = salamandre::DigitalRecord::getFileName();
    std::string FMTName = salamandre::MedicalRecord::getFileName();

    QStringList fileFilter = QStringList() << QString::fromStdString(FCTName)
                                           << QString::fromStdString(FMTName)
                                           << QString::fromStdString(FMNName)
                                           << QString::fromStdString(FECName);

    for(int i = 0; i < nbSubDir; ++i){
        QFileInfo dInfo = listInfo.at(i);
        QDir dirClient(dInfo.absoluteFilePath());

        QFileInfoList listFileInfo = dirClient.entryInfoList(fileFilter, QDir::Files | QDir::NoDotAndDotDot, QDir::Time);
        int nbSubFile = listFileInfo.size();

        patientData *data = new patientData(dInfo.fileName(), nbSubFile != 4);

        QStandardItem *item = new QStandardItem(dInfo.fileName());
        item->setData(QVariant::fromValue(data));

        if(data->needUpdate){
            QFileInfo fInfo;
            qDebug() << "nbsubfile " << nbSubFile;

            for(int i = 0; i < nbSubFile; ++i){
                 fInfo = listFileInfo.at(i);
                 std::string fName = fInfo.fileName().toStdString();
                 qDebug() << "nbsubfile " << QString::fromStdString(fName);

                 if(fName == FCTName)
                     data->needUpdateFCT = false;
                 else if(fName == FECName)
                     data->needUpdateFEC = false;
                 else if(fName == FMTName)
                     data->needUpdateFMT = false;
                 else if(fName == FMNName)
                     data->needUpdateFMN = false;
            }

            item->setIcon(QPixmap(":/icons/controls/update.png"));

            if(nbSubFile == 3)
                item->setToolTip("Une fiche est manquante, elle sera récupérée.");
            else
                item->setToolTip("Certaines fiches sont manquantes, elles seront récupérées.");
        }

        this->model->appendRow(item);
    }
}

chooseDialog::~chooseDialog()
{
    delete this->model;
    delete this->filterModel;
    delete this->contextMenu;
    delete this->actionMaj;
    delete this->ui;
}

void chooseDialog::reject()
{
    delete this->patient;
    QDialog::reject();
}

void chooseDialog::accept()
{
    QString idPatient = "";
    bool needUpdate = false;
    bool needFullUpdate = false;

    patientData *data = nullptr;

    salamandre::Patient::TypePatient typePatient = salamandre::Patient::TypePatient::NEW_PATIENT;

    switch(this->ui->stackedWidget->currentIndex()){
    case salamandre::Doctor::TypeDoctor::NEW_DOCTOR:
        idPatient = this->ui->lineEdit_newMedecinAndClientData->text();
        typePatient = salamandre::Patient::TypePatient::NEW_PATIENT;
        break;
    case salamandre::Doctor::TypeDoctor::DOCTOR_ALREADY_EXIST:
        if(this->ui->radioButton_availablePatient->isChecked()){
            QItemSelectionModel *index = this->ui->listView_availablePatient->selectionModel();
            QModelIndex mIndex = index->currentIndex();

            if(mIndex.isValid()){
                data = this->model->item(mIndex.row())->data().value<patientData*>();
                idPatient = data->id;
                needUpdate = data->needUpdate;
                typePatient = salamandre::Patient::TypePatient::PATIENT_ALREADY_EXIST;

                if(needUpdate){
                    QMessageBox::StandardButton updateBox;
                    updateBox = QMessageBox::question(this, "Mise à jour requise", "Certaines données sont manquantes, voulez-vous tenter de les récupérer sur le réseau ?<br/>", QMessageBox::Yes|QMessageBox::No|QMessageBox::Cancel);

                    if(updateBox == QMessageBox::No){
                        needUpdate = false;
                    }
                    else if(updateBox == QMessageBox::Cancel){
                        return;
                    }
                }
            }
            else
                return;
        }
        else if(this->ui->radioButton_newClientData->isChecked()){
            idPatient = this->ui->lineEdit_newClientData->text();
            typePatient = salamandre::Patient::TypePatient::NEW_PATIENT;
        }
        else if(this->ui->radioButton_getDataClient->isChecked()){
            idPatient = this->ui->lineEdit_getDataClient->text();
            needFullUpdate = true;
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

    if(needUpdate || needFullUpdate){
        this->updatePatientRecord();
        return;
    }

    QDialog::accept();
}

void chooseDialog::updateRecord(patientData *data)
{
    QString idPatient = data->id;

    if(data->needUpdateFCT)
        this->updfileDialog->askFile(this->doctor->getId().toInt(), idPatient.toInt(), salamandre::ConfidentialRecord::getFileName());
    if(data->needUpdateFEC)
        this->updfileDialog->askFile(this->doctor->getId().toInt(), idPatient.toInt(), salamandre::RegistryRecord::getFileName());
    if(data->needUpdateFMT)
        this->updfileDialog->askFile(this->doctor->getId().toInt(), idPatient.toInt(), salamandre::MedicalRecord::getFileName());
    if(data->needUpdateFMN)
        this->updfileDialog->askFile(this->doctor->getId().toInt(), idPatient.toInt(), salamandre::DigitalRecord::getFileName());
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
void chooseDialog::on_pushButton_resfreshPatient_clicked()
{
    this->updfileDialog->askFile(this->doctor->getId().toInt());
}

void chooseDialog::on_listView_availablePatient_doubleClicked(const QModelIndex &index)
{
    Q_UNUSED(index);
    this->accept();
}

void chooseDialog::updatePatientRecord()
{
    qDebug() << "update patient record";
    QString idPatient;
    bool needFullUpdate = false;

    if(this->ui->radioButton_availablePatient->isChecked()){
        QItemSelectionModel *index = this->ui->listView_availablePatient->selectionModel();  
        QModelIndex mIndex = index->currentIndex();

        if(mIndex.isValid()){
            patientData *data = this->model->item(mIndex.row())->data().value<patientData*>();
            idPatient = data->id;

            bool needUpdate = data->needUpdate;
            needFullUpdate = false;

            if(needUpdate){
                if(data->needUpdateFCT && data->needUpdateFEC && data->needUpdateFMN && data->needUpdateFMT)
                    needFullUpdate = true;
                else
                    this->updateRecord(data);
            }
        }
        else
            return;
    }
    else if(this->ui->radioButton_getDataClient->isChecked()){
        idPatient = this->ui->lineEdit_getDataClient->text();
        needFullUpdate = true;
    }

    if(needFullUpdate)
        this->updfileDialog->askFile(this->doctor->getId().toInt(), idPatient.toInt());
}

void chooseDialog::showContextMenu(QPoint p)
{
    this->contextMenu->exec(this->mapToGlobal(p));
}

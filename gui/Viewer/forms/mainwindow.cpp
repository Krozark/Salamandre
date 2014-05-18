#include <forms/mainwindow.hpp>
#include <ui_mainwindow.h>
#include <forms/choosedialog.hpp>
#include <forms/patientnamedialog.hpp>

#include <objects/socksender.hpp>
#include <objects/settings.hpp>

#include <QDir>
#include <QMessageBox>
#include <QInputDialog>
#include <QDebug>
#include <QDate>
#include <QFileDialog>

#define STATUS_BAR_HEIGHT 22

MainWindow::MainWindow(salamandre::Doctor *doctor, salamandre::Patient *patient, QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    this->doctor = doctor;
    this->patient = patient;

    this->init();
}

MainWindow::~MainWindow()
{
    this->clearPatient();

    delete this->doctor;
    delete ui;
}

void MainWindow::closeEvent(QCloseEvent *event)
{
    this->currentAction = ACTION_QUIT;

    if(!this->badPass)
        this->checkNeedSave();

    if(this->threadSaveRecord->isRunning())
        event->ignore();
}

void MainWindow::init()
{
    qDebug() << "init";
    this->listViewDigitalFiles = new ListView(this->patient->getDirPath(), nullptr);
    this->threadUpload = new threadUploadFile(this->patient, this->doctor, nullptr);
    this->threadSaveRecord = new threadSave(this->patient, this->doctor, nullptr);
    this->saveRecordDialog = new saveDialog(this);
    this->currentAction = ACTION_NO;
    this->restartApplication = false;

    this->connect(this->listViewDigitalFiles, SIGNAL(dropFile(QStringList)), SLOT(startUploadDigitalFile(QStringList)));
    this->connect(this->listViewDigitalFiles, SIGNAL(needToRestartApps()), this, SLOT(restartNeeded()));
    this->connect(this->threadUpload, SIGNAL(newFileInserted()), this, SLOT(refreshDigitalFile()));
    this->connect(this->threadSaveRecord, SIGNAL(setProgressText(QString)), this->saveRecordDialog, SLOT(setProgressText(QString)));
    this->connect(this->threadSaveRecord, SIGNAL(setProgress(int)), this->saveRecordDialog, SLOT(setProgress(int)));
    this->connect(this->threadSaveRecord, SIGNAL(saveProgress(int)), this, SLOT(saveProgress(int)));
    this->connect(this->threadSaveRecord, SIGNAL(finished()), this, SLOT(saveEnd()));

    this->setWindowTitle(QString(APPS_NAME) + " - Patient n°"+this->patient->getId());

    this->ui->verticalLayout_listView->addWidget(this->listViewDigitalFiles);

    this->ui->lineEdit_patientIdNumber->setText(this->patient->getId());
    this->ui->lineEdit_confidentialPatientNumber->setText(this->patient->getId());
    this->ui->lineEdit_medicalPatientNumber->setText(this->patient->getId());
    this->ui->lineEdit_numericalPatientNumber->setText(this->patient->getId());

    this->doctor->setType(salamandre::Doctor::TypeDoctor::DOCTOR_ALREADY_EXIST);

    this->initStatusBar();
    this->initStatusBarValue();

    this->ui->actionNouveau_patient->setShortcut(QKeySequence("Ctrl+n"));
    this->ui->actionEnregistrer->setShortcut(QKeySequence("Ctrl+s"));
    this->ui->actionChanger_de_patient->setShortcut(QKeySequence("Ctrl+o"));
    this->ui->actionD_connection->setShortcut(QKeySequence("Ctrl+d"));
    this->ui->actionQuitter->setShortcut(QKeySequence("Ctrl+q"));

    if(this->loadRecords())
        this->badPass = false;
    else
        this->badPass = true;
}

bool MainWindow::isBadPass()
{
    return this->badPass;
}

void MainWindow::initStatusBar()
{
    this->ui->statusBar->setVisible(false);

    this->labelProcessNumber = new QLabel(this->ui->statusBar);
    this->labelSeparator = new QLabel("/", this->ui->statusBar);
    this->labelTotalNumber = new QLabel(this->ui->statusBar);
    this->progressBarProcessing = new QProgressBar(this->ui->statusBar);

    this->ui->statusBar->setMaximumHeight(STATUS_BAR_HEIGHT);

    this->ui->statusBar->addWidget(this->labelProcessNumber, 1);
    this->ui->statusBar->addWidget(this->labelSeparator, 1);
    this->ui->statusBar->addWidget(this->labelTotalNumber, 1);
    this->ui->statusBar->addWidget(this->progressBarProcessing, 17);
    // total stretch is 20 so status bar width is separated in 20, if stretch of widget is 1, it take 5% of total width.

    this->connect(this->threadUpload, SIGNAL(fileInserted(int)), this, SLOT(refreshNumberInsertFile(int)));
    this->connect(this->threadUpload, SIGNAL(fileProcess(int)), this, SLOT(refreshNumberProcessFile(int)));
    this->connect(this->threadUpload, SIGNAL(uploadProgression(int)), this->progressBarProcessing, SLOT(setValue(int)));
    this->connect(this->threadUpload, SIGNAL(setProgressText(QString)), this, SLOT(setProgressBarText(QString)));
}

void MainWindow::initStatusBarValue()
{
    this->progressBarProcessing->setRange(0, 100);
    this->progressBarProcessing->setValue(0);
    this->progressBarProcessing->setTextVisible(true);
}

void MainWindow::startDownloadClientData(int clientNumber)
{
    Q_UNUSED(clientNumber);
}

bool MainWindow::loadRecords()
{
    bool noError = true;

    noError = this->loadFEC();
    qDebug() << "load : " << noError;

    if(noError)
        noError = this->loadFCT();
qDebug() << "load : " << noError;
    if(noError)
        noError = this->loadFMT();
qDebug() << "load : " << noError;
    if(noError)
        noError = this->loadFMN();
qDebug() << "load : " << noError;
    return noError;
}

bool MainWindow::loadFEC()
{
    salamandre::RegistryRecord *record = this->patient->getRegistryRecord();

    if(QFile::exists(QString::fromStdString(record->getFilePath())))
    {
        if(!record->load(this->doctor->getPass().toStdString())){
            return false;
        }

        this->ui->lineEdit_patientLastName->setText(QString::fromStdString(record->getLastName()));
        this->ui->lineEdit_patientFirstName->setText(QString::fromStdString(record->getFirstName()));
        this->ui->lineEdit_patientAdress->setText(QString::fromStdString(record->getAdress()));
        this->ui->dateTimeEdit_patientBirthDate->setDateTime(QDateTime::fromString(QString::fromStdString(record->getBirthDate()), "yyyy-MM-ddThh:mm:ss"));
        if(QString::fromStdString(record->getSex()) == "M")
            this->ui->radioButton_patientSexMale->setChecked(true);
        else
            this->ui->radioButton_patientSexFemale->setChecked(true);
    }
    else{
        this->ui->lineEdit_patientLastName->setText("");
        this->ui->lineEdit_patientFirstName->setText("");
        this->ui->lineEdit_patientAdress->setText("");
        this->ui->dateTimeEdit_patientBirthDate->setDateTime(QDateTime(QDate(1970,1,1), QTime(0,0,0)));
        this->ui->radioButton_patientSexMale->setChecked(true);

        record->setVersionNumber(0); // will be automatically increment at save.
    }

    return true;
}

bool MainWindow::loadFCT()
{
    salamandre::ConfidentialRecord *record = this->patient->getConfidentialRecord();

    if(QFile::exists(QString::fromStdString(record->getFilePath())))
    {
        if(!record->load(this->doctor->getPass().toStdString())){
            return false;
        }

        this->ui->plainTextEdit_confidentialTextPatient->setPlainText(QString::fromStdString(record->getContent()));
    }
    else{
        this->ui->plainTextEdit_confidentialTextPatient->setPlainText("");

        record->setVersionNumber(0); // will be automatically increment at save.
    }

    this->saveFCTNeeded = false;
    return true;
}

bool MainWindow::loadFMT()
{
    salamandre::MedicalRecord *record = this->patient->getMedicalRecord();

    if(QFile::exists(QString::fromStdString(record->getFilePath())))
    {
        if(!record->load(this->doctor->getPass().toStdString())){
            return false;
        }

        this->ui->plainTextEdit_medicalTextPatient->setPlainText(QString::fromStdString(record->getContent()));
    }
    else{
        this->ui->plainTextEdit_medicalTextPatient->setPlainText("");

        record->setVersionNumber(0); // will be automatically increment at save.
    }

    this->saveFMTNeeded = false;
    return true;
}

bool MainWindow::loadFMN()
{
    salamandre::DigitalRecord *record = this->patient->getDigitalRecord();

    if(QFile::exists(QString::fromStdString(record->getFilePath()))){
        QFile f(QString::fromStdString(record->getFilePath()));
        f.copy(this->patient->getDirPath()+"/tmp/"+QString::fromStdString(record->getFileName()));
        if(!record->load(this->doctor->getPass().toStdString())){
            qDebug() << "fail to load fmn";
            return false;
        }
        this->refreshDigitalFile();
    }
    else{
        record->setVersionNumber(0); // will be automatically increment at save.
    }

    return true;
}

void MainWindow::saveRecords()
{
    this->ui->actionEnregistrer->setEnabled(false);

    if(this->doctor->getType() == salamandre::Doctor::TypeDoctor::NEW_DOCTOR)
        this->doctor->setType(salamandre::Doctor::TypeDoctor::DOCTOR_ALREADY_EXIST);

    bool save = false;

    save |= this->saveFEC();
    save |= this->saveFCT();
    save |= this->saveFMT();
    save |= this->saveFMN();

    if(save){
        this->saveRecordDialog->show();
        this->saveRecordDialog->setModal(true);
        this->threadSaveRecord->start(QThread::HighPriority);
    }
    else{
        this->ui->actionEnregistrer->setEnabled(true);
    }
}

void MainWindow::saveProgress(int save)
{
    switch(save){
    case threadSave::SAVE_OF_FCT:
        this->saveFCTNeeded = false;
        break;
    case threadSave::SAVE_OF_FEC:
        break;
    case threadSave::SAVE_OF_FMN:
        this->listViewDigitalFiles->needToSave = false;
        break;
    case threadSave::SAVE_OF_FMT:
        this->saveFMTNeeded = false;
        break;
    }
}

void MainWindow::saveEnd()
{
    if(this->saveRecordDialog->isVisible()){
        this->saveRecordDialog->close();
        delete this->saveRecordDialog;
        this->saveRecordDialog = new saveDialog(this);
    }

    switch(this->currentAction){
    case ACTION_NEW_PATIENT:
    {
        patientNameDialog dialog;
        dialog.exec();
        QString id = dialog.getText();

        if(!id.isEmpty()){
            salamandre::Patient *newPatient = new salamandre::Patient(doctor->getDirPath()+"/"+id);
            newPatient->setId(id);
            QDir dirPatient = QDir(newPatient->getDirPath());
            if(!dirPatient.exists()){
                dirPatient.mkdir(dirPatient.path());

                this->clearPatient();
                this->patient =  newPatient;
                this->init();
            }
            else{
                delete newPatient;
                QMessageBox::warning(nullptr, APPS_NAME, "Ce patient existe déjà dans votre répertoire.");
            }
        }

        break;
    }
    case ACTION_CHANGE_PATIENT:
    {
        chooseDialog *chDialog = new chooseDialog(this->doctor, nullptr);
        int res = chDialog->exec();

        if(res  == QDialog::Accepted){
            this->clearPatient();
            this->patient = chDialog->getPatient();
            this->init();
        }

        delete chDialog;
        break;
    }
    case ACTION_QUIT:
        this->close();
        break;
    case ACTION_NO:
        break;
    }

    this->currentAction = ACTION_NO;
    this->ui->actionEnregistrer->setEnabled(true);
}

bool MainWindow::saveFEC()
{
    if(this->checkNeedSaveFEC()){
        salamandre::RegistryRecord *record = this->patient->getRegistryRecord();
        record->setAdress(this->ui->lineEdit_patientAdress->text().toStdString());
        record->setBirthDate(this->ui->dateTimeEdit_patientBirthDate->dateTime().toString("yyyy-MM-ddThh:mm:ss").toStdString());
        record->setFirstName(this->ui->lineEdit_patientFirstName->text().toStdString());
        record->setLastName(this->ui->lineEdit_patientLastName->text().toStdString());
        if(this->ui->radioButton_patientSexMale->isChecked())
            record->setSex("M");
        else
            record->setSex("F");

        this->threadSaveRecord->saveFEC = true;
    }
    else
        this->threadSaveRecord->saveFEC = false;

    return this->threadSaveRecord->saveFEC;
}

bool MainWindow::saveFCT()
{
    if(this->checkNeedSaveFCT()){
        salamandre::ConfidentialRecord *record = this->patient->getConfidentialRecord();
        record->setContent(this->ui->plainTextEdit_confidentialTextPatient->toPlainText().toStdString());

        this->threadSaveRecord->saveFCT = true;
    }
    else
        this->threadSaveRecord->saveFCT = false;

    return this->threadSaveRecord->saveFCT;
}

bool MainWindow::saveFMT()
{
    if(this->checkNeedSaveFMT()){
        salamandre::MedicalRecord *record = this->patient->getMedicalRecord();
        record->setContent(this->ui->plainTextEdit_medicalTextPatient->toPlainText().toStdString());

        this->threadSaveRecord->saveFMT = true;
    }
    else
        this->threadSaveRecord->saveFMT = false;

    return this->threadSaveRecord->saveFMT;
}

bool MainWindow::saveFMN()
{
    if(this->checkNeedSaveFMN()){
        this->threadSaveRecord->saveFMN = true;
    }
    else
        this->threadSaveRecord->saveFMN = false;

    return this->threadSaveRecord->saveFMN;
}

void MainWindow::refreshDigitalFile()
{
    salamandre::DigitalRecord *record = this->patient->getDigitalRecord();

    this->listViewDigitalFiles->modelListFile->clear();

    int nbFile = record->vFile.size();
    QStandardItem *item;
    for(int i = 0; i < nbFile; ++i){
        salamandre::DigitalContent *digit;
        digit = record->vFile.at(i);
        item = new QStandardItem(QString::fromStdString(digit->fileName));
        item->setData(QVariant::fromValue(digit));
        this->listViewDigitalFiles->modelListFile->setItem(i, 0, item);
        this->listViewDigitalFiles->modelListFile->setItem(i, 1, new QStandardItem());
    }
}

void MainWindow::checkNeedSave()
{
    bool needToSave = this->checkNeedSaveFCT() || this->checkNeedSaveFEC() || this->checkNeedSaveFMN() || this->checkNeedSaveFMT();

    if(needToSave){
        QMessageBox::StandardButton saveBox;
        saveBox = QMessageBox::question(this, "Sauvegarde requise", "Voulez-vous sauvegarder les modifications ?<br/>", QMessageBox::Yes|QMessageBox::No);
        QCoreApplication::processEvents();

        if (saveBox == QMessageBox::Yes){
            this->saveRecords();
        }
        else{
            // restore FMN.
            salamandre::DigitalRecord *record = this->patient->getDigitalRecord();

            QString stringTmpFileFMN = this->patient->getDirPath()+"/tmp/"+QString::fromStdString(record->getFileName());
            QString stringFileFMN = QString::fromStdString(record->getFilePath());
            QString stringFileFMNTmp = QString::fromStdString(record->getFilePath())+"_tmp";

            QFile f(stringTmpFileFMN);
            QFile fileFMN(stringFileFMN);
            if(fileFMN.exists() && f.exists()){
                if(fileFMN.rename(stringFileFMNTmp)){
                    if(f.rename(stringFileFMN)){
                        fileFMN.remove();
                    }
                }
            }

            this->saveEnd();
        }
    }
    else
        this->saveEnd();

    QDir dirTmp(this->patient->getDirPath()+"/tmp");
    if(dirTmp.exists())
        dirTmp.removeRecursively();

    dirTmp.mkdir(dirTmp.path());
}

bool MainWindow::checkNeedSaveFEC()
{
    salamandre::RegistryRecord *record = this->patient->getRegistryRecord();

    if(record->getAdress() != this->ui->lineEdit_patientAdress->text().toStdString())
        return true;
    if(record->getBirthDate() != this->ui->dateTimeEdit_patientBirthDate->dateTime().toString("yyyy-MM-ddThh:mm:ss").toStdString())
        return true;
    if(record->getFirstName() != this->ui->lineEdit_patientFirstName->text().toStdString())
        return true;
    if(record->getLastName() != this->ui->lineEdit_patientLastName->text().toStdString())
        return true;
    if(record->getSex() == "M" && !this->ui->radioButton_patientSexMale->isChecked())
        return true;
    if(record->getSex() == "F" && !this->ui->radioButton_patientSexFemale->isChecked())
        return true;

    return false;
}

bool MainWindow::checkNeedSaveFCT()
{
    return this->saveFCTNeeded;
}

bool MainWindow::checkNeedSaveFMT()
{
    return this->saveFMTNeeded;
}

bool MainWindow::checkNeedSaveFMN()
{
    return this->listViewDigitalFiles->needToSave;
}

void MainWindow::on_actionNouveau_patient_triggered()
{
    this->currentAction = ACTION_NEW_PATIENT;
    this->checkNeedSave();
}

void MainWindow::on_actionChanger_de_patient_triggered()
{
    this->currentAction = ACTION_CHANGE_PATIENT;
    this->checkNeedSave();
}

void MainWindow::on_actionEnregistrer_triggered()
{
    this->saveRecords();
}

void MainWindow::on_actionQuitter_triggered()
{
    this->close();
}

void MainWindow::on_actionD_connection_triggered()
{
    this->restartApplication = true;
    this->on_actionQuitter_triggered();
}

bool MainWindow::restartApps()
{
    return this->restartApplication;
}

void MainWindow::startUploadDigitalFile(QStringList listFile)
{
    u_int32_t nbFileToAdd = listFile.size();
    QString tmpFileName;

    if(nbFileToAdd > 0){
        if(!this->ui->statusBar->isVisible())
            this->ui->statusBar->setVisible(true);
        QVector<salamandre::DigitalContent*> vFile;

        for(u_int32_t i = 0; i < nbFileToAdd; ++i){
            salamandre::DigitalContent *digitalContent = new salamandre::DigitalContent();

            tmpFileName = QString(this->patient->getDirPath()+QString("/tmp/")+QFileInfo(listFile.at(i)).fileName());
            digitalContent->fileName = QFileInfo(listFile.at(i)).fileName().toStdString();
            digitalContent->filePath = tmpFileName.toStdString();
            digitalContent->sourcePath = listFile.at(i).toStdString();

            QFile tmpFile(tmpFileName);
            if(tmpFile.exists()){
                tmpFile.remove();
            }

            vFile.push_back(digitalContent);
        }

        this->threadUpload->addFileToUpload(vFile);
    }
}

void MainWindow::clearPatient()
{
    delete this->patient;
    delete this->threadUpload;
    delete this->threadSaveRecord;
    delete this->saveRecordDialog;
    delete this->listViewDigitalFiles;

    this->ui->statusBar->removeWidget(this->labelProcessNumber);
    this->ui->statusBar->removeWidget(this->labelSeparator);
    this->ui->statusBar->removeWidget(this->labelTotalNumber);
    this->ui->statusBar->removeWidget(this->progressBarProcessing);

    delete this->labelProcessNumber;
    delete this->labelSeparator;
    delete this->labelTotalNumber;
    delete this->progressBarProcessing;
}

void MainWindow::on_toolButton_numericalExporter_clicked()
{
    this->listViewDigitalFiles->startUpload();
}

void MainWindow::on_toolButton_numericalImporter_clicked()
{
    QStringList fileList = QFileDialog::getOpenFileNames(nullptr, tr("Choix des fichiers"), QDir::homePath());

    int nbFile = fileList.size();

    if(nbFile > 0){
        this->listViewDigitalFiles->needToSave = true;
        this->startUploadDigitalFile(fileList);
    }
}

void MainWindow::on_plainTextEdit_confidentialTextPatient_textChanged()
{
    this->saveFCTNeeded = true;
}

void MainWindow::on_plainTextEdit_medicalTextPatient_textChanged()
{
    this->saveFMTNeeded= true;
}

void MainWindow::refreshNumberInsertFile(int number)
{
    this->labelTotalNumber->setText(QString::number(number));
}

void MainWindow::refreshNumberProcessFile(int number)
{
    this->labelProcessNumber->setText(QString::number(number));
}

void MainWindow::setProgressBarText(QString text)
{
    this->progressBarProcessing->setFormat(text);
}

void MainWindow::restartNeeded()
{
    this->badPass = true;
    this->on_actionD_connection_triggered();
}

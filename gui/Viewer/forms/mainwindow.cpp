#include <forms/mainwindow.hpp>
#include <ui_mainwindow.h>
#include <forms/choosedialog.hpp>

#include <QDir>
#include <QMessageBox>
#include <QInputDialog>
#include <QDebug>
#include <QDate>
#include <QFileDialog>
#include <QProcess>

#include <Salamandre-daemon/GuiFunctions.hpp>

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

void MainWindow::closeEvent(QCloseEvent *)
{
    this->checkNeedSave();
}

void MainWindow::init()
{
    this->listViewDigitalFiles = new ListView(this->patient->getDirPath(), nullptr);
    this->threadUpload = new threadUploadFile(this->patient, this->doctor, nullptr);

    this->connect(this->listViewDigitalFiles, SIGNAL(dropFile(QStringList)), SLOT(startUploadDigitalFile(QStringList)));
    this->connect(this->threadUpload, SIGNAL(newFileInserted()), this, SLOT(refreshDigitalFile()));

    this->setWindowTitle(QString("Salamandre") + " - Patient n°"+this->patient->getId());

    this->ui->verticalLayout_listView->addWidget(this->listViewDigitalFiles);

    this->ui->lineEdit_patientIdNumber->setText(this->patient->getId());
    this->ui->lineEdit_confidentialPatientNumber->setText(this->patient->getId());
    this->ui->lineEdit_medicalPatientNumber->setText(this->patient->getId());
    this->ui->lineEdit_numericalPatientNumber->setText(this->patient->getId());

    this->loadRecords();
    this->doctor->setType(salamandre::Doctor::TypeDoctor::DOCTOR_ALREADY_EXIST);

    this->initStatusBar();
    this->initStatusBarValue();

    this->ui->actionNouveau_patient->setShortcut(QKeySequence("Ctrl+n"));
    this->ui->actionEnregistrer->setShortcut(QKeySequence("Ctrl+s"));
    this->ui->actionChanger_de_patient->setShortcut(QKeySequence("Ctrl+o"));
    this->ui->actionQuitter->setShortcut(QKeySequence("Ctrl+q"));
}

void MainWindow::initStatusBar()
{
    this->labelProcessNumber = new QLabel(this->ui->statusBar);
    this->labelSeparator = new QLabel("/", this->ui->statusBar);
    this->labelTotalNumber = new QLabel(this->ui->statusBar);
    this->progressBarProcessing = new QProgressBar(this->ui->statusBar);

    this->ui->statusBar->setMaximumHeight(STATUS_BAR_HEIGHT);

    this->ui->statusBar->addWidget(this->labelProcessNumber, 1);
    this->ui->statusBar->addWidget(this->labelSeparator, 1);
    this->ui->statusBar->addWidget(this->labelTotalNumber, 1);
    this->ui->statusBar->addWidget(this->progressBarProcessing, 3);
    // total stretch is 6 so status bar width is separated in 6, if stretch of widget is 1, it take 16,67% of total width.

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
    //this->progressBarProcessing->setFormat("");
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
        this->ui->plainTextEdit_confidentialTextPatient->setPlainText("");

        record->setVersionNumber(0); // will be automatically increment at save.
    }

    this->saveFCTNeeded = false;
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
        this->ui->plainTextEdit_medicalTextPatient->setPlainText("");

        record->setVersionNumber(0); // will be automatically increment at save.
    }

    this->saveFMTNeeded = false;
}

void MainWindow::loadFMN()
{
    salamandre::DigitalRecord *record = this->patient->getDigitalRecord();

    if(QFile::exists(QString::fromStdString(record->getFilePath()))){
        QFile f(QString::fromStdString(record->getFilePath()));
        f.copy(this->patient->getDirPath()+"/tmp/"+QString::fromStdString(record->getFileName()));
        record->load(this->doctor->getPass().toStdString());
        this->refreshDigitalFile();
    }
    else{
        record->setVersionNumber(0); // will be automatically increment at save.
    }
}

void MainWindow::saveRecords()
{
    if(this->doctor->getType() == salamandre::Doctor::TypeDoctor::NEW_DOCTOR)
        this->doctor->setType(salamandre::Doctor::TypeDoctor::DOCTOR_ALREADY_EXIST);

    this->saveFEC();
    this->saveFCT();
    this->saveFMT();
    this->saveFMN();
}

void MainWindow::saveFEC()
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

        record->save(this->doctor->getPass().toStdString());
    }
}

void MainWindow::saveFCT()
{
    if(this->checkNeedSaveFCT()){
        salamandre::ConfidentialRecord *record = this->patient->getConfidentialRecord();
        record->setContent(this->ui->plainTextEdit_confidentialTextPatient->toPlainText().toStdString());
        record->save(this->doctor->getPass().toStdString());
        this->saveFCTNeeded = false;
    }
}

void MainWindow::saveFMT()
{
    if(this->checkNeedSaveFMT()){
        salamandre::MedicalRecord *record = this->patient->getMedicalRecord();
        record->setContent(this->ui->plainTextEdit_medicalTextPatient->toPlainText().toStdString());
        record->save(this->doctor->getPass().toStdString());
        this->saveFMTNeeded = false;
    }
}

void MainWindow::saveFMN()
{
    if(this->checkNeedSaveFMN()){
        salamandre::DigitalRecord *record = this->patient->getDigitalRecord();
        record->save();
        this->listViewDigitalFiles->needToSave = false;
    }

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
                    if(f.copy(stringFileFMN)){
                        fileFMN.remove();
                    }
                }
            }
            else{
                fileFMN.remove();
            }
        }
    }

    QDir dirTmp(this->patient->getDirPath()+"/tmp");
    if(dirTmp.exists())
        dirTmp.removeRecursively();
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
    this->checkNeedSave();

    QString id = QInputDialog::getText(this, QString("Salamandre"), QString("Entrer le numéro du nouveau patient"), QLineEdit::Normal, QString(), nullptr, Qt::Dialog, Qt::ImhDigitsOnly);

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
            QMessageBox::warning(nullptr, "Salamandre", "Ce patient existe déjà dans votre répertoire.");
        }
    }
}

void MainWindow::on_actionChanger_de_patient_triggered()
{
    this->checkNeedSave();

    chooseDialog *chDialog = new chooseDialog(this->doctor, nullptr);
    int res = chDialog->exec();

    if(res  == QDialog::Accepted){
        this->clearPatient();
        this->patient = chDialog->getPatient();
        this->init();
    }

    delete chDialog;
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
    this->on_actionQuitter_triggered();
    QProcess::startDetached(QApplication::applicationFilePath());
}

void MainWindow::startUploadDigitalFile(QStringList listFile)
{
    u_int32_t nbFileToAdd = listFile.size();
    QVector<salamandre::DigitalContent*> vFile;

    for(u_int32_t i = 0; i < nbFileToAdd; ++i){
        salamandre::DigitalContent *digitalContent = new salamandre::DigitalContent();
        digitalContent->fileName = QFileInfo(listFile.at(i)).fileName().toStdString();
        digitalContent->filePath = QString(this->patient->getDirPath()+QString("/tmp/")+QFileInfo(listFile.at(i)).fileName()).toStdString();
        digitalContent->sourcePath = listFile.at(i).toStdString();

        vFile.push_back(digitalContent);
    }

    this->threadUpload->addFileToUpload(vFile);
}

void MainWindow::clearPatient()
{
    delete this->patient;
    delete this->threadUpload;
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
    this->saveFMTNeeded = true;
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

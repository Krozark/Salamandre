#include <objects/threaduploadfile.hpp>
#include <record/DigitalRecord.hpp>

#include <QDebug>
#include <QDir>
#include <QFile>
#include <QFileInfo>
#include <stdio.h>

threadUploadFile::threadUploadFile(salamandre::Patient *patient, salamandre::Doctor *doctor, QObject *parent) :
    QThread(parent)
{
    this->patient = patient;
    this->doctor = doctor;

    this->destinationTmp = this->patient->getDirPath()+"/tmp";
    this->destinationFmn = this->patient->getDirPath()+"/FMN";

    this->totalInsertedFile = 0;
    this->totalProcessFile = 0;
    this->totalProgress = 0;

    QDir tmpDir = QDir(this->destinationTmp);
    if(!tmpDir.exists())
        tmpDir.mkdir(tmpDir.path());
}

void threadUploadFile::run()
{
    salamandre::DigitalRecord *record = this->patient->getDigitalRecord();
    u_int32_t nbDigitalFile;
    //bool exists;

    while(!this->uploadFileList.isEmpty()){
        ++this->totalProcessFile;
        nbDigitalFile = record->vFile.size();

        salamandre::DigitalContent *digit = this->uploadFileList.at(0);
        this->uploadFileList.pop_front();

        QFile f(QString::fromStdString(digit->sourcePath));
        QString fileName = QFileInfo(f.fileName()).fileName();
        /*exists = false;

        for(u_int32_t j = 0; j < nbDigitalFile; ++j){
            salamandre::DigitalContent *digitFile = record->vFile.at(j);
            if(fileName.toStdString() == digitFile->fileName){
                exists = true;
                break;
            }
        }

        if(!exists){*/
        f.copy(this->destinationTmp+"/"+fileName);

        digit->key = this->doctor->getPass().toStdString();

        emit setProgressText("Encrypting : " + fileName);
        salamandre::Record::encrypt(digit->key, digit->filePath);

        emit setProgressText("Writting : " + fileName);
        salamandre::DigitalRecord::insertDigitFile(this->destinationFmn.toStdString(), digit);


        record->vFile.push_back(digit);
        QFile::remove(this->destinationTmp+"/"+fileName);

        ++nbDigitalFile;

        emit newFileInserted();
        //}

        this->totalProgress = (this->totalProcessFile * 100) / this->totalInsertedFile;
        emit fileProcess(this->totalProcessFile);
        emit uploadProgression(this->totalProgress);
    }

    emit setProgressText("Import terminé !");
}

void threadUploadFile::addFileToUpload(QVector<salamandre::DigitalContent *> vFile)
{
    int nbFile = vFile.size();

    for(int i = 0; i < nbFile; ++i)
        this->addFileToUpload(vFile.at(i));
}

void threadUploadFile::addFileToUpload(salamandre::DigitalContent *file)
{
    ++this->totalInsertedFile;
    emit fileInserted(this->totalInsertedFile);
    this->uploadFileList << file;

    if(!this->isRunning()){
        this->totalInsertedFile -= this->totalProcessFile;
        this->totalProcessFile = 0;
        this->totalProgress = 0;

        emit fileInserted(this->totalInsertedFile);
        emit fileProcess(this->totalProcessFile);
        emit uploadProgression(this->totalProcessFile);

        this->start(QThread::HighPriority);
    }
}

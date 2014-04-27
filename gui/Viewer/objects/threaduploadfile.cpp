#include <objects/threaduploadfile.hpp>
#include <record/DigitalRecord.hpp>

#include <QDebug>
#include <QDir>
#include <QFile>
#include <QFileInfo>

threadUploadFile::threadUploadFile(salamandre::Patient *patient, salamandre::Doctor *doctor, QObject *parent) :
    QThread(parent)
{
    this->patient = patient;
    this->doctor = doctor;

    this->destinationTmp = this->patient->getDirPath()+"/tmp";
    this->destinationFmn = this->patient->getDirPath()+"/FMN";

    QDir tmpDir = QDir(this->destinationTmp);
    if(!tmpDir.exists())
        tmpDir.mkdir(tmpDir.path());
}

void threadUploadFile::run()
{
    salamandre::DigitalRecord *record = this->patient->getDigitalRecord();
    u_int32_t nbDigitalFile;
    bool exists;

    while(!this->uploadFileList.isEmpty()){
        nbDigitalFile = record->vFile.size();
        std::cout << "nb digital files : " << nbDigitalFile << std::endl;

        salamandre::DigitalContent *digit = this->uploadFileList.at(0);
        this->uploadFileList.pop_front();

        QFile f(QString::fromStdString(digit->sourcePath));
        QString fileName = QFileInfo(f.fileName()).fileName();
        exists = false;

        for(u_int32_t j = 0; j < nbDigitalFile; ++j){
            salamandre::DigitalContent *digitFile = record->vFile.at(j);
            if(fileName.toStdString() == digitFile->fileName){
                exists = true;
                break;
            }
        }

        if(!exists){
            f.copy(this->destinationTmp+"/"+fileName);
            salamandre::DigitalRecord::insertDigitFile(this->destinationFmn.toStdString(), this->doctor->getPass().toStdString(), digit);
            record->vFile.push_back(digit);
            ++nbDigitalFile;
            QFile::remove(this->destinationTmp+"/"+fileName);
            emit newFileInserted();
        }
    }
}

void threadUploadFile::addFileToUpload(QVector<salamandre::DigitalContent *> vFile)
{
    int nbFile = vFile.size();

    for(int i = 0; i < nbFile; ++i)
        this->addFileToUpload(vFile.at(i));
}

void threadUploadFile::addFileToUpload(salamandre::DigitalContent *file)
{
    this->uploadFileList << file;

    if(!this->isRunning()){
        this->start(QThread::HighPriority);
    }
}

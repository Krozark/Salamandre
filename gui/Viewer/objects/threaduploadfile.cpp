#include <objects/threaduploadfile.hpp>

#include <QDebug>

threadUploadFile::threadUploadFile(QString destination, QObject *parent) :
    QThread(parent)
{
    this->destination = destination;
}

void threadUploadFile::start(Priority)
{
    //this->run();
    QThread::run();
}

void threadUploadFile::run()
{
    while(this->uploadFileList.size() > 0){
        QString fileToUpload = this->uploadFileList.at(0);
        this->uploadFileList.pop_front();
        qDebug() << "running " << fileToUpload;
    }
}

void threadUploadFile::addFileToUpload(QStringList listFile)
{
    int nbFile = listFile.size();

    for(int i = 0; i < nbFile; ++i)
        this->addFileToUpload(listFile.at(i));
}

void threadUploadFile::addFileToUpload(QString file)
{
    this->uploadFileList << file;

    if(!this->isRunning()){
        this->start(QThread::NormalPriority);
    }
}

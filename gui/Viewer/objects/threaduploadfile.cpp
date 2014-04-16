#include <objects/threaduploadfile.hpp>

#include <QDebug>
#include <QDir>
#include <QFile>
#include <QFileInfo>

threadUploadFile::threadUploadFile(QString destination, QObject *parent) :
    QThread(parent)
{
    this->destination = destination;

    QDir tmpDir = QDir(this->destination+"/tmp");
    if(!tmpDir.exists())
        tmpDir.mkdir(tmpDir.path());
}

void threadUploadFile::start(Priority)
{
    this->run();
}

void threadUploadFile::run()
{
    while(this->uploadFileList.size() > 0){
        QString fileToUpload = this->uploadFileList.at(0);
        this->uploadFileList.pop_front();
        qDebug() << "running " << fileToUpload;
        QFile f(fileToUpload);
        qDebug() << "dest : " << this->destination+"/"+QFileInfo(f.fileName()).fileName();
        f.copy(this->destination+"/tmp/"+QFileInfo(f.fileName()).fileName());
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

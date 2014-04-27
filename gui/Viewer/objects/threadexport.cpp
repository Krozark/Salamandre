#include <objects/threadexport.hpp>

#include <stdio.h>

#include <QFileInfo>
#include <QDebug>

threadExport::threadExport(QString source, QObject *parent) :
    QThread(parent)
{
    this->source = source;
}

void threadExport::run()
{
    while(this->exportFileList.size() > 0){
        salamandre::DigitalContent *fileToExport = this->exportFileList.at(0);
        this->exportFileList.pop_front();

        salamandre::DigitalRecord::extractDigitFile(this->source.toStdString(), fileToExport);
        emit fileExtracted(QString::fromStdString(fileToExport->filePathExport));
    }
}

void threadExport::addFileToExport(QVector<salamandre::DigitalContent *> listFile)
{
    int nbFile = listFile.size();

    for(int i = 0; i < nbFile; ++i)
        this->addFileToExport(listFile.at(i));
}

void threadExport::addFileToExport(salamandre::DigitalContent *file)
{
    this->exportFileList << file;

    if(!this->isRunning()){
        this->start(QThread::HighPriority);
    }
}

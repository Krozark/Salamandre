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
    bool removeIfExist;
    while(this->exportFileList.size() > 0){
        removeIfExist = true;

        salamandre::DigitalContent *fileToExport = this->exportFileList.at(0);
        this->exportFileList.pop_front();

        // suppression des éventuels fichiers temporaires au cas ou la GUI
        // aurait été arrété de manière anormale lors de l'exécution précédente.
        QFile fTmp(this->source+"/tmp/"+QString::fromStdString(fileToExport->fileName)+".tmp");
        if(fTmp.exists()){
            removeIfExist = fTmp.remove();
        }

        QFile f(this->source+"/tmp/"+QString::fromStdString(fileToExport->fileName));
        if(f.exists()){
            removeIfExist &= f.remove();
        }

        if(removeIfExist){
            salamandre::DigitalRecord::extractDigitFile(this->source.toStdString(), fileToExport);
            salamandre::Record::decrypt(fileToExport->key, this->source.toStdString()+"/tmp/"+fileToExport->fileName+".tmp", fileToExport->filePathExport);
            remove((this->source.toStdString()+"/tmp/"+fileToExport->fileName+".tmp").c_str());

            emit fileExtracted(QString::fromStdString(fileToExport->filePathExport));
        }
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

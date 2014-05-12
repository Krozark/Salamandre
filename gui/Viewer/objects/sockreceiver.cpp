#include <objects/sockreceiver.hpp>
#include <objects/socksender.hpp>

#include <record/Record.hpp>
#include <record/ConfidentialRecord.hpp>
#include <record/DigitalRecord.hpp>
#include <record/MedicalRecord.hpp>
#include <record/RegistryRecord.hpp>

#include <QFileInfo>
#include <QDir>
#include <QCoreApplication>
#include <QStringList>

bool operator ==(const getFile &gfFirst, const getFile &gfSecond){
    return gfFirst.idDoctor == gfSecond.idDoctor && gfFirst.idPatient == gfSecond.idPatient && gfFirst.filename == gfSecond.filename;
}

sockReceiver sockReceiver::sock;

sockReceiver::sockReceiver()
{
    sock.srvPort = DEFAULT_NOTIF_SERVER_PORT;
    sock.srvIpAddress = DEFAULT_NOTIF_IP;
}

sockReceiver::~sockReceiver()
{
    delete sock.server;
}

void sockReceiver::init(){
    sock.server = new ntw::srv::Server(sock.srvPort, sock.srvIpAddress, notification_dispatch, 1, 1);
    sock.server->start();
}

bool sockReceiver::connectToDaemon(){
    return sockSender::setGuiServerPort(sock.srvPort);
}

void sockReceiver::closeConnectionToDaemon(){
    sock.server->stop();
    sock.server->wait();
}

void sockReceiver::askFile(getFile *file){

    sock.patientDataList.push_back(file);
    std::cout << "asking to get file " << file->filename << " for patient " << std::to_string(file->idPatient) << " of doctor " << std::to_string(file->idDoctor) << std::endl;

    sockSender::getFile(file->idDoctor, file->idPatient, file->filename);

    /* TODO remove next line then daemon will send notification, now its just to test ... */
    emit sock.fileIsRecv(file);

    QStringList fileFilter = QStringList() << QString::fromStdString(salamandre::ConfidentialRecord::getFileName())
                                           << QString::fromStdString(salamandre::MedicalRecord::getFileName())
                                           << QString::fromStdString(salamandre::DigitalRecord::getFileName())
                                           << QString::fromStdString(salamandre::RegistryRecord::getFileName());

    std::string backupPath = sockSender::getDaemonBackupPath()+"/"+std::to_string(file->idDoctor);

    if(file->idPatient == -1){
        QDir dirBackup(QString::fromStdString(backupPath));
        QFileInfoList listInfo = dirBackup.entryInfoList(QDir::AllDirs | QDir::NoDotAndDotDot, QDir::Time);
        int nbSubDir = listInfo.size();
        QFileInfo dInfo;

        for(int i = 0; i < nbSubDir; ++i){
            dInfo = listInfo.at(i);
            QDir dirApps = QCoreApplication::applicationDirPath()+"/save/"+QString::number(file->idDoctor)+"/"+dInfo.fileName();
            dirApps.mkpath(dirApps.path());

            QString pathFiles = QString::fromStdString(backupPath)+"/"+dInfo.fileName();
            QDir dirBackupFiles(pathFiles);

            QFileInfoList listFileInfo = dirBackupFiles.entryInfoList(fileFilter, QDir::Files | QDir::NoDotAndDotDot);
            int nbSubFile = listFileInfo.size();
            QFileInfo fInfo;

            for(int j = 0; j < nbSubFile; ++j){
                fInfo = listFileInfo.at(j);

                QFile f(pathFiles+"/"+fInfo.fileName());
                f.rename(dirApps.path()+"/"+fInfo.fileName());
            }
        }
    }
    else if(file->filename == ""){
        QDir dirApps = QCoreApplication::applicationDirPath()+"/save/"+QString::number(file->idDoctor)+"/"+QString::number(file->idPatient);
        dirApps.mkpath(dirApps.path());

        QString pathFiles = QString::fromStdString(backupPath)+"/"+QString::number(file->idPatient);
        QDir dirBackupFiles(pathFiles);
        QFileInfoList listFileInfo = dirBackupFiles.entryInfoList(fileFilter, QDir::Files | QDir::NoDotAndDotDot);
        int nbSubFile = listFileInfo.size();
        QFileInfo fInfo;

        for(int i = 0; i < nbSubFile; ++i){
            fInfo = listFileInfo.at(i);

            QFile f(pathFiles+"/"+fInfo.fileName());
            f.rename(dirApps.path()+"/"+fInfo.fileName());
        }
    }
    else{
        QDir dirApps = QCoreApplication::applicationDirPath()+"/save/"+QString::number(file->idDoctor)+"/"+QString::number(file->idPatient);
        QString pathFiles = QString::fromStdString(backupPath)+"/"+QString::number(file->idPatient)+"/"+QString::fromStdString(file->filename);
        QFile f(pathFiles);
        f.rename(dirApps.path()+"/"+QString::fromStdString(file->filename));
    }
    /* TODO end remove */
}

void sockReceiver::funcFileIsSend(ntw::SocketSerialized& socket,int idDoctor, int idPatient, std::string filename)
{
    (void) socket;
    std::cout<<"File send:"<<idDoctor<<"/"<<idPatient<<"/"<<filename<<std::endl;
}

void sockReceiver::funcFileIsRecv(ntw::SocketSerialized& socket,int idDoctor, int idPatient, std::string filename)
{
    (void) socket;
    std::cout<<"File recv:"<<idDoctor<<"/"<<idPatient<<"/"<<filename<<std::endl;

    getFile *fileRecv = new getFile(idDoctor, idPatient, filename);
    getFile *file = nullptr;

    int nbFileToGet = sock.patientDataList.size();
    for(int i = 0; i < nbFileToGet; ++i){
        file = sock.patientDataList.at(i);

        if(*file == *fileRecv){
            sock.patientDataList.remove(i);
            break;
        }
    }

    QStringList fileFilter = QStringList() << QString::fromStdString(salamandre::ConfidentialRecord::getFileName())
                                           << QString::fromStdString(salamandre::MedicalRecord::getFileName())
                                           << QString::fromStdString(salamandre::DigitalRecord::getFileName())
                                           << QString::fromStdString(salamandre::RegistryRecord::getFileName());

    std::string backupPath = sockSender::getDaemonBackupPath()+"/"+std::to_string(idDoctor);

    if(idPatient == -1){
        QDir dirBackup(QString::fromStdString(backupPath));
        QFileInfoList listInfo = dirBackup.entryInfoList(QDir::AllDirs | QDir::NoDotAndDotDot, QDir::Time);
        int nbSubDir = listInfo.size();
        QFileInfo dInfo;

        for(int i = 0; i < nbSubDir; ++i){
            dInfo = listInfo.at(i);
            QDir dirApps = QCoreApplication::applicationDirPath()+"/save/"+QString::number(idDoctor)+"/"+dInfo.fileName();
            dirApps.mkpath(dirApps.path());

            QString pathFiles = QString::fromStdString(backupPath)+"/"+dInfo.fileName();
            QDir dirBackupFiles(pathFiles);

            QFileInfoList listFileInfo = dirBackupFiles.entryInfoList(fileFilter, QDir::Files | QDir::NoDotAndDotDot);
            int nbSubFile = listFileInfo.size();
            QFileInfo fInfo;

            for(int j = 0; j < nbSubFile; ++j){
                fInfo = listFileInfo.at(j);

                QFile f(pathFiles+"/"+fInfo.fileName());
                f.rename(dirApps.path()+"/"+fInfo.fileName());
            }
        }
    }
    else if(filename == ""){
        QDir dirApps = QCoreApplication::applicationDirPath()+"/save/"+QString::number(idDoctor)+"/"+QString::number(idPatient);
        dirApps.mkpath(dirApps.path());

        QString pathFiles = QString::fromStdString(backupPath)+"/"+QString::number(idPatient);
        QDir dirBackupFiles(pathFiles);
        QFileInfoList listFileInfo = dirBackupFiles.entryInfoList(fileFilter, QDir::Files | QDir::NoDotAndDotDot);
        int nbSubFile = listFileInfo.size();
        QFileInfo fInfo;

        for(int i = 0; i < nbSubFile; ++i){
            fInfo = listFileInfo.at(i);

            QFile f(pathFiles+"/"+fInfo.fileName());
            f.rename(dirApps.path()+"/"+fInfo.fileName());
        }
    }
    else{
        QDir dirApps = QCoreApplication::applicationDirPath()+"/save/"+QString::number(idDoctor)+"/"+QString::number(idPatient);
        QString pathFiles = QString::fromStdString(backupPath)+"/"+QString::number(idPatient)+"/"+QString::fromStdString(filename);
        QFile f(pathFiles);
        f.rename(dirApps.path()+"/"+QString::fromStdString(filename));
    }

    delete fileRecv;
    emit sock.fileIsRecv(file);
}

int sockReceiver::notification_dispatch(int id,ntw::SocketSerialized& request)
{
        int res= ntw::Status::wrong_id;
        std::cout<<"[notification_dispatch] id:"<<id<<std::endl<<std::flush;
        switch(id)
        {
            case salamandre::gui::func::fileIsSend:{
                res = ntw::FuncWrapper::srv::exec(funcFileIsSend,request);
            }
            break;
            case salamandre::gui::func::fileIsRecv:{
                res = ntw::FuncWrapper::srv::exec(funcFileIsRecv,request);
            }
            break;
            default:{
                std::cout<<"[notification_dispatch] Function id not found"<<std::endl;
            }
            break;
        }
        return res;
}

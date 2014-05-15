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

sockReceiver sockReceiver::sock;

sockReceiver::sockReceiver()
{
    sock.srvIpAddress = DEFAULT_NOTIF_IP;
    sock.srvPort = DEFAULT_NOTIF_SERVER_PORT;
}

sockReceiver::~sockReceiver()
{
    delete sock.server;
}

void sockReceiver::init(){
    sock.server = new ntw::srv::Server(sock.srvPort, sock.srvIpAddress, notification_dispatch, 1, 1);

    std::cout << "sockReceiver init on IP : " << sock.srvIpAddress << ":" << std::to_string(sock.srvPort) << std::endl;
    sock.server->start();
}

void sockReceiver::setParamsCo(int srvPort, std::string ipAdress)
{
    sock.srvIpAddress = ipAdress;
    sock.srvPort = srvPort;
}

bool sockReceiver::connectToDaemon(){
    std::cout << "inform server port to daemon " << std::to_string(sock.srvPort) << std::endl;
    return sockSender::setGuiServerPort();
}

void sockReceiver::closeConnectionToDaemon(){
    sockSender::setGuiServerPort();
    sock.server->stop();
    sock.server->wait();
}

void sockReceiver::askFile(getFile *file){

    sock.patientDataList.push_back(file);
    std::cout << "asking to get file " << file->filename << " for patient " << std::to_string(file->idPatient) << " of doctor " << std::to_string(file->idDoctor) << std::endl;

    sockSender::getFile(file->idDoctor, file->idPatient, file->filename);
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

    QStringList fileFilter = QStringList() << QString::fromStdString(salamandre::ConfidentialRecord::getFileName())
                                           << QString::fromStdString(salamandre::MedicalRecord::getFileName())
                                           << QString::fromStdString(salamandre::DigitalRecord::getFileName())
                                           << QString::fromStdString(salamandre::RegistryRecord::getFileName());

    if(fileFilter.contains(QString::fromStdString(filename)) && !idDoctor < 0){
        getFile *fileRecv = new getFile(idDoctor, idPatient, filename);
        emit sock.fileIsRecv(fileRecv);
    }
}

void sockReceiver::funcEndOfSync(ntw::SocketSerialized& socket,int idDoctor, int idPatient, std::string filename)
{
    (void) socket;
    if(idDoctor < 0)
        return;

    getFile *fileRecv = new getFile(idDoctor, idPatient, filename);
    getFile *file = nullptr;

    int nbFileToGet = sock.patientDataList.size();
    for(int i = 0; i < nbFileToGet; ++i){
        file = sock.patientDataList.at(i);

        if(file->equals(*fileRecv)){
            sock.patientDataList.remove(i);
            break;
        }
    }

    QStringList fileFilter = QStringList() << QString::fromStdString(salamandre::ConfidentialRecord::getFileName())
                                           << QString::fromStdString(salamandre::MedicalRecord::getFileName())
                                           << QString::fromStdString(salamandre::DigitalRecord::getFileName())
                                           << QString::fromStdString(salamandre::RegistryRecord::getFileName());

    std::string backupPath = sockSender::getBackupPath()+"/"+std::to_string(idDoctor);

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
    emit sock.syncIsFinish(file);
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
        case salamandre::gui::func::endOfSync:{
            res = ntw::FuncWrapper::srv::exec(funcEndOfSync, request);
        }
        break;
        default:{
            std::cout<<"[notification_dispatch] Function id not found"<<std::endl;
        }
        break;
        }
        return res;
}

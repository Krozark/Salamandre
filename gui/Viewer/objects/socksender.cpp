#include <forms/daemonconnectiondialog.hpp>

#include <objects/socksender.hpp>
#include <objects/settings.hpp>
#include <record/Record.hpp>

#include <Salamandre-daemon/GuiFunctions.hpp>

#include <QCoreApplication>
#include <QFileInfo>
#include <QDir>
#include <QProcess>
#include <QDebug>

#include <sys/file.h>
#include <sys/stat.h>

sockSender sockSender::sock;
ntw::cli::Client sockSender::client;

sockSender::sockSender()
{
    sock.srvPort = DEFAULT_SERVEUR_PORT;
    sock.srvIpAddress = DEFAULT_IP;
}

void sockSender::init()
{
    sock.daemonBinPath = settings::getDaemonSettingValue("pathExe").toString().toStdString();
    sock.daemonBackupPath = settings::getDaemonSettingValue("pathBackup").toString().toStdString();
    sock.daemonSavePath = settings::getDaemonSettingValue("pathSave").toString().toStdString();
}

bool sockSender::connectToDaemon()
{
    bool res = true;

    if(client.connect(sock.srvIpAddress, sock.srvPort) != ntw::Status::connexion){
        qDebug() << "Gui is now connect to daemon and ready to send request";
        sockSender::initConnectionToDaemon();
    }
    else{
        if(sockSender::restartDaemon()){
            daemonConnectionDialog *dialogConnectionToDaemon = new daemonConnectionDialog(nullptr);
            bool resConnectRes = false;

            for(int i = 0; i < CONNECTION_TEST_NUMBER; ++i){ // loop for CONNECTION_TEST_NUMBER seconds
                dialogConnectionToDaemon->increaseNbTest(i+1);
                if(client.connect(sock.srvIpAddress, sock.srvPort) != ntw::Status::connexion){
                    resConnectRes = true;
                    break;
                }

                dialogConnectionToDaemon->show();
                QCoreApplication::processEvents(QEventLoop::WaitForMoreEvents, 1000);

                sleep(1);
            }

            if(dialogConnectionToDaemon->isVisible())
                dialogConnectionToDaemon->close();

            delete dialogConnectionToDaemon;
            QCoreApplication::processEvents(QEventLoop::WaitForMoreEvents, 1000);

            if(resConnectRes)
                sockSender::initConnectionToDaemon();
            else
                return false;
        }
        else{
            res = false;
            std::cerr << "Gui connection to daemon has failed" << std::endl;
        }
    }

    return res;
}

void sockSender::initConnectionToDaemon()
{
    sock.daemonSavePath = sockSender::getDaemonSavePath();
    sock.daemonBackupPath = sockSender::getDaemonBackupPath();
    sock.daemonBinPath = sockSender::getDaemonBinPath();

    settings::setDaemonSettingValue("pathExe", QString::fromStdString(sock.daemonBinPath));
    settings::setDaemonSettingValue("pathSave", QString::fromStdString(sock.daemonSavePath));
    settings::setDaemonSettingValue("pathBackup", QString::fromStdString(sock.daemonBackupPath));

    sock.guiPath = (QCoreApplication::applicationDirPath()+"/save/").toStdString();
}

void sockSender::closeConnectionToDaemon()
{
    client.disconnect();
}

std::string sockSender::getDaemonBinPath()
{
    return client.call<std::string>(salamandre::gui::func::getMyBinPath);
}

std::string sockSender::getDaemonSavePath()
{
    return client.call<std::string>(salamandre::gui::func::getMySavePath);
}

std::string sockSender::getDaemonBackupPath()
{
    return client.call<std::string>(salamandre::gui::func::getMyBackupPath);
}

void sockSender::sendFile(int idDoctor, int idPatient, std::string filename)
{
    QString path = QString::number(idDoctor)+"/"+QString::number(idPatient)+"/"+QString::fromStdString(filename);
    QStringList pathList = QStringList() << QString::fromStdString(sock.daemonSavePath)  << QString::number(idDoctor);

    qDebug() << "create dir : "<< pathList.join('/');
    QDir dirDoctor(pathList.join('/'));
    dirDoctor.mkdir(dirDoctor.path());

    pathList << QString::number(idPatient);

    QDir dirPatient(pathList.join('/'));
    dirPatient.mkdir(dirPatient.path());

    QFileInfo pathInfo(QString::fromStdString(sock.guiPath)+path);
    QStringList nameFilter = QStringList() << "FMN" << "FMT" << "FEC" << "FCT";
    QString dest;

    if(pathInfo.isDir()){ // filename is ""
        QDir dir(path);
        QFileInfoList listFile = dir.entryInfoList(nameFilter, QDir::Files);
        QFileInfo fInfo;
        int nbFile = listFile.size();

        for(int i = 0; i < nbFile; ++i){
            fInfo = listFile.at(i);
            pathList << fInfo.fileName();
            dest = pathList.join('/');

            FILE *fSrc = fopen((sock.guiPath+path.toStdString()+fInfo.fileName().toStdString()).c_str(), "rb");
            FILE *fDest = fopen(dest.toStdString().c_str(), "wb+");

            salamandre::Record::copyFile(fSrc, fDest);

            fclose(fDest);
            fclose(fSrc);

            pathList.removeLast();

            sock.informNewFile(idDoctor, idPatient, fInfo.fileName().toStdString());
        }
    }
    else if(pathInfo.isFile()){
        if(nameFilter.contains(QString::fromStdString(filename))){
            pathList << QString::fromStdString(filename);
            dest = pathList.join('/');


            FILE *fSrc = fopen((sock.guiPath+path.toStdString()).c_str(), "rb");
            FILE *fDest = fopen(dest.toStdString().c_str(), "wb+");

            salamandre::Record::copyFile(fSrc, fDest);

            fclose(fDest);
            fclose(fSrc);

            qDebug() << "file add : " << QString::fromStdString(filename);
            sock.informNewFile(idDoctor, idPatient, filename);
        }
        else{
            qDebug() << "filename isn't valid";
        }
    }
}

bool sockSender::informNewFile(int idDoctor, int idPatient, std::string filename)
{
    client.call<void>(salamandre::gui::func::newFile, idDoctor, idPatient, filename);

    if(checkStatus() < 0){
        std::cout << "ERROR on send new file info(" << filename << ")" << std::endl;
        return false;
    }

    client.request_sock.clear();
    return true;
}

bool sockSender::getFile(int idDoctor, int idPatient, std::string filename)
{
    client.call<void>(salamandre::gui::func::sync, idDoctor, idPatient, filename);

    if(checkStatus() < 0){
        std::cout << "ERROR on ask for an sync on all file of the patient (" << idPatient << ")" << std::endl;
        return false;
    }

    client.request_sock.clear();
    return true;
}

int sockSender::checkStatus()
{
    int status = client.request_sock.getStatus();
    switch(status)
    {
    case ntw::Status::stop :{
            std::cerr<<"[ERROR] The server is probably down."<<std::endl;
            std::cout<<"[Recv] Trying to restart the server"<<std::endl;
            sock.closeConnectionToDaemon();
            sockSender::connectToDaemon();
        }
        break;
    case ntw::Status::ok :{
            return 0;
        }
        default :
        {
            std::cout<<"[ERROR] Recv server code <"<<status<<"> whene sending file : "<<salamandre::gui::statusToString(status)<<std::endl;
            /// server error???
            return 1;
        }
    }

    return status;
}

bool sockSender::restartDaemon()
{
    bool res = QProcess::startDetached(QString::fromStdString(sock.daemonBinPath), QStringList() << "20001" << "20000" << "1", QString::fromStdString(sock.daemonBinPath));
    return res;
}

bool sockSender::setGuiServerPort(int srvPort)
{
    client.call<void>(salamandre::gui::func::setGuiNotificationPort,srvPort);
    if(checkStatus() < 0)
    {
        std::cerr << "Error on init notification server" << std::endl;
        return false;
    }

    return true;
}

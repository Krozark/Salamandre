#include <objects/threadgestionapps.hpp>
#include <objects/sockreceiver.hpp>
#include <objects/socksender.hpp>
#include <objects/settings.hpp>

#include <QMessageBox>
#include <QFileInfo>
#include <QCoreApplication>
#include <QDir>
#include <QFileDialog>
#include <QDebug>

threadGestionApps::threadGestionApps(QObject *parent) :
    QThread(parent)
{
    this->currentCmd = cmdApps::START_APPS;
}

void threadGestionApps::run()
{
    switch(this->currentCmd){
    case cmdApps::START_APPS:
        qDebug() << "RUNNING THREAD 1";
        if(this->_startApps() == 0)
            emit connectionSuccess();
        else
            emit connectionFailed();

        break;
    case cmdApps::STOP_APPS:{
        this->_stopApps();
        emit deconnectionSuccess();
        break;
    }
    }

    qDebug() << "RUNNING THREAD 2";
}

void threadGestionApps::_stopApps()
{
    sockReceiver::closeConnectionToDaemon();
    sockSender::closeConnectionToDaemon();
    settings::saveSettings();

    ntw::Socket::close();
}

int threadGestionApps::_startApps()
{
    settings::loadSettings();
    ntw::Socket::init();
    sockSender::init();
    sockSender::setParamsCo(this->srvGuiPort);

    int daemonConnectionRes = sockSender::connectToDaemon();

    if(daemonConnectionRes == sockSender::ERROR_WITH_BIN_DAEMON || daemonConnectionRes == sockSender::ERROR_TO_START_DAEMON){
        QMessageBox::warning(nullptr, "Serveur introuvable", "Impossible de localiser le serveur de mise à jour des fiches patients, \nmerci d'indiquer le chemin de l'exécutable dans la fenêtre suivante.");
        QString file = QFileDialog::getOpenFileName(nullptr, "Choix des fichiers", QDir::homePath(), "salamandre-daemon");

        if(file.isNull()){
            ntw::Socket::close();
            return -3;
        }
        else{
            sockSender::init();
            QFileInfo info(file);
            settings::setDaemonSettingValue("pathBin", info.filePath());
            daemonConnectionRes = sockSender::connectToDaemon();
        }
    }

    if(daemonConnectionRes == sockSender::ERROR_TO_CONNECT_DAEMON){
        //QMessageBox::critical(nullptr, "Erreur fatale", "La connexion au serveur de fichier à échoué, veuillez vérifier que l'application n'a pas déjà été lancée.");
        return -3;
    }

    if(!sockSender::checkPaths()){
        //QMessageBox::critical(nullptr, "Erreur fatale", "Les dossiers du serveur de synchronisation sont corrompus, merci de relancer le serveur.");
        return -4;
    }

    sockReceiver::setParamsCo(this->srvGuiPort+1);
    sockReceiver::init();
    sockReceiver::connectToDaemon();

    QDir dir(QCoreApplication::applicationDirPath()+"/save");
    if(!dir.exists()){
        dir.mkdir(dir.path());
    }

    return 0;
}

void threadGestionApps::startApps(int srvGuiPort)
{
    this->currentCmd = cmdApps::START_APPS;
    this->srvGuiPort = srvGuiPort;
}

void threadGestionApps::stopApps()
{
    this->currentCmd = cmdApps::STOP_APPS;
}

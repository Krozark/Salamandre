#include <forms/mainwindow.hpp>
#include <forms/connexiondialog.hpp>
#include <forms/choosedialog.hpp>

#include <objects/socksender.hpp>
#include <objects/settings.hpp>
#include <objects/sockreceiver.hpp>

#include <QApplication>
#include <QFileDialog>
#include <QMessageBox>
#include <QDebug>
#include <QDir>
#include <QProcess>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    QStringList args = a.arguments();

    int srvGuiPort = DEFAULT_SERVEUR_PORT;
    int guiNotifPort = DEFAULT_NOTIF_SERVER_PORT;
    std::string srvGuiIp = DEFAULT_IP;
    std::string guiNotifIp = DEFAULT_NOTIF_IP;

    if(args.size() >= 2){
        srvGuiPort = args.at(1).toInt();
    }
    if(args.size() >= 3){
        guiNotifPort = args.at(2).toInt();
    }
    if(args.size() >= 4){
        srvGuiIp = args.at(1).toInt();
    }
    if(args.size() >= 5){
        guiNotifIp = args.at(2).toInt();
    }

    settings::loadSettings();

    int daemonConnectionRes;
    int returnError = 0;
    bool restart = true;

    ntw::Socket::init();
    sockSender::init(srvGuiPort, srvGuiIp);
    daemonConnectionRes = sockSender::connectToDaemon();

    if(daemonConnectionRes == sockSender::ERROR_WITH_BIN_DAEMON || daemonConnectionRes == sockSender::ERROR_TO_START_DAEMON){
        QMessageBox::warning(nullptr, "Serveur introuvable", "Impossible de localiser le serveur de mise à jour des fiches patients, \nmerci d'indiquer le chemin de l'exécutable dans la fenêtre suivante.");
        QString file = QFileDialog::getOpenFileName(nullptr, "Choix des fichiers", QDir::homePath(), "salamandre-daemon");

        if(file.isNull()){
            ntw::Socket::close();
            return -3;
        }
        else{
            QFileInfo info(file);
            settings::setDaemonSettingValue("pathBin", info.filePath());
            daemonConnectionRes = sockSender::connectToDaemon();
        }
    }

    if(daemonConnectionRes == sockSender::ERROR_TO_CONNECT_DAEMON){
        QMessageBox::critical(nullptr, "Erreur fatale", "La connexion au serveur de fichier à échoué, veuillez vérifier que l'application n'a pas déjà été lancée.");
        return -3;
    }

    if(!sockSender::checkPaths()){
        QMessageBox::critical(nullptr, "Erreur fatale", "Les dossiers du serveur de synchronisation sont corrompus, merci de relancer le serveur.");
        return -4;
    }

    sockReceiver::init(guiNotifPort, guiNotifIp);
    sockReceiver::connectToDaemon();

    QDir dir(QCoreApplication::applicationDirPath()+"/save");
    if(!dir.exists()){
        dir.mkdir(dir.path());
    }

    while(restart && returnError == 0){
        restart = false;

        connexionDialog *coDialog = new connexionDialog(nullptr);
        int res = coDialog->exec();

        if(res == QDialog::Accepted){
            salamandre::Doctor *doctor = coDialog->getDoctor();

            chooseDialog *chDialog = new chooseDialog(doctor, nullptr);
            res = chDialog->exec();

            if(res  == QDialog::Rejected){
                delete doctor;
                returnError = -1;
            }
            else{
                salamandre::Patient *patient = chDialog->getPatient();
                QCoreApplication::processEvents();

                MainWindow w(doctor, patient, nullptr);

                if(w.isBadPass()){
                    restart = true;
                }
                else{
                    QVariant pos = settings::getMainwindowSettingValue("pos");
                    QVariant size = settings::getMainwindowSettingValue("size");

                    if(pos != QVariant())
                        w.move(pos.toPoint());
                    if(size != QVariant())
                        w.resize(size.toSize());

                    w.show();
                    returnError = a.exec();
                    restart = w.restartApps();
                }

                if(w.isBadPass()){ // on revérifie isBadPass, car le résultat peut changer après a.exec()
                    QMessageBox::critical(nullptr, "Erreur critique", "Mot de passe incorrect, vous aller être redirigé vers l'interface de connexion.");
                }
                else{
                    settings::setMainwindowSettingValue("pos", w.pos());
                    settings::setMainwindowSettingValue("size", w.size());
                }
            }

            delete chDialog;
        }
        else{
            returnError = -2;
        }

        delete coDialog;
    }

    sockReceiver::closeConnectionToDaemon();
    sockSender::closeConnectionToDaemon();
    ntw::Socket::close();

    settings::saveSettings();

    return returnError;
}

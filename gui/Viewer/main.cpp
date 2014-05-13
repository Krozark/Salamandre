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

    settings::loadSettings();

    int daemonConnectionRes;
    int returnError = 0;
    bool restart = true;

    ntw::Socket::init();
    sockSender::init();
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

            sockSender::init();
            daemonConnectionRes = sockSender::connectToDaemon();
        }
    }

    if(daemonConnectionRes == sockSender::ERROR_TO_CONNECT_DAEMON){
        QMessageBox::critical(nullptr, "Erreur fatale", "La connexion au serveur de fichier à échoué, veuillez vérifier que l'application n'a pas déjà été lancée.");
        return -3;
    }

    sockReceiver::init();
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
                    w.show();
                    returnError = a.exec();
                    restart = w.restartApps();
                }

                if(w.isBadPass()){
                    QMessageBox::critical(nullptr, "Erreur critique", "Mot de passe incorrect, vous aller être redirigé vers l'interface de connexion.");
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

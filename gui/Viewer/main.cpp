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
#include <QSharedMemory>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    settings::loadSettings();

    bool daemonConnectionSuccess;

    ntw::Socket::init();
    sockSender::init();
    daemonConnectionSuccess = sockSender::connectToDaemon();


    QString pathExe = settings::getDaemonSettingValue("pathExe").toString();

    if(pathExe == "" && !daemonConnectionSuccess){
        QMessageBox::warning(nullptr, "Serveur introuvable", "Impossible de localiser le serveur de mise à jour des fiches patients, \nmerci d'indiquer le chemin de l'exécutable dans la fenêtre suivante.");
        QString file = QFileDialog::getOpenFileName(nullptr, "Choix des fichiers", QDir::homePath(), "salamandre-daemon");
        QFileInfo info(file);
        settings::setDaemonSettingValue("pathExe", info.dir().path());

        sockSender::init();
        daemonConnectionSuccess = sockSender::connectToDaemon();
    }

    if(!daemonConnectionSuccess){
        QMessageBox::critical(nullptr, "Erreur fatale", "La connexion au serveur de fichier à échoué, veuillez vérifier que l'application n'a pas déjà été lancée.");
        return -3;
    }

    sockReceiver::init();
    sockReceiver::connectToDaemon();

    QDir dir(QCoreApplication::applicationDirPath()+"/save");
    if(!dir.exists()){
        dir.mkdir(dir.path());
    }

    int returnError;

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
            w.show();

            returnError = a.exec();
        }

        delete chDialog;
    }
    else{
        returnError = -2;
    }

    delete coDialog;

    sockReceiver::closeConnectionToDaemon();
    sockSender::closeConnectionToDaemon();
    ntw::Socket::close();

    settings::saveSettings();

    return returnError;
}

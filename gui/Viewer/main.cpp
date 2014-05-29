#include <forms/mainwindow.hpp>
#include <forms/connexiondialog.hpp>
#include <forms/choosedialog.hpp>
#include <forms/connectiondeconnectiondialog.hpp>

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

    if(args.size() == 2)
        srvGuiPort = args.at(1).toInt();

    connectionDeconnectionDialog *dialogCoDeco = new connectionDeconnectionDialog(nullptr);
    dialogCoDeco->start(srvGuiPort);

    if(dialogCoDeco->exec() == QDialog::Rejected){
        return -1;
    }

    dialogCoDeco->close();
    delete dialogCoDeco;

    int returnError = 0;
    bool restart = true;

    while(restart && returnError == 0){
        restart = false;

        connexionDialog *coDialog = new connexionDialog(nullptr);
        int res = coDialog->exec();

        if(res == QDialog::Accepted){
            salamandre::Doctor *doctor = coDialog->getDoctor();
            bool isDoctorUpdating = sockSender::isUpdating(doctor->getId().toInt());

            chooseDialog *chDialog = new chooseDialog(doctor, nullptr);

            if(isDoctorUpdating){
                QMessageBox::warning(nullptr, APPS_NAME, "Les fichiers du médecin sont en cours de récupération, l'accès sera débloqué dés que l'opération aura aboutie");

                chDialog->disableForUpdate(true);
                chDialog->showUpdate();
            }
            res = chDialog->exec();

            if(res  == QDialog::Rejected){
                delete doctor;
                returnError = -1;
            }
            else{
                salamandre::Patient *patient = chDialog->getPatient();
                QCoreApplication::processEvents();

                MainWindow w(doctor, patient, nullptr);

                bool badPass = w.isBadPass();
                if(badPass){
                    QMessageBox::critical(nullptr, "Erreur critique", "Mot de passe incorrect, vous allez être redirigé vers l'interface de connexion.");
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

                    if(returnError == 0){
                        settings::setMainwindowSettingValue("pos", w.pos());
                        settings::setMainwindowSettingValue("size", w.size());
                    }

                    badPass = w.isBadPass();
                    if(badPass){
                        QMessageBox::critical(nullptr, "Erreur critique", "Mot de passe incorrect, vous allez être redirigé vers l'interface de connexion.");
                        restart = true;
                    }
                }
            }

            delete chDialog;
        }
        else{
            returnError = -2;
        }

        delete coDialog;
    }

    dialogCoDeco = new connectionDeconnectionDialog(nullptr);
    dialogCoDeco->stop();
    dialogCoDeco->exec();

    delete dialogCoDeco;

    return returnError;
}

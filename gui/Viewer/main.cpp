#include <forms/mainwindow.hpp>
#include <forms/connexiondialog.hpp>
#include <forms/choosedialog.hpp>

#include <QApplication>
#include <QDebug>
#include <QDir>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    QDir dir(QCoreApplication::applicationDirPath()+"/save");
    if(!dir.exists()){
        dir.mkdir(dir.path());
    }

    connexionDialog *coDialog = new connexionDialog(nullptr);
    int res = coDialog->exec();

    if(res == QDialog::Rejected){
        return -1;
    }

    Doctor *doctor = coDialog->getDoctor();

    chooseDialog *chDialog = new chooseDialog(doctor, nullptr);
    chDialog->setWindowFlags(Qt::Dialog | Qt::FramelessWindowHint | Qt::CustomizeWindowHint | Qt::WindowTitleHint | Qt::WindowMinMaxButtonsHint);

    res = chDialog->exec();

    if(res  == QDialog::Rejected){
        return -1;
    }

    Patient *patient = chDialog->getPatient();

    //chooseDialog::Choice choice = chDialog->getChoice();

    /*switch(choice){
    case chooseDialog::Choice::GET_ALL_CLIENTS_DATA:
        this->startDownloadClientData();
        break;
    case chooseDialog::Choice::GET_ONE_CLIENT_DATA:
        this->startDownloadClientData(this->currentIdPatient.toInt());
        break;
    case chooseDialog::Choice::NEW_CLIENT_DATA:
        this->createNewClientData();
        break;
    default:
        break;
    }*/

    MainWindow w(doctor, patient, nullptr);
    w.show();

    delete coDialog;
    delete chDialog;

    return a.exec();
}

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
        return 0;
    }

    salamandre::Doctor *doctor = coDialog->getDoctor();
    delete coDialog;

    chooseDialog *chDialog = new chooseDialog(doctor, nullptr);
    res = chDialog->exec();

    if(res  == QDialog::Rejected){
        delete doctor;
        return 0;
    }

    salamandre::Patient *patient = chDialog->getPatient();
    delete chDialog;

    QCoreApplication::processEvents();

    MainWindow w(doctor, patient, nullptr);
    w.show();

    return a.exec();
}

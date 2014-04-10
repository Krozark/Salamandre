#include <forms/mainwindow.hpp>
#include <forms/connexiondialog.hpp>

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

    MainWindow w(coDialog->getIdMedecin(), coDialog->getPassMedecin());
    w.show();

    delete coDialog;

    return a.exec();
}

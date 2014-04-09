#include <forms/mainwindow.hpp>
#include <forms/connexiondialog.hpp>

#include <QApplication>
#include <QDebug>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

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

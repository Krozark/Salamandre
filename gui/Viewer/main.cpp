#include <forms/mainwindow.hpp>
#include <forms/connexiondialog.hpp>

#include <QApplication>
#include <QDebug>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    connexionDialog *coDialog = new connexionDialog(nullptr);
    int res = coDialog->exec();
    delete coDialog;

    if(res == QDialog::Rejected){
        return -1;
    }

    MainWindow w;
    w.show();

    return a.exec();
}

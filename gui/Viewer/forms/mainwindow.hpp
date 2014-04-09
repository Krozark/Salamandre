#ifndef MAINWINDOW_HPP
#define MAINWINDOW_HPP

#include <QMainWindow>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QString idMedecin, QString passMedecin, QWidget *parent = 0);
    ~MainWindow();

    void init();
private:
    Ui::MainWindow *ui;

    QString currentUserId;
    QString currentUserPass;

    QString idMedecin, passMedecin;

    void startDownloadClientData(int clientNumber = -1);
};

#endif // MAINWINDOW_HPP

#ifndef MAINWINDOW_HPP
#define MAINWINDOW_HPP

#include <objects/doctor.hpp>
#include <objects/patient.hpp>
#include <objects/listview.hpp>
#include <objects/threaduploadfile.hpp>

#include <QMainWindow>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(salamandre::Doctor *doctor, salamandre::Patient *patient, QWidget *parent = 0);
    ~MainWindow();

    void init();
private:
    Ui::MainWindow *ui;

    salamandre::Doctor *doctor;
    salamandre::Patient *patient;

    ListView *listViewDigitalFiles;
    threadUploadFile *threadUpload;

    void startDownloadClientData(int clientNumber = -1);
    void loadRecords();
    void loadFEC();
    void loadFCT();
    void loadFMT();
    void loadFMN();

    void saveRecords();
    void saveFEC();
    void saveFCT();
    void saveFMT();
    void saveFMN();

    bool checkNeedSave();

private slots:
    void startUploadDigitalFile(QStringList listFile);

    void on_actionQuitter_triggered();
    void on_actionNouveau_patient_triggered();
    void on_actionChanger_de_patient_triggered();
    void on_actionEnregistrer_triggered();


};

#endif // MAINWINDOW_HPP

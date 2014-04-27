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

    bool saveFCTNeeded, saveFMTNeeded;

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

    void checkNeedSave();
    bool checkNeedSaveFEC();
    bool checkNeedSaveFCT();
    bool checkNeedSaveFMT();
    bool checkNeedSaveFMN();

    void clearPatient();

    void closeEvent(QCloseEvent *);

private slots:
    void startUploadDigitalFile(QStringList listFile);
    void refreshDigitalFile();

    void on_actionQuitter_triggered();
    void on_actionNouveau_patient_triggered();
    void on_actionChanger_de_patient_triggered();
    void on_actionEnregistrer_triggered();


    void on_toolButton_numericalExporter_clicked();
    void on_toolButton_numericalImporter_clicked();
    void on_plainTextEdit_confidentialTextPatient_textChanged();
    void on_plainTextEdit_medicalTextPatient_textChanged();
};

#endif // MAINWINDOW_HPP

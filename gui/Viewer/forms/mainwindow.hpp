#ifndef MAINWINDOW_HPP
#define MAINWINDOW_HPP

#include <forms/savedialog.hpp>

#include <objects/doctor.hpp>
#include <objects/patient.hpp>
#include <objects/listview.hpp>
#include <objects/threaduploadfile.hpp>
#include <objects/threadsave.hpp>
#include <objects/sockreceiver.hpp>

#include <QMainWindow>
#include <QLabel>
#include <QProgressBar>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(salamandre::Doctor *doctor, salamandre::Patient *patient, QWidget *parent = 0);
    ~MainWindow();

    bool restartApps();
    bool isBadPass();
private:

    enum actionEmit{
        ACTION_NEW_PATIENT = 0,
        ACTION_CHANGE_PATIENT,
        ACTION_QUIT,
        ACTION_NO
    };

    Ui::MainWindow *ui;

    salamandre::Doctor *doctor;
    salamandre::Patient *patient;

    ListView *listViewDigitalFiles;

    bool saveFCTNeeded, saveFMTNeeded;
    bool restartApplication;
    bool badPass;

    // status bar objects.
    QProgressBar *progressBarProcessing;
    QLabel *labelTotalNumber;
    QLabel *labelSeparator;
    QLabel *labelProcessNumber;

    threadUploadFile *threadUpload;
    threadSave *threadSaveRecord;

    saveDialog *saveRecordDialog;
    actionEmit currentAction;

    void init();
    void initStatusBar();
    void initStatusBarValue();

    void startDownloadClientData(int clientNumber = -1);
    bool loadRecords();
    bool loadFEC();
    bool loadFCT();
    bool loadFMT();
    bool loadFMN();

    void saveRecords();
    bool saveFEC();
    bool saveFCT();
    bool saveFMT();
    bool saveFMN();

    void checkNeedSave();
    bool checkNeedSaveFEC();
    bool checkNeedSaveFCT();
    bool checkNeedSaveFMT();
    bool checkNeedSaveFMN();

    void clearPatient();
    void setTabsEnable(bool enable);

    void closeEvent(QCloseEvent *event);

private slots:
    void startUploadDigitalFile(QStringList listFile);
    void refreshDigitalFile();
    void refreshNumberProcessFile(int number);
    void refreshNumberInsertFile(int number);
    void setProgressBarText(QString text);
    void saveProgress(int save);
    void saveEnd();
    void restartNeeded();
    void syncFinish(getFile *file);

    void on_actionQuitter_triggered();
    void on_actionNouveau_patient_triggered();
    void on_actionChanger_de_patient_triggered();
    void on_actionEnregistrer_triggered();


    void on_toolButton_numericalExporter_clicked();
    void on_toolButton_numericalImporter_clicked();
    void on_plainTextEdit_confidentialTextPatient_textChanged();
    void on_plainTextEdit_medicalTextPatient_textChanged();
    void on_actionD_connection_triggered();
};

#endif // MAINWINDOW_HPP

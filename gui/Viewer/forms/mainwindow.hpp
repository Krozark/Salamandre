#ifndef MAINWINDOW_HPP
#define MAINWINDOW_HPP

#include <objects/doctor.hpp>
#include <objects/patient.hpp>

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

    void startDownloadClientData(int clientNumber = -1);
    void createNewClientData();
};

#endif // MAINWINDOW_HPP

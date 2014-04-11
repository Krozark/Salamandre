#ifndef CHOOSEDIALOG_HPP
#define CHOOSEDIALOG_HPP

#include <objects/doctor.hpp>
#include <objects/patient.hpp>

#include <QDialog>
#include <QStandardItemModel>

namespace Ui {
class chooseDialog;
}

class chooseDialog : public QDialog
{
    Q_OBJECT
    
public:

    enum Choice{
        GET_ALL_CLIENTS_DATA = 0,
        GET_ONE_CLIENT_DATA,
        NEW_CLIENT_DATA
    };

    explicit chooseDialog(Doctor *doctor, QWidget *parent = 0);
    ~chooseDialog();

    Choice getChoice();
    QString getPatientNumber();

    Patient* getPatient();

    void close();
    
private slots:
    void on_radioButton_getAllClientsFiles_clicked();
    void on_radioButton_getClientFiles_clicked();
    void on_radioButton_createClientFiles_clicked();
    void on_lineEdit_clientNumber_textChanged(const QString &arg1);
    void on_radioButton_newClientData_clicked();
    void on_radioButton_getDataClient_clicked();
    void on_radioButton_availablePatient_clicked();

private:
    Ui::chooseDialog *ui;
    QStandardItemModel *model;
    Doctor *doctor;
    Patient *patient;
};

#endif // CHOOSEDIALOG_HPP

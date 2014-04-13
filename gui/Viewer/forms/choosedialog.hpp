#ifndef CHOOSEDIALOG_HPP
#define CHOOSEDIALOG_HPP

#include <objects/doctor.hpp>
#include <objects/patient.hpp>

#include <QDialog>
#include <QSortFilterProxyModel>
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

    explicit chooseDialog(salamandre::Doctor *doctor, QWidget *parent = 0);
    ~chooseDialog();

    salamandre::Patient* getPatient();

    void accept();
    
private slots:
    void on_radioButton_newClientData_clicked();
    void on_radioButton_getDataClient_clicked();
    void on_radioButton_availablePatient_clicked();

    void on_lineEdit_newMedecinAndClientData_textChanged(const QString &arg1);
    void on_listView_availablePatient_clicked(const QModelIndex &index);

    void on_lineEdit_newClientData_textChanged(const QString &arg1);
    void on_lineEdit_getDataClient_textChanged(const QString &arg1);
    void on_lineEdit_researchPatient_textChanged(const QString &arg1);

private:
    Ui::chooseDialog *ui;
    QStandardItemModel *model;
    QSortFilterProxyModel *filterModel;
    salamandre::Doctor *doctor;
    salamandre::Patient *patient;

    void createNewRecords();
    void loadAllRecords();
};

#endif // CHOOSEDIALOG_HPP

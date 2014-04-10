#ifndef CHOOSEDIALOG_HPP
#define CHOOSEDIALOG_HPP

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

    enum TypeChoice{
        NEW_MEDECIN = 0,
        MEDECIN_ALREADY_EXIST
    };

    explicit chooseDialog(TypeChoice type = NEW_MEDECIN, QString idMedecin = "", QWidget *parent = 0);
    ~chooseDialog();

    Choice getChoice();
    int getClientNumber();
    
private slots:
    void on_radioButton_getAllClientsFiles_clicked();
    void on_radioButton_getClientFiles_clicked();
    void on_radioButton_createClientFiles_clicked();
    void on_lineEdit_clientNumber_textChanged(const QString &arg1);

private:
    Ui::chooseDialog *ui;
    QStandardItemModel *model;
};

#endif // CHOOSEDIALOG_HPP

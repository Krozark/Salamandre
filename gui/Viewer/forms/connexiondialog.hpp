#ifndef CONNEXIONDIALOG_HPP
#define CONNEXIONDIALOG_HPP

#include <QDialog>

namespace Ui {
class connexionDialog;
}

class connexionDialog : public QDialog
{
    Q_OBJECT
    
public:
    explicit connexionDialog(QWidget *parent = 0);
    ~connexionDialog();

    QString getIdMedecin();
    QString getPassMedecin();

    void checkToEnableConnection();
    
private slots:
    void on_pushButton_connection_clicked();

    void on_checkBox_userNew_stateChanged(int arg1);

    void on_pushButton_next_clicked();

    void on_pushButton_connectionNewMedecin_clicked();

    void on_lineEdit_textChanged(const QString &arg1);

    void on_lineEdit_userLogin_textChanged(const QString &arg1);

    void on_lineEdit_userPassword_textChanged(const QString &arg1);

private:
    Ui::connexionDialog *ui;
};

#endif // CONNEXIONDIALOG_HPP

#ifndef PATIENTNAMEDIALOG_HPP
#define PATIENTNAMEDIALOG_HPP

#include <QDialog>
#include <QIntValidator>

namespace Ui {
class patientNameDialog;
}

class patientNameDialog : public QDialog
{
    Q_OBJECT

public:
    explicit patientNameDialog(QWidget *parent = 0);
    ~patientNameDialog();

    QString getText();

private:
    Ui::patientNameDialog *ui;
    QIntValidator *intValidator;
};

#endif // PATIENTNAMEDIALOG_HPP

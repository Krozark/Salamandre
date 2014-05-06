#ifndef DAEMONCONNECTIONDIALOG_HPP
#define DAEMONCONNECTIONDIALOG_HPP

#include <QDialog>
#include <QMovie>

namespace Ui {
class daemonConnectionDialog;
}

class daemonConnectionDialog : public QDialog
{
    Q_OBJECT

public:
    explicit daemonConnectionDialog(QWidget *parent = 0);
    ~daemonConnectionDialog();

private:
    Ui::daemonConnectionDialog *ui;
};

#endif // DAEMONCONNECTIONDIALOG_HPP

#ifndef DAEMONCONNECTIONDIALOG_HPP
#define DAEMONCONNECTIONDIALOG_HPP

#include <QDialog>
#include <QMovie>

#define CONNECTION_TEST_NUMBER 30

namespace Ui {
class daemonConnectionDialog;
}

class daemonConnectionDialog : public QDialog
{
    Q_OBJECT

public:
    explicit daemonConnectionDialog(QWidget *parent = 0);
    ~daemonConnectionDialog();

    void setMaxNbTest(int nb = CONNECTION_TEST_NUMBER);
    void increaseNbTest(int nb);

private:
    Ui::daemonConnectionDialog *ui;

    QMovie *movie;
    int maxTest;
};

#endif // DAEMONCONNECTIONDIALOG_HPP

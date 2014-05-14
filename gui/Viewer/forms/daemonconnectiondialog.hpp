#ifndef DAEMONCONNECTIONDIALOG_HPP
#define DAEMONCONNECTIONDIALOG_HPP

#include <QDialog>
#include <QMovie>
#include <QCloseEvent>

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
    void closeEvent(QCloseEvent *event);

    QMovie *movie;
    int maxTest;
    int nbTest;
};

#endif // DAEMONCONNECTIONDIALOG_HPP

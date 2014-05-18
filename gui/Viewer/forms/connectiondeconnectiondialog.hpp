#ifndef CONNECTIONDECONNECTIONDIALOG_HPP
#define CONNECTIONDECONNECTIONDIALOG_HPP

#include <objects/threadgestionapps.hpp>

#include <QDialog>
#include <QMovie>
#include <QCloseEvent>

namespace Ui {
class connectionDeconnectionDialog;
}

class connectionDeconnectionDialog : public QDialog
{
    Q_OBJECT

public:
    explicit connectionDeconnectionDialog(QWidget *parent = 0);
    ~connectionDeconnectionDialog();

    void start(int srvGuiPort);
    void stop();

    bool getStartResult();
    bool getEndResult();

private:
    threadGestionApps *thread;
    Ui::connectionDeconnectionDialog *ui;

    void closeEvent(QCloseEvent *event);

    bool startResult;
    bool endResult;

    QMovie *movie;

    void enableBottom(bool enable);

private slots:
    void connectionIsSuccess();
    void connectionIsFailed();
    void deconnectionIsSuccess();
    void deconnectionIsFailed();
    void setConnectionText(int nbConnection, int maxNbConnection);
};

#endif // CONNECTIONDECONNECTIONDIALOG_HPP

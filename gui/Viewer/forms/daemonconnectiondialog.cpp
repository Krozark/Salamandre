#include "daemonconnectiondialog.hpp"
#include "ui_daemonconnectiondialog.h"

daemonConnectionDialog::daemonConnectionDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::daemonConnectionDialog)
{
    ui->setupUi(this);
}

daemonConnectionDialog::~daemonConnectionDialog()
{
    delete ui;
}

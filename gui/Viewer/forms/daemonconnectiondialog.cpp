#include "daemonconnectiondialog.hpp"
#include "ui_daemonconnectiondialog.h"

daemonConnectionDialog::daemonConnectionDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::daemonConnectionDialog)
{
    ui->setupUi(this);

    this->maxTest = CONNECTION_TEST_NUMBER;
}

daemonConnectionDialog::~daemonConnectionDialog()
{
    delete ui;
}

void daemonConnectionDialog::setMaxNbTest(int nb)
{
    this->maxTest = nb;
}

void daemonConnectionDialog::increaseNbTest(int nb)
{
    this->ui->label_nbTest->setText("Tentative "+QString::number(nb)+"/"+QString::number(this->maxTest));
}

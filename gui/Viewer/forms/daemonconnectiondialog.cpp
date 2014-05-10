#include "daemonconnectiondialog.hpp"
#include "ui_daemonconnectiondialog.h"

daemonConnectionDialog::daemonConnectionDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::daemonConnectionDialog)
{
    ui->setupUi(this);

    this->maxTest = CONNECTION_TEST_NUMBER;

    this->movie = new QMovie(":/icons/controls/waiting.gif");
    this->movie->setScaledSize(QSize(25,25));
    this->ui->label_movie->setMovie(this->movie);
    this->movie->start();

    QCoreApplication::processEvents();
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

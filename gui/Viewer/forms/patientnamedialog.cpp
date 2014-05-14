#include "patientnamedialog.hpp"
#include "ui_patientnamedialog.h"

patientNameDialog::patientNameDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::patientNameDialog)
{
    ui->setupUi(this);

    this->intValidator = new QIntValidator();
    this->ui->lineEdit->setValidator(this->intValidator);
}

patientNameDialog::~patientNameDialog()
{
    delete this->intValidator;
    delete ui;
}

QString patientNameDialog::getText()
{
    return this->ui->lineEdit->text();
}

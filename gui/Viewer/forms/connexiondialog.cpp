#include "connexiondialog.hpp"
#include "ui_connexiondialog.h"

connexionDialog::connexionDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::connexionDialog)
{
    ui->setupUi(this);
}

connexionDialog::~connexionDialog()
{
    delete ui;
}

void connexionDialog::on_pushButton_clicked()
{
    this->accept();
}

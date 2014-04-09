#include "choosedialog.hpp"
#include "ui_choosedialog.h"

chooseDialog::chooseDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::chooseDialog)
{
    ui->setupUi(this);

    this->ui->widget_clientNumber->setVisible(false);
    this->ui->lineEdit_clientNumber->setValidator(new QIntValidator());
}

chooseDialog::~chooseDialog()
{
    delete this->ui->lineEdit_clientNumber->validator();
    delete ui;
}

chooseDialog::Choice chooseDialog::getChoice()
{
    if(this->ui->radioButton_getAllClientsFiles->isChecked()){
        return Choice::GET_ALL_CLIENTS_DATA;
    }
    else if(this->ui->radioButton_getClientFiles->isChecked()){
        return Choice::GET_ONE_CLIENT_DATA;
    }
    else if(this->ui->radioButton_createClientFiles->isChecked()){
        return Choice::NEW_CLIENT_DATA;
    }

    return Choice::NEW_CLIENT_DATA;
}

int chooseDialog::getClientNumber()
{
    return this->ui->lineEdit_clientNumber->text().toInt();
}

void chooseDialog::on_radioButton_getAllClientsFiles_clicked()
{
    this->ui->widget_clientNumber->setVisible(false);
    this->ui->buttonBox->setEnabled(true);
}

void chooseDialog::on_radioButton_getClientFiles_clicked()
{
    this->ui->widget_clientNumber->setVisible(true);
    this->ui->buttonBox->setEnabled(!this->ui->lineEdit_clientNumber->text().isEmpty());
}

void chooseDialog::on_radioButton_createClientFiles_clicked()
{
    this->ui->widget_clientNumber->setVisible(true);
    this->ui->buttonBox->setEnabled(!this->ui->lineEdit_clientNumber->text().isEmpty());
}

void chooseDialog::on_lineEdit_clientNumber_textChanged(const QString &arg1)
{
    this->ui->buttonBox->setEnabled(!arg1.isEmpty());
}

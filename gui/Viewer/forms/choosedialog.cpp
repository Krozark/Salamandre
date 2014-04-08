#include "choosedialog.hpp"
#include "ui_choosedialog.h"

chooseDialog::chooseDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::chooseDialog)
{
    ui->setupUi(this);
}

chooseDialog::~chooseDialog()
{
    delete ui;
}

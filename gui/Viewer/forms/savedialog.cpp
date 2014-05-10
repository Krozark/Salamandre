#include <forms/savedialog.hpp>
#include <ui_savedialog.h>

saveDialog::saveDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::saveDialog)
{
    ui->setupUi(this);

    this->setModal(false);
    QCoreApplication::processEvents();
}

saveDialog::~saveDialog()
{
    delete ui;
}

void saveDialog::close()
{
    QDialog::close();
    QCoreApplication::processEvents();
}

void saveDialog::setProgressText(QString text)
{
    this->ui->textEdit->insertHtml(text);
    this->ui->textEdit->insertPlainText("\n");
}

void saveDialog::setProgress(int val)
{
    this->ui->progressBar->setValue(val);
}

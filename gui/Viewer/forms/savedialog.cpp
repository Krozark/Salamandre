#include <forms/savedialog.hpp>
#include <ui_savedialog.h>

saveDialog::saveDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::saveDialog)
{
    ui->setupUi(this);

    this->setModal(false);

    this->movie = new QMovie(":/icons/controls/waiting.gif");
    this->movie->setScaledSize(QSize(25,25));
    this->ui->label_movie->setMovie(this->movie);
    this->movie->start();

    QCoreApplication::processEvents();
}

saveDialog::~saveDialog()
{
    delete this->movie;
    delete this->ui;
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

#include <forms/connexiondialog.hpp>
#include <ui_connexiondialog.h>

#include <crypto++/default.h>
#include <QDir>

connexionDialog::connexionDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::connexionDialog)
{
    ui->setupUi(this);

    this->isNew = false;
    this->ui->pushButton_next->setVisible(false);
    this->ui->stackedWidget->setCurrentIndex(0);
    this->checkToEnableConnection();
}

connexionDialog::~connexionDialog()
{
    delete ui;
}

void connexionDialog::accept()
{
    if(this->isNew){
        QDir dir(QCoreApplication::applicationDirPath()+"/save/"+this->getIdMedecin());
        if(!dir.exists()){
            dir.mkdir(dir.path());
        }
    }

    QDialog::accept();
}

QString connexionDialog::getIdMedecin()
{
    return this->ui->lineEdit_userLogin->text();
}

QString connexionDialog::getPassMedecin()
{
    return this->ui->lineEdit_userPassword->text();
}

void connexionDialog::on_pushButton_connection_clicked()
{
    this->accept();
}

void connexionDialog::on_checkBox_userNew_stateChanged(int arg1)
{
    if(arg1 == Qt::Checked){
        this->ui->pushButton_connection->setVisible(false);
        this->ui->pushButton_next->setVisible(true);
    }
    else{
        this->ui->pushButton_next->setVisible(false);
        this->ui->pushButton_connection->setVisible(true);
    }
}

void connexionDialog::on_pushButton_next_clicked()
{
    this->ui->stackedWidget->setCurrentIndex(1);
}

void connexionDialog::on_pushButton_connectionNewMedecin_clicked()
{
    this->isNew = true;
    this->accept();
}

void connexionDialog::on_lineEdit_textChanged(const QString &arg1)
{
    if(this->getPassMedecin() == arg1){
        this->ui->pushButton_connectionNewMedecin->setEnabled(true);
        this->ui->label_differentPassword->setVisible(false);
    }
    else{
        this->ui->pushButton_connectionNewMedecin->setEnabled(false);
        this->ui->label_differentPassword->setVisible(true);
    }
}

void connexionDialog::checkToEnableConnection()
{
    if(!this->ui->lineEdit_userLogin->text().isEmpty() && !this->ui->lineEdit_userPassword->text().isEmpty()){
        this->ui->pushButton_connection->setEnabled(true);
        this->ui->pushButton_next->setEnabled(true);
    }
    else{
        this->ui->pushButton_connection->setEnabled(false);
        this->ui->pushButton_next->setEnabled(false);
    }
}

void connexionDialog::on_lineEdit_userLogin_textChanged(const QString &arg1)
{
    Q_UNUSED(arg1);
    this->checkToEnableConnection();
}

void connexionDialog::on_lineEdit_userPassword_textChanged(const QString &arg1)
{
    Q_UNUSED(arg1);
    this->checkToEnableConnection();
}

#include <forms/connexiondialog.hpp>
#include <ui_connexiondialog.h>

#include <crypto++/default.h>
#include <QDir>
#include <QDebug>

connexionDialog::connexionDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::connexionDialog)
{
    ui->setupUi(this);

    this->isNew = false;
    this->doctor = new salamandre::Doctor();

    this->ui->label_doctorAlwaysRegister->setText("");
    this->ui->pushButton_next->setVisible(false);
    this->ui->stackedWidget->setCurrentIndex(0);
    this->checkToEnableConnection();

    this->validator = new QIntValidator(nullptr);
    this->ui->lineEdit_userLogin->setValidator(validator);
}

connexionDialog::~connexionDialog()
{
    delete this->validator;
    delete ui;
}

void connexionDialog::accept()
{
    this->doctor->setId(this->getIdDoctor());
    this->doctor->setPass(this->getPassDoctor());
    this->doctor->setDirPath(QCoreApplication::applicationDirPath()+"/save/"+this->doctor->getId());

    QDir dir(this->doctor->getDirPath());

    if(this->isNew){
        if(!dir.exists()){
            dir.mkdir(dir.path());
        }

        this->doctor->setType(salamandre::Doctor::TypeDoctor::NEW_DOCTOR);
    }
    else{
        if(!dir.exists()){
            dir.mkdir(dir.path());
        }

        if(dir.entryInfoList(QDir::AllDirs | QDir::NoDotAndDotDot).size() == 0){
            this->doctor->setType(salamandre::Doctor::TypeDoctor::DOCTOR_ALREADY_EXIST_BUT_NOTHING);
        }
        else{
            this->doctor->setType(salamandre::Doctor::TypeDoctor::DOCTOR_ALREADY_EXIST);
        }
    }

    QDialog::accept();
}

void connexionDialog::reject()
{
    delete this->doctor;
    QDialog::reject();
}

salamandre::Doctor* connexionDialog::getDoctor()
{
    return this->doctor;
}

QString connexionDialog::getIdDoctor()
{
    return this->ui->lineEdit_userLogin->text();
}

QString connexionDialog::getPassDoctor()
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

    this->checkToEnableConnection();
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
    if(this->getPassDoctor() == arg1){
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
    this->ui->pushButton_connection->setEnabled(false);
    this->ui->pushButton_next->setEnabled(false);

    if(!this->ui->lineEdit_userLogin->text().isEmpty() && !this->ui->lineEdit_userPassword->text().isEmpty()){
        QDir dir(QCoreApplication::applicationDirPath()+"/save/"+this->ui->lineEdit_userLogin->text());
        if(dir.exists()){
            if(this->ui->checkBox_userNew->isChecked()){
                this->ui->label_doctorAlwaysRegister->setText("Ce numéro est déjà enregistré !");
                this->ui->pushButton_connection->setEnabled(false);
                this->ui->pushButton_next->setEnabled(false);
            }
            else{
                this->ui->label_doctorAlwaysRegister->setText("");
                this->ui->pushButton_connection->setEnabled(true);
                this->ui->pushButton_next->setEnabled(false);
            }

        }
        else{
            if(this->ui->checkBox_userNew->isChecked()){
                this->ui->label_doctorAlwaysRegister->setText("");
                this->ui->pushButton_next->setEnabled(true);
                this->ui->pushButton_connection->setEnabled(false);
            }
            else{
                this->ui->label_doctorAlwaysRegister->setText("");
                this->ui->pushButton_next->setEnabled(false);
                this->ui->pushButton_connection->setEnabled(true);
            }
        }
    }
}

void connexionDialog::on_lineEdit_userLogin_textChanged(const QString &arg1)
{
    Q_UNUSED(arg1);
    this->ui->label_doctorAlwaysRegister->setText("");
    this->checkToEnableConnection();
}

void connexionDialog::on_lineEdit_userPassword_textChanged(const QString &arg1)
{
    Q_UNUSED(arg1);
    this->checkToEnableConnection();
}

void connexionDialog::on_lineEdit_userLogin_returnPressed()
{
    this->nextStep();
}

void connexionDialog::on_lineEdit_userPassword_returnPressed()
{
    this->nextStep();
}

void connexionDialog::nextStep()
{
    this->checkToEnableConnection();

    if(this->ui->pushButton_connection->isEnabled())
        this->on_pushButton_connection_clicked();
    else if(this->ui->pushButton_next->isEnabled())
        this->on_pushButton_next_clicked();
}

void connexionDialog::on_lineEdit_returnPressed()
{
    this->on_pushButton_connectionNewMedecin_clicked();
}

#include <forms/updatefiledialog.hpp>
#include <ui_updatefiledialog.h>

#include <QDebug>

updateFileDialog::updateFileDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::updateFileDialog)
{
    this->ui->setupUi(this);
    this->ui->buttonBox->setVisible(false);
    this->setModal(true);
    this->isSync = true;

    this->connect(&(sockReceiver::sock), SIGNAL(syncIsFinish(getFile*)), this, SLOT(syncFinished(getFile*)));
    this->connect(&(sockReceiver::sock), SIGNAL(syncIsFinish(getFile*)), this, SLOT(fileRcv(getFile*)));
}

updateFileDialog::~updateFileDialog()
{
    int nbData = this->dataList.size();
    for(int i = 0; i < nbData; ++i){
        delete this->dataList.at(i);
    }

    delete this->ui;
}

void updateFileDialog::closeEvent(QCloseEvent *event)
{
    if(this->isSync)
        event->ignore();
}

void updateFileDialog::askFile(int idDoctor, int idPatient, std::string filename)
{
    if(!this->isVisible())
        this->show();

    getFile *file = new getFile(idDoctor, idPatient, filename);

    if(filename == "" && idPatient >= 0){
        this->ui->textEdit_progressGetFile->insertHtml("<span style=\"color:orange\"> Récupération des fichiers du patient " + QString::number(idPatient) + " ... </span>");
    }
    else if(filename == "" && idPatient == -1){
        this->ui->textEdit_progressGetFile->insertHtml("<span style=\"color:orange\"> Récupération des dossiers patients du docteur " + QString::number(idDoctor) + " ... </span>");
    }
    else{
        this->ui->textEdit_progressGetFile->insertHtml("<span style=\"color:orange\"> Récupération de la fiche " + QString::fromStdString(filename) + " du patient " + QString::number(idPatient) + " ... </span>");
    }

    this->ui->textEdit_progressGetFile->insertPlainText("\n");

    this->dataList.push_back(file);
    sockReceiver::askFile(file);
    qDebug() << "file has been ask";
}

void updateFileDialog::syncFinished(getFile *fileRecv)
{
    getFile *file = nullptr;
    bool found = false;
    int nbFileToGet = this->dataList.size();

    for(int i = 0; i < nbFileToGet; ++i){
        file = this->dataList.at(i);

        if(file == fileRecv){
            this->dataList.remove(i);
            found = true;
            break;
        }
    }

    if(!found)
        return;

    if(this->dataList.size() == 0){
        this->isSync = false;
        this->ui->buttonBox->setVisible(true);
    }
}

void updateFileDialog::fileRcv(getFile *fileRecv)
{
    std::string filename = fileRecv->filename;
    int idDoctor = fileRecv->idDoctor;
    int idPatient = fileRecv->idPatient;

    if(filename == "" && idPatient >= 0){
        this->ui->textEdit_progressGetFile->insertHtml("<span style=\"color:darkgreen\">Les fichiers du patient " +QString::number(idPatient)+" du docteur " +QString::number(idDoctor)+ " ont été reçus</span>");
    }
    else if(filename == "" && idPatient == -1){
        this->ui->textEdit_progressGetFile->insertHtml("<span style=\"color:darkgreen\">Les dossiers patients du docteur " +QString::number(idDoctor)+ " ont été reçus</span>");
    }
    else{
        this->ui->textEdit_progressGetFile->insertHtml("<span style=\"color:darkgreen\">Le fichier " + QString::fromStdString(filename)+ " du patient " +QString::number(idPatient)+ " du docteur " +QString::number(idDoctor)+ " à été reçu</span>");
    }

    this->ui->textEdit_progressGetFile->insertPlainText("\n");
}
void updateFileDialog::on_buttonBox_clicked(QAbstractButton *button)
{
    QDialogButtonBox::StandardButton buttonBox = this->ui->buttonBox->standardButton(button);

    switch(buttonBox){
    case QDialogButtonBox::Close:
        this->accept();
        break;
    default:
        break;
    }
}

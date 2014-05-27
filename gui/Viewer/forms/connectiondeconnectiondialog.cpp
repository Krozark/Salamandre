#include <forms/connectiondeconnectiondialog.hpp>
#include <ui_connectiondeconnectiondialog.h>

#include <objects/socksender.hpp>
#include <objects/settings.hpp>

#include <QMessageBox>
#include <QFileDialog>

connectionDeconnectionDialog::connectionDeconnectionDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::connectionDeconnectionDialog)
{
    ui->setupUi(this);

    this->thread = new threadGestionApps(this);
    this->startResult = false;
    this->endResult = false;

    this->movie = new QMovie(":/icons/controls/waiting.gif");
    this->movie->setScaledSize(QSize(50,50));
    this->ui->label_movie->setMovie(this->movie);
    this->movie->start();
    this->setWindowIcon(QPixmap(":/icons/salamandre.png"));
    this->setWindowTitle(APPS_NAME);

    this->connect(this->thread, SIGNAL(connectionSuccess()), this, SLOT(connectionIsSuccess()));
    this->connect(this->thread, SIGNAL(connectionFailed(int)), this, SLOT(connectionIsFailed(int)));
    this->connect(this->thread, SIGNAL(deconnectionSuccess()), this, SLOT(deconnectionIsSuccess()));
    this->connect(this->thread, SIGNAL(deconnectionFailed()), this, SLOT(deconnectionIsFailed()));
    this->connect(this->thread, SIGNAL(askFileDaemon()), this, SLOT(getFileDaemon()));
    this->connect(&(sockSender::sock), SIGNAL(reconnect(int,int)), this, SLOT(setConnectionText(int,int)));
}

connectionDeconnectionDialog::~connectionDeconnectionDialog()
{
    delete this->thread;
    delete this->movie;
    delete ui;
}

void connectionDeconnectionDialog::start(int srvGuiPort)
{
    this->enableBottom(true);
    this->ui->label->setText("Connexion ...");
    this->srvGuiPort = srvGuiPort;
    this->thread->startApps(srvGuiPort);
    this->thread->start(QThread::HighPriority);
}

void connectionDeconnectionDialog::stop()
{
    this->enableBottom(false);
    this->ui->label->setText("Déconnexion ...");
    this->thread->stopApps();
    this->thread->start(QThread::HighPriority);
}

void connectionDeconnectionDialog::closeEvent(QCloseEvent *event)
{
    this->thread->quit();
    event->accept();
}

void connectionDeconnectionDialog::connectionIsSuccess()
{
    this->accept();
}

void connectionDeconnectionDialog::connectionIsFailed(int res)
{
    if(res != -5)
        this->reject();
}

void connectionDeconnectionDialog::getFileDaemon()
{
    QMessageBox::warning(this, "Serveur introuvable", "Impossible de localiser le serveur de mise à jour des fiches patients, \nmerci d'indiquer le chemin de l'exécutable dans la fenêtre suivante.");
    QString file = QFileDialog::getOpenFileName(this, "Choix des fichiers", QDir::homePath(), "salamandre-daemon");

    if(file != ""){
        sockSender::setDaemonBinPath(file.toStdString());

        settings::setDaemonSettingValue("pathBin", file);
        settings::saveSettings();

        this->start(this->srvGuiPort);
    }
    else
        this->close();
}

void connectionDeconnectionDialog::deconnectionIsSuccess()
{
    this->accept();
}

void connectionDeconnectionDialog::deconnectionIsFailed()
{
    this->reject();
}

void connectionDeconnectionDialog::setConnectionText(int nbConnection, int maxNbConnection)
{
    if(nbConnection < 5)
        this->enableBottom(false);
    else
        this->enableBottom(true);

    this->ui->label->setText("Connexion (tentative "+QString::number(nbConnection)+" / "+QString::number(maxNbConnection)+") ...");
}

void connectionDeconnectionDialog::enableBottom(bool enable)
{
    this->ui->label_infoPixMap->setVisible(enable);
    this->ui->label_infoText->setVisible(enable);
}

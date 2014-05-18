#include <forms/connectiondeconnectiondialog.hpp>
#include <ui_connectiondeconnectiondialog.h>

#include <objects/socksender.hpp>
#include <objects/settings.hpp>

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
    this->connect(this->thread, SIGNAL(connectionFailed()), this, SLOT(connectionIsFailed()));
    this->connect(this->thread, SIGNAL(deconnectionSuccess()), this, SLOT(deconnectionIsSuccess()));
    this->connect(this->thread, SIGNAL(deconnectionFailed()), this, SLOT(deconnectionIsFailed()));
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

void connectionDeconnectionDialog::connectionIsFailed()
{
    this->reject();
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

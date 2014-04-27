#include <objects/listview.hpp>

#include <QDebug>
#include <QMimeData>
#include <QFileInfo>
#include <QFileDialog>
#include <QDesktopServices>

ListView::ListView(QString sourceFMN, QWidget *parent) :
    QListView(parent)
{
    this->setAcceptDrops(true);
    this->setEditTriggers(EditTrigger::NoEditTriggers);

    this->sourceDirFMN = sourceFMN;
    this->needToSave = false;

    this->threadToRead = new threadExport(this->sourceDirFMN, nullptr);
    this->threadToUpload = new threadExport(this->sourceDirFMN, nullptr);

    this->modelListFile = new QStandardItemModel();

    this->contextMenu = new QMenu();
    this->actionOpen = new QAction(QPixmap(":/icons/controls/open.png"),"Ouvrir le fichier", nullptr);
    this->actionUpload = new QAction(QPixmap(":/icons/controls/upload.png"),"Télécharger le fichier", nullptr);

    this->connect(this->actionOpen, SIGNAL(triggered()), this, SLOT(startReading()));
    this->connect(this->actionUpload, SIGNAL(triggered()), this, SLOT(startUpload()));

    this->contextMenu->addAction(this->actionOpen);
    this->contextMenu->addAction(this->actionUpload);

    this->setContextMenuPolicy(Qt::CustomContextMenu);
    this->setSelectionMode(QAbstractItemView::SingleSelection);
    this->setModel(this->modelListFile);

    this->connect(this, SIGNAL(customContextMenuRequested(QPoint)), this, SLOT(showContextMenu(const QPoint)));
    this->connect(this->threadToRead, SIGNAL(fileExtracted(QString)), SLOT(readExtractFile(QString)));;
}

void ListView::dropEvent(QDropEvent *event)
{
    const QMimeData* mimeData = event->mimeData();
    QStringList currentDragFiles;

    if(mimeData->hasUrls())
    {
        this->setStyleSheet("QListView{background-color: rgba(207, 255, 255, 120);border: 2px inset rgb(220, 220, 220);}");
        QList<QUrl> urlList = mimeData->urls();

        for(int i = 0; i < urlList.size(); ++i){
            QString file = urlList.at(i).toLocalFile();

            if(QFileInfo(file).isFile()){
                this->needToSave = true;
                currentDragFiles << file;
            }
        }

        event->acceptProposedAction();
    }

    this->resetStyle();
    emit this->dropFile(currentDragFiles);
}

void ListView::dragEnterEvent(QDragEnterEvent *event)
{
    this->setStyleSheet("QListView{background-color: rgba(207, 255, 255, 120);border: 2px inset rgb(220, 220, 220);}");
    event->acceptProposedAction();
}

void ListView::dragMoveEvent(QDragMoveEvent *event)
{
    event->acceptProposedAction();
}

void ListView::dragLeaveEvent(QDragLeaveEvent *event)
{
    this->resetStyle();
    event->accept();
}

void ListView::resetStyle()
{
    this->setStyleSheet("");
}

void ListView::showContextMenu(QPoint p)
{
    this->contextMenu->exec(this->mapToGlobal(p));
}

void ListView::startReading()
{
    QModelIndexList listIndex = this->selectedIndexes();

    int nbFileChoosen = listIndex.size();

    if(nbFileChoosen > 0){
        QModelIndex index = listIndex.at(0); // just one index for reading available to avoid read a lot a file in single time
        if(index.isValid()){
            salamandre::DigitalContent *digitFile = this->modelListFile->item(index.row(), 0)->data().value<salamandre::DigitalContent*>();
            digitFile->filePathExport = this->sourceDirFMN.toStdString().append("/tmp/");
            this->threadToRead->addFileToExport(digitFile);
        }
    }
}

void ListView::startUpload()
{
    QString dir = QFileDialog::getExistingDirectory(this, tr("Choisissez le répertoire de destination"), QDir::homePath(), QFileDialog::ShowDirsOnly | QFileDialog::DontResolveSymlinks);

    QModelIndexList listIndex = this->selectedIndexes();

    int nbFileChoosen = listIndex.size();

    if(nbFileChoosen > 0){

        for(int i = 0; i < nbFileChoosen; ++i){
            QModelIndex index = listIndex.at(i); // just one index because list is in one selected line
            if(index.isValid()){
                salamandre::DigitalContent *digitFile = this->modelListFile->item(index.row(), 0)->data().value<salamandre::DigitalContent*>();
                digitFile->filePathExport = (dir+"/").toStdString();
                this->threadToUpload->addFileToExport(digitFile);
            }
        }
    }
}

void ListView::readExtractFile(QString filePath)
{
    QDesktopServices::openUrl(QUrl(filePath));
}

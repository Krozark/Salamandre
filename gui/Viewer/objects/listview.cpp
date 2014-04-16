#include <objects/listview.hpp>

#include <QDebug>
#include <QMimeData>
#include <QFileInfo>

ListView::ListView(QWidget *parent) :
    QListView(parent)
{
    this->setAcceptDrops(true);
}

void ListView::dropEvent(QDropEvent *event)
{
    const QMimeData* mimeData = event->mimeData();
    QStringList currentDragFiles;

    if (mimeData->hasUrls())
    {
        this->setStyleSheet("QListView{background-color: rgba(207, 255, 255, 120);border: 2px inset rgb(220, 220, 220);}");
        QList<QUrl> urlList = mimeData->urls();

        for (int i = 0; i < urlList.size() && i < 32; ++i)
        {
            QString file = urlList.at(i).toLocalFile();

            if(QFileInfo(file).isFile())
                currentDragFiles << file;
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

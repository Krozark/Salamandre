#include <objects/listview.hpp>

#include <QDebug>

ListView::ListView(QWidget *parent) :
    QListView(parent)
{
    this->setAcceptDrops(true);
}

void ListView::dropEvent(QDropEvent *e)
{
    Q_UNUSED(e);
    qDebug() << "drop";
}

void ListView::dragEnterEvent(QDragEnterEvent *event)
{
    Q_UNUSED(event)
    qDebug() << "drag";
}

#include "graphicsitemcomputer.hpp"

#include <QDebug>
#include <QCursor>


graphicsItemComputer::graphicsItemComputer()
{
    this->init();
}

graphicsItemComputer::~graphicsItemComputer()
{
    delete(this->actionBoot);
    delete(this->actionReboot);
    delete(this->actionStop);

    delete(this->menu);
}

void graphicsItemComputer::init()
{
    this->setCacheMode(QGraphicsItem::ItemCoordinateCache);
    this->setFlags(QGraphicsItem::ItemIsMovable | QGraphicsItem::ItemIsSelectable);
    this->setCursor(Qt::OpenHandCursor);

    this->links = new QVector<QPair<graphicsItemLink*, bool> >();

    this->initContextMenu();

    this->isLeftButtonPressed = false;
    this->isRightButtonPressed = false;

    this->pixmap = QPixmap("://gui/icons/graphics/monitor.png");

    this->updateGraphicsComponent();
}

void graphicsItemComputer::initContextMenu()
{
    this->menu = new QMenu();

    this->actionBoot = new QAction(QPixmap("://gui/icons/graphics/stop.png"), "Arreter", this->menu);
    this->actionReboot = new QAction(QPixmap("://gui/icons/graphics/reboot.png"), "Redemarrer", this->menu);
    this->actionStop = new QAction(QPixmap("://gui/icons/graphics/boot.png"), "Demarrer", this->menu);

    this->menu->addAction(this->actionBoot);
    this->menu->addAction(this->actionReboot);
    this->menu->addAction(this->actionStop);
}

void graphicsItemComputer::setComputerRect(qreal x, qreal y, qreal w, qreal h)
{
    this->setComputerRect(QRectF(x, y, w, h));
}

void graphicsItemComputer::setComputerRect(QRectF rect)
{
    this->rectF = rect;
    this->setRect(this->rectF);
    this->updateGraphicsComponent();
}

QRectF graphicsItemComputer::boundingRect()
{
    return this->rectF;
}

void graphicsItemComputer::updateGraphicsComponent()
{
    QPixmap pixmap = this->pixmap.scaledToHeight(this->rect().height());

    this->brush.setTexture(pixmap);
    this->pen.setBrush(this->brush);
    this->pen.setColor(QColor(255,255,255,0));

    this->setPen(this->pen);
}

void graphicsItemComputer::setNumber(int number){
    this->number = number;
}

int graphicsItemComputer::getNumber()
{
    return this->number;
}

void graphicsItemComputer::setName(QString name){
    this->name = name;
}

QString graphicsItemComputer::getName(){
    return this->name;
}

void graphicsItemComputer::setIp(QString ip){
    this->ip = ip;
}

QString graphicsItemComputer::getIp(){
    return this->ip;
}

void graphicsItemComputer::setPort(int port){
    this->port = port;
}

int graphicsItemComputer::getPort(){
    return this->port;
}

void graphicsItemComputer::setLinks(QVector<QPair<graphicsItemLink *, bool> > *links)
{
    this->links = links;
}

void graphicsItemComputer::addLink(graphicsItemLink *link, bool isSource)
{
    this->links->push_back(qMakePair(link, isSource));
}

QVector<QPair<graphicsItemLink*, bool> >* graphicsItemComputer::getLinks()
{
    return this->links;
}

void graphicsItemComputer::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    Qt::MouseButtons buttonPressed = event->button();
    this->setCursor(Qt::ClosedHandCursor);

    if(buttonPressed == Qt::LeftButton){
        qDebug() << "[graphicsComputer] Left click";
        if(this->isRightButtonPressed){
            qDebug() << "[graphicsComputer] Cancel click";
            this->isRightButtonPressed = false;
            this->isLeftButtonPressed = false;
        }
        else{
            this->isLeftButtonPressed = true;
        }
    }
    else if(buttonPressed == Qt::RightButton){
        qDebug() << "[graphicsComputer] Right click";
        if(this->isLeftButtonPressed){
            qDebug() << "[graphicsComputer] Cancel click";
            this->isRightButtonPressed = false;
            this->isLeftButtonPressed = false;
        }
        else{
            this->isRightButtonPressed = true;
        }
    }
}

void graphicsItemComputer::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
    Qt::MouseButtons buttonReleased = event->button();
    this->setCursor(Qt::OpenHandCursor);

    if(buttonReleased == Qt::LeftButton){
        this->isLeftButtonPressed = false;
    }
    else if(buttonReleased == Qt::RightButton){
        this->isRightButtonPressed = false;
    }
}

void graphicsItemComputer::mouseMoveEvent(QGraphicsSceneMouseEvent *event)
{
    if(this->isLeftButtonPressed){
        int posX = event->scenePos().x() - this->rect().width()/2;
        int posY = event->scenePos().y() - this->rect().height()/2;
        this->setPos(QPoint(posX, posY));

        int nbLink = this->links->size();

        for(int i = 0; i < nbLink; ++i){
            this->links->at(i).first->refreshPosition(this->links->at(i).second, this->pos());
        }
    }
}

void graphicsItemComputer::contextMenuEvent(QGraphicsSceneContextMenuEvent *event)
{
    qDebug() << "[graphicsComputer] Popup context menu";

    this->menu->popup(event->screenPos());
}

void graphicsItemComputer::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    Q_UNUSED(option);
    Q_UNUSED(widget);

    painter->setPen(this->pen);
    painter->drawRect(this->rect());
    painter->fillRect(this->rect(), this->brush);
}

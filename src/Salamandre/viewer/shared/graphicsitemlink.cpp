#include <Salamandre/viewer/shared/graphicsitemlink.hpp>

#include <QDebug>
#include <QStyleOptionGraphicsItem>

graphicsItemLink::graphicsItemLink()
{
    this->init();
}

void graphicsItemLink::init()
{
    this->setCacheMode(QGraphicsItem::ItemCoordinateCache);

    this->pen.setColor(QColor(100,100,100,255));
    this->pen.setBrush(QBrush(Qt::SolidPattern));
    this->pen.setStyle(Qt::SolidLine);
    this->setPen(this->pen);
    this->setLine(QLineF(0,0,0,0));
}

void graphicsItemLink::refreshPosition(bool isSource, QPointF pos)
{
    if(isSource)
        this->setLine(QLineF(pos, this->line().p2()));
    else
        this->setLine(QLineF(this->line().p1(), pos));
}

void graphicsItemLink::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    Q_UNUSED(event);
}

void graphicsItemLink::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    Q_UNUSED(option);
    Q_UNUSED(widget);

    painter->setBrush(this->pen.brush());
    painter->setPen(this->pen);
    painter->drawLine(this->line());
}

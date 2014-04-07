#ifndef GRAPHICSITEMLINK_HPP
#define GRAPHICSITEMLINK_HPP

//#include "graphicsitemcomputer.hpp"

#include <QGraphicsLineItem>
#include <QGraphicsSceneMouseEvent>
#include <QPainter>

class graphicsItemLink : public QGraphicsLineItem
{
public:
    graphicsItemLink();

    void init();
    void refreshPosition(bool isSource, QPointF pos);

    void mousePressEvent(QGraphicsSceneMouseEvent *event);
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);

private:
    QPen pen;
signals:

public slots:

};

#endif // GRAPHICSITEMLINK_HPP

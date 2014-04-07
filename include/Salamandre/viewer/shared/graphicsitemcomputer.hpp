#ifndef GRAPHICSITEMCOMPUTER_HPP
#define GRAPHICSITEMCOMPUTER_HPP

#include <Salamandre/viewer/shared/graphicsitemlink.hpp>

#include <QGraphicsRectItem>
#include <QGraphicsSceneMouseEvent>
#include <QPainter>
#include <QMenu>

class graphicsItemComputer : public QGraphicsRectItem
{
public:
    graphicsItemComputer();
    ~graphicsItemComputer();

    void init();
    void initContextMenu();
    void updateGraphicsComponent();
    void setComputerRect(qreal x, qreal y, qreal w, qreal h);
    void setComputerRect(QRectF rect);

    void setNumber(int number);
    int getNumber();

    void setName(QString name);
    QString getName();

    void setIp(QString ip);
    QString getIp();

    void setPort(int port);
    int getPort();

    void addLink(graphicsItemLink *link, bool isSource);
    void setLinks(QVector<QPair<graphicsItemLink *, bool> > *links);
    QVector<QPair<graphicsItemLink *, bool> > *getLinks();

    QRectF boundingRect();
    void contextMenuEvent(QGraphicsSceneContextMenuEvent *event);
    void mousePressEvent(QGraphicsSceneMouseEvent *event);
    void mouseMoveEvent(QGraphicsSceneMouseEvent *event);
    void mouseReleaseEvent(QGraphicsSceneMouseEvent *event);
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);


    QVector<QPair<graphicsItemLink*, bool> > *links;

private:
    bool isLeftButtonPressed, isRightButtonPressed;
    QPixmap pixmap;
    QPen pen;
    QBrush brush;

    QMenu *menu;
    QAction *actionBoot, *actionReboot, *actionStop;
    QRectF rectF;

    QString name;
    QString ip;
    int port;
    int number;
signals:

public slots:

};

#endif // GRAPHICSITEMCOMPUTER_HPP

#ifndef GRAPHICSSCENE_HPP
#define GRAPHICSSCENE_HPP

#include <Salamandre/viewer/shared/graphicsitemcomputer.hpp>
#include <Salamandre/viewer/shared/graphicsitemlink.hpp>

#include <QGraphicsScene>
#include <QGraphicsSceneEvent>
#include <QPair>

class graphicsScene : public QGraphicsScene
{
    Q_OBJECT
public:
    explicit graphicsScene(QObject *parent = 0);

    QList<QPair<graphicsItemComputer*, graphicsItemComputer*>> listOfComputerPair;

    QList<QPair<graphicsItemComputer *, graphicsItemComputer *> > getComputerPairList(QString fileName);
    bool checkIpStructure(QString ip);
    graphicsItemComputer* getComputerInList(int num, QList<QPair<graphicsItemComputer *, graphicsItemComputer *> > list);

private:
    //void mouseMoveEvent(QGraphicsSceneMouseEvent *event);
signals:

public slots:

};

#endif // GRAPHICSSCENE_HPP

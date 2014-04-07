#include "graphicsscene.hpp"

#include <QDebug>
#include <QGraphicsRectItem>
#include <QFile>
#include <QCoreApplication>

graphicsScene::graphicsScene(QObject *parent) :
    QGraphicsScene(parent)
{
    this->setSceneRect(QRectF(0,0,800,600));

    this->listOfComputerPair = this->getComputerPairList(QCoreApplication::applicationDirPath()+"/networkConfig/networkConfiguration.txt");
    int nbPair = this->listOfComputerPair.size();

    QVector<graphicsItemComputer*> *vComputerItem = new QVector<graphicsItemComputer*>();
    QVector<graphicsItemLink*> *vLinkItem = new QVector<graphicsItemLink*>();

    graphicsItemComputer *first, *second;
    graphicsItemLink *link;
    bool firstIn, secondIn;

    for(int i = 0; i < nbPair; ++i){
        int nbComputer = vComputerItem->size();

        first = this->listOfComputerPair.at(i).first;
        second = this->listOfComputerPair.at(i).second;

        firstIn = false;
        secondIn = false;

        for(int j = 0; j < nbComputer; ++j){
            if(vComputerItem->at(j) == first){
                firstIn = true;
            }
            if(vComputerItem->at(j) == second){
                secondIn = true;
            }
            if(firstIn && secondIn)
                break;
        }

        if(!firstIn)
            vComputerItem->push_back(first);
        if(!secondIn)
            vComputerItem->push_back(second);

        link = new graphicsItemLink();
        first->addLink(link, true);
        second->addLink(link, false);
        vLinkItem->push_back(link);
    }

    int nbComputer = vComputerItem->size();
    int nbLink = vLinkItem->size();

    for(int i = 0; i < nbComputer; ++i){
        vComputerItem->at(i)->setComputerRect(0,0,64,64);
        this->addItem(vComputerItem->at(i));
    }

    for(int i = 0; i < nbLink; ++i){
        this->addItem(vLinkItem->at(i));
    }
}

QList<QPair<graphicsItemComputer *, graphicsItemComputer *> > graphicsScene::getComputerPairList(QString fileName)
{
    QFile *file = new QFile(fileName);
    QList<QPair<graphicsItemComputer*,graphicsItemComputer*> > listOfPair = QList<QPair<graphicsItemComputer*,graphicsItemComputer*> >();

    if(!file->exists()){
        qWarning() << "the file \"" << fileName << "\" doesn't exists !";
        return QList<QPair<graphicsItemComputer*,graphicsItemComputer*> >();
    }

    if(!file->open(QIODevice::ReadOnly)){
        qWarning() << "Unable to open file : " << fileName;
        return QList<QPair<graphicsItemComputer*,graphicsItemComputer*> >();
    }

    QString line;
    QStringList computerInfos;

    int nbLine = 0;

    graphicsItemComputer *computer, *computerNeighbor;

    while(!file->atEnd()){
        ++nbLine;
        line = QString(file->readLine());
        qDebug() << QString(line);

        if(line.startsWith('#')) // let add comment in file
            continue;

        computerInfos = line.split(" ");

        if(computerInfos.size() != 5){
            qWarning() << "The structure of line" << nbLine << "is not correct. It must be [pc number] [ip adress] [port] [name] [relations {separated by coma \',\'}]\nConsequently, line hasn't been considered.";
            continue;
        }

        if(!this->checkIpStructure(computerInfos.at(1))){
            qWarning() << "The structure of line" << nbLine << "is not correct. The ip adress must be at IPv4 or IPv6 format.\nConsequently, line hasn't been considered.";
            continue;
        }

        int numComputer = computerInfos.at(0).toInt();

        computer = this->getComputerInList(numComputer, listOfPair);

        if(computer == nullptr){
            computer = new graphicsItemComputer();
        }

        computer->setNumber(numComputer);
        computer->setIp(computerInfos.at(1));
        computer->setPort(computerInfos.at(2).toInt());
        computer->setName(computerInfos.at(3));

        QStringList neighbors = computerInfos.at(4).split(",");
        int nbRelation = neighbors.size();

        for(int i = 0; i < nbRelation; ++i){
            int numNeighbor = neighbors.at(i).toInt();
            computerNeighbor = this->getComputerInList(numNeighbor, listOfPair);

            if(computerNeighbor == nullptr){
                computerNeighbor = new graphicsItemComputer();
                computerNeighbor->setNumber(numNeighbor);
            }

            listOfPair.push_back(qMakePair(computer, computerNeighbor));
        }
    }

    return listOfPair;
}

graphicsItemComputer* graphicsScene::getComputerInList(int num, QList<QPair<graphicsItemComputer *, graphicsItemComputer *> > list)
{
    graphicsItemComputer* computer = nullptr;
    int nbPair = list.size();

    for(int i = 0; i < nbPair; ++i){
        if(list.at(i).first->getNumber() == num){
            computer = list.at(i).first;
            break;
        }
        else if(list.at(i).second->getNumber() == num){
            computer = list.at(i).second;
            break;
        }
    }

    return computer;
}

bool graphicsScene::checkIpStructure(QString ip)
{
    QRegExp regExpIpv4 = QRegExp("([0-9]{1,3}\\.){3}[0-9]{1,3}");
    QRegExp regExpIpv6 = QRegExp("([0-9a-fA-F]{1,4}\\:){7}[0-9a-fA-F]{1,4}");

    if(regExpIpv4.exactMatch(ip)){
        return true;
    }
    else{
        // try to check ipv6.
        int nbDoubleColon = ip.count("::");
        if(nbDoubleColon > 1){
            return false;
        }
        else if(nbDoubleColon == 1){
            int nbColon = ip.count(":") - (ip.startsWith("::")?1:0) - (ip.endsWith("::")?1:0);
            int leakColon = 8 - nbColon;
            QStringList zeroBlock;

            for(int i = 0; i < leakColon; ++i){
                zeroBlock << "0000";
            }

            QString ipv6 = ip.replace("::", (ip.startsWith("::")?"":":")+zeroBlock.join(":")+(ip.endsWith("::")?"":":"));
            qDebug() << ipv6;
            return regExpIpv6.exactMatch(ipv6);
        }
        else{
            return regExpIpv6.exactMatch(ip);
        }
    }

    return false;
}

/*
void graphicsScene::mouseMoveEvent(QGraphicsSceneMouseEvent *event)
{
    qDebug() << "x : " << event->scenePos().x() << " y : " << event->scenePos().y();
}
*/


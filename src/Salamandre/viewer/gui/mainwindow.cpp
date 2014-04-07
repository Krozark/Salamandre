/**
  * \file       mainwindow.cpp
  * \author     Samuel MARPAUX.
  * \version    0.1
  * \date       04 Avril 2014
  *
  * \brief      Classe de la fenêtre principale
  */

#include <Salamandre/viewer/gui/mainwindow.hpp>

#include "ui_mainwindow.h"

#include <QPixmap>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    this->initGui();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::initGui()
{
    this->initGraphicsView();

    this->setWindowTitle(APPLICATION_NAME+" "+APPLICATION_VERSION);
    this->setWindowIcon(QPixmap(":/gui/icons/salamandre.png"));
}

void MainWindow::initGraphicsView()
{
    this->scene = new graphicsScene(nullptr);

    this->ui->graphicsView->setMouseTracking(true);
    this->ui->graphicsView->setScene(this->scene);
    this->ui->graphicsView->setOptimizationFlags(QGraphicsView::DontClipPainter | QGraphicsView::DontSavePainterState | QGraphicsView::DontAdjustForAntialiasing | QGraphicsView::IndirectPainting);
    this->ui->graphicsView->setViewportUpdateMode(QGraphicsView::SmartViewportUpdate);

    this->ui->graphicsView->setMinimumSize(800, 600);
    //this->ui->graphicsView->setMaximumSize(800, 600);

    this->ui->graphicsView->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    this->ui->graphicsView->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
}

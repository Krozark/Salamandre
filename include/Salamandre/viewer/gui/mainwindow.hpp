#ifndef MAINWINDOW_HPP
#define MAINWINDOW_HPP

#include <Salamandre/viewer/shared/graphicsscene.hpp>

#include <QMainWindow>

#define APPLICATION_NAME QString("Salamandre")
#define APPLICATION_VERSION QString("0.1")

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

    /**
     * \brief Permet d'initialiser les divers éléments graphiques de la GUI.
     */
    void initGui();

    /**
     * \brief Permet d'initialiser le graphicsView.
     */
    void initGraphicsView();

private:
    Ui::MainWindow *ui;

    graphicsScene *scene;
};

#endif // MAINWINDOW_HPP

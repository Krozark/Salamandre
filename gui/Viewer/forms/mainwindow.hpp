#ifndef MAINWINDOW_HPP
#define MAINWINDOW_HPP

#include <QMainWindow>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

    void init();
private:
    Ui::MainWindow *ui;

    QString currentUserId;
    QString currentUserPass;
};

#endif // MAINWINDOW_HPP

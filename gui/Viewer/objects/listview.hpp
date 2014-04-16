#ifndef LISTVIEW_HPP
#define LISTVIEW_HPP

#include <QListView>
#include <QDragEnterEvent>
#include <QDragLeaveEvent>
#include <QDragMoveEvent>
#include <QMouseEvent>

class ListView : public QListView
{
    Q_OBJECT
public:
    explicit ListView(QWidget *parent = 0);

    void dropEvent(QDropEvent *event);
    void dragEnterEvent(QDragEnterEvent *event);
    void dragMoveEvent(QDragMoveEvent *event);
    void dragLeaveEvent(QDragLeaveEvent *event);

    void resetStyle();

private:

signals:
    void dropFile(QStringList);
public slots:
    
};

#endif // LISTVIEW_HPP

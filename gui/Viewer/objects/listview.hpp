#ifndef LISTVIEW_HPP
#define LISTVIEW_HPP

#include <QListView>

class ListView : public QListView
{
    Q_OBJECT
public:
    explicit ListView(QWidget *parent = 0);

    void dropEvent(QDropEvent *e);
    void dragEnterEvent(QDragEnterEvent *event);
    
signals:
    
public slots:
    
};

#endif // LISTVIEW_HPP

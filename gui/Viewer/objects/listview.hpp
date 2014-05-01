#ifndef LISTVIEW_HPP
#define LISTVIEW_HPP

#include <QListView>
#include <QDragEnterEvent>
#include <QDragLeaveEvent>
#include <QDragMoveEvent>
#include <QMouseEvent>
#include <QMenu>
#include <QStandardItemModel>

#include <record/DigitalRecord.hpp>
#include <objects/threadexport.hpp>

Q_DECLARE_METATYPE(salamandre::DigitalContent*)

class ListView : public QListView
{
    Q_OBJECT
public:
    explicit ListView(QString sourceDirFMN, QWidget *parent = 0);
    ~ListView();

    void dropEvent(QDropEvent *event);
    void dragEnterEvent(QDragEnterEvent *event);
    void dragMoveEvent(QDragMoveEvent *event);
    void dragLeaveEvent(QDragLeaveEvent *event);
    void mouseDoubleClickEvent(QMouseEvent *event);

    void resetStyle();

    QStandardItemModel *modelListFile;

    bool needToSave;

private:
    QMenu *contextMenu;
    QAction *actionUpload;
    QAction *actionOpen;

    QString sourceDirFMN;

    threadExport *threadToUpload;
    threadExport *threadToRead;

signals:
    void dropFile(QStringList);
public slots:
    void startUpload();
    void startReading();

private slots:
    void showContextMenu(QPoint p);
    void readExtractFile(QString filePath);
    
};

#endif // LISTVIEW_HPP

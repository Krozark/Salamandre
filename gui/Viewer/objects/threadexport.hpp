#ifndef THREADEXPORT_HPP
#define THREADEXPORT_HPP

#include <record/DigitalRecord.hpp>

#include <QThread>
#include <QVector>

class threadExport : public QThread
{
    Q_OBJECT
public:
    explicit threadExport(QString source, QObject *parent = 0);

    void addFileToExport(QVector<salamandre::DigitalContent*> listFile);
    void addFileToExport(salamandre::DigitalContent *file);

private:
    void run();

    QString source;
    QVector<salamandre::DigitalContent*> exportFileList;

private slots:

signals:
    void fileExtracted(QString destFile);
    void decryptError();

public slots:
};

#endif // THREADEXPORT_HPP

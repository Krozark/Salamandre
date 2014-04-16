#ifndef THREADUPLOADFILE_HPP
#define THREADUPLOADFILE_HPP

#include <QThread>
#include <QStringList>

class threadUploadFile : public QThread
{
    Q_OBJECT
public:
    explicit threadUploadFile(QString destination, QObject *parent = 0);

    void addFileToUpload(QStringList listFile);
    void addFileToUpload(QString file);

private:
    void run();

    QStringList uploadFileList;
    QString destination;

private slots:
    void start(Priority);
    
signals:
    
public slots:
    
};

#endif // THREADUPLOADFILE_HPP

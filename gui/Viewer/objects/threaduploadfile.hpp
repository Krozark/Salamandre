#ifndef THREADUPLOADFILE_HPP
#define THREADUPLOADFILE_HPP

#include <QThread>
#include <QStringList>
#include <QVector>

#include <objects/patient.hpp>
#include <objects/doctor.hpp>

class threadUploadFile : public QThread
{
    Q_OBJECT
public:
    explicit threadUploadFile(salamandre::Patient *patient, salamandre::Doctor *doctor, QObject *parent = 0);

    void addFileToUpload(QVector<salamandre::DigitalContent*> listFile);
    void addFileToUpload(salamandre::DigitalContent *file);

private:
    QVector<salamandre::DigitalContent*> uploadFileList;
    QString destinationTmp;
    QString destinationFmn;

    salamandre::Patient *patient;
    salamandre::Doctor *doctor;

    int totalInsertedFile;
    int totalProcessFile;
    int totalProgress;

    void run();
private slots:

    
signals:
    void newFileInserted();
    void fileInserted(int);
    void fileProcess(int);
    void uploadProgression(int);
    void setProgressText(QString text);
    void encryptError();
public slots:
    
};

#endif // THREADUPLOADFILE_HPP

#ifndef THREADSAVE_HPP
#define THREADSAVE_HPP

#include <objects/doctor.hpp>
#include <objects/patient.hpp>

#include <QThread>

#define RECORD_TO_SAVE 4

class threadSave : public QThread
{
    Q_OBJECT
public:
    explicit threadSave(salamandre::Patient *patient, salamandre::Doctor *doctor, QObject *parent = 0);

    enum saveProgress{
        SAVE_OF_FEC = 0,
        SAVE_OF_FCT = 1,
        SAVE_OF_FMT = 2,
        SAVE_OF_FMN = 3
    };

    bool saveFEC;
    bool saveFCT;
    bool saveFMT;
    bool saveFMN;

    void run();
private:
    salamandre::Doctor *doctor;
    salamandre::Patient *patient;

signals:
    void setProgressText(QString);
    void setProgress(int);
    void saveProgress(int);

public slots:

};

#endif // THREADSAVE_HPP

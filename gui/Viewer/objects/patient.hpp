#ifndef PATIENT_HPP
#define PATIENT_HPP

#include <QMetaType>
#include <QString>

class Patient
{
public:
    Patient();

    void setDirPath(QString dirPath);
    void setId(QString id);

    QString getDirPath();
    QString getId();

private:
    QString id;
    QString dirPath;
};

Q_DECLARE_METATYPE(Patient*)

#endif // PATIENT_HPP

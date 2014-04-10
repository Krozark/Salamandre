#ifndef DOCTOR_HPP
#define DOCTOR_HPP

#include <QString>

class Doctor
{
public:

    enum TypeDoctor{
        NEW_DOCTOR = 0,
        DOCTOR_ALREADY_EXIST = 1,
        DOCTOR_ALREADY_EXIST_BUT_NOTHING = 2
    };


    void setId(QString getId);
    void setPass(QString getPass);
    void setDirPath(QString getDirPath);
    void setType(TypeDoctor getType);

    QString getId();
    QString getPass();
    QString getDirPath();
    TypeDoctor getType();

    Doctor();
    ~Doctor();

private:
    QString id;
    QString pass;
    QString dirPath;
    TypeDoctor type;
};

#endif // DOCTOR_HPP

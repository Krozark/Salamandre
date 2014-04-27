#ifndef PATIENT_HPP
#define PATIENT_HPP

#include <QString>

#include <record/ConfidentialRecord.hpp>
#include <record/DigitalRecord.hpp>
#include <record/MedicalRecord.hpp>
#include <record/RegistryRecord.hpp>

namespace salamandre {
    class Patient
    {
    public:

        enum TypePatient{
            NEW_PATIENT = 0,
            PATIENT_ALREADY_EXIST
        };

        Patient();
        ~Patient();

        void setDirPath(QString dirPath);
        void setId(QString id);
        void setType(TypePatient type);

        void setConfidentialRecord(ConfidentialRecord *confidentialRecord);
        void setDigitalRecord(DigitalRecord *digitalRecord);
        void setMedicalRecord(MedicalRecord *medicalRecord);
        void setRegistryRecord(RegistryRecord *registryRecord);

        QString getDirPath();
        QString getId();
        TypePatient getType();

        ConfidentialRecord *getConfidentialRecord();
        DigitalRecord *getDigitalRecord();
        MedicalRecord *getMedicalRecord();
        RegistryRecord *getRegistryRecord();

    private:
        QString id;
        QString dirPath;
        TypePatient type;

        ConfidentialRecord *confidentialRecord;
        DigitalRecord *digitalRecord;
        MedicalRecord *medicalRecord;
        RegistryRecord *registryRecord;
    };
}

#endif // PATIENT_HPP

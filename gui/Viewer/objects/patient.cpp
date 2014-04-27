#include "patient.hpp"

namespace salamandre {
    Patient::Patient()
    {
    }

    Patient::~Patient()
    {
        delete this->confidentialRecord;
        delete this->digitalRecord;
        delete this->medicalRecord;
        delete this->registryRecord;
    }

    QString Patient::getId()
    {
        return this->id;
    }

    void Patient::setId(QString id)
    {
        this->id = id;
    }

    QString Patient::getDirPath()
    {
        return this->dirPath;
    }

    void Patient::setDirPath(QString dirPath)
    {
        this->dirPath = dirPath;
    }

    Patient::TypePatient Patient::getType()
    {
        return this->type;
    }

    void Patient::setType(TypePatient type)
    {
        this->type = type;
    }

    void Patient::setConfidentialRecord(ConfidentialRecord *confidentialRecord)
    {
        this->confidentialRecord = confidentialRecord;
    }

    ConfidentialRecord* Patient::getConfidentialRecord()
    {
        return this->confidentialRecord;
    }

    void Patient::setDigitalRecord(DigitalRecord *digitalRecord)
    {
        this->digitalRecord = digitalRecord;
    }

    DigitalRecord* Patient::getDigitalRecord()
    {
        return this->digitalRecord;
    }

    void Patient::setMedicalRecord(MedicalRecord *medicalRecord)
    {
        this->medicalRecord = medicalRecord;
    }

    MedicalRecord* Patient::getMedicalRecord()
    {
        return this->medicalRecord;
    }

    void Patient::setRegistryRecord(RegistryRecord *registryRecord)
    {
        this->registryRecord = registryRecord;
    }

    RegistryRecord* Patient::getRegistryRecord()
    {
        return this->registryRecord;
    }
}

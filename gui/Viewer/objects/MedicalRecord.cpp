#include <objects/MedicalRecord.hpp>

namespace salamandre
{
    const std::string MedicalRecord::fileName = "FMT";
    
    MedicalRecord::MedicalRecord(const std::string path)
    {
        this->setFilePath(path);
    }

    std::string MedicalRecord::getFilePath()const
    {
        return this->filePath;
    }

    void MedicalRecord::setFilePath(std::string filePath)
    {
        this->filePath = filePath;
    }

    std::string MedicalRecord::serialize()
    {
        return std::string();
    }

    void MedicalRecord::unSerialize(std::string string)
    {
        (void) string;
    }

    void MedicalRecord::save(std::string key)
    {
        (void) key;
    }

    void MedicalRecord::load(std::string key)
    {
        (void) key;
    }
}

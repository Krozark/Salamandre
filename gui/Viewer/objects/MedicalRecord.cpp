#include <objects/MedicalRecord.hpp>

namespace salamandre
{
    const std::string MedicalRecord::fileName = "FMT";
    
    MedicalRecord::MedicalRecord(const std::string path)
    {
        this->setFilePath(path);
    }

    std::string MedicalRecord::serialize()
    {
        return std::string();
    }

    void MedicalRecord::unSerialize(std::string string)
    {
        (void) string;
    }
}

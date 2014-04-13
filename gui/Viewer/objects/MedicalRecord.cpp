#include <objects/MedicalRecord.hpp>

namespace salamandre
{
    const std::string MedicalRecord::fileName = "FMT";
    
    MedicalRecord::MedicalRecord(const unsigned int pat_id) : Record(pat_id)
    {
    }

    std::string MedicalRecord::getFilePath()const
    {
        return getDirPath()+fileName;
    }
}

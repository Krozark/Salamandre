#include <objects/ConfidentialRecord.hpp>

namespace salamandre
{
    const std::string ConfidentialRecord::fileName = "FCT";

    ConfidentialRecord::ConfidentialRecord(const unsigned int pat_id) : Record(pat_id)
    {
    }
    
    std::string ConfidentialRecord::getFilePath()const
    {
        return getDirPath()+fileName;
    }
}

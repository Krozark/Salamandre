#include <objects/DigitalRecord.hpp>

namespace salamandre
{
    const std::string DigitalRecord::fileName = "FMN";

    DigitalRecord::DigitalRecord(const unsigned int pat_id) : Record(pat_id)
    {
    }

    std::string DigitalRecord::getFilePath() const
    {
        return getDirPath()+fileName;
    }
}

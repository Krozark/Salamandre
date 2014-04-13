#include <objects/RegistryRecord.hpp>

namespace salamandre
{
    const std::string RegistryRecord::fileName = "FEC";

    RegistryRecord::RegistryRecord(const unsigned int pat_id) : Record(pat_id)
    {
    }

    std::string RegistryRecord::getFilePath() const
    {
        return getDirPath()+fileName;
    }
}

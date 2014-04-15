#include <objects/ConfidentialRecord.hpp>

namespace salamandre
{
    const std::string ConfidentialRecord::fileName = "FCT";

    ConfidentialRecord::ConfidentialRecord(const std::string path)
    {
        this->setFilePath(path);
    }

    std::string ConfidentialRecord::serialize()
    {
        return std::string();
    }

    void ConfidentialRecord::unSerialize(std::string string)
    {
        (void) string;
    }
}

#include <objects/ConfidentialRecord.hpp>

namespace salamandre
{
    const std::string ConfidentialRecord::fileName = "FCT";

    ConfidentialRecord::ConfidentialRecord(const std::string path)
    {
        this->setFilePath(path);
    }
    
    std::string ConfidentialRecord::getFilePath()const
    {
        return this->filePath;
    }

    void ConfidentialRecord::setFilePath(std::string filePath)
    {
        this->filePath = filePath;
    }

    std::string ConfidentialRecord::serialize()
    {
        return std::string();
    }

    void ConfidentialRecord::unSerialize(std::string string)
    {
        (void) string;
    }

    void ConfidentialRecord::save(std::string key)
    {
        (void) key;
    }

    void ConfidentialRecord::load(std::string key)
    {
        (void) key;
    }
}

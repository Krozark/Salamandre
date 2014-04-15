#include <objects/DigitalRecord.hpp>

namespace salamandre
{
    const std::string DigitalRecord::fileName = "FMN";

    DigitalRecord::DigitalRecord(const std::string path)
    {
        this->setFilePath(path);
    }

    std::string DigitalRecord::serialize()
    {
        return std::string();
    }

    void DigitalRecord::unSerialize(std::string string)
    {
        (void) string;
    }

    void DigitalRecord::save(std::string key)
    {
        (void) key;
    }

    void DigitalRecord::load(std::string key)
    {
        (void) key;
    }
}

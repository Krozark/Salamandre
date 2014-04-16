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
        std::ostringstream os;
        os << *this;
        return os.str();
    }

    void DigitalRecord::unSerialize(std::string string)
    {
        std::istringstream is;
        is.str(string);
        is >> *this;
    }
}

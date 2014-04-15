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
        std::ostringstream os;
        os << *this;
        return os.str();
    }

    void ConfidentialRecord::unSerialize(std::string string)
    {
        std::istringstream is;
        is.str(string);
        is >> *this;
    }

    std::string ConfidentialRecord::getContent()
    {
        return this->fileContent;
    }

    void ConfidentialRecord::setContent(std::string content)
    {
        this->fileContent = content;
    }
}

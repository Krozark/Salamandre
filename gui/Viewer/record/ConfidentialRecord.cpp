#include <record/ConfidentialRecord.hpp>

namespace salamandre
{
    const std::string ConfidentialRecord::fileName = "FCT";

    ConfidentialRecord::ConfidentialRecord(const std::string path)
    {
        this->setFilePath(path);
    }

    ConfidentialRecord::~ConfidentialRecord()
    {

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

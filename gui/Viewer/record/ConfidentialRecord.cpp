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

    std::string ConfidentialRecord::serialize(std::string key)
    {
        std::ostringstream os;
        os << *this;
        std::string s = os.str();
        return this->strEncrypt(key, &s);
    }

    void ConfidentialRecord::unSerialize(std::string key, std::string *string)
    {
        std::istringstream is;
        is.str(this->strDecrypt(key,string));
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

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
        std::string strEncrypt = this->strEncrypt(key, &s);

        return Record::strCompress(strEncrypt);
    }

    bool ConfidentialRecord::unSerialize(std::string key, std::string *string)
    {
        std::istringstream is;
        std::string decompressString = Record::strDecompress(*string);
        std::string decryptString = this->strDecrypt(key, &decompressString);

        if(decryptString != std::string()){
            is.str(decryptString);
            is >> *this;
            return true;
        }

        return false;
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

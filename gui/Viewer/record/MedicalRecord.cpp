#include <record/MedicalRecord.hpp>

namespace salamandre
{
    const std::string MedicalRecord::fileName = "FMT";
    
    MedicalRecord::MedicalRecord(const std::string path)
    {
        this->setFilePath(path);
    }

    std::string MedicalRecord::serialize(std::string key)
    {
        std::ostringstream os;
        os << *this;
        return this->strEncrypt(key, os.str());
    }

    void MedicalRecord::unSerialize(std::string key, std::string string)
    {
        std::istringstream is;
        is.str(this->strDecrypt(key,string));
        is >> *this;
    }

    std::string MedicalRecord::getContent()
    {
        return this->fileContent;
    }

    void MedicalRecord::setContent(std::string content)
    {
        this->fileContent = content;
    }

    std::ios_base::openmode MedicalRecord::openMode()
    {
        return std::ios::out | std::ios::trunc | std::ios::binary;
    }
}

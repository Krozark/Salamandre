#include <record/MedicalRecord.hpp>

namespace salamandre
{
    const std::string MedicalRecord::fileName = "FMT";
    
    MedicalRecord::MedicalRecord(const std::string path)
    {
        this->setFilePath(path);
    }

    MedicalRecord::~MedicalRecord()
    {

    }

    std::string MedicalRecord::serialize(std::string key)
    {
        std::ostringstream os;
        os << *this;
        std::string s = os.str();
        std::string strEncrypt = this->strEncrypt(key, &s);

        return Record::strCompress(strEncrypt);
    }

    bool MedicalRecord::unSerialize(std::string key, std::string *string)
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

    std::string MedicalRecord::getContent()
    {
        return this->fileContent;
    }

    void MedicalRecord::setContent(std::string content)
    {
        this->fileContent = content;
    }
}

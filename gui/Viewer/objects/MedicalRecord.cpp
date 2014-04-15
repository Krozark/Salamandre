#include <objects/MedicalRecord.hpp>

namespace salamandre
{
    const std::string MedicalRecord::fileName = "FMT";
    
    MedicalRecord::MedicalRecord(const std::string path)
    {
        this->setFilePath(path);
    }

    std::string MedicalRecord::serialize()
    {
        std::ostringstream os;
        os << *this;
        return os.str();
    }

    void MedicalRecord::unSerialize(std::string string)
    {
        std::istringstream is;
        is.str(string);
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
}

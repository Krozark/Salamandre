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

    std::string MedicalRecord::getContent()
    {
        return this->fileContent;
    }

    void MedicalRecord::setContent(std::string content)
    {
        this->fileContent = content;
    }
}

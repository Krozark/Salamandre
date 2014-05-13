#include <record/RegistryRecord.hpp>

namespace salamandre
{
    const std::string RegistryRecord::fileName = "FEC";

    RegistryRecord::RegistryRecord(const std::string path)
    {
        this->setFilePath(path);
    }

    RegistryRecord::~RegistryRecord()
    {

    }

    void RegistryRecord::setFirstName(std::string firstName)
    {
        this->firstName = firstName;
    }

    std::string RegistryRecord::getFirstName()
    {
        return this->firstName;
    }

    void RegistryRecord::setLastName(std::string lastName)
    {
        this->lastName = lastName;
    }

    std::string RegistryRecord::getLastName()
    {
        return this->lastName;
    }

    void RegistryRecord::setBirthDate(std::string birthDate)
    {
        this->birthDate = birthDate;
    }

    std::string RegistryRecord::getBirthDate()
    {
        return this->birthDate;
    }

    void RegistryRecord::setSex(std::string sex)
    {
        this->sex = sex;
    }

    std::string RegistryRecord::getSex()
    {
        return this->sex;
    }

    void RegistryRecord::setAdress(std::string adress)
    {
        this->adress = adress;
    }

    std::string RegistryRecord::getAdress()
    {
        return this->adress;
    }
}

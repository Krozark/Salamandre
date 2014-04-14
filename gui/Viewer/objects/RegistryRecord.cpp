#include <objects/RegistryRecord.hpp>

#include <sstream>
#include <fstream>
#include <iostream>
#include <string>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <QDebug>

namespace salamandre
{
    const std::string RegistryRecord::fileName = "FEC";

    RegistryRecord::RegistryRecord(const std::string path, const std::string key)
    {
        this->setFilePath(path);
        this->load(key);
    }

    std::string RegistryRecord::getFilePath() const
    {
        return this->filePath;
    }

    void RegistryRecord::setFilePath(std::string filePath)
    {
        this->filePath = filePath;
    }

    std::string RegistryRecord::serialize()
    {
        std::ostringstream os;
        os << *this;
        return os.str();
    }

    void RegistryRecord::unSerialize(std::string string)
    {
        std::istringstream is;
        is.str(string);
        is >> *this;
    }

    void RegistryRecord::save(std::string key)
    {
        this->setAdress("1 rue du test");
        std::string str = this->serialize();
        std::string strEncrypt = this->strEncrypt(key, str);

        qDebug() << QString("writting to : ") << QString::fromStdString(this->getFilePath());

        std::ofstream outputFile(this->getFilePath(), std::ios::out | std::ios::trunc );

        //outputFile << this->getVersionNumber() << strEncrypt;
        outputFile << strEncrypt;

        outputFile.close();

    }

    void RegistryRecord::load(std::string key)
    {
        std::ifstream inputFile(this->getFilePath().c_str(), std::ios::in );
        std::string str;
        getline(inputFile, str);

        std::string decrypt = this->strDecrypt(key, str);
        this->unSerialize(decrypt);

        qDebug() << "test load: " << QString::fromStdString(this->getAdress());
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

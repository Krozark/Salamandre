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

    RegistryRecord::RegistryRecord(const std::string path)
    {
        this->setFilePath(path);
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
        std::string str = this->serialize();
        std::ofstream outputFile(this->getFilePath().c_str(), std::ios::out | std::ios::trunc | std::ios::binary);
        outputFile.flush();

        char *header = new char[SIZE_HEADER];
        strcpy(header, std::to_string(this->getVersionNumber()+1).c_str());

        outputFile.write(header, SIZE_HEADER);
        delete[] header;

        outputFile << this->strEncrypt(key, str);
    }

    void RegistryRecord::load(std::string key)
    {
        this->loadHeader();

        std::ifstream inputFile(this->getFilePath().c_str(), std::ios::in | std::ios::binary);
        std::cout << "opening FEC version n°" << this->getVersionNumber() << std::endl;

        std::string str((std::istreambuf_iterator<char>(inputFile)), std::istreambuf_iterator<char>());
        this->unSerialize(this->strDecrypt(key, str.substr(SIZE_HEADER, str.size()-SIZE_HEADER)));
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

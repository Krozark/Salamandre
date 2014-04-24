#include <record/DigitalRecord.hpp>

namespace salamandre
{
    const std::string DigitalRecord::fileName = "FMN";

    DigitalRecord::DigitalRecord(const std::string path)
    {
        this->setFilePath(path);
    }

    std::string DigitalRecord::serialize(std::string key)
    {
        this->key = key;
        std::ostringstream os;
        os << *this;
        return os.str();
    }

    void DigitalRecord::unSerialize(std::string key, std::string string)
    {
        this->key = key;
        std::istringstream is;
        is.str(string);
        is >> *this;
    }

    void DigitalRecord::save(std::string key)
    {
        std::string str = this->serialize(key);
        std::ofstream outputFile(this->getFilePath().c_str(), openMode());
        outputFile << str;
    }

    void DigitalRecord::load(std::string key)
    {
        std::ifstream inputFile(this->getFilePath().c_str(), std::ios::in | std::ios::binary);
        std::string str((std::istreambuf_iterator<char>(inputFile)), std::istreambuf_iterator<char>());
        this->unSerialize(key, str);

        inputFile.close();
    }

    std::ios_base::openmode DigitalRecord::openMode()
    {
        return std::ios::out | std::ios::app | std::ios::ate | std::ios::binary;
    }
}

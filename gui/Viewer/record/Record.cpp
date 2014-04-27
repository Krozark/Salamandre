#include <record/Record.hpp>

#include <crypto++/files.h>
#include <crypto++/default.h>
#include <crypto++/aes.h>
#include <crypto++/hex.h>

namespace salamandre
{
    void Record::encrypt(const std::string pass)
    {
        CryptoPP::FileSource(getFilePath().c_str(), true,
                               new CryptoPP::DefaultEncryptorWithMAC((byte*)pass.data(), pass.size(), new CryptoPP::FileSink((getFilePath()+".eas").c_str())));
    }

    Record::~Record(){}

    void Record::decrypt(const std::string pass)
    {
        CryptoPP::FileSource((getFilePath()+".eas").c_str(), true,
                               new CryptoPP::DefaultDecryptorWithMAC((byte*)pass.data(), pass.size(), new CryptoPP::FileSink(getFilePath().c_str())));
    }

    const std::string Record::strEncrypt(const std::string pass, const std::string string)
    {
        std::string output;
        CryptoPP::StringSource(string, true, new CryptoPP::DefaultEncryptorWithMAC((byte*)pass.data(), pass.size(), new CryptoPP::HexEncoder(new CryptoPP::StringSink(output))));

        return output;
    }

    const std::string Record::strDecrypt(const std::string pass, std::string string)
    {
        std::string output;
        CryptoPP::StringSource(string, true, new CryptoPP::HexDecoder(new CryptoPP::DefaultDecryptorWithMAC((byte*)pass.data(), pass.size(), new CryptoPP::StringSink(output))));

        return output;
    }

    std::string Record::getFilePath()
    {
        return this->filePath;
    }

    void Record::setFilePath(std::string filePath)
    {
        this->filePath = filePath;
    }

    void Record::save(std::string key)
    {
        std::string str = this->serialize(key);
        std::ofstream outputFile(this->getFilePath().c_str(), std::ios::out | std::ios::trunc | std::ios::binary);

        char *header = new char[SIZE_HEADER];
        const char *version = std::to_string(this->getVersionNumber()+1).c_str();
        memcpy(header, version, sizeof(version));

        outputFile.seekp(0, outputFile.beg);
        outputFile.write(header, SIZE_HEADER);
        outputFile.seekp(0, outputFile.end);

        delete[] header;

        outputFile << str;
    }

    void Record::load(std::string key)
    {
        this->loadHeader();

        std::ifstream inputFile(this->getFilePath().c_str(), std::ios::in | std::ios::binary);
        std::string str((std::istreambuf_iterator<char>(inputFile)), std::istreambuf_iterator<char>());
        this->unSerialize(key, str.substr(SIZE_HEADER, str.size()-SIZE_HEADER));

        inputFile.close();
    }

    void Record::loadHeader()
    {
        FILE *file = fopen(this->getFilePath().c_str(), "rb");
        if(file){
            char header[SIZE_HEADER];
            fseek(file, 0, SEEK_SET);
            fread(header, SIZE_HEADER, 1, file);
            this->setVersionNumber(std::stoll(header));
            fclose(file);
        }
    }

    void Record::setVersionNumber(long long versionNumber)
    {
        std::cout << "open file : " << this->getFilePath() << " version : " << std::to_string(versionNumber) << std::endl;
        this->versionNumber = versionNumber;
    }

    u_int64_t Record::getVersionNumber()
    {
        return this->versionNumber;
    }
}

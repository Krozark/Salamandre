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
        std::ofstream outputFile(this->getFilePath().c_str(), openMode());

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
        std::cout << "opening " << this->getFileName() << " version n°" << this->getVersionNumber() << std::endl;

        std::string str((std::istreambuf_iterator<char>(inputFile)), std::istreambuf_iterator<char>());
        this->unSerialize(key, str.substr(SIZE_HEADER, str.size()-SIZE_HEADER));

        inputFile.close();
    }

    void Record::loadHeader()
    {
        std::ifstream inputFile(this->getFilePath().c_str(), std::ios::in | std::ios::binary);

        char *header = new char[SIZE_HEADER];
        inputFile.read(header, SIZE_HEADER);
        std::string strHeader(header);
        delete[] header;

        this->setVersionNumber(std::stoll(strHeader));
    }

    void Record::setVersionNumber(long long versionNumber)
    {
        this->versionNumber = versionNumber;
    }

    u_int64_t Record::getVersionNumber()
    {
        return this->versionNumber;
    }
}

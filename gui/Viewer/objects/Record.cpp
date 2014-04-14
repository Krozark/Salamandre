#include <objects/Record.hpp>

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

    std::string Record::strEncrypt(const std::string pass, const std::string string)
    {
        std::string output;
        CryptoPP::StringSource(string, true, new CryptoPP::DefaultEncryptorWithMAC((byte*)pass.data(), pass.size(), new CryptoPP::HexEncoder(new CryptoPP::StringSink(output))));

        return output;
    }

    std::string Record::strDecrypt(const std::string pass, std::string string)
    {
        std::string output;
        CryptoPP::StringSource(string, true, new CryptoPP::HexDecoder(new CryptoPP::DefaultDecryptorWithMAC((byte*)pass.data(), pass.size(), new CryptoPP::StringSink(output))));

        return output;
    }

    void Record::stream(std::fstream& s)const
    {
        s.open(getFilePath(), std::fstream::in | std::fstream::out | std::fstream::binary);
    }
}

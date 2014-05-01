#include <record/Record.hpp>

#include <crypto++/files.h>
#include <crypto++/default.h>
#include <crypto++/aes.h>
#include <crypto++/hex.h>

#include <string>
#include <stdexcept>
#include <iostream>
#include <iomanip>
#include <sstream>

namespace salamandre
{
    const std::string Record::strCompress(const std::string& str, int compressionlevel)
    {
        z_stream zs;
        memset(&zs, 0, sizeof(zs));

        if (deflateInit(&zs, compressionlevel) != Z_OK)
            throw(std::runtime_error("deflateInit failed while compressing."));

        zs.next_in = (Bytef*)str.data();
        zs.avail_in = str.size();

        int ret;
        char outbuffer[32768];
        std::string outstring;

        // retrieve the compressed bytes blockwise
        do{
            zs.next_out = reinterpret_cast<Bytef*>(outbuffer);
            zs.avail_out = sizeof(outbuffer);

            ret = deflate(&zs, Z_FINISH);

            if (outstring.size() < zs.total_out) {
                outstring.append(outbuffer,
                                 zs.total_out - outstring.size());
            }
        } while (ret == Z_OK);

        deflateEnd(&zs);

        if (ret != Z_STREAM_END) {
            std::ostringstream oss;
            oss << "Exception during zlib compression: (" << ret << ") " << zs.msg;
            throw(std::runtime_error(oss.str()));
        }

        return outstring;
    }

    const std::string Record::strDecompress(const std::string& str)
    {
        z_stream zs;
        memset(&zs, 0, sizeof(zs));

        if (inflateInit(&zs) != Z_OK)
            throw(std::runtime_error("inflateInit failed while decompressing."));

        zs.next_in = (Bytef*)str.data();
        zs.avail_in = str.size();

        int ret;
        char outbuffer[32768];
        std::string outstring;

        // get the decompressed bytes blockwise using repeated calls to inflate
        do{
            zs.next_out = reinterpret_cast<Bytef*>(outbuffer);
            zs.avail_out = sizeof(outbuffer);

            ret = inflate(&zs, 0);

            if (outstring.size() < zs.total_out) {
                outstring.append(outbuffer,
                                 zs.total_out - outstring.size());
            }
        } while (ret == Z_OK);

        inflateEnd(&zs);

        if (ret != Z_STREAM_END) {
            std::ostringstream oss;
            oss << "Exception during zlib decompression: (" << ret << ") "
                << zs.msg;
            throw(std::runtime_error(oss.str()));
        }

        return outstring;
    }

    void Record::encrypt(const std::string pass, const std::string filePath)
    {
        std::cout << "start to encrypt file : " << filePath << std::endl;
        CryptoPP::FileSource(filePath.c_str(), true,
                               new CryptoPP::DefaultEncryptorWithMAC((byte*)pass.data(), pass.size(), new CryptoPP::FileSink((filePath+".tmp").c_str())));
        std::cout << "end to encrypt file" << std::endl;
    }

    Record::~Record(){}

    void Record::decrypt(const std::string pass, const std::string filePath)
    {
        std::cout << "start to decrypt file : " << filePath << std::endl;
        CryptoPP::FileSource((filePath).c_str(), true,
                               new CryptoPP::DefaultDecryptorWithMAC((byte*)pass.data(), pass.size(), new CryptoPP::FileSink((filePath.substr(0, filePath.size()-4).c_str()))));
        std::cout << "end to decrypt file" << std::endl;
    }

    const std::string Record::strEncrypt(const std::string pass, const std::string *string)
    {
        std::string output;
        std::cout << "start to encrypt string" << std::endl;
        CryptoPP::StringSource(*string, true, new CryptoPP::DefaultEncryptorWithMAC((byte*)pass.data(), pass.size(), new CryptoPP::HexEncoder(new CryptoPP::StringSink(output))));
        std::cout << "end of encrypt string" << std::endl;
        return Record::strCompress(output);
    }

    const std::string Record::strDecrypt(const std::string pass, std::string *string)
    {
        std::string output;
        std::cout << "start of decrypt string" << std::endl;
        CryptoPP::StringSource(Record::strDecompress(*string), true, new CryptoPP::HexDecoder(new CryptoPP::DefaultDecryptorWithMAC((byte*)pass.data(), pass.size(), new CryptoPP::StringSink(output))));
        std::cout << "end of decrypt string" << std::endl;
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

        ntw::Serializer serializer;
        long int version = this->getVersionNumber()+1;
        serializer << version;
        char buf[SIZE_HEADER];
        serializer.read(buf, SIZE_HEADER);

        outputFile.seekp(0, outputFile.beg);
        outputFile.write(buf, SIZE_HEADER);
        outputFile.seekp(0, outputFile.end);

        outputFile << str;
    }

    void Record::load(std::string key)
    {
        this->loadHeader();

        std::ifstream inputFile(this->getFilePath().c_str(), std::ios::in | std::ios::binary);
        std::string str((std::istreambuf_iterator<char>(inputFile)), std::istreambuf_iterator<char>());
        std::string subStr = str.substr(SIZE_HEADER, str.size()-SIZE_HEADER);
        this->unSerialize(key, &subStr);

        inputFile.close();
    }

    void Record::loadHeader()
    {
        FILE *file = fopen(this->getFilePath().c_str(), "rb");
        if(file){
            char header[SIZE_HEADER];
            fseek(file, 0, SEEK_SET);
            fread(header, SIZE_HEADER, 1, file);

            ntw::Serializer serializer;
            serializer.write(header, SIZE_HEADER);
            long int version;
            serializer >> version;
            this->setVersionNumber(version);
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

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
#include <sys/file.h>

namespace salamandre
{
    std::string Record::passStr = std::string("salamandre");

    Record::~Record(){}

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

    bool Record::encrypt(const std::string pass, const std::string filePath)
    {
        try{
            CryptoPP::FileSource(filePath.c_str(), true, new CryptoPP::DefaultEncryptorWithMAC((byte*)pass.data(), pass.size(), new CryptoPP::FileSink((filePath+".tmp").c_str())));
            return true;
        }
        catch(const CryptoPP::Exception& e){
            std::cerr << e.what() << std::endl;
            return false;
        }
    }

    bool Record::decrypt(const std::string pass, const std::string filePathSrc, std::string filePathTo)
    {
        try{
            CryptoPP::FileSource((filePathSrc).c_str(), true, new CryptoPP::DefaultDecryptorWithMAC((byte*)pass.data(), pass.size(), new CryptoPP::FileSink(filePathTo.c_str())));
            return true;
        }
        catch(const CryptoPP::Exception& e){
            std::cerr << e.what() << std::endl;
            return false;
        }
    }

    const std::string Record::strEncrypt(const std::string pass, const std::string string)
    {
        try{
            std::string output;
            CryptoPP::StringSource(string, true, new CryptoPP::DefaultEncryptorWithMAC((byte*)pass.data(), pass.size(), new CryptoPP::HexEncoder(new CryptoPP::StringSink(output))));
            return output;
        }
        catch(const CryptoPP::Exception& e){
            std::cerr << e.what() << std::endl;
            return std::string();
        }
    }

    const std::string Record::strDecrypt(const std::string pass, std::string string)
    {
        try{
            std::string output;
            CryptoPP::StringSource(string, true, new CryptoPP::HexDecoder(new CryptoPP::DefaultDecryptorWithMAC((byte*)pass.data(), pass.size(), new CryptoPP::StringSink(output))));
            return output;
        }
        catch(const CryptoPP::Exception& e){
            std::cerr << e.what() << std::endl;
        }

        return std::string();
    }

    void Record::copyFile(FILE *fSrc, FILE *fDest)
    {
        flock(fileno(fDest), LOCK_EX);

        fseek(fSrc, 0, SEEK_END);
        u_int64_t sizeFileSrc = ftell(fSrc);
        size_t readSize;

        int nbCopy = sizeFileSrc / BUFSIZ;
        int rest = sizeFileSrc - nbCopy * BUFSIZ;

        fseek(fSrc, 0, SEEK_SET);

        for(int i = 0; i < nbCopy; ++i){
            char buf[BUFSIZ];
            if((readSize = fread(buf, BUFSIZ, 1, fSrc)) == 0)
                std::cerr << "attempt to read " << BUFSIZ << " but " << readSize << " have been read" << std::endl;
            fwrite(buf, BUFSIZ, 1, fDest);
        }

        char buf[rest];
        if((readSize = fread(buf, rest, 1, fSrc)) == 0)
            std::cerr << "attempt to read " << rest << " but " << readSize << " have been read" << std::endl;
        fwrite(buf, rest, 1, fDest);

        flock(fileno(fDest), LOCK_UN);
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

        uint64_t version = this->getVersionNumber()+1;

        char buf[SIZE_HEADER];
        ntw::Serializer::convert(version, buf);

        std::string passStrEncrypt;
        passStrEncrypt = Record::strEncrypt(key, passStr);

        outputFile.seekp(0, outputFile.beg);
        outputFile.write(buf, SIZE_HEADER);
        outputFile.write(passStrEncrypt.c_str(), passStrEncrypt.size());
        outputFile.seekp(0, outputFile.end);

        outputFile << str;
    }

    bool Record::load(std::string key)
    {
        if(!this->loadHeader(key)){
            std::cout << "failed to load header " << std::endl;
            return false;
        }

        std::string passStrEncrypt = Record::strEncrypt(key, passStr);
        std::ifstream inputFile(this->getFilePath().c_str(), std::ios::in | std::ios::binary);
        std::string str((std::istreambuf_iterator<char>(inputFile)), std::istreambuf_iterator<char>());
        std::string subStr = str.substr(SIZE_HEADER+passStrEncrypt.size(), str.size()-(SIZE_HEADER+passStrEncrypt.size()));

        inputFile.close();

        if(str.size() > (SIZE_HEADER+passStrEncrypt.size()))
            return this->unSerialize(key, subStr);

        return true;
    }

    bool Record::loadHeader(std::string key)
    {
        bool res = true;

        FILE *file = fopen(this->getFilePath().c_str(), "rb");
        if(file){
            char header[SIZE_HEADER];
            fseek(file, 0, SEEK_SET);

            size_t readSize;

            if((readSize = fread(header, SIZE_HEADER, 1, file)) == 0)
                std::cerr << "attempt to read " << SIZE_HEADER << " but " << readSize << " have been read" << std::endl;

            uint64_t version;
            ntw::Serializer::convert(header, version);

            this->setVersionNumber(version);

            std::string passStrEncrypt = Record::strEncrypt(key, passStr);

            char passStrBuf[passStrEncrypt.size()];
            if((readSize = fread(passStrBuf, passStrEncrypt.size(), 1, file)) == 0)
                std::cerr << "attempt to read " << passStrEncrypt.size() << " but " << readSize << " have been read" << std::endl;

            passStrBuf[passStrEncrypt.size()] = '\0';
            std::string strBuf;
            strBuf.assign(passStrBuf);

            if(Record::strDecrypt(key, strBuf) != Record::strDecrypt(key, passStrEncrypt)){
                res = false;
            }

            fclose(file);
        }

        return res;
    }

    void Record::setVersionNumber(uint64_t versionNumber)
    {
        std::cout << "open file : " << this->getFilePath() << " version : " << std::to_string(versionNumber) << std::endl;
        this->versionNumber = versionNumber;
    }

    u_int64_t Record::getVersionNumber()
    {
        return this->versionNumber;
    }
}

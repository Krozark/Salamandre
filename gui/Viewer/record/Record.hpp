#ifndef SALAMANDRE_FICHE_HPP
#define SALAMANDRE_FICHE_HPP

#include <stdio.h>
#include <stdlib.h>
#include <sstream>
#include <fstream>
#include <iostream>
#include <string>

#include <Socket/Serializer.hpp>
#include <zlib.h>

constexpr int HEADER_SIZE = ntw::Serializer::Size<long int>::value;


namespace salamandre
{
    class Record
    {
    public:
        Record(){}
        Record(const std::string path)
        {
            this->setFilePath(path);
        }

        Record(const Record&) = delete;
        Record& operator=(const Record&) = delete;
        virtual ~Record() = 0;

        static void encrypt(const std::string pass, const std::string filePath);
        static void decrypt(const std::string pass, const std::string filePathSrc, std::string filePathTo);

        static const std::string strDecrypt(const std::string pass, std::string *string);
        static const std::string strEncrypt(const std::string pass, const std::string *string);

        static const std::string strCompress(const std::string& str, int compressionlevel = Z_BEST_COMPRESSION);
        static const std::string strDecompress(const std::string& str);

        static void copyFile(FILE *fSrc, FILE *fDest);

        std::string getFilePath();
        void setFilePath(std::string filePath);

        virtual std::string serialize(std::string key) = 0;
        virtual void unSerialize(std::string key, std::string *string) = 0;
        static std::string getFileName();

        void save(std::string key);
        void load(std::string key);
        void loadHeader();

        static u_int64_t getVersion(u_int32_t idMedecin, u_int32_t idPatient, std::string nameRecord);

        void setVersionNumber(long long versionNumber);
        u_int64_t getVersionNumber();

    private:
        u_int64_t versionNumber;
        std::string filePath;

    };
}
#endif

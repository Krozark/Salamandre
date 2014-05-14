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

constexpr int SIZE_HEADER = ntw::Serializer::Size<uint64_t>::value;

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

        static bool encrypt(const std::string pass, const std::string filePath);
        static bool decrypt(const std::string pass, const std::string filePathSrc, std::string filePathTo);

        static const std::string strDecrypt(const std::string pass, std::string string);
        static const std::string strEncrypt(const std::string pass, const std::string string);

        static const std::string strCompress(const std::string& str, int compressionlevel = Z_BEST_COMPRESSION);
        static const std::string strDecompress(const std::string& str);

        static void copyFile(FILE *fSrc, FILE *fDest);

        std::string getFilePath();
        void setFilePath(std::string filePath);

        virtual std::string serialize(std::string key) = 0;
        virtual bool unSerialize(std::string key, std::string string) = 0;
        static std::string getFileName();

        void save(std::string key);
        bool load(std::string key);
        bool loadHeader(std::string key);

        static uint64_t getVersion(uint32_t idMedecin, uint32_t idPatient, std::string nameRecord);
        static std::string passStr;

        void setVersionNumber(uint64_t versionNumber);
        uint64_t getVersionNumber();

    private:
        u_int64_t versionNumber;
        std::string filePath;

    };
}
#endif

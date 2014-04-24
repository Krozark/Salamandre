#ifndef SALAMANDRE_FICHE_HPP
#define SALAMANDRE_FICHE_HPP

#include <stdio.h>
#include <stdlib.h>
#include <sstream>
#include <fstream>
#include <iostream>
#include <string>

#define SIZE_HEADER 64 // 8 octets


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

        void encrypt(const std::string pass);
        void decrypt(const std::string pass);

        static const std::string strDecrypt(const std::string pass, std::string string);
        static const std::string strEncrypt(const std::string pass, const std::string string);

        std::string getFilePath();
        void setFilePath(std::string filePath);

        virtual std::string serialize(std::string key) = 0;
        virtual void unSerialize(std::string key, std::string string) = 0;
        virtual std::string getFileName() = 0;
        virtual std::ios_base::openmode openMode() = 0;

        void save(std::string key);
        void load(std::string key);
        void loadHeader();

        void setVersionNumber(long long versionNumber);
        u_int64_t getVersionNumber();

    private:
        u_int64_t versionNumber;
        std::string filePath;

    };
}
#endif

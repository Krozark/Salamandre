#ifndef SALAMANDRE_FICHE_HPP
#define SALAMANDRE_FICHE_HPP

#include <string>
#include <fstream>

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

        std::string strDecrypt(const std::string pass, std::string string);
        std::string strEncrypt(const std::string pass, const std::string string);

        virtual std::string getFilePath() const = 0;
        virtual void setFilePath(std::string filePath) = 0;
        virtual std::string serialize() = 0;
        virtual void unSerialize(std::string string) = 0;
        virtual void save(std::string key) = 0;
        virtual void load(std::string key) = 0;

        void stream(std::fstream& stream)const;

        void loadHeader(){
            std::ifstream inputFile(this->getFilePath().c_str(), std::ios::in | std::ios::binary);

            char *header = new char[SIZE_HEADER];
            inputFile.read(header, SIZE_HEADER);
            std::string strHeader(header);
            delete[] header;

            this->setVersionNumber(std::stoll(strHeader));
        }

        void setVersionNumber(long long versionNumber){
            this->versionNumber = versionNumber;
        }

        long long getVersionNumber(){
            return this->versionNumber;
        }

    private:
        long long versionNumber;

    };
}
#endif

#ifndef SALAMANDRE_FICHE_HPP
#define SALAMANDRE_FICHE_HPP

#include <string>
#include <fstream>

#define SIZE_HEADER 72


namespace salamandre
{
    class Record
    {
    public:
        Record(){};
        Record(const std::string path, const std::string key)
        {
            this->setFilePath(path);
            this->load(key);
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

        void setVersionNumber(int versionNumber){
            this->versionNumber = versionNumber;
        }
        int getVersionNumber(){
            return this->versionNumber;
        }

    private:
        int versionNumber;

    };
}
#endif

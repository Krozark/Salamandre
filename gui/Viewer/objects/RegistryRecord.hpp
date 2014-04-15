#ifndef SALAMANDRE_FICHEETATCIVIL_HPP
#define SALAMANDRE_FICHEETATCIVIL_HPP

#include <objects/Record.hpp>

namespace salamandre
{
    /**
    * /brief Une fiche d’etat civil (FEC, comportant le nom, la date de naissance, le sexe,
l’adresse, . . . , et surtout, un numero d’identification)
    */
    class RegistryRecord : public Record
    {
        public:
            RegistryRecord(const std::string path);
            RegistryRecord(const RegistryRecord&) = delete;
            RegistryRecord& operator=(const RegistryRecord&) = delete;

            friend std::ostream& operator<<(std::ostream& os, const RegistryRecord& registry)
            {
                unsigned len;
                len = registry.adress.size();
                os.write(reinterpret_cast<const char*>(&len), sizeof(len));
                os.write(registry.adress.c_str(), len);

                len = registry.birthDate.size();
                os.write(reinterpret_cast<const char*>(&len), sizeof(len));
                os.write(registry.birthDate.c_str(), len);

                len = registry.firstName.size();
                os.write(reinterpret_cast<const char*>(&len), sizeof(len));
                os.write(registry.firstName.c_str(), len);

                len = registry.lastName.size();
                os.write(reinterpret_cast<const char*>(&len), sizeof(len));
                os.write(registry.lastName.c_str(), len);

                len = registry.sex.size();
                os.write(reinterpret_cast<const char*>(&len), sizeof(len));
                os.write(registry.sex.c_str(), len);

                return os;
            }

            friend std::istream& operator>>(std::istream& is, RegistryRecord& registry)
            {
                unsigned lgth;
                char* buf;

                is.read(reinterpret_cast<char*>(&lgth), sizeof(lgth));
                if(lgth > 0){
                  buf = new char[lgth];
                  is.read(buf, lgth);
                  registry.adress.assign(buf, lgth);
                  delete[] buf;
                }

                is.read(reinterpret_cast<char*>(&lgth), sizeof(lgth));
                if(lgth > 0){
                  buf = new char[lgth];
                  is.read(buf, lgth);
                  registry.birthDate.assign(buf, lgth);
                  delete[] buf;
                }

                is.read(reinterpret_cast<char*>( &lgth ), sizeof(lgth));
                if(lgth > 0){
                  buf = new char[lgth];
                  is.read(buf, lgth);
                  registry.firstName.assign(buf, lgth);
                  delete[] buf;
                }

                is.read(reinterpret_cast<char*>(&lgth), sizeof(lgth));
                if(lgth > 0){
                  buf = new char[lgth];
                  is.read(buf, lgth);
                  registry.lastName.assign(buf, lgth);
                  delete[] buf;
                }

                is.read(reinterpret_cast<char*>(&lgth), sizeof(lgth));
                if(lgth > 0){
                  buf = new char[lgth];
                  is.read(buf, lgth);
                  registry.sex.assign(buf, lgth);
                  delete[] buf;
                }

                return is;
            }

            std::string getFilePath() const;
            void setFilePath(std::string filePath);
            std::string serialize();
            void unSerialize(std::string string);

            const static std::string fileName;

            void setFirstName(std::string firstName);
            std::string getFirstName();

            void setLastName(std::string lastName);
            std::string getLastName();

            void setBirthDate(std::string birthDate);
            std::string getBirthDate();

            void setSex(std::string sex);
            std::string getSex();

            void setAdress(std::string adress);
            std::string getAdress();

            void save(std::string key);
            void load(std::string key);
        protected:

        private:
            std::string filePath;
            std::string lastName;
            std::string firstName;
            std::string birthDate;
            std::string sex;
            std::string adress;
    };
}
#endif

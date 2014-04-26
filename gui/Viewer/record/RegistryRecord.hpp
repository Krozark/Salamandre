#ifndef SALAMANDRE_FICHEETATCIVIL_HPP
#define SALAMANDRE_FICHEETATCIVIL_HPP

#include <record/Record.hpp>

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

            std::string serialize(std::string key);
            void unSerialize(std::string key, std::string string);
            std::ios_base::openmode openMode();

            friend std::ostream& operator<<(std::ostream& os, const RegistryRecord& registry)
            {
                os << registry.adress << '\n';
                os << registry.birthDate << '\n';
                os << registry.firstName << '\n';
                os << registry.lastName << '\n';
                os << registry.sex;

                return os;
            }

            friend std::istream& operator>>(std::istream& is, RegistryRecord& registry)
            {
                is >> registry.adress;
                is >> registry.birthDate;
                is >> registry.firstName;
                is >> registry.lastName;
                is >> registry.sex;

                return is;
            }

            std::string getFileName(){
                return this->fileName;
            }

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
        protected:

        private:
            std::string lastName;
            std::string firstName;
            std::string birthDate;
            std::string sex;
            std::string adress;
    };
}
#endif

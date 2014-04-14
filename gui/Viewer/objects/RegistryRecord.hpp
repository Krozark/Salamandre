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
            RegistryRecord(const std::string path, const std::string key);
            RegistryRecord(const RegistryRecord&) = delete;
            RegistryRecord& operator=(const RegistryRecord&) = delete;

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
                std::string adress, birthDate, firstName, lastName, sex;
                is >> adress >> birthDate >> firstName >> lastName >> sex;

                registry.setAdress(adress);
                registry.setBirthDate(birthDate);
                registry.setFirstName(firstName);
                registry.setLastName(lastName);
                registry.setSex(sex);

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

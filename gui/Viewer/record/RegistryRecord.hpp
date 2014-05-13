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
            ~RegistryRecord();

            std::string serialize(std::string key);
            bool unSerialize(std::string key, std::string *string);

            friend std::ostream& operator<<(std::ostream& os, const RegistryRecord& registry)
            {
                ntw::Serializer serializer;
                int size;

                size = registry.adress.size();
                serializer << size;
                serializer.write(registry.adress.c_str(), size);

                size = registry.birthDate.size();
                serializer << size;
                serializer.write(registry.birthDate.c_str(), size);

                size = registry.firstName.size();
                serializer << size;
                serializer.write(registry.firstName.c_str(), size);

                size = registry.lastName.size();
                serializer << size;
                serializer.write(registry.lastName.c_str(), size);

                size = registry.sex.size();
                serializer << size;
                serializer.write(registry.sex.c_str(), size);

                int sizeSer = serializer.size();

                char buf[sizeSer];
                serializer.read(&buf, sizeSer);
                os.write(buf, sizeSer);

                return os;
            }

            friend std::istream& operator>>(std::istream& is, RegistryRecord& registry)
            {
                ntw::Serializer serializer;
                is.seekg(0, is.end);
                int sizeIs = is.tellg();
                is.seekg(0, is.beg);

                char buf[sizeIs];
                is.read(buf, sizeIs);
                serializer.write(buf, sizeIs);

                int size;

                serializer >> size;
                char bufAdress[size];
                serializer.read(bufAdress, size);
                registry.adress.assign(bufAdress, size);

                serializer >> size;
                char bufBirthDate[size];
                serializer.read(bufBirthDate, size);
                registry.birthDate.assign(bufBirthDate, size);

                serializer >> size;
                char bufFirstName[size];
                serializer.read(bufFirstName, size);
                registry.firstName.assign(bufFirstName, size);

                serializer >> size;
                char bufLastName[size];
                serializer.read(bufLastName, size);
                registry.lastName.assign(bufLastName, size);

                serializer >> size;
                char bufSex[size];
                serializer.read(bufSex, size);
                registry.sex.assign(bufSex, size);

                return is;
            }

            static std::string getFileName(){
                return fileName;
            }

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
            const static std::string fileName;

            std::string lastName;
            std::string firstName;
            std::string birthDate;
            std::string sex;
            std::string adress;
    };
}
#endif

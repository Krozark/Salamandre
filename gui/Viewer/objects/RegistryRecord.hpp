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
            RegistryRecord(const unsigned int pat_id);
            RegistryRecord(const RegistryRecord&) = delete;
            RegistryRecord& operator=(const RegistryRecord&) = delete;

            virtual std::string getFilePath()const;
            const static std::string fileName;
        
        protected:

        private:
    };
}
#endif

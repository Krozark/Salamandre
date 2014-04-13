#ifndef SALAMANDRE_FICHEDONNEESNUMERIQUES_HPP
#define SALAMANDRE_FICHEDONNEESNUMERIQUES_HPP

#include <objects/Record.hpp>

namespace salamandre
{
    /**
     * /brief Une fiche de donnees medicales numeriques (FMN) contenant typiquement de l’imagerie medicale (radios, scanner, IRM, ...), mais aussi un electrocardiogramme, ...). Cette fiche est une collection de fichiers numeriques et le numero d’identification du patient.
     */
    class DigitalRecord : public Record
    {
        public:
            DigitalRecord(const unsigned int pat_id);
            DigitalRecord(const DigitalRecord&) = delete;
            DigitalRecord& operator=(const DigitalRecord&) = delete;

            virtual std::string getFilePath()const;
            const static std::string fileName;

        protected:

        private:
    };
}
#endif

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
            DigitalRecord(const std::string path, const std::string key);
            DigitalRecord(const DigitalRecord&) = delete;
            DigitalRecord& operator=(const DigitalRecord&) = delete;

            std::string getFilePath() const;
            void setFilePath(std::string filePath);
            std::string serialize();
            void unSerialize(std::string string);
            void save(std::string key);
            void load(std::string key);

            const static std::string fileName;

        protected:

        private:
            std::string filePath;
    };
}
#endif

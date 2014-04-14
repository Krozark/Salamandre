#ifndef SALAMANDRE_FICHEDONNEESMEDICALES_HPP
#define SALAMANDRE_FICHEDONNEESMEDICALES_HPP

#include <objects/Record.hpp>

namespace salamandre
{
    /**
     * /brief Une fiche de donnees medicales textuelles (FMT) contenant typiquement les con-sultations, prescriptions, compte-rendu operatoires, etc... Comme pour la fiche confidentielle, la fiche medicale textuelle contient une zone de texte et le numero d’identification du patient.
     */
    class MedicalRecord : public Record
    {
        public:
            MedicalRecord(const std::string path, const std::string key);
            MedicalRecord(const MedicalRecord&) = delete;
            MedicalRecord& operator=(const MedicalRecord&) = delete;

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

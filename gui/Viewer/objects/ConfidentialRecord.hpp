#ifndef SALAMANDRE_FICHECONFIDENTIELLE_HPP
#define SALAMANDRE_FICHECONFIDENTIELLE_HPP

#include <objects/Record.hpp>

namespace salamandre
{
    /**
     * /breif Une fiche confidentielle textuelle (FCT) comportant des notes privees d’ordre non
medical prises par le medecin. La fiche comporte une zone de texte et le numero d’identification du patient.
    */
    class ConfidentialRecord : public Record
    {
        public:
            ConfidentialRecord(const std::string path, const std::string key);
            ConfidentialRecord(const ConfidentialRecord&) = delete;
            ConfidentialRecord& operator=(const ConfidentialRecord&) = delete;

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
            std::string fileContent;
    };
}
#endif

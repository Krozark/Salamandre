#ifndef SALAMANDRE_FICHEDONNEESNUMERIQUES_HPP
#define SALAMANDRE_FICHEDONNEESNUMERIQUES_HPP

#include <record/Record.hpp>
#include <vector>

namespace salamandre
{
    /**
     * /brief Une fiche de donnees medicales numeriques (FMN) contenant typiquement de l’imagerie medicale (radios, scanner, IRM, ...), mais aussi un electrocardiogramme, ...). Cette fiche est une collection de fichiers numeriques et le numero d’identification du patient.
     */

    struct DigitalContent{
        std::string fileName;
        std::string filePath;
        std::string filePathExport;
        std::string key;
        std::string sourcePath;
        uint64_t offset;
        uint64_t size;
    };

    class DigitalRecord : public Record
    {
        public:
            DigitalRecord(const std::string path);
            DigitalRecord(const DigitalRecord&) = delete;
            DigitalRecord& operator=(const DigitalRecord&) = delete;
            ~DigitalRecord();

            std::string serialize(std::string key){
                (void) key;
                return "";
            }
            bool unSerialize(std::string key, std::string string){
                (void) key;
                (void) string;

                return true;
            }

            void save(std::string key);
            bool load(std::string key);

            friend std::ostream& operator<<(std::ostream& os, const DigitalRecord& digital)
            {
                (void) digital;
                return os;
            }

            friend std::istream& operator>>(std::istream& is, DigitalRecord& digital)
            {
                (void) digital;
                return is;
            }

            static std::string getFileName(){
                return fileName;
            }

            static void extractDigitFile(std::string source, DigitalContent *digit);
            static void insertDigitFile(std::string source, DigitalContent *digit);

            std::string key;
            std::vector<DigitalContent*> vFile;
        protected:

        private:
            const static std::string fileName;
    };
}
#endif

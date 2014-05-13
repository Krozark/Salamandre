#ifndef SALAMANDRE_FICHEDONNEESMEDICALES_HPP
#define SALAMANDRE_FICHEDONNEESMEDICALES_HPP

#include <record/Record.hpp>

namespace salamandre
{
    /**
     * /brief Une fiche de donnees medicales textuelles (FMT) contenant typiquement les con-sultations, prescriptions, compte-rendu operatoires, etc... Comme pour la fiche confidentielle, la fiche medicale textuelle contient une zone de texte et le numero d’identification du patient.
     */
    class MedicalRecord : public Record
    {
        public:
            MedicalRecord(const std::string path);
            MedicalRecord(const MedicalRecord&) = delete;
            MedicalRecord& operator=(const MedicalRecord&) = delete;
            ~MedicalRecord();

            std::string serialize(std::string key);
            bool unSerialize(std::string key, std::string string);

            static std::string getFileName(){
                return fileName;
            }

            std::string getContent();
            void setContent(std::string content);

            friend std::ostream& operator<<(std::ostream& os, const MedicalRecord& medical)
            {
                os.write(medical.fileContent.c_str(), medical.fileContent.size());
                return os;
            }

            friend std::istream& operator>>(std::istream& is, MedicalRecord& medical)
            {
                is.seekg(0, is.end);
                int size = is.tellg();
                is.seekg(0, is.beg);
                char buf[size+1];
                is.read(buf, size);

                medical.fileContent.assign(buf, size);

                return is;
            }

        protected:

        private:
            const static std::string fileName;

            std::string fileContent;
    };
}
#endif

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

            std::string serialize(std::string key);
            void unSerialize(std::string key, std::string string);
            std::ios_base::openmode openMode();

            std::string getFileName(){
                return this->fileName;
            }

            std::string getContent();
            void setContent(std::string content);

            friend std::ostream& operator<<(std::ostream& os, const MedicalRecord& medical)
            {
                unsigned len;
                len = medical.fileContent.size();
                os.write(reinterpret_cast<const char*>(&len), sizeof(len));
                os.write(medical.fileContent.c_str(), len);

                return os;
            }

            friend std::istream& operator>>(std::istream& is, MedicalRecord& medical)
            {
                unsigned lgth;
                char* buf;

                is.read(reinterpret_cast<char*>(&lgth), sizeof(lgth));
                if(lgth > 0){
                  buf = new char[lgth];
                  is.read(buf, lgth);
                  medical.fileContent.assign(buf, lgth);
                  delete[] buf;
                }

                return is;
            }

            const static std::string fileName;
        protected:

        private:
            std::string fileContent;
    };
}
#endif

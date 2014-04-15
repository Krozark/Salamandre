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
            ConfidentialRecord(const std::string path);
            ConfidentialRecord(const ConfidentialRecord&) = delete;
            ConfidentialRecord& operator=(const ConfidentialRecord&) = delete;

            std::string serialize();
            void unSerialize(std::string string);

            std::string getFileName(){
                return this->fileName;
            }

            std::string getContent();
            void setContent(std::string content);

            friend std::ostream& operator<<(std::ostream& os, const ConfidentialRecord& confidential)
            {
                unsigned len;
                len = confidential.fileContent.size();
                os.write(reinterpret_cast<const char*>(&len), sizeof(len));
                os.write(confidential.fileContent.c_str(), len);

                return os;
            }

            friend std::istream& operator>>(std::istream& is, ConfidentialRecord& confidential)
            {
                unsigned lgth;
                char* buf;

                is.read(reinterpret_cast<char*>(&lgth), sizeof(lgth));
                if(lgth > 0){
                  buf = new char[lgth];
                  is.read(buf, lgth);
                  confidential.fileContent.assign(buf, lgth);
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

#ifndef SALAMANDRE_FICHECONFIDENTIELLE_HPP
#define SALAMANDRE_FICHECONFIDENTIELLE_HPP

#include <record/Record.hpp>

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
            ~ConfidentialRecord();

            static std::string getFileName(){
                return fileName;
            }

            std::string getContent();
            void setContent(std::string content);

            friend std::ostream& operator<<(std::ostream& os, const ConfidentialRecord& confidential)
            {
                os.write(confidential.fileContent.c_str(), confidential.fileContent.size());
                return os;
            }

            friend std::istream& operator>>(std::istream& is, ConfidentialRecord& confidential)
            {
                is.seekg(0, is.end);
                int size = is.tellg();
                is.seekg(0, is.beg);
                char buf[size+1];
                is.read(buf, size);

                confidential.fileContent.assign(buf, size);
                return is;
            }

        protected:

        private:
            const static std::string fileName;
            std::string fileContent;
    };
}
#endif

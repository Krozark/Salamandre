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
            ConfidentialRecord(const unsigned int pat_id);
            ConfidentialRecord(const ConfidentialRecord&) = delete;
            ConfidentialRecord& operator=(const ConfidentialRecord&) = delete;

            virtual std::string getFilePath()const;
            const static std::string fileName;

        protected:

        private:
    };
}
#endif

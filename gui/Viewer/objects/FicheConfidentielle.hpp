#ifndef SALAMANDRE_FICHECONFIDENTIELLE_HPP
#define SALAMANDRE_FICHECONFIDENTIELLE_HPP

#include <objects/Fiche.hpp>

namespace salamandre
{
    /**
     * /breif Une fiche confidentielle textuelle (FCT) comportant des notes privees d’ordre non
medical prises par le medecin. La fiche comporte une zone de texte et le numero d’identification du patient.
    */
    class FicheConfidentielle : public Fiche
    {
        public:
            FicheConfidentielle(const unsigned int cli_id);
            FicheConfidentielle(const FicheConfidentielle&) = delete;
            FicheConfidentielle& operator=(const FicheConfidentielle&) = delete;

            virtual std::string getFilePath()const;

        protected:

        private:
    };
}
#endif

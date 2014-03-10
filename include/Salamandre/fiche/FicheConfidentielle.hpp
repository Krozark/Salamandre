#ifndef SALAMANDRE_FICHECONFIDENTIELLE_HPP
#define SALAMANDRE_FICHECONFIDENTIELLE_HPP

#include <Salamandre/fiche/Fiche.hpp>

namespace salamandre
{
    /**
     * /breif Une fiche confidentielle textuelle (FCT) comportant des notes privees d’ordre non
medical prises par le medecin. La fiche comporte une zone de texte et le numero d’identification du patient.
    */
    class FicheConfidentielle : public Fiche
    {
        public:
            FicheConfidentielle();
            FicheConfidentielle(const FicheConfidentielle&) = delete;
            FicheConfidentielle& operator=(const FicheConfidentielle&) = delete;

        protected:

        private:
    };
}
#endif

#ifndef SALAMANDRE_FICHEDONNEESMEDICALES_HPP
#define SALAMANDRE_FICHEDONNEESMEDICALES_HPP

#include <salamandre/fiche/Fiche.hpp>

namespace salamandre
{
    /**
     * /brief Une fiche de donnees medicales textuelles (FMT) contenant typiquement les con-sultations, prescriptions, compte-rendu operatoires, etc... Comme pour la fiche confidentielle, la fiche medicale textuelle contient une zone de texte et le numero d’identification du patient.
     */
    class FicheDonneesMedicales : public Fiche
    {
        public:
            FicheDonneesMedicales();
            FicheDonneesMedicales(const FicheDonneesMedicales&) = delete;
            FicheDonneesMedicales& operator=(const FicheDonneesMedicales&) = delete;

        protected:

        private:
    };
}
#endif

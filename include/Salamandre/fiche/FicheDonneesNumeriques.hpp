#ifndef SALAMANDRE_FICHEDONNEESNUMERIQUES_HPP
#define SALAMANDRE_FICHEDONNEESNUMERIQUES_HPP

#include <Salamandre/fiche/Fiche.hpp>

namespace salamandre
{
    /**
     * /brief Une fiche de donnees medicales numeriques (FMN) contenant typiquement de l’imagerie medicale (radios, scanner, IRM, ...), mais aussi un electrocardiogramme, ...). Cette fiche est une collection de fichiers numeriques et le numero d’identification du patient.
     */
    class FicheDonneesNumeriques : public Fiche
    {
        public:
            FicheDonneesNumeriques();
            FicheDonneesNumeriques(const FicheDonneesNumeriques&) = delete;
            FicheDonneesNumeriques& operator=(const FicheDonneesNumeriques&) = delete;

        protected:

        private:
    };
}
#endif

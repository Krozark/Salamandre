#ifndef SALAMANDRE_FICHEDONNEESMEDICALES_HPP
#define SALAMANDRE_FICHEDONNEESMEDICALES_HPP

#include <Salamandre/fiche/Fiche.hpp>

namespace salamandre
{
    /**
     * /brief Une fiche de donnees medicales textuelles (FMT) contenant typiquement les con-sultations, prescriptions, compte-rendu operatoires, etc... Comme pour la fiche confidentielle, la fiche medicale textuelle contient une zone de texte et le numero d’identification du patient.
     */
    class FicheDonneesMedicales : public Fiche
    {
        public:
            FicheDonneesMedicales(const unsigned int cli_id);
            FicheDonneesMedicales(const FicheDonneesMedicales&) = delete;
            FicheDonneesMedicales& operator=(const FicheDonneesMedicales&) = delete;

            virtual std::string getFilePath()const;

        protected:

        private:
    };
}
#endif

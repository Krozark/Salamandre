#ifndef SALAMANDRE_FICHEETATCIVIL_HPP
#define SALAMANDRE_FICHEETATCIVIL_HPP

#include <Salamandre/fiche/Fiche.hpp>

namespace salamandre
{
    /**
    * /brief Une fiche d’etat civil (FEC, comportant le nom, la date de naissance, le sexe,
l’adresse, . . . , et surtout, un numero d’identification)
    */
    class FicheEtatCivil : public Fiche
    {
        public:
            FicheEtatCivil(const unsigned int cli_id);
            FicheEtatCivil(const FicheEtatCivil&) = delete;
            FicheEtatCivil& operator=(const FicheEtatCivil&) = delete;

            virtual std::string getFilePath()const;
        
        protected:

        private:
    };
}
#endif

#include <objects/FicheEtatCivil.hpp>

namespace salamandre
{
    
    FicheEtatCivil::FicheEtatCivil(const unsigned int cli_id) : Fiche(cli_id)
    {
    }

    std::string FicheEtatCivil::getFilePath() const
    {
        return getDirPath()+"FEC";
    }
}

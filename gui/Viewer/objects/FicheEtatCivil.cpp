#include <objects/FicheEtatCivil.hpp>

namespace salamandre
{
    const std::string FicheEtatCivil::fileName = "FEC";

    FicheEtatCivil::FicheEtatCivil(const unsigned int cli_id) : Fiche(cli_id)
    {
    }

    std::string FicheEtatCivil::getFilePath() const
    {
        return getDirPath()+fileName;
    }
}

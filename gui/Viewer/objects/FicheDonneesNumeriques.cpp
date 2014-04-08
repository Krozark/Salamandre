#include <objects/FicheDonneesNumeriques.hpp>

namespace salamandre
{
    
    FicheDonneesNumeriques::FicheDonneesNumeriques(const unsigned int cli_id) : Fiche(cli_id)
    {
    }

    std::string FicheDonneesNumeriques::getFilePath() const
    {
        return getDirPath()+"FMN";
    }
}

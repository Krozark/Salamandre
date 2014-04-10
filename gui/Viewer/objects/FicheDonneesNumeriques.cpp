#include <objects/FicheDonneesNumeriques.hpp>

namespace salamandre
{
    const std::string FicheDonneesNumeriques::fileName = "FMN";

    FicheDonneesNumeriques::FicheDonneesNumeriques(const unsigned int cli_id) : Fiche(cli_id)
    {
    }

    std::string FicheDonneesNumeriques::getFilePath() const
    {
        return getDirPath()+fileName;
    }
}

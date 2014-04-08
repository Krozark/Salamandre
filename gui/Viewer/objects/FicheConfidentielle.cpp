#include <objects/FicheConfidentielle.hpp>

namespace salamandre
{
    
    FicheConfidentielle::FicheConfidentielle(const unsigned int cli_id) : Fiche(cli_id)
    {
    }
    
    std::string FicheConfidentielle::getFilePath()const
    {
        return getDirPath()+"FCT";
    }
}

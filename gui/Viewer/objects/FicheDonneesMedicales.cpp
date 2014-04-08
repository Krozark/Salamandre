#include <objects/FicheDonneesMedicales.hpp>

namespace salamandre
{
    
    FicheDonneesMedicales::FicheDonneesMedicales(const unsigned int cli_id) : Fiche(cli_id)
    {
    }

    std::string FicheDonneesMedicales::getFilePath()const 
    {
        return getDirPath()+"FMT";
    }
}

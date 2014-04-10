#include <objects/FicheDonneesMedicales.hpp>

namespace salamandre
{
    const std::string FicheDonneesMedicales::fileName = "FMT";
    
    FicheDonneesMedicales::FicheDonneesMedicales(const unsigned int cli_id) : Fiche(cli_id)
    {
    }

    std::string FicheDonneesMedicales::getFilePath()const 
    {
        return getDirPath()+fileName;
    }
}

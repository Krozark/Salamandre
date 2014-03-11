#include <Salamandre/fiche/Fiche.hpp>

#include <crypto++/files.h>
#include <crypto++/default.h>

namespace salamandre
{
    
    Fiche::Fiche(const unsigned int cli_id) : related_cli(cli_id)
    {
    }


    void Fiche::encrypt(const char* pass)
    {
        CryptoPP::FileSource f(getFilePath().c_str(), true,
                               new CryptoPP::DefaultEncryptorWithMAC(pass, new CryptoPP::FileSink((getFilePath()+".eas").c_str())));

    }

    void Fiche::decrypt(const char* pass)
    {
        CryptoPP::FileSource f((getFilePath()+".eas").c_str(), true,
                               new CryptoPP::DefaultDecryptorWithMAC(pass, new CryptoPP::FileSink(getFilePath().c_str())));
    }

    void Fiche::stream(std::fstream& s)const
    {
        s.open(getFilePath(), std::fstream::in | std::fstream::out | std::fstream::binary);
    }

    std::string Fiche::getDirPath()const
    {
        //\todo TODO remplacer le "0" par l'id du medecin
        return "0/"+std::to_string(related_cli)+"/";
    }
}

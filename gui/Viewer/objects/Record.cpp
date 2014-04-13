#include <objects/Record.hpp>

#include <crypto++/files.h>
#include <crypto++/default.h>

namespace salamandre
{    
    Record::Record(const unsigned int pat_id) : related_pat(pat_id)
    {
    }


    void Record::encrypt(const char* pass)
    {
        CryptoPP::FileSource f(getFilePath().c_str(), true,
                               new CryptoPP::DefaultEncryptorWithMAC(pass, new CryptoPP::FileSink((getFilePath()+".eas").c_str())));
    }

    void Record::decrypt(const char* pass)
    {
        CryptoPP::FileSource f((getFilePath()+".eas").c_str(), true,
                               new CryptoPP::DefaultDecryptorWithMAC(pass, new CryptoPP::FileSink(getFilePath().c_str())));
    }

    void Record::stream(std::fstream& s)const
    {
        s.open(getFilePath(), std::fstream::in | std::fstream::out | std::fstream::binary);
    }

    std::string Record::getDirPath()const
    {
        //\todo TODO remplacer le "0" par l'id du medecin
        return "0/"+std::to_string(related_pat)+"/";
    }
}

#ifndef FUNCTIONS_HPP
#define FUNCTIONS_HPP

#include <string>

/**
 * \brief define some class used later
 */
namespace ntw
{
    namespace cli
    {
        class Client;
    }
}

namespace test
{

    void init();
    
    int random(int min,int max);
    
    /**
     * \param id_medecin the medecin id
     * \param id_patient the patient id
     * \param filepath the filename
     */
    void createFile(int id_medecin,int id_patient,const std::string& filename);

    /**
     * \brief start the client
     * \param client the client to start
     */
    void sendNewFile(ntw::cli::Client& client);

}

#endif

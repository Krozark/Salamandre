#ifndef FUNCTIONS_HPP
#define FUNCTIONS_HPP


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

/**
 * \brief start the client
 * \param client the client to start
 */
void run(ntw::cli::Client& client);

#endif

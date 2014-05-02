#ifndef SOCKSENDER_HPP
#define SOCKSENDER_HPP

#include <Socket/client/Client.hpp>
#include <Salamandre-daemon/GuiFunctions.hpp>

#define DEFAULT_SERVEUR_PORT 20001
#define DEFAULT_IP std::string("127.0.0.1")

class sockSender
{
public:


    static sockSender& getInstance()
    {
        static sockSender instance;
        return instance;
    }

    void init();
    bool connectToDaemon();
    void closeConnectionToDaemon();

private:
    sockSender();
    sockSender(sockSender const&);
    void operator=(sockSender const&);


    ntw::cli::Client client;
    int srvPort;
    std::string srvIpAddress;

};

#endif // SOCKSENDER_HPP

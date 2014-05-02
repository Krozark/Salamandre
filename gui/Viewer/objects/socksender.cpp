#include "socksender.hpp"

sockSender::sockSender()
{
    this->srvPort = srvPort;
    this->srvIpAddress = DEFAULT_IP;
}

void sockSender::init()
{
    ntw::Socket::init();
}

bool sockSender::connectToDaemon()
{
    bool res;
    if((res = client.connect(this->srvIpAddress, this->srvPort)) != NTW_ERROR_CONNEXION){
        std::cout << "Gui is now connect to daemon and ready to send request" << std::endl;
    }
    else{
        std::cerr << "Gui connection to daemon has failed" << std::endl;
    }

    return res;
}

void sockSender::closeConnectionToDaemon()
{
    ntw::Socket::close();
}



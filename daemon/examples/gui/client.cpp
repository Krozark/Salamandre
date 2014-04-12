#include "defines.hpp"
#include "functions.hpp"
///data base
#include <Socket/client/Client.hpp>


#define SERVER_HOST 1
#define SERVER_PORT 2

int main(int argc,char* argv[])
{
    if(argc < SERVER_PORT+1)
    {
        std::cout<<"Usage are: "<<argv[0]<<" <server-host> <server-port>"<<std::endl;
        return 1;
    }


    ntw::Socket::init();
    ntw::cli::Client client;
    if(client.connect(argv[SERVER_HOST],atoi(argv[SERVER_PORT])) != NTW_ERROR_CONNEXION)
        run(client);

    ntw::Socket::close();

    return 0;
}

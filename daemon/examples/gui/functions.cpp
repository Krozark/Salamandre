#include "functions.hpp"
#include "defines.hpp"

#include <Socket/SocketSerialized.hpp>
#include <Socket/client/Client.hpp>

#include <Salamandre-daemon/GuiFunctions.hpp>

#include <sstream>

void run(ntw::cli::Client& client)
{
    client.call<void>(salamandre::gui::func::newFile,-1,-1,"");

    short int status = client.request_sock.getStatus();
    switch(status)
    {
        case ERRORS::STOP :
        {
            std::cerr<<" : The server is probably down."<<std::endl;
            std::cout<<"[Recv] Stop"<<std::endl
                <<"The programme will now stop"<<std::endl;
            client.request_sock.clear();
        }break;
        default :
        {
            std::cout<<"[Recv] Server error code:"<<status<<":"<<salamandre::gui::statusToString((salamandre::gui::status)status)<<std::endl;
            client.request_sock.clear();
            /// server error???
        }break;
    }
}

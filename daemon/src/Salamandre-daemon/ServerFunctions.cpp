#include <Salamandre-daemon/ServerFunctions.hpp>

#include <Socket/FuncWrapper.hpp>
#include <Socket/client/Client.hpp>

#include <iostream>

namespace salamandre
{
namespace srv
{
    extern ntw::cli::Client gui_client_notif_sender;

    int dispatch(int id,ntw::SocketSerialized& request)
    {
        int res= ntw::FuncWrapper::Status::st::wrong_id;
        std::cout<<"[srv::dispatch] id:"<<id<<std::endl<<std::flush;

        switch(id)
        {
            case func::thisIsMyInfos :
            {
            }break;
        }

        return res;
    }
}
}

#include <Salamandre-daemon/ServerFunctions.hpp>

#include <Socket/FuncWrapper.hpp>

#include <iostream>

namespace salamandre
{
namespace srv
{
    int dispatch(int id,ntw::SocketSerialized& request)
    {
        int res= ntw::FuncWrapper::Status::st::wrong_id;
        std::cout<<"[srv::dispatch] id:"<<id<<std::endl<<std::flush;

        return res;
    }
}
}

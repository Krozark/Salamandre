#include <Salamandre-daemon/GuiFunctions.hpp>

#include <Socket/FuncWrapper.hpp>

#include <iostream>

namespace salamandre
{
namespace gui
{
    int dispatch(int id,ntw::SocketSerialized& request)
    {
        int res= ntw::FuncWrapper::Status::st::wrong_id;
        std::cout<<"[gui::dispatch] id:"<<id<<std::endl<<std::flush;

        return res;
    }
}   
}

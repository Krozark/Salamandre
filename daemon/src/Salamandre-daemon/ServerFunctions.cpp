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

        switch(id)
        {
            case func::thisIsMyInfos :
            {
            }break;
        }

        return res;
    }

    void funcILostMyData(int id_medecin,int id_patient,const std::string& filename,unsigned int port,const std::string& ip)
    {
    }
}
}

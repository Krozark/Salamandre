#include <Salamandre-daemon/GuiFunctions.hpp>

#include <Socket/FuncWrapper.hpp>

#include <iostream>
#include <sstream>

namespace salamandre
{
namespace gui
{
    std::string statusToString(salamandre::gui::status code)
    {
        std::string msg;

        switch(code)
        {
            case status::OK:
            {
                msg = "No errors";
            }break;
            case status::TODO:
            {
                msg = "functionality not made";
            }break;
            case status::WRONG_PARAM:
            {
                msg = "Wrong param value";
            }break;
            default:
            {
                msg = "Unknow code value";
            }break;
        }
        return msg;
    }

    int dispatch(int id,ntw::SocketSerialized& request)
    {
        int res= ntw::FuncWrapper::Status::st::wrong_id;
        std::cout<<"[gui::dispatch] id:"<<id<<std::endl<<std::flush;

        switch(id)
        {
            case func::newFile :
            {
                res = ntw::FuncWrapper::srv::exec(funcNewFile,request);
            }break;
            case func::sync :
            {
                res = ntw::FuncWrapper::srv::exec(funcSync,request);
            }break;
            default:
            {
                std::cout<<"[dispatch] Function id not found"<<std::endl;                
            }break;
        }
        return res;
    }

    void funcNewFile(ntw::SocketSerialized& sock,int id_medecin, int id_patient, std::string filepath)
    {
        if(id_medecin <=0)
        {
            sock.setStatus(status::WRONG_PARAM);
            return;
        }
        sock.setStatus(status::TODO);
    }

    void funcSync(ntw::SocketSerialized& sock,int id_medecin, int id_patient, std::string filename)
    {
        sock.setStatus(status::TODO);
    }


    std::string toPath(int id_medecin,int id_patient, std::string filename)
    {
        std::stringstream stream;
        stream<<id_medecin<<"/"<<id_patient<<"/"<<filename;
        return stream.str();
    }
}   
}

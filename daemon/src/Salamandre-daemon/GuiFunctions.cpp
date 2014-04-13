#include <Salamandre-daemon/GuiFunctions.hpp>

#include <Salamandre-daemon/FileManager.hpp>

#include <Socket/FuncWrapper.hpp>

#include <iostream>

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
            case status::ENABLE_TO_SEND_FILE :
            {
                msg = "Enable to save file on network";
            }break;
            default:
            {
                msg = "Unknow function id";
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

    void funcNewFile(ntw::SocketSerialized& sock,int id_medecin, int id_patient,std::string filename)
    {
        if(id_medecin <=0)
        {
            sock.setStatus(status::WRONG_PARAM);
            return;
        }
        
        bool status;
        if(id_patient > 0 and filename.size() > 1)
            status = FileManager::prepareForUpload(id_medecin,id_patient,filename);
        else if(id_patient > 0)
            status = FileManager::prepareForUpload(id_medecin,id_patient);
        else
            status = FileManager::prepareForUpload(id_medecin);

        if(status == false)
            sock.setStatus(gui::status::ENABLE_TO_SEND_FILE);
    }

    void funcSync(ntw::SocketSerialized& sock,int id_medecin, int id_patient, std::string filename)
    {
        sock.setStatus(gui::status::TODO);
    }


}   
}

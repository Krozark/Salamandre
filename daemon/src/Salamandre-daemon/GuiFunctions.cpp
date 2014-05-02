#include <Salamandre-daemon/GuiFunctions.hpp>

#include <Salamandre-daemon/FileManager.hpp>

#include <Socket/FuncWrapper.hpp>
#include <Socket/client/Client.hpp>

#include <iostream>

namespace salamandre
{
namespace gui
{
    ntw::cli::Client gui_client_notif_sender; ///< the socket that send notification to the client.

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
            case func::isInUpdate :
            {
                res = ntw::FuncWrapper::srv::exec(funcIsInUpdate,request);
            }break;
            case func::setGuiNotificationPort :
            {
                res = ntw::FuncWrapper::srv::exec(funcSetGuiNotificationPort,request);
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

    bool funcIsInUpdate(ntw::SocketSerialized& sock,int id_medecin, int id_patient,std::string filename)
    {
        if(id_medecin <=0)
        {
            sock.setStatus(status::WRONG_PARAM);
            return false;
        }
        bool status = false;
        ///\todo TODO
        return status;
    }

    void funcSetGuiNotificationPort(ntw::SocketSerialized& sock,unsigned int port)
    {
        gui_client_notif_sender.disconnect();
        gui_client_notif_sender.connect("127.0.0.1",port);
    }
}   
}

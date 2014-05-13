#include <Salamandre-daemon/GuiFunctions.hpp>
#include <Salamandre-daemon/FileManager.hpp>
#include <Salamandre-daemon/Daemon.hpp>
#include <Salamandre-daemon/define.hpp>

#include <Socket/FuncWrapper.hpp>

#include <utils/sys.hpp>
#include <utils/string.hpp>
#include <utils/log.hpp>

#include <unistd.h>
#include <iostream>

namespace salamandre
{
namespace gui
{

    int dispatch(int id,ntw::SocketSerialized& request)
    {
        int res= ntw::Status::wrong_id;
        utils::log::info("gui::dispatch","id",id);

        switch(id)
        {
            case func::newFile :
            {
                res = ntw::FuncWrapper::srv::exec(funcNewFile,request);
                //\todo TODO
                utils::log::info("Notifier","Send fake info with fileIsRecv");
                daemon->gui_client_notif_sender.call<void>(salamandre::gui::func::fileIsRecv,-1,-1,"test");
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
            case func::getMyBinPath :
            {
                res = ntw::FuncWrapper::srv::exec(funcGetMyBinPath,request);
            }break;
            case func::getMyBackupPath :
            {
                res = ntw::FuncWrapper::srv::exec(funcGetMyBackupPath,request);
            }break;
            case func::getMySavePath :
            {
                res = ntw::FuncWrapper::srv::exec(funcGetMySavePath,request);
            }
            default:
            {
                utils::log::error("gui::dispatch","Function of id",id,"not found");
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
        daemon->broadcaster.sendILostMyData(id_medecin,id_patient,filename);
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
        sock.setStatus(gui::status::TODO);
        return status;
    }

    std::string funcGetMyBinPath(ntw::SocketSerialized& sock)
    {
        return utils::string::join("/",utils::sys::dir::pwd(),DAEMON_NAME);
    }

    std::string funcGetMyBackupPath(ntw::SocketSerialized& sock)
    {
        return utils::string::join("/",utils::sys::dir::pwd(),FileManager::backup_file_dir_path);
    }

    std::string funcGetMySavePath(ntw::SocketSerialized& sock)
    {
        return utils::string::join("/",utils::sys::dir::pwd(),FileManager::new_file_dir_path);
    }

    void funcSetGuiNotificationPort(ntw::SocketSerialized& sock,unsigned int port)
    {
        utils::log::info("gui::funcSetGuiNotificationPort","Get qui notifier port:",port);
        daemon->gui_client_notif_sender.disconnect();
        if(daemon->gui_client_notif_sender.connect("127.0.0.1",port) != ntw::Status::connexion)
            utils::log::error("gui::funcSetGuiNotificationPort","Unable to connect to client notifier");

    }
}   
}

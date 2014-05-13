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
#include <chrono>

namespace salamandre
{
namespace gui
{
    class update {
        public:
        int id_medecin;
        int id_patient;
        std::string filename;
            
        update(int m,int p,const std::string& f):id_medecin(m),id_patient(p),filename(f)
        {
        }

    };

    std::mutex update_list_mutex;
    std::list<update> update_list;

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
        std::thread thread([id_medecin,id_patient,filename]()->void {
            daemon->broadcaster.sendILostMyData(id_medecin,id_patient,filename);

            update_list_mutex.lock();
            update tmp(id_medecin,id_patient,filename);
            update_list.push_back(tmp);
            update_list_mutex.unlock();
            //wait x sec
            std::this_thread::sleep_for(std::chrono::seconds(60));
            //askfiles

            update_list_mutex.lock();
            auto end = update_list.end();
            for(auto i=update_list.begin();i!=end;++i)
            {
                update& ii=*i;
                if(ii.id_medecin==id_medecin and ii.id_patient==id_patient and ii.filename==filename)
                {
                    update_list.erase(i);
                    break;
                }
            }
            update_list_mutex.unlock();
        });
        thread.detach();
    }

    bool funcIsInUpdate(ntw::SocketSerialized& sock,int id_medecin, int id_patient,std::string filename)
    {
        update_list_mutex.lock();
        auto end = update_list.end();
        bool res = false;
        if(id_medecin > 0 and id_patient > 0 and filename != "")
        {
            for(auto i=update_list.begin();i!=end;++i)
            {
                update& ii=*i;
                if(ii.id_medecin==id_medecin and ii.id_patient==id_patient and ii.filename==filename)
                {
                    res = true;
                    break;
                }
            }
        }
        else if (id_medecin > 0 and id_patient > 0)
        {
            for(auto i=update_list.begin();i!=end;++i)
            {
                update& ii=*i;
                if(ii.id_medecin==id_medecin and ii.id_patient==id_patient)
                {
                    res = true;
                    break;
                }
            }
        }
        else if (id_medecin > 0)
        {
            for(auto i=update_list.begin();i!=end;++i)
            {
                update& ii=*i;
                if(ii.id_medecin==id_medecin)
                {
                    res = true;
                    break;
                }
            }
        }
        else
        {
            res = update_list.size() > 0;
        }
        update_list_mutex.unlock();
        return res;
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

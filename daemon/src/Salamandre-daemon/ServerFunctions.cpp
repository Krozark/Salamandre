#include <Salamandre-daemon/ServerFunctions.hpp>
#include <Salamandre-daemon/GuiFunctions.hpp>
#include <Salamandre-daemon/FileManager.hpp>
#include <Salamandre-daemon/Daemon.hpp>

#include <Socket/FuncWrapper.hpp>
#include <Socket/client/Client.hpp>
//#include <Socket/server/Client.hpp>
//#include <Socket/server/Server.hpp>
#include <Socket/Status.hpp>


#include <utils/string.hpp>
#include <utils/log.hpp>
#include <utils/sys.hpp>

#include <iostream>
#include <sys/file.h>


namespace salamandre
{
namespace srv
{

    static int _ask_for_file_nb = 0;

    int dispatch(int id,ntw::SocketSerialized& request)
    {
        int res= ntw::Status::wrong_id;
        utils::log::info("srv::dispatch","id:",id);

        switch(id)
        {
            case func::thisIsMyFiles :
            {
                res = ntw::FuncWrapper::srv::exec(funcThisIsMyFiles_Recv,request);
            }break;
            case func::saveThisFile :
            {
                int id_medecin,id_patient;
                std::string filename;

                request>>id_medecin
                    >>id_patient
                    >>filename;

                std::string dir_path = utils::string::join("/",FileManager::backup_file_dir_path,id_medecin,id_patient);
                utils::sys::dir::create(dir_path);
                std::string file_path = utils::string::join("/",dir_path,filename);
                request.save(file_path);

                request.clear();
                request.setStatus(ntw::Status::ok);
                request.sendCl();
                
                daemon->notify(salamandre::gui::func::fileIsRecv,id_medecin,id_patient,filename);

            }break;
            case func::sendThisFile :
            {
               res = ntw::FuncWrapper::srv::exec(funcSendThisFile_Recv,request); 
            }break;
            default :
            {
                utils::log::error("srv::dispatch","Function of id",id,"not found");
            }break;
        }
        return res;
    }


    void funcILostMyData_BroadcastRecv(int id_medecin,int id_patient,const std::string& filename,unsigned int port,const std::string& ip)
    {
        ntw::cli::Client client;
        if(client.connect(ip,port) == ntw::Status::ok)
        {
            std::list<FileInfo> file_list = FileManager::list(id_medecin,id_patient,filename);

            client.call<void>(thisIsMyFiles,daemon->getServerPort(),file_list);

            client.disconnect();
        }
    }

    void funcThisIsMyFiles_Recv(ntw::SocketSerialized& request,int port,std::list<FileInfo> files)
    {
        utils::log::info("srv::funcThisIsMyFiles_Recv","Recv datas ip:",request.getIp(),"port: ",port);

        file_info_mutex.lock();
        for(auto& f : files)
        {
            FileInfoFrom tmp = {
                .version=f.version,
                .id_medecin=f.id_medecin,
                .id_patient=f.id_patient,
                .filename=f.filename,
                .port=port,
                .ip=request.getIp()
            };

            file_info_from.push_back(std::move(tmp));
        }
        file_info_mutex.unlock();
    }

    void funcSendThisFile_Recv(ntw::SocketSerialized& request,int id_medecin,int id_patient,std::string filename)
    {
        std::string path = utils::string::join("/",FileManager::backup_file_dir_path,id_medecin,id_patient,filename);
        FILE* f = ::fopen(path.c_str(),"rb");
        if(f != nullptr)
        {
            if(::flock(::fileno(f),LOCK_EX) == 0)
            {
                utils::log::info("ServerFunctions::funcSendThisFile_Recv",path);
                ::fseek(f,0,SEEK_SET);
                char buf[BUFSIZ];
                size_t size; //file size
                request<<salamandre::srv::saveThisFile;
                //add file datas
                while ((size = ::fread(buf, 1, BUFSIZ,f)) > 0)
                    request.write(buf,size);

                ::flock(::fileno(f),LOCK_UN);
            }
            else
                request.setStatus(gui::ENABLE_TO_SEND_FILE);
            ::fclose(f);
        }
        else
            request.setStatus(gui::ENABLE_TO_SEND_FILE);
    }


    bool askFile(const FileInfoFrom& info)
    {
        utils::log::info("ServerFunctions::askFile","to ip:",info.ip,"port:",info.port,"id_medecin:",info.id_medecin,"id_patient",info.id_patient,"filename",info.filename);
        
        ntw::cli::Client* client = new ntw::cli::Client;
        if(client->connect(info.ip,info.port) == ntw::Status::ok)
        {
            std::thread thread([info,client]()->void {
                ask_for_file_mutex.lock();
                ++_ask_for_file_nb;
                ask_for_file_mutex.unlock();

                client->call<void>(sendThisFile,info.id_medecin,info.id_patient,info.filename);
                int st = client->request_sock.getStatus();

                if(st == ntw::Status::ok)
                {
                    int f;
                    client->request_sock>>f;
                    if(f == func::saveThisFile)
                    {
                        std::string path = utils::string::join("/",FileManager::backup_file_dir_path,info.id_medecin,info.id_patient);
                        if(utils::sys::dir::create(path))
                        {
                            //TODO lock file
                            path = utils::string::join("/",path,info.filename);
                            FILE* f = ::fopen(path.c_str(),"wb");
                            if(f != nullptr)
                            {
                                if(::flock(::fileno(f),LOCK_EX) == 0)
                                {
                                    char buf[BUFSIZ];
                                    auto size_to_read = client->request_sock.size();
                                    while(size_to_read >= BUFSIZ)
                                    {
                                        client->request_sock.read(buf,BUFSIZ);
                                        size_to_read-=BUFSIZ;
                                        ::fwrite(buf,1,BUFSIZ,f);
                                    }

                                    if(size_to_read > 0)
                                    {
                                        client->request_sock.read(buf,size_to_read);
                                        ::fwrite(buf,1,size_to_read,f);
                                    }
                                    ::flock(::fileno(f),LOCK_UN);
                                    daemon->notify(salamandre::gui::func::fileIsRecv,info.id_medecin,info.id_patient,info.filename);
                                }
                                ::fclose(f);
                            }
                        }
                    }
                }
                delete client;

                ask_for_file_mutex.lock();
                --_ask_for_file_nb;
                ask_for_file_mutex.unlock();
            });
            thread.detach();
            return true;
        }
        else
            delete client;
        return false;
    }

    void askForFile_helper(int id_medecin)
    {
        const auto end = file_info_from.end();
        auto current = file_info_from.begin();
        while(current != end)
        {
            FileInfoFrom& f = *current;
            ++current;
            if(f.id_medecin == id_medecin)
                if(askFile(f))
                    // skip the nex one
                    while(current != end
                          and current->id_medecin == id_medecin
                          and current->id_patient == f.id_patient
                          and current->filename == f.filename)
                        ++current;
            if(current->id_medecin > id_medecin)
                break;
        }
    }

    void askForFile_helper(int id_medecin,int id_patient)
    {
        const auto end = file_info_from.end();
        auto current = file_info_from.begin();
        while(current != end)
        {
            FileInfoFrom& f = *current;
            ++current;
            if(f.id_medecin == id_medecin and f.id_patient == id_patient)
                if(askFile(f))
                    // skip the nex one
                    while(current != end
                          and current->id_medecin == id_medecin
                          and current->id_patient == id_patient
                          and current->filename == f.filename)
                        ++current;
            if(current->id_medecin == id_medecin and current->id_patient > id_patient)
                break;
        }
    }

    void askForFile_helper(int id_medecin,int id_patient,const std::string& filename)
    {
        const auto end = file_info_from.end();
        auto current = file_info_from.begin();
        while(current != end)
        {
            FileInfoFrom& f = *current;
            ++current;
            if(f.id_medecin == id_medecin and f.id_patient == id_patient and f.filename == filename)
                if(askFile(f))
                    // skip the nex one
                    while(current != end
                          and current->id_medecin == id_medecin
                          and current->id_patient == id_patient
                          and current->filename == filename)
                        ++current;
            if(current->id_medecin == id_medecin and current->id_patient == id_patient and current->filename > filename)
                break;
        }
    }

    void askForFile(int id_medecin,int id_patient,const std::string& filename)
    {
        file_info_mutex.lock();

        file_info_from.sort([](const FileInfoFrom& _1,const FileInfoFrom& _2)->bool {
            if(_1.id_medecin == _2.id_medecin) {
                if(_1.id_patient == _2.id_patient) {
                    if(_1.filename == _2.filename) {
                        return _1.version < _2.version;
                    }
                    return _1.filename < _2.filename;
                }
                return _1.id_patient < _2.id_medecin;
            }
            return _1.id_medecin < _2.id_medecin;
        });

        std::cout<<"askForFile sorted list:"<<id_medecin<<" "<<id_patient<<" "<<filename<<std::endl;
        for(auto& f : file_info_from)
            std::cout<<"version: "<<f.version
                <<" id_medecin: "<<f.id_medecin
                <<" id_patient: "<<f.id_patient
                <<" filename: "<<f.filename
                <<" ip:port: "<<f.ip<<":"<<f.port
                <<std::endl;
        
        if(id_medecin > 0 and id_patient > 0 and filename != "")
            askForFile_helper(id_medecin,id_patient,filename);
        else if (id_medecin > 0 and id_patient > 0)
            askForFile_helper(id_medecin,id_patient);
        else if (id_medecin > 0)
            askForFile_helper(id_medecin);

        file_info_from.clear();
        file_info_mutex.unlock();
    }

    int ask_for_file_nb()
    {
        return _ask_for_file_nb;
    }
}
}

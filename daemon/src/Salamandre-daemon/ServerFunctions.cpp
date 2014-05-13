#include <Salamandre-daemon/ServerFunctions.hpp>
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

            }break;
            default :
            {
                utils::log::error("srv::dispatch","Function of id",id,"not found");
            }break;
        }
        return res;
    }

    /*void on_delete_client(ntw::srv::Server& self,ntw::srv::Client& client)
    {
        utils::log::todo("on_delete_client","TEST");
        file_info_mutex.lock();
        file_info_from.remove_if([&client,&self](const FileInfoFrom& f)->bool{
            return f.port == self.port() and f.ip == client.sock().getIp();
        }); 

        file_info_mutex.unlock();
    }*/

    void funcILostMyData_BroadcastRecv(int id_medecin,int id_patient,const std::string& filename,unsigned int port,const std::string& ip)
    {
        ntw::cli::Client client;
        if(client.connect(ip,port) == ntw::Status::ok)
        {
            //std::cout<<"srv::funcILostMyData_BroadcastRecv("<<id_medecin<<","<<id_patient<<","<<filename<<")"<<std::endl;
            std::list<FileInfo> file_list = FileManager::list(id_medecin,id_patient,filename);
            client.call<void>(thisIsMyFiles,daemon->file_server.port(),file_list);

            /*if(client.request_sock.getStatus() != ntw::Status::stop)
            {
                client.request_sock.receive();//sendFile
                int id;
                client.request_sock>>id;

                if(id == sendFile)
                {
                    std::list<FileManager::FileInfo> to_send;
                    client.request_sock>>to_send;
                    client.request_sock.clear();

                    for(FileManager::FileInfo& file : to_send)
                    {
                        std::string file_path = utils::string::join("/",file.id_medecin,file.id_patient,file.filename);
                        FILE* source = ::fopen(file_path.c_str(), "rb");
                        if(source != NULL)
                        {
                            if(flock(::fileno(source),LOCK_EX) == 0)
                            {
                                client.request_sock<<sendFile //f id
                                    <<file;//file info
                                ::fseek(source,0,SEEK_SET);
                                char buf[BUFSIZ];
                                size_t size;
                                while ((size = ::fread(buf, 1, BUFSIZ, source)) > 0)
                                    client.request_sock.write(buf,size);
                                //unlock it
                                ::flock(::fileno(source), LOCK_UN);
                                client.request_sock.sendCl();
                                if(client.request_sock.getStatus() == ntw::Status::stop)
                                    break;
                            }
                            ::fclose(source);
                        }
                    }
                }
            }*/
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

            std::cout<<"version: "<<f.version
                <<" id_medecin: "<<f.id_medecin
                <<" id_patient: "<<f.id_patient
                <<" filename: "<<f.filename
                <<std::endl;
        }
        file_info_mutex.unlock();
    }


    bool askFile(const FileInfoFrom& info)
    {
        ntw::cli::Client* client = new ntw::cli::Client;
        if(client->connect(info.ip,info.port) == ntw::Status::ok)
        {
            std::thread thread([info,client]()->void{
                client->call<void>(sendThisFile,info.id_medecin,info.id_patient,info.filename);
                int st = client->request_sock.getStatus();
                if(st != ntw::Status::wrong_id)
                {
                    std::string path = utils::string::join("/",FileManager::backup_file_dir_path,info.id_medecin,info.id_patient);
                    if(utils::sys::dir::create(path))
                        client->request_sock.save(path+"/"+info.filename);
                }
                delete client;
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
            FileInfoFrom& i = *current;
            int m = i.id_medecin;
            if(m == id_medecin)
            {
                int p = i.id_patient;
                std::string filename = i.filename;
                if (askFile(i))
                {
                    while(p== current->id_patient and filename == current->filename)
                    {
                        p = current->id_patient;
                        filename = current->filename;
                        ++current;
                    }
                }
            }
            else if(m > id_medecin)
                break;
            ++current;
        }
    }

    void askForFile_helper(int id_medecin,int id_patient)
    {
    }

    void askForFile_helper(int id_medecin,int id_patient,const std::string& filename)
    {
    }

    void askForFile(int id_medecin,int id_patient,const std::string& filename)
    {
        file_info_mutex.lock();

        file_info_from.sort([](const FileInfoFrom& _1,const FileInfoFrom& _2)->bool {
            if(_1.id_medecin == _2.id_medecin){
                if(_1.id_patient == _2.id_patient){
                    if(_1.filename == _2.filename){
                        return _1.version < _2.version;
                    }
                    return _1.filename < _2.filename;
                }
                return _1.id_patient < _2.id_medecin;
            }
            return _1.id_medecin < _2.id_medecin;
        });
        
        if(id_medecin > 0 and id_patient > 0 and filename != "")
            askForFile_helper(id_medecin,id_patient,filename);
        else if (id_medecin > 0 and id_patient > 0)
            askForFile_helper(id_medecin,id_patient);
        else if (id_medecin > 0)
            askForFile_helper(id_medecin);

        file_info_mutex.unlock();
    }
}
}

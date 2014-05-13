#include <Salamandre-daemon/ServerFunctions.hpp>
#include <Salamandre-daemon/FileManager.hpp>

#include <Socket/FuncWrapper.hpp>
#include <Socket/client/Client.hpp>
#include <Socket/Status.hpp>

#include <utils/string.hpp>
#include <utils/log.hpp>

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
            case func::sendFile :
            {
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
            //std::cout<<"srv::funcILostMyData_BroadcastRecv("<<id_medecin<<","<<id_patient<<","<<filename<<")"<<std::endl;
            std::list<FileManager::FileInfo> file_list = FileManager::list(id_medecin,id_patient,filename);
            client.call<void>(thisIsMyFiles,file_list);
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

    void funcThisIsMyFiles_Recv(ntw::SocketSerialized& request,std::list<FileManager::FileInfo> files)
    {
        utils::log::info("srv::funcThisIsMyFiles_Recv","Recv datas");
        file_info_mutex.lock();
        for(auto& f : files)
        {
            FileInfoFrom tmp = {
                .version=f.version,
                .id_medecin=f.id_medecin,
                .id_patient=f.id_patient,
                .filename=f.filename,
                .request=&request
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
}
}

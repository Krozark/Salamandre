#include <Salamandre-daemon/ServerFunctions.hpp>
#include <Salamandre-daemon/FileManager.hpp>

#include <Socket/FuncWrapper.hpp>
#include <Socket/client/Client.hpp>
#include <Socket/Status.hpp>

#include <iostream>

namespace salamandre
{
namespace srv
{
    int dispatch(int id,ntw::SocketSerialized& request)
    {
        int res= ntw::Status::wrong_id;
        std::cout<<"[srv::dispatch] id:"<<id<<std::endl<<std::flush;

        switch(id)
        {
            case func::thisIsMyInfos :
            {
            }break;
        }

        return res;
    }

    void funcILostMyData_BroadcastRecv(int id_medecin,int id_patient,const std::string& filename,unsigned int port,const std::string& ip)
    {
        ntw::cli::Client client;
        if(client.connect(ip,port) == ntw::Status::ok)
        {
            std::list<FileManager::FileInfo> file_list = FileManager::list(id_medecin,id_patient,filename);
            client.call<void>(thisIsMyFiles,file_list);
            if(client.request_sock.getStatus() != ntw::Status::stop)
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
                        /*std::string file_path = utils::string::join("/",std::vector<std::string>({std::to_string(file.id_medecin),
                                                                                                 std::to_string(file.id_patient),
                                                                                                 file.filename}));*/
                    }
                }
                else
                {
                }
            }
            client.disconnect();
        }
    }
}
}

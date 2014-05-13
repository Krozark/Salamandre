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
            }
            client.disconnect();
        }
    }
}
}

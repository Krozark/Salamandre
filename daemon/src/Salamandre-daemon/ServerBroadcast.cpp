#include <Salamandre-daemon/ServerBroadcast.hpp>
#include <Salamandre-daemon/ServerFunctions.hpp>
#include <Socket/FuncWrapper.hpp>



namespace salamandre
{
    ServerBroadcast::ServerBroadcast(int port,int server_port):
        port(port),
        server_port(server_port),
        run(false),
        sock_listen(ntw::Socket::Dommaine::IP,ntw::Socket::Type::UDP,IPPROTO_UDP),
        sock_send(ntw::Socket::Dommaine::IP,ntw::Socket::UDP,IPPROTO_UDP)
    {
        sock_listen.connect("127.255.255.255",port);
        sock_listen.setReusable(true);
        sock_listen.bind();

        sock_send.connect(port);
        sock_send.setBroadcast(true);
        sock_send.connect("255.255.255.255",port);

    }

    void ServerBroadcast::start()
    {
        run = true;
        this->thread = std::thread(&ServerBroadcast::start_thread,this);
        //thread for broadcast.sendThisIsMyInfo(server_port);
    }

    void ServerBroadcast::wait()
    {
        this->thread.join();
    }

    
    void ServerBroadcast::sendThisIsMyInfo()
    {
        sock_send<<(int)salamandre::srv::thisIsMyInfos
            <<server_port;
        sock_send.send(sock_listen);
        sock_send.clear();
    }

    void ServerBroadcast::sendILostMyData(int id_medecin,int id_patient,std::string filename)
    {
        sock_send<<(int)salamandre::srv::iLostMyData
            <<id_medecin
            <<id_patient
            <<filename
            <<server_port;
        std::cout<<"Send:"<<sock_send<<std::endl;
        sock_send.send(sock_listen);
        sock_send.clear();
    }

    void ServerBroadcast::start_thread()
    {
        while(run)
        {
            sock_listen.clear();
            ntw::SocketSerialized from(ntw::Socket::Dommaine::IP,ntw::Socket::Type::UDP);
            from.connect(port);
            sock_listen.receive(from);
            std::cout<<sock_listen<<std::endl;
            int id;
            sock_listen>>id;

            std::cout<<"Recv id "<<id<<std::endl;

            switch(id)
            {
                case salamandre::srv::func::thisIsMyInfos :
                    {
                        int port;
                        sock_listen>>port;
                        funcThisIsMyInfos(from,port);
                    }break;
                case salamandre::srv::func::iLostMyData :
                    {
                        int id_medecin,id_patient,port;
                        std::string filename;
                        sock_listen>>id_medecin>>id_patient>>filename>>port;
                        funcILostMyData(from,id_medecin,id_patient,filename,port);
                    }break;
                default:
                    ///\todo WTF are you doing?
                    break;
            }
        }
    }

    void ServerBroadcast::stop()
    {
        run = false;
    }

    void ServerBroadcast::funcThisIsMyInfos(ntw::SocketSerialized& from,int port)
    {
        std::cout<<"funcThisIsMyInfos From:"<<from.getIp()<<", Port"<<port<<std::endl;
        ///TODO add in node
    }

    void ServerBroadcast::funcILostMyData(ntw::SocketSerialized& from,int id_medecin,int id_patient,std::string filename,int port)
    {
        std::cout<<"funcILostMyData from:"<<from.getIp()<<" id_medecin:"<<id_medecin<<" id_patient:"<<id_patient<<" filename:"<<filename<<" port:"<<port<<std::endl;
        std::thread t(salamandre::srv::funcILostMyData,id_medecin,id_patient,filename,port,from.getIp());
        t.detach();
    }
    
}

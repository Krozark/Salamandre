#include <Salamandre-daemon/ServerBroadcast.hpp>
#include <Salamandre-daemon/ServerFunctions.hpp>
#include <Socket/FuncWrapper.hpp>



namespace salamandre
{
    ServerBroadcast::ServerBroadcast(int port):
        port(port),
        run(false),
        sock_listen(ntw::Socket::Dommaine::IP,ntw::Socket::Type::UDP,IPPROTO_UDP),
        sock_send(ntw::Socket::Dommaine::IP,ntw::Socket::UDP,IPPROTO_UDP)
    {
        sock_send.connect("127.255.255.255",port);
        sock_send.setBroadcast(true);

        sock_listen.connect(port);
        //sock_listen.setBroadcast(true);
        sock_listen.setReusable(true);
        sock_listen.bind();
    }

    void ServerBroadcast::start()
    {
        run = true;
        this->thread = std::thread(&ServerBroadcast::start_thread,this);
    }

    void ServerBroadcast::wait()
    {
        this->thread.join();
    }

    
    void ServerBroadcast::sendThisIsMyInfo(int port)
    {
        sock_send<<(int)salamandre::srv::thisIsMyInfos
            <<port;
        sock_send.sendCl();
    }

    void ServerBroadcast::sendILostMyData(int id_medecin,int id_patient,std::string filename,int port)
    {
        sock_send<<"test";
        /*
        sock_send<<(int)salamandre::srv::iLostMyData
            <<id_medecin
            <<id_patient
            <<filename
            <<port;
        */
        std::cout<<sock_send<<std::endl;
        sock_send.sendCl();
    }

    void ServerBroadcast::start_thread()
    {
        while(run)
        {
            sock_listen.clear();
            ntw::SocketSerialized from(ntw::Socket::Dommaine::IP,ntw::Socket::Type::UDP);
            from.connect(port);
            sock_listen.receive();
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
        std::string from_ip = from.getIp();
        std::cout<<"funcThisIsMyInfos"<<std::endl;
    }

    void ServerBroadcast::funcILostMyData(ntw::SocketSerialized& from,int id_medecin,int id_patient,std::string filename,int port)
    {
        std::cout<<"funcILostMyData"<<std::endl;

    }
    
}

#include <Salamandre-daemon/ServerBroadcast.hpp>
#include <Salamandre-daemon/ServerFunctions.hpp>
#include <Socket/FuncWrapper.hpp>

#include <thread>


namespace salamandre
{
    ServerBroadcast::ServerBroadcast(int port):
        port(port),
        run(false),
        sock_listen(ntw::Socket::Dommaine::IP,ntw::Socket::Type::UDP),
        sock_send(ntw::Socket::Dommaine::IP,ntw::Socket::UDP)
    {
    }

    void ServerBroadcast::start()
    {
        std::thread t(&ServerBroadcast::start_thread,this);
        t.join();
    }

    void ServerBroadcast::start_thread()
    {
        while(run)
        {
            ntw::SocketSerialized from(ntw::Socket::Dommaine::IP,ntw::Socket::Type::UDP);
            sock_listen.receive(from);
            int id;
            sock_listen>>id;

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
                        int id_medecin,id_patient;
                        std::string filename;
                        sock_listen>>id_medecin>>id_patient>>filename;
                        funcILostMyData(from,id_medecin,id_patient,filename);
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
    }

    void ServerBroadcast::funcILostMyData(ntw::SocketSerialized& from,int id_medecin,int id_patient,std::string filename)
    {

    }
    
}

#include <Salamandre-daemon/ServerBroadcast.hpp>
#include <Salamandre-daemon/ServerFunctions.hpp>
#include <Socket/FuncWrapper.hpp>

#include <Salamandre-stats/stats.hpp>

#include <chrono>

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

    void ServerBroadcast::init()
    {
        stats::Stats::init();
    }

    void ServerBroadcast::close()
    {
        stats::Stats::close();
    }

    void ServerBroadcast::start()
    {
        run = true;
        this->thread[0] = std::thread(&ServerBroadcast::start_listener,this);
        this->thread[1] = std::thread(&ServerBroadcast::start_sender,this);
    }

    void ServerBroadcast::wait()
    {
        this->thread[0].join();
        this->thread[1].join();
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

    void ServerBroadcast::start_listener()
    {
        while(this->run)
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

    void ServerBroadcast::start_sender()
    {
        const std::chrono::seconds duration(30);
        const std::chrono::milliseconds step(500);

        std::chrono::milliseconds elapsed_time(0);

        sendThisIsMyInfo();

        while(this->run)
        {
            if (elapsed_time > duration)
            {
                sendThisIsMyInfo();
                elapsed_time = std::chrono::milliseconds(0);
            }
            std::this_thread::sleep_for(step);
            elapsed_time += step;
        }
    }

    void ServerBroadcast::stop()
    {
        run = false;
        sock_listen.shutdown();
    }

    void ServerBroadcast::funcThisIsMyInfos(ntw::SocketSerialized& from,int port)
    {
        std::cout<<"Recv info from:"<<from.getIp()<<", Port"<<port<<std::endl;
        ///\todo TODO check if this is me
        stats::Stats::add_node(from.getIp(),port);
    }

    void ServerBroadcast::funcILostMyData(ntw::SocketSerialized& from,int id_medecin,int id_patient,std::string filename,int port)
    {
        std::cout<<"funcILostMyData from:"<<from.getIp()<<" id_medecin:"<<id_medecin<<" id_patient:"<<id_patient<<" filename:"<<filename<<" port:"<<port<<std::endl;
        std::thread t(salamandre::srv::funcILostMyData,id_medecin,id_patient,filename,port,from.getIp());
        t.detach();
    }
    
}

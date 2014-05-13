#include <Salamandre-daemon/ServerBroadcast.hpp>
#include <Salamandre-daemon/ServerFunctions.hpp>
#include <Socket/FuncWrapper.hpp>

#include <Salamandre-stats/stats.hpp>

#include <utils/log.hpp>

#include <chrono>

#include <ifaddrs.h>

namespace salamandre
{
    ServerBroadcast::ServerBroadcast(int port,int server_port,bool local):
        port(port),
        server_port(server_port),
        run(false),
        sock_listen(ntw::Socket::Domain::IP,ntw::Socket::Type::UDP, IPPROTO_UDP),
        sock_send(ntw::Socket::Domain::IP,ntw::Socket::Type::UDP, IPPROTO_UDP)
    {
        sock_listen.connect(port);
        sock_listen.setReusable(true);
        sock_listen.bind();

        if(local)
            sock_send.connect("127.255.255.255", port);
        else
            sock_send.connect("255.255.255.255", port);
        if(!sock_send.setBroadcast(true))
        {
            ::perror("Unable to set enable broadcast: ");
        }

        this->getaddrs();
    }

    void ServerBroadcast::getaddrs()
    {
        struct ifaddrs *ifaddr, *ifa;
        sockaddr_in * address;
        if (::getifaddrs(&ifaddr) != 0)
        {
            ::perror("Unable to get interface addrs: ");
        }

        for (ifa = ifaddr; ifa != NULL; ifa = ifa->ifa_next)
        {
            if (ifa->ifa_addr == NULL)
            {
                continue;
            }

            if (ifa->ifa_addr->sa_family == AF_INET)
            { // We don't support IPv6, yet
                address = (sockaddr_in*) ifa->ifa_addr;
                this->my_ips.insert(address->sin_addr.s_addr);
                utils::log::info("ServerBroadcast::getaddrs","Adding", inet_ntoa(address->sin_addr), "to my known ip addresses.");
            }
        }
        ::freeifaddrs(ifaddr);
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
        sock_send.send();
        sock_send.clear();
    }

    void ServerBroadcast::sendILostMyData(int id_medecin,int id_patient,std::string filename)
    {
        sock_send<<(int)salamandre::srv::iLostMyData
            <<id_medecin
            <<id_patient
            <<filename
            <<server_port;
        sock_send.send();
        sock_send.clear();
    }

    void ServerBroadcast::start_listener()
    {
        while(this->run)
        {
            sock_listen.clear();
            ntw::SocketSerialized from(ntw::Socket::Domain::IP,ntw::Socket::Type::UDP);
            from.connect(port);
            sock_listen.receive(from);
            int id;
            sock_listen>>id;
            utils::log::info("ServerBroadcast::listen","Recv id ",id,"with status",sock_listen.getStatus());

            if(sock_listen.getStatus() == ntw::Status::stop) {
                utils::log::warning("ServerBroadcast::listen","Whether we received shutdown");
                continue;
            }

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
        std::string remote_ip = from.getIp();
        utils::log::info("ServerBroadcast::funcThisIsMyInfos", "Recv info from", remote_ip, " Port",port);
        in_addr_t remote_addr = ::inet_addr(remote_ip.c_str());

        // We don't add messages from ourselves
        if (this->my_ips.find(remote_addr) == this->my_ips.end() || port != this->server_port) {
            utils::log::info("ServerBroadcast::funcThisIsMyInfos", "Adding", remote_ip, "port", port, "to the known nodes.");
            stats::Stats::add_node(remote_ip, port);
        } else {
            utils::log::info("ServerBroadcast::funcThisIsMyInfos", "Not adding", remote_ip, "port", port, ": This is me!");
        }
    }

    void ServerBroadcast::funcILostMyData(ntw::SocketSerialized& from,int id_medecin,int id_patient,std::string filename,int port)
    {
        utils::log::info("ServerBroadcast::funcILostMyData","Recv from:",from.getIp()," id_medecin:",id_medecin," id_patient:",id_patient," filename:",filename," port:",port);
        std::thread t(salamandre::srv::funcILostMyData_BroadcastRecv,id_medecin,id_patient,filename,port,from.getIp());
        t.detach();
    }
    
}

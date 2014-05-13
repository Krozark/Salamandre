#ifndef SALAMANDRE_SERVERBROADCAST_HPP
#define SALAMANDRE_SERVERBROADCAST_HPP

#include <Socket/SocketSerialized.hpp>
#include <string>
#include <thread>
#include <unordered_set>

namespace salamandre
{
    class ServerBroadcast
    {
        public:
            ServerBroadcast(int port,int server_port,bool local);
            ServerBroadcast(const ServerBroadcast&) = delete;
            ServerBroadcast& operator=(const ServerBroadcast&) = delete;

            //function ids in ServerFunctions.hpp

            static void init();
            static void close();

            void start();
            void wait();
            void stop();

            void sendThisIsMyInfo();

            void sendILostMyData(int id_medecin,int id_patient,std::string filename);

        private:
            void start_listener();
            void start_sender();

            void getaddrs();

            std::thread thread[2]; //[0] = listener, [1] = sender for info

            const int port;
            const int server_port;
            bool run;
            ntw::SocketSerialized sock_listen;
            ntw::SocketSerialized sock_send;

            std::unordered_set<in_addr_t> my_ips;

            /**
             * \param port port to connect to
             */
            void funcThisIsMyInfos(ntw::SocketSerialized& from,int port);
            
            void funcILostMyData(ntw::SocketSerialized& from,int id_medecin,int id_patient,std::string filename,int port);
    };
}
#endif

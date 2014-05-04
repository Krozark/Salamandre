#ifndef SALAMANDRE_SERVERBROADCAST_HPP
#define SALAMANDRE_SERVERBROADCAST_HPP

#include <Socket/SocketSerialized.hpp>
#include <string>
#include <thread>

namespace salamandre
{
    class ServerBroadcast
    {
        public:
            ServerBroadcast(int port,int server_port);
            ServerBroadcast(const ServerBroadcast&) = delete;
            ServerBroadcast& operator=(const ServerBroadcast&) = delete;

            //function ids in ServerFunctions.hpp

            void start();
            void wait();
            void stop();

            void sendThisIsMyInfo();

            void sendILostMyData(int id_medecin,int id_patient,std::string filename);

        private:
            void start_thread();

            std::thread thread;

            const int port;
            const int server_port;
            bool run;
            ntw::SocketSerialized sock_listen;
            ntw::SocketSerialized sock_send;

            /**
             * \param port port to connect to
             */
            void funcThisIsMyInfos(ntw::SocketSerialized& from,int port);
            
            void funcILostMyData(ntw::SocketSerialized& from,int id_medecin,int id_patient,std::string filename,int port);
    };
}
#endif

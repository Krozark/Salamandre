#ifndef SALAMANDRE_SERVERBROADCAST_HPP
#define SALAMANDRE_SERVERBROADCAST_HPP

#include <Socket/SocketSerialized.hpp>
#include <string>

namespace salamandre
{
    class ServerBroadcast
    {
        public:
            ServerBroadcast(int port);
            ServerBroadcast(const ServerBroadcast&) = delete;
            ServerBroadcast& operator=(const ServerBroadcast&) = delete;

            //function ids in ServerFunctions.hpp

            void start();
            void stop();

        private:
            void start_thread();

            const int port;
            bool run;
            ntw::SocketSerialized sock_listen;
            ntw::SocketSerialized sock_send;

            /**
             * \param port port to connect to
             */
            void funcThisIsMyInfos(ntw::SocketSerialized& from,int port);
            
            void funcILostMyData(ntw::SocketSerialized& from,int id_medecin,int id_patient,std::string filename);
    };
}
#endif

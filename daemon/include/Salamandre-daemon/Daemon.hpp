#ifndef SALAMANDRE_DAEMON_HPP
#define SALAMANDRE_DAEMON_HPP

#include <Socket/server/Server.hpp>
#include <Socket/client/Client.hpp>
#include <Salamandre-daemon/ServerBroadcast.hpp>
#include <Salamandre-daemon/FileManager.hpp>

/**
 * \brief namespace for the project
 */
namespace salamandre
{
    /***
     * \brief This is the main class for the daemon.
     * contain all the socket servers, and deal with them.
     */
    class Daemon
    {
        public:
            /**
             * \brief The deamon class
             * \param listen_gui_port the port for the gui
             * \param listen_port the port to listen the others deamon
            */
            Daemon(int listen_gui_port, int listen_port, int broadcast_port);
            Daemon(const Daemon&) = delete;
            Daemon& operator=(const Daemon&) = delete;


            /**
             * \brief start the socket servers
             */
            void start();

            /**
             * \brief wait until stop is call
             */
            void wait();

            /**
             * \brief stop the server
             * Note : use it in a signal handler
             */
            void stop();


            /**
             * \brief init socket, and depandances
             */
            static void init();

            /**
             * \brief close socket, and depandances
             */
            static void close();

        //private:
            ntw::srv::Server gui_server; ///< the gui socket listener
            ntw::cli::Client gui_client_notif_sender; ///< the socket that send notification to the client.

            ntw::srv::Server file_server; ///< the external daemon socket listener

            salamandre::ServerBroadcast broadcaster;
            //BroadCast broadcast

            //void on_new_file_client(ntw::srv::Server& self,ntw::srv::Client& client);
    };
}
#endif

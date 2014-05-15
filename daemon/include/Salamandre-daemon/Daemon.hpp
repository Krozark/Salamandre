#ifndef SALAMANDRE_DAEMON_HPP
#define SALAMANDRE_DAEMON_HPP

#include <Socket/server/Server.hpp>
#include <Socket/client/Client.hpp>
#include <Salamandre-daemon/ServerBroadcast.hpp>
#include <Salamandre-daemon/FileManager.hpp>
#include <Salamandre-daemon/FileInfo.hpp>
#include <mutex>
#include <thread>

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
            Daemon(const std::string &path, int listen_gui_port, int listen_port, int broadcast_port, bool local=false);
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

            void notify(int type,int id_medecin,int id_patient,const std::string& filename);
            void setNotifierPort(unsigned int port);
            
            int getServerPort()const;

            std::string path;

            salamandre::ServerBroadcast broadcaster;///<BroadCast broadcast
    
        private:
            ntw::srv::Server file_server; ///< the external daemon socket listener
            ntw::srv::Server gui_server; ///< the gui socket listener

            FileManager file_manager;
            bool is_connect;
            bool run;

            std::mutex notifications_mutex; 
            std::thread notifications_thread;
            std::list<FileInfo> notifications;
            ntw::cli::Client gui_client_notif_sender; ///< the socket that send notification to the client.

            void _start_notifier();


            //void on_new_file_client(ntw::srv::Server& self,ntw::srv::Client& client);
    };
}
#endif

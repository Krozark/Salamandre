#ifndef SALAMANDRE_DAEMON_HPP
#define SALAMANDRE_DAEMON_HPP

#include <Socket/server/Server.hpp>

namespace salamandre
{
    class Daemon
    {
        public:
            /**
             * \brief The deamon class
             * \param listen_gui_port the port for the gui
             * \param listen_port the port to listen the others deamon
            */
            Daemon(int listen_gui_port, int listen_port);
            Daemon(const Daemon&) = delete;
            Daemon& operator=(const Daemon&) = delete;


            void start();

            void wait();

            void stop();

            /**
             * \brief store all the callback ids for the gui
             */
            enum funcGui {
                newFile = 1, ///< to call whene the gui add files to save
                sync ///< to call whene un sync is need
            };

            /**
             * \brief store the callback ids for external calls (with other Daemons)
             */
            enum funcExternal {
                thisIsMyInfos = 1, ///< recv infos of an othen Daemon (listen on broadcast)
                lostData, ///< is call whene a Daemon need to rebuild all his (listen on broadcast) datas                
                sendDatas, ///< recv files to save
            };

        private:
            ntw::srv::Server gui_server;
            ntw::srv::Server file_server;

            //void on_new_file_client(ntw::srv::Server& self,ntw::srv::Client& client);
    };
}
#endif

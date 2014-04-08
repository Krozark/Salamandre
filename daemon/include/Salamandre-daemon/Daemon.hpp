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

        private:
            const int gui_port;
            const int port;

            ntw::srv::Server ntw_server;
    };
}
#endif

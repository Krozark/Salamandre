#include <Salamandre-daemon/Daemon.hpp>
#include <Socket/server/Server.hpp>

namespace salamandre
{
    Daemon::Daemon(int listen_gui_port,int listen_port): gui_port(listen_gui_port), port(listen_port)
    {
    }
}

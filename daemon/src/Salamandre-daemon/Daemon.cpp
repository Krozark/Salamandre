#include <Salamandre-daemon/Daemon.hpp>

#include <Salamandre-daemon/GuiFunctions.hpp>
#include <Salamandre-daemon/ServerFunctions.hpp>


namespace salamandre
{
    Daemon::Daemon(int listen_gui_port,int listen_port,int broadcast_port,bool local): 
        gui_server(listen_gui_port,"127.0.0.1",salamandre::gui::dispatch,1,1),
        file_server(listen_port,"",salamandre::srv::dispatch,100),
        broadcaster(broadcast_port,listen_port,local)
    {
    }

    void Daemon::start()
    {
        FileManager::init();
        gui_server.start();
        file_server.start();
        broadcaster.start();
    }

    void Daemon::wait()
    {
        gui_server.wait();
        file_server.wait();
        broadcaster.wait();
    }

    void Daemon::stop()
    {
        gui_server.stop();
        file_server.stop();
        broadcaster.stop();
    }

    void Daemon::init()
    {
        ntw::Socket::init();
        ServerBroadcast::init();
    }

    void Daemon::close()
    {
        ServerBroadcast::close();
        ntw::Socket::close();
    }
}

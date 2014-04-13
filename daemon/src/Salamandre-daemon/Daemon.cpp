#include <Salamandre-daemon/Daemon.hpp>

#include <Salamandre-daemon/GuiFunctions.hpp>
#include <Salamandre-daemon/ServerFunctions.hpp>

namespace salamandre
{
    Daemon::Daemon(int listen_gui_port,int listen_port): 
        gui_server(listen_gui_port,"127.0.0.1",salamandre::gui::dispatch,1),
        file_server(listen_port,"",salamandre::srv::dispatch,100)
    {
    }

    void Daemon::start()
    {
        std::cout<<"[gui] start: please wait."<<std::endl;
        gui_server.start();
        std::cout<<"[gui] started"<<std::endl;

        std::cout<<"[srv] start: please wait."<<std::endl;
        file_server.start();
        std::cout<<"[srv] started"<<std::endl;
    }

    void Daemon::wait()
    {
        gui_server.wait();
        std::cout<<"[gui] is now down"<<std::endl;

        file_server.wait();
        std::cout<<"[srv] is now down"<<std::endl;
    }

    void Daemon::stop()
    {
        std::cout<<"[gui] stop: send signal."<<std::endl;
        gui_server.stop();

        std::cout<<"[srv] stop: send signal."<<std::endl;
        file_server.stop();
    }

    void Daemon::init()
    {
        ntw::Socket::init();
    }

    void Daemon::close()
    {
        ntw::Socket::close();
    }
}

#include <Salamandre-daemon/Daemon.hpp>

#include <Salamandre-daemon/GuiFunctions.hpp>
#include <Salamandre-daemon/ServerFunctions.hpp>

#include <Salamandre-stats/stats.hpp>

namespace salamandre
{
    Daemon::Daemon(int listen_gui_port,int listen_port,int broadcast_port): 
        gui_server(listen_gui_port,"127.0.0.1",salamandre::gui::dispatch,1,1),
        file_server(listen_port,"",salamandre::srv::dispatch,100),
        broadcaster(broadcast_port,listen_port)
    {
    }

    void Daemon::start()
    {
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
        stats::Stats::init();

        stats::Node::clear();

        stats::Stats::add_node(std::string("127.0.0.1"), 3000);
        stats::Stats::add_node(std::string("127.0.0.1"), 3500);
        stats::Stats::add_node(std::string("127.0.0.1"), 4000);
        stats::Stats::add_node(std::string("127.0.0.1"), 4500);
        stats::Stats::add_node(std::string("127.0.0.1"), 5000);
        stats::Stats::add_node(std::string("127.0.0.1"), 5500);
        stats::Stats::add_node(std::string("127.0.0.1"), 6000);
        stats::Stats::add_node(std::string("127.0.0.1"), 6500);
        stats::Stats::add_node(std::string("127.0.0.1"), 7000);
        stats::Stats::add_node(std::string("127.0.0.1"), 7500);
        stats::Stats::add_node(std::string("127.0.0.1"), 8000);
        stats::Stats::add_node(std::string("127.0.0.1"), 8500);
    }

    void Daemon::close()
    {
        ntw::Socket::close();
        stats::Stats::close();
    }
}

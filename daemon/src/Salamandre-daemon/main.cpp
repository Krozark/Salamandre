#include <Salamandre-daemon/Daemon.hpp>
#include <Salamandre-daemon/ServerBroadcast.hpp>
#include <Salamandre-daemon/define.hpp>

#include <utils/log.hpp>

#include <iostream>
#include <csignal>
#include <tclap/CmdLine.h>

namespace salamandre
{
    Daemon* daemon = nullptr;
}

void stop_server_handler(int sig)
{
    utils::log::info("Stop","Recv signal",sig,"Stoping server.\n Please wait.");
    if(salamandre::daemon)
        salamandre::daemon->stop();
}


int main(int argc,char* argv[])
{
    int gui_port = 3842;
    int server_port = 3843;
    int broadcast_port = 5001;
    bool local_only = false;
    try {

        TCLAP::CmdLine cmd("Salamandre Daemon", ' ', DAEMON_VERSION);
        TCLAP::ValueArg<int> gui_port_arg("g", "gui-port", "Port for the GUI", false, gui_port, "int", cmd);
        TCLAP::ValueArg<int> server_port_arg("s", "server-port", "Port for the server to listen", false, server_port, "int", cmd);
        TCLAP::SwitchArg local_switch("l", "local", "Whether daemon sould run stricly locally", cmd, false);

        cmd.parse(argc, argv);

        local_only = local_switch.getValue();
        gui_port = gui_port_arg.getValue();
        server_port = server_port_arg.getValue();

    } catch(TCLAP::ArgException &e) {
        utils::log::critical(1,"Args ","error",e.error(),"for arg",e.argId());
    }

    std::cout<<"Daemon start on:"
        <<"\n\tgui listen on port : "<<gui_port
        <<"\n\tfile server listen on port : "<<server_port
        <<"\n\tbrodcast server listen on port: "<<broadcast_port
        <<"\n\tlocal only : "<<local_only
        <<"\nPress ^C to exit"
        <<std::endl<<std::endl;
    std::signal(SIGINT, stop_server_handler);

    try
    {
        salamandre::Daemon::init();

        salamandre::daemon = new salamandre::Daemon(gui_port,server_port,broadcast_port,local_only);
        salamandre::daemon->start();
        salamandre::daemon->wait();

        delete salamandre::daemon;
        salamandre::daemon = nullptr;

        salamandre::Daemon::close();

    }
    catch(ntw::SocketExeption& e)
    {
        utils::log::error("Socket error",e.what());
    }

    utils::log::info("End","Daemon is now close, Good bye");

    return 0;
}

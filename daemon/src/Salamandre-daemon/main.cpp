#include <Salamandre-daemon/Daemon.hpp>
#include <Salamandre-daemon/ServerBroadcast.hpp>

#include <utils/log.hpp>

#include <iostream>
#include <csignal>

#define GUI_PORT 1
#define SERVER_PORT 2
#define LOCAL_ONLY 3
#define NB_ARGS (LOCAL_ONLY + 1)

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


    if(argc < NB_ARGS)
    {
        utils::log::error("Usage",argv[0],"<gui port> <server port> <local only>");
        if(argc <= GUI_PORT)
            utils::log::info("Useage",gui_port,"is used as default gui port");
        if(argc <= SERVER_PORT)
            utils::log::info("Usage",server_port,"is use as default server port");
        if(argc <= LOCAL_ONLY)
            utils::log::info("Useage",local_only,"is use as default local only value");
    }
    if(argc > GUI_PORT)
        gui_port = atoi(argv[GUI_PORT]);
    if(argc > SERVER_PORT)
        server_port = atoi(argv[SERVER_PORT]);
    if(argc > LOCAL_ONLY)
        local_only = atoi(argv[LOCAL_ONLY]);


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

#include <Salamandre-daemon/Daemon.hpp>
#include <Salamandre-daemon/ServerBroadcast.hpp>

#include <iostream>
#include <csignal>

#include <Salamandre-daemon/std.hpp>

#define GUI_PORT 1
#define SERVER_PORT 2
#define NB_ARGS (SERVER_PORT + 1)

namespace salamandre
{
    Daemon* daemon = nullptr;
}

void stop_server_handler(int sig)
{
    std::cout<<"Recv signal "<<sig<<". Stoping server.\n Please wait."<<std::endl;
    if(salamandre::daemon)
        salamandre::daemon->stop();
}


int main(int argc,char* argv[])
{
    int gui_port = 3842;
    int server_port = 3843;
    int broadcast_port = 5001;


    if(argc < NB_ARGS)
    {
        std::cout<<"Usage is:\n\t"<<argv[0]<<" <gui port> <server port>"<<std::endl;
        if(argc <= GUI_PORT)
            std::cout<<"Use "<<gui_port<<" as default gui port"<<std::endl;
        if(argc <= SERVER_PORT)
            std::cout<<"Use "<<server_port<<" as default server port"<<std::endl;
    }
    if(argc > GUI_PORT)
        gui_port = atoi(argv[GUI_PORT]);
    if(argc > SERVER_PORT)
        server_port = atoi(argv[SERVER_PORT]);


    std::cout<<"Daemon start on:"
        <<"\n\tgui port : "<<gui_port
        <<"\n\tfile server port : "<<server_port
        <<"\nPress ^C to exit"
        <<std::endl<<std::endl;
    std::signal(SIGINT, stop_server_handler);

    try
    {
        salamandre::Daemon::init();

        salamandre::daemon = new salamandre::Daemon(gui_port,server_port,broadcast_port);
        salamandre::daemon->start();
        salamandre::daemon->wait();

        delete salamandre::daemon;
        salamandre::daemon = nullptr;

        salamandre::Daemon::close();

    }
    catch(ntw::SocketExeption& e)
    {
        std::cout<<e.what()<<std::endl;
    }

    std::cout<<"Daemon is now close. Good bye"<<std::endl;    

    return 0;
}

#include <Salamandre-stats/stats.hpp>
#include <Salamandre-daemon/Daemon.hpp>
#include <Salamandre-daemon/ServerBroadcast.hpp>
#include <Salamandre-daemon/define.hpp>

#include <utils/log.hpp>

#include <sys/types.h>
#include <sys/stat.h>
#include <sys/file.h>
#include <fcntl.h>
#include <unistd.h>

#include <iostream>
#include <csignal>
#include <tclap/CmdLine.h>

namespace salamandre
{
    Daemon* daemon = nullptr;
    int fd_pid;
}

void stop_server_handler(int sig)
{
    utils::log::info("Stop","Recv signal",sig,"Stoping server.\n Please wait.");
    if(salamandre::daemon)
        salamandre::daemon->stop();
    close(salamandre::fd_pid);
}

void daemonize(std::string log_path, std::string pid_path)
{


    int pid = fork();

    if (pid < 0)
    {
        std::cerr << "Unable to fork, exiting" << std::endl;
        exit(1);
    } else if( pid != 0)
    {
        // Parent process, exiting
        std::cout << "Turning into background... PID: " << pid << std::endl;
        exit(0);
    }

    // Detaching from any controlling terminal
    if (setsid() == -1)
    {
        perror("Unable to detach from terminal");
        exit(1);
    }

    salamandre::fd_pid = ::open(pid_path.c_str(), O_CREAT | O_WRONLY, S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH);
    if (salamandre::fd_pid == -1)
    {
        ::perror("Unable to create and open the pid_file");
        ::close(salamandre::fd_pid);
        ::exit(1);
    }

    if (flock(salamandre::fd_pid, LOCK_EX | LOCK_NB) != 0)
    {
        ::perror("Unable to lock the pid_file, is the daemon already running ?");
        ::close(salamandre::fd_pid);
        ::exit(1);
    }
    ::ftruncate(salamandre::fd_pid, 0);

    char buffer[32];
    ::snprintf(buffer, 32, "%d", ::getpid());
    ::write(salamandre::fd_pid, buffer, strlen(buffer));
    ::syncfs(salamandre::fd_pid);

   std::ifstream in("/dev/null");
   std::cin.rdbuf(in.rdbuf());

   // That should be the good solution, but it causes segfaults
   /*
   std::ofstream out(log_path.c_str());
   std::cout.rdbuf(out.rdbuf());
   std::cerr.rdbuf(out.rdbuf());
   */

   FILE * log_file = ::fopen(log_path.c_str(), "a");
   ::close(STDOUT_FILENO);
   ::dup(::fileno(log_file));
   ::dup2(STDOUT_FILENO, STDERR_FILENO);
   ::close(::fileno(log_file));
}

int main(int argc,char* argv[])
{
    int gui_port = 3842;
    int server_port = 3843;
    int broadcast_port = 5001;
    std::string node_file_path;
    bool local_only = false;
    bool daemon = false;
    std::string path = argv[0];
    try {

        TCLAP::CmdLine cmd("Salamandre Daemon", ' ', DAEMON_VERSION);
        TCLAP::ValueArg<int> gui_port_arg("g", "gui-port", "Port for the GUI", false, gui_port, "int", cmd);
        TCLAP::ValueArg<int> server_port_arg("s", "server-port", "Port for the server to listen", false, server_port, "int", cmd);
        TCLAP::ValueArg<std::string> nodes_file_arg("f", "nodes-file", "File containing node list", false, "", "string", cmd);
        TCLAP::SwitchArg local_switch("l", "local", "Whether daemon sould run stricly locally", cmd, false);
        TCLAP::SwitchArg daemon_switch("d", "daemonize", "Start the server in a daemon", cmd, false);

        cmd.parse(argc, argv);

        local_only = local_switch.getValue();
        gui_port = gui_port_arg.getValue();
        server_port = server_port_arg.getValue();
        node_file_path = nodes_file_arg.getValue();
        daemon = daemon_switch.getValue();

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
    std::signal(SIGTERM, stop_server_handler);
    if (daemon)
    {
        daemonize("salamandre.log", "salamandre.pid");
    }
    try
    {
        salamandre::Daemon::init();

        if (!node_file_path.empty())
        {
            salamandre::stats::Stats::load(node_file_path.c_str());
        }

        salamandre::daemon = new salamandre::Daemon(path, gui_port,server_port,broadcast_port,local_only);
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

    if (daemon)
    {
        ::unlink("salamandre.pid");
    }
    return 0;
}

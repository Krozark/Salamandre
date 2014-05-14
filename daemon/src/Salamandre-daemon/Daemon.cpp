#include <Salamandre-daemon/Daemon.hpp>

#include <Salamandre-daemon/GuiFunctions.hpp>
#include <Salamandre-daemon/ServerFunctions.hpp>

#include <utils/log.hpp>


namespace salamandre
{
    Daemon::Daemon(const std::string &path, int listen_gui_port,int listen_port,int broadcast_port, bool local):
        path(path),
        broadcaster(broadcast_port,listen_port,local),
        file_server(listen_port,"",salamandre::srv::dispatch,100),
        gui_server(listen_gui_port,"127.0.0.1",salamandre::gui::dispatch,1,1),
        file_manager(10),
        is_connect(false)
    {
        //file_server.on_delete_client = salamandre::srv::on_delete_client;
    }

    void Daemon::start()
    {
        file_manager.start();
        gui_server.start();
        file_server.start();
        broadcaster.start();
    }

    void Daemon::wait()
    {
        file_manager.wait();
        gui_server.wait();
        file_server.wait();
        broadcaster.wait();
    }

    void Daemon::stop()
    {
        file_manager.stop();
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

    void Daemon::notify(int type,int id_medecin,int id_patient,const std::string& filename)
    {
        if(is_connect)
        {
            FileInfo tmp;
            tmp.version = type;
            tmp.id_medecin = id_medecin;
            tmp.id_patient = id_patient;
            tmp.filename = filename;

            notifications_mutex.lock();
            notifications.push_back(std::move(tmp));
            notifications_mutex.unlock();

            //daemon->is_connect = (daemon->gui_client_notif_sender.request_sock.getStatus() == ntw::Status::ok);
        }
    }

    void Daemon::setNotifierPort(unsigned int port)
    {
        gui_client_notif_sender.disconnect();
        is_connect = false;
        if(gui_client_notif_sender.connect("127.0.0.1",port) == ntw::Status::ok)
            daemon->is_connect = true;
        else
            utils::log::error("Daemon::setNotifierPort","Unable to connect to client notifier");
    }

    int Daemon::getServerPort()const
    {
        return file_server.port();
    }
}

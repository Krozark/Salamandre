#ifndef SOCKSENDER_HPP
#define SOCKSENDER_HPP

#include <Socket/client/Client.hpp>

#define DEFAULT_SERVEUR_PORT 20001
#define DEFAULT_IP std::string("127.0.0.1")

class sockSender
{
public:
    static void init();
    static bool connectToDaemon();
    static void closeConnectionToDaemon();
    static bool setGuiServerPort(int srvPort);

    static void sendFile(int idDoctor, int idPatient, std::string filename = "");
    static bool getFile(int idDoctor, int idPatient = -1, std::string filename = "");

    static std::string getDaemonBackupPath();
    static std::string getDaemonSavePath();
    static std::string getDaemonBinPath();

private:
    sockSender();
    sockSender(sockSender const&) = delete;
    void operator=(sockSender const&) = delete;

    static bool informNewFile(int idDoctor, int idPatient, std::string filename);
    static bool restartDaemon();
    static void initConnectionToDaemon();


    static sockSender sock;
    static ntw::cli::Client client;

    static int checkStatus();

    int srvPort;
    std::string srvIpAddress;

    std::string daemonBinPath;
    std::string daemonSavePath;
    std::string daemonBackupPath;
    std::string guiPath;
};

#endif // SOCKSENDER_HPP

#ifndef SOCKSENDER_HPP
#define SOCKSENDER_HPP

#include <Socket/client/Client.hpp>

#include <QString>

#define DEFAULT_SERVEUR_PORT 20001
#define DEFAULT_IP std::string("127.0.0.1")
#define CONNECTION_TEST_NUMBER 30

class sockSender
{
public:
    static void init();
    static bool connectToDaemon();
    static void closeConnectionToDaemon();

    static void sendFile(int idDoctor, int idPatient, std::string filename = "");
    static bool getFile(int idDoctor, int idPatient = -1, std::string filename = "");

private:
    sockSender();
    sockSender(sockSender const&);
    void operator=(sockSender const&);

    static bool informNewFile(int idDoctor, int idPatient, std::string filename);
    static bool restartDaemon();
    static void initConnectionToDaemon();

    static sockSender sock;
    static ntw::cli::Client client;

    static int checkStatus();



    int srvPort;
    std::string srvIpAddress;

    std::string daemonPath;
    std::string daemonGuiSavePath;
    std::string guiPath;
};

#endif // SOCKSENDER_HPP

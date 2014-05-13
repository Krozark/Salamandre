#ifndef SOCKRECEIVER_HPP
#define SOCKRECEIVER_HPP

#include <Socket/server/Server.hpp>
#include <Socket/FuncWrapper.hpp>
#include <Salamandre-daemon/GuiFunctions.hpp>

#include <QVector>
#include <QObject>

#define DEFAULT_NOTIF_SERVER_PORT 20005
#define DEFAULT_NOTIF_IP std::string("127.0.0.1")

class getFile{

public:
    getFile(){}
    getFile(int idDoctor, int idPatient = -1, std::string filename = "") : idDoctor(idDoctor), idPatient(idPatient), filename(filename){}

    int idDoctor;
    int idPatient;
    std::string filename;
};

Q_DECLARE_METATYPE(getFile)

class sockReceiver : public QObject
{
    Q_OBJECT
public:
    static void init(int srvPort = DEFAULT_NOTIF_SERVER_PORT, std::string ipAdress = DEFAULT_NOTIF_IP);
    static bool connectToDaemon();
    static void closeConnectionToDaemon();

    static void askFile(getFile *file);

    static void funcFileIsSend(ntw::SocketSerialized& socket,int idDoctor, int idPatient, std::string filename);
    static void funcFileIsRecv(ntw::SocketSerialized& socket, int idDoctor, int idPatient, std::string filename);
    static void funcSyncIsFinished(ntw::SocketSerialized& socket,int idDoctor, int idPatient, std::string filename);

    static int notification_dispatch(int id,ntw::SocketSerialized& request);

    static sockReceiver sock;
private:
    sockReceiver();
    ~sockReceiver();
    sockReceiver(sockReceiver const&) = delete;
    void operator=(sockReceiver const&) = delete;

    ntw::srv::Server *server;
    int srvPort;
    std::string srvIpAddress;

    QVector<getFile*> patientDataList;

signals:
    void fileIsRecv(getFile*);
    void syncIsFinish(getFile*);
};

#endif // SOCKRECEIVER_HPP

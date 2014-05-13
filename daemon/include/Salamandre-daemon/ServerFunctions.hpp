#ifndef SALAMANDRE_SERVERFUNCTIONS_HPP
#define SALAMANDRE_SERVERFUNCTIONS_HPP

#include <Socket/SocketSerialized.hpp>
#include <Salamandre-daemon/FileManager.hpp>
#include <list>
#include <mutex>

namespace salamandre
{
    class Daemon;
    extern Daemon* daemon; ///<The current daemon
/**
 * \brief namespace for server of external calls (other daemons)
 */
namespace srv
{
    /**
     * \brief store the callback ids for external calls (with other Daemons)
     */
    enum func {
        //broadcast
        thisIsMyInfos = 1, ///< recv infos of an othen Daemon (listen on broadcast)
        iLostMyData, ///< is call whene a Daemon need to rebuild all his (listen on broadcast) datas                

        //connected
        thisIsMyFiles,///< send a filename list
        sendFile, ///< recv files to save
    };

    /***
     * \brief The distpatch function
     */
    int dispatch(int id,ntw::SocketSerialized& request);

    /**
     * \brief connect to the ip:port and send a file list
     */
    void funcILostMyData_BroadcastRecv(int id_medecin,int id_patient,const std::string& filename,unsigned int port,const std::string& ip);

    void funcThisIsMyFiles_Recv(ntw::SocketSerialized& request,std::list<FileManager::FileInfo>);

    struct FileInfoFrom
    {
        //file
        long int version;
        int id_medecin;
        int id_patient;
        std::string filename;
        //src
        ntw::SocketSerialized* request;
    };

    static std::mutex file_info_mutex;
    static std::list<FileInfoFrom> file_info_from;
}
}
#endif

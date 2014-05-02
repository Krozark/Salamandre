#ifndef SALAMANDRE_GUIFUNCTIONS_HPP
#define SALAMANDRE_GUIFUNCTIONS_HPP

#include <Socket/define.hpp>
#include <Socket/SocketSerialized.hpp>

#include <string>

namespace salamandre
{
/**
 * \brief namespace for the  local calls (with the gui)
 */
namespace gui
{
    /**
     * \brief store all the callback ids for the gui
     */
    enum func {
        newFile = 1, ///< to call whene the gui add files to save
        sync, ///< to call whene un sync is need
        isInUpdate, ///< is in update
        setGuiNotificationPort,///< set the notification port to use
        ///NOTIFICATIONS
        fileIsSend, ///< send a notification to the gui with the info of sended file
        fileIsRecv ///< send a notification to the gui with the info of recv file
    };

    /**
     * \brief enum that contain the error code put as socket status
     */
    enum status {
        STOP = NTW_STOP_CONNEXION,
        OK = 0, 
        TODO = 1,
        WRONG_PARAM = 2,
        ENABLE_TO_SEND_FILE = 3
    };

    /***
     * \brief Convert status code to string for human
     * \return a string for humans
     */
    inline std::string statusToString(int code);

    /***
     * \brief The distpatch function
     */
    int dispatch(int id,ntw::SocketSerialized& request);

    /***
     * \brief save new file
     * \param sock the socket
     * \param id_medecin the medecin id
     * \param id_patient the patient id (if <=0, all are check)
     * \param filename the filename (if empty, all files are saves)
     */
    void funcNewFile(ntw::SocketSerialized& sock,int id_medecin, int id_patient, std::string filename);

    /**
     * \brief ask for a sync
     * \param sock the socket
     * \param id_medecin the medecin id
     * \param id_patient the patient id (if <=0, all are check)
     * \param filepath the filename (if empty, all files are get)
     */
    void funcSync(ntw::SocketSerialized& sock,int id_medecin, int id_patient, std::string filename);

    /**
     * \brief is in update
     * \param sock the socket
     * \param id_medecin the medecin id
     * \param id_patient the patient id (if <=0, all are check)
     * \param filepath the filename (if empty, all files are get)
     */
    bool funcIsInUpdate(ntw::SocketSerialized& sock,int id_medecin, int id_patient,std::string filename);

    ///////////// NOTIFICATIONS ///////////////////
    //This fuctions have to be implemented in the gui side

    /**
     * \brief Sent to the gui information about the file that has been send
     * \param sock the socket
     * \param id_medecin the medecin id
     * \param id_patient the patient id
     * \param filepath the filename
     */
    void funcFileIsSend(ntw::SocketSerialized& sock,int id_medecin, int id_patient, std::string filename);

    /**
     * \brief Sent to the gui information about the file that has been recv from  metwork
     * \param sock the socket
     * \param id_medecin the medecin id
     * \param id_patient the patient id
     * \param filepath the filename
     */
    void funcFileIsRecv(ntw::SocketSerialized& sock,int id_medecin, int id_patient, std::string filename);

}
}
#include <Salamandre-daemon/GuiFunctions.tpl>
#endif

#ifndef SALAMANDRE_SERVERFUNCTIONS_HPP
#define SALAMANDRE_SERVERFUNCTIONS_HPP

#include <Socket/SocketSerialized.hpp>

namespace salamandre
{
/**
 * \brief namespace for server of external calls (other daemons)
 */
namespace srv
{
    /**
     * \brief store the callback ids for external calls (with other Daemons)
     */
    enum func {
        thisIsMyInfos = 1, ///< recv infos of an othen Daemon (listen on broadcast)
        iLostData, ///< is call whene a Daemon need to rebuild all his (listen on broadcast) datas                
        thisIsMyFiles,///< serd a filename list
        sendFile, ///< recv files to save
    };

    /***
     * \brief The distpatch function
     */
    int dispatch(int id,ntw::SocketSerialized& request);
}
}
#endif

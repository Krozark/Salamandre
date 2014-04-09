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
        lostData, ///< is call whene a Daemon need to rebuild all his (listen on broadcast) datas                
        sendDatas, ///< recv files to save
    };

    /***
     * \brief The distpatch function
     */
    int dispatch(int id,ntw::SocketSerialized& request);
}
}
#endif
